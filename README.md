# id3esp32obd2
The *id3esp32obd2* is an ESP32 based DIY OBD2 Bluetooth dongle and an Android app to show diagnostics data from a VW ID.3 car on an Android device.

![Overview](assets/images/Overview.jpg)

Watch the device in version 1.0.0 on [YouTube](https://youtu.be/gbIHqaEMSuo)

The following diagnostics data values are supported at the moment:
- SOC (BMS)
- Car operation mode
- ODOMETER
- VIN
- Speed
- Driving mode position
- HV auxiliary consumer power
- HV battery main temperature
- HV battery circulation pump 
- HV battery voltage
- HV battery current
- HV dynamic limit for charging
- HV Total charge
- HV Total discharge
- HV battery capacity
- 12V battery voltage
- HV PTC heater current
- Outside temperature
- Inside temperature
- Cruising range
- Charging state
- CO2 content interior
- GPS time
- GPS height
- GPS latitude
- GPS longitude

**WARNING:**
- Connecting something to your VW ID.3 is your own risk and do this ONLY, if you know, what are you doing. If you make a mistake, it could be a very expensive mistake.
- This is no a complete ISO-TP and/or UDS implementation. It is just enough to receive some data from a VW ID.3. I'm no CAN expert, just a beginner
  
## License and copyright
This project is licensed under the terms of the 2-Clause BSD License [Copyright (c) 2023-2024 codingABI](LICENSE). 

## Appendix
### Hardware
- ESP-WROOM-32 NodeMCU (Board manager: ESP32 Dev Model)
- SN65HVD230 CAN transceiver (60 Ohm resistor R2 between CANHigh and CANLow was removed)
- OBD2 Connector cable (Plugged in to the OBD2 female connector below the steering wheel)
- VW ID.3 Car (Code was developed and tested on a ID.3 Pro S)
- Two resistors (470k, 100k) for a voltage divider to measure the 12V car battery voltage on an ESP32 analog pin 
- LM2596 Step down voltage converter (to convert the 12V car battery to 3.3V as a power supply for the ESP32)
- Android device with the self-made app: *id3esp32obd2* (https://github.com/codingABI/id3esp32obd2/id3esp32obd2.apk)

Power consumption: 0.6W (0.3W when Android device is not connected), 0W when powered off by the toggle switch S1

### Used Arduino development environment

- Arduino IDE 2.3.4 or 1.8.19
- arduino-esp32 3.0.5 (Version 2.0.x works with older version [id3esp32obd2 v2.0.0](https://github.com/codingABI/id3esp32obd2/releases/tag/v2.0.0))

### OBD2 and power supply
The VW ID.3 has an ODB2 female connector below the steering wheel:

![ID3 OBD2 Connector](assets/images/id3obd2ConnectorFemale.jpg)

The *id3esp32obd2* uses a male OBD2 connector and pin 16 (=12V), pin 5 (=GND) to power up the ESP32 (Converted down to 3.3V by a LM2596). Pin 6 (=CAN High) and 14 (=CAN Low) are used to access the CAN bus. 

![OBD2 connector male](assets/images/obd2ConnectorMale.jpg)

The 12V line can be switch on and off with the toggle switch S1.
### Device description
An ESP32 has a builtin CAN controller, but needs an additional CAN transceiver. The ESP32 based *id3esp32obd2* uses a SN65HVD230 CAN transceiver. 

![SN65HVD230 removed resistor](assets/images/SN65HVD230.jpg)

The red marked 60 Ohm resistor R2 between CANHigh and CANLow was removed. 

The ESP32 waits for the *"allowed"* Android device connecting via Bluetooth (The *"allowed"* Android device can be defined in  [secrets.h](/id3esp32obd2/secrets.h)). After connecting with the *"allowed"* Android device the ESP32 requests CAN data and forwards the response via Bluetooth. While the *allowed* Android device is connected the blue, ESP32 builtin led (Pin 2) is enabled. When the Android device disconnects the Bluetooth connection the CAN requests will be stopped.

**WARNING**
Do not lock the car without disconnecting the Bluetooth connection or power off the device by the toggle switch S1, because the VW ID.3 may triggers a car alarm, if CAN requests are received in a locked state. 

The *id3esp32obd2* waits 500ms between each CAN bus request, because I got weired responses when sending requests too quickly.

All components are soldered on two pieces perfboard (I had no single perfboard big enough) and the ESP32 is stacked over the SN65HVD230 and the buzzer.

![Perfboard](assets/images/Perfboard.jpg)

![Perfboard backside](assets/images/PerfboardBackside.jpg)

![Device without case](assets/images/Device.jpg)

The toggle switch S1 to power on/off the device in mounted on a wooden, black painted clothespeg and can be sticked on a the ventilation grille in the car. The unused pins of the OBD2 cable are protected by heat shrinking tubes.

![Device with open case](assets/images/DeviceWithOpenCase.jpg)

The *id3esp32obd2* is in a wooden self-made case.

![Device with case](assets/images/DeviceWithCase.jpg)

The window in the case is to show the builtin leds (Blue led on pin 2 and red power led) of the ESP32.
### Code
The code for the ESP32 was written with the Arduino IDE and can be found in the [Arduino-Sketch folder](/id3esp32obd2)

### Schematic
![Schematic](assets/images/Schematic.png)

The voltage divider R1/R2 is used to measure the 12V car battery voltage with an ESP32 analog pin.

### Android App
To show the VW ID.3 diagnostics data sent by the device via Bluetooth an Android device with the app [id3esp32obd2 app](id3esp32obd2.apk) is needed. This app is a small app made with https://appinventor.mit.edu/ (Source code: [App source code](id3esp32obd2.aia)) for this project and was tested on a Samsung Galaxy S10 5G with Android 12 and Samsung Galaxy S23 with Android 13 and 14. To install the apk file you have to temporary allow the installation (Samsung: Settings->Apps->Top right dots...->Special access->Install unknown app).

#### Bluetooth pairing and selecting the device
Before you can use the app, you have to pair the Android device with the *id3esp32obd2* device listed as *"id3esp32obd2"* in your Android Bluetooth management.

After launching the app, you have to connect to the previously paired Bluetooth device by pressing 🔗 to open the list "Bluetooth device" and select *"aa:bb... id3esp32obd2"* (*aa:bb...* would be the MAC address of your ESP32). To manually disconnect an existing connection you can select *"Disconnect"* in the list "Bluetooth device". Next time you start the app again, the connection to the last selected Bluetooth device will be restored automatically.

#### General usage
In the app you can switch between several dashboards with the left ⇦ and right ⇨ buttons. When you move to another dashboard it could take up to 1.5 seconds to receive first data for the new dashboard and it could take several seconds to receive all dashboard data. When you receive *"ERR"* all the time, the device could not get the requested data from the CAN bus. In this case you should check the Serial output of the ESP32 for more details.

#### Titlebar
![Title bar](assets/images/appTitleBar.jpg)

In the title bar you can change the selected *id3esp32obd2* Bluetooth device with 🔗, see the Bluetooth connection status *"Connected"* or *"Disconnected"*, reset the ESP32 with ♺ or open the [info screen](#info-screen) with with ⚙

#### Dashboard: Base
![Dashboard 0](assets/images/appDash0.jpg)

The "12V battery" value is measured by the ESP32 directly and the "Compile time" shows the firmware compile time for the ESP32.
#### Dashboard: Speed
![Dashboard 1](assets/images/appDash1.jpg)

Shows the current car speed
#### Dashboard: HV Battery
![Dashboard 2](assets/images/appDash2.jpg)

Values mainly for the high voltage HV battery.

#### Dashboard: Charge/Discharge
![Dashboard 3](assets/images/appDash3.jpg)

Mixed dashboard to collect data for power consumption. You can reset the delta/differential values with ♺ right of the value.

#### Dashboard: GPS
![Dashboard 4](assets/images/appDash4.jpg)

Values from the car GPS. The GPS time will be used as ESP32 device time. 

#### Info screen
![App info screen](assets/images/appInfoScreen.jpg)
