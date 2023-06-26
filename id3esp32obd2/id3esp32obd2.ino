/*
 * Project: id3esp32obd2 (Ding19)
 * DIY OBD2 Bluetooth dongle for an VW ID.3
 * An ESP32 is used to forward the following VW ID.3 diagnostics data from the OBD2 connector via Bluetooth to an Android device:
 * - SOC (BMS)
 * - Car operation mode
 * - ODOMETER
 * - VIN
 * - Speed
 * - Driving mode position
 * - HV auxiliary consumer power
 * - HV battery main temperature
 * - HV battery circulation pump 
 * - HV battery voltage
 * - HV battery current
 * - HV dynamic limit for charging
 * - HV Total charge
 * - HV Total discharge
 * - HV battery capacity
 * 
 * License: 2-Clause BSD License
 * Copyright (c) 2023 codingABI
 * For details see: License.txt
 * 
 * created by codingABI https://github.com/codingABI/id3esp32obd2
 * 
 * Warning:
 * - Connecting something to your VW ID.3 is your own risk and do this ONLY, if you know, what are you doing. If you make a mistake, it could be a very expensive mistake.
 * - This is no a complete ISO-TP and/or UDS implementation. It is just enough to receive some data from a VW ID.3. I'm no CAN expert, just a beginner
 * 
 * Hardware:
 * - ESP-WROOM-32 NodeMCU (Board manager: ESP32 Dev Model)
 * - SN65HVD230 CAN transceiver (60 Ohm resistor R2 between CANHigh and CANLow was removed)
 * - OBD2 Connector cable (Plugged in to the OBD2 female connector below the steering wheel)
 * - VW ID.3 Car (Code was developed and tested on a ID.3 Pro S)
 * - Two resistors (470k, 100k) for a voltage divider to measure the 12V car battery voltage on an ESP32 analog pin 
 * - LM2596 Step down voltage converter (to convert the 12V car battery to 3.3V as a power supply for the ESP32)
 * - Android device with the self-made app: id3esp32obd2 (https://github.com/codingABI/id3esp32obd2/id3esp32obd2.apk)
 *  
 * Power consumption:
 * - 0.6W (0.3W when no Android device is connected)
 *  
 * VW ID.3 OBD2 connector characteristics (without anything plugged in):
 * - 70 Ohm between CANHigh and CANLow
 * - 2.5V between CANHigh and GND
 * - 2.5V between CANLow and GND
 * 
 * Usefull links:
 * https://automotive.wiki/index.php/ISO_14229
 * https://mk4-wiki.denkdose.de/artikel/can-bus/protokolle/uds/sid
 * https://www.goingelectric.de/wiki/Liste-der-OBD2-Codes/
 * https://github.com/nickn17/evDash/blob/master/src/CarVWID3.cpp
 * https://github.com/spot2000/Volkswagen-MEB-EV-CAN-parameters/blob/main/VW%20MEB%20UDS%20PIDs%20list.csv
 * https://www.meinid.com/thread/1640-obd2-header-pid-liste/
 */

#include <driver/gpio.h>
#include <driver/can.h>
#include "secrets.h"
#include <BluetoothSerial.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define BTDEVICENAME "id3esp32obd2"

// Some CAN/UDS/ISOTP definitions
#define CAN_RECEIVE_TIMEOUT 1000 // 10000
#define CAN_SEND_TIMEOUT 4000
#define UDS_ReadDataByIdentifier_0x22 0x22
#define UDS_DiagnosticSessionControl_0x10 0x10
#define UDS_ExtendedDiagnosticSession_0x03 0x03
#define UDS_RequestOutOfRange_0x31 0x31
#define UDS_ServiceNotSupported_0x11 0x11
#define UDS_NegativeResponse_0x7f 0x7f
#define UDS_PositiveResponseOffset_0x40 0x40
#define UDS_positiveResponse_0x00 0x00
#define ISOTP_singleFrame_0x0 0x0
#define ISOTP_firstFrame_0x1 0x1
#define ISOTP_consecutiveFrame_0x2 0x2
#define ISOTP_flowControlFrame_0x3 0x3
#define ISOTP_functionalRequests_0x18DB33F1 0x18DB33F1

// Car parameters, that can sent by Bluetooth 
enum dataIDs { 
  idNOP,
  idSOCBMS, 
  idSPEED, 
  idCARMODE,
  idAUXPOWER,
  idODOMETER,
  idDRIVINGMODE,
  idVIN,
  idBATTEMPERATURE,
  idBAT12V,
  idCOMPILETIME,
  idHVCIRCULATIONPUMP,
  idHVBATV,
  idHVBATA,
  idHVCHARGINGLIMIT,
  idHVTOTALCHARGE,
  idHVTOTALDISCHARGE,
  idHVCAPACITY  
};

// A dashboard is a set of car parameters. A Bluetooth client can request a dashboard.
enum dashboards { 
  DEFAULTDASHBOARD,
  SPEEDDASHBOARD,
  HVDASHBOARD,
  MYDASHBOARD
};

// Pin definitions
#define LED_PIN 2
#define MEASURE12V_PIN 4
#define BUZZER_PIN 16

// CAN transceiver SN65HVD230
#define CTX_PIN GPIO_NUM_19
#define CRX_PIN GPIO_NUM_18

// Classic Bluetooth
BluetoothSerial g_SerialBT;

// Receive buffer (multi frames can have a length up to 4095 bytes)
#define MAXBUFFER 4095
byte g_dataBuffer[MAXBUFFER];
long g_dataBufferLength; // Length of payload in receive buffer

bool g_btAuthenticated = false; // Is it allowed to send data via Bluetooth?
bool g_VINdone = false; // VIN already received?
byte g_requestDashboard = DEFAULTDASHBOARD; // Default dashboard

// Beep types
enum beepTypes { DEFAULTBEEP, SHORTBEEP, LONGBEEP, HIGHSHORTBEEP, LASER };

// Buzzer
void beep(int type=DEFAULTBEEP) {
  // User PWM to improve quality
  switch(type) {
    case DEFAULTBEEP: { // 500 Hz for 200ms
      ledcSetup(0, 500, 8);
      ledcAttachPin(BUZZER_PIN, 0);
      ledcWrite(0, 128);
      delay(200);
      ledcWrite(0, 0);
      ledcDetachPin(BUZZER_PIN);          
      break;
    }
    case SHORTBEEP: { // 1 kHz for 100ms
      ledcSetup(0, 1000, 8);
      ledcAttachPin(BUZZER_PIN, 0);
      ledcWrite(0, 128);
      delay(100);
      ledcWrite(0, 0);
      ledcDetachPin(BUZZER_PIN);          
      break;
    }
    case LONGBEEP: { // 250 Hz for 400ms
      ledcSetup(0, 250, 8);
      ledcAttachPin(BUZZER_PIN, 0);
      ledcWrite(0, 128);
      delay(400);
      ledcWrite(0, 0);
      ledcDetachPin(BUZZER_PIN);          
      break;
    }
    case HIGHSHORTBEEP: { // High and short beep 
      ledcSetup(0, 5000, 8);
      ledcAttachPin(BUZZER_PIN, 0);
      ledcWrite(0, 128);
      delay(100);
      ledcWrite(0, 0);
      ledcDetachPin(BUZZER_PIN);          
      break;
    }
    case LASER: { // Laser like sound
      int i = 5000; // Start frequency in Hz (goes down to 300 Hz)
      int j = 300; // Start duration in microseconds (goes up to 5000 microseconds)
      ledcSetup(0, i, 8);
      ledcAttachPin(BUZZER_PIN, 0);
      ledcWrite(0, 0);
      while (i>300) {
        i -=50;
        j +=50;
        ledcSetup(0, i, 8);
        ledcWrite(0, 128);
        delayMicroseconds(j);
        ledcWrite(0,0);
        delayMicroseconds(1000);
      }
      ledcWrite(0, 0);
      ledcDetachPin(BUZZER_PIN);          
      break;
    }
    default: {
    }
  }
}

// Bluetooth callback function
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (event == ESP_SPP_SRV_OPEN_EVT) { // Client connected
    Serial.print("Bluetooth client connected with address: ");
    for (int i = 0; i < 6; i++) {
      snprintf(strData,MAXSTRDATALENGTH+1,"%02X",param->srv_open.rem_bda[i]);
      Serial.print(strData);
      if (i < 5) Serial.print(":"); else Serial.println();
    }
    beep(SHORTBEEP);
    if ( // Send data only to the Android device defined in secrets.h
      (param->srv_open.rem_bda[0] == g_macMobileDevice[0]) && 
      (param->srv_open.rem_bda[1] == g_macMobileDevice[1]) &&
      (param->srv_open.rem_bda[2] == g_macMobileDevice[2]) &&
      (param->srv_open.rem_bda[3] == g_macMobileDevice[3]) &&
      (param->srv_open.rem_bda[4] == g_macMobileDevice[4]) &&
      (param->srv_open.rem_bda[5] == g_macMobileDevice[5])
    ) { 
      g_btAuthenticated = true;
      g_VINdone = false;
    }
  }
  if (event == ESP_SPP_CLOSE_EVT) { // Client disconnected
    g_btAuthenticated = false;
    beep(LONGBEEP);
  }
}

// Check commands received via Bluetooth
void checkBTReceive() {
  byte received;
  while (g_SerialBT.available()) {
    received = g_SerialBT.read();
    if ((received == 255) && g_btAuthenticated) { // Reset command
      Serial.println("Reset command received");
      Serial.flush();
      ESP.restart();
    } else g_requestDashboard = received;
  }
}

void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(LED_PIN,OUTPUT);
  pinMode(MEASURE12V_PIN,INPUT);
  
  // Reduce cpu speed to reduce power consumption (Serial.print and Bluetooth does not work below 80 MHz)
  setCpuFrequencyMhz(80);

  // Init CAN
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(CTX_PIN, CRX_PIN, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
  if (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("CAN driver installed");
  } else {
    Serial.println("Failed to install CAN driver");
  }

  if (can_start() == ESP_OK) {
    Serial.println("CAN driver started");
  } else {
    Serial.println("Failed to start CAN driver");
  }

  // Clear receive buffer
  clearBuffer();

  // Init classic Bluetooth 
  g_SerialBT.register_callback(btCallback);
  if (!g_SerialBT.begin(BTDEVICENAME)) {
    Serial.println("Can not enable Bluetooth");
  } else {
    Serial.print("Bluetooth ready at ");
    Serial.println(g_SerialBT.getBtAddressString());    
  }

  Serial.println("Setup finished");
  beep(LASER);
}

void loop() {
  // Check Bluetooth before checking g_btAuthenticated to process the receive queue anyway
  checkBTReceive();
  if (!g_SerialBT.hasClient() && g_btAuthenticated) {
    Serial.println("No Bluetooth client connected");
    g_btAuthenticated = false;
  }
  if (!g_btAuthenticated) return;

  // Enable LED to show CAN activity
  digitalWrite(LED_PIN,HIGH);

  // Process current dashboard
  switch(g_requestDashboard) {
    case DEFAULTDASHBOARD:
      requestDefaultDashboard(g_requestDashboard);
      break;
    case SPEEDDASHBOARD:
      requestSpeedDashboard();
      break;
    case HVDASHBOARD:
      requestHVDashboard(g_requestDashboard);
      break;    
    case MYDASHBOARD:
      requestMyDashboard(g_requestDashboard);
      break;    
    default:
      Serial.print("Unknown dashboard requested ");
      Serial.println(g_requestDashboard);
  }

  // Disable LED
  digitalWrite(LED_PIN,LOW);  
}
