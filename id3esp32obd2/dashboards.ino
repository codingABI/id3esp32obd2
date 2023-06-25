/* ----------- Dashboards  ----------
 * License: 2-Clause BSD License
 * Copyright (c) 2023 codingABI
 */

// First/starting dashboard
void requestDefaultDashboard(byte boardID) {
  // Driving mode position
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendDrivingMode();

  // Car operation mode
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendCarMode();

  // ODOMETER
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendODOMETER();

  // VIN
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendVIN();

  // Voltage for 12V battery from analog pin
  readAndSendBAT12V();

  // Compile time
  readAndSendCompileTime();
}

// Speed dashboard
void requestSpeedDashboard() {
  // Speed
  checkBTReceive();
  readAndSendSpeed();
}

// HV dashboard
void requestHVDashboard(byte boardID) {
  // SOC (BMS)
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendSOCBMS();

  // HV auxilary consumer power
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVAuxilaryPower();

  // HV Battery main temperature
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVMainTemperature();

  // Circulation pump HV battery
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVCirculationPump();

  // HV Battery voltage
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVBatteryV();

  // HV Battery current
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVBatteryA();

  // HV Dynamic limit for charging
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVDynamicChargeLimit();

  // HV total charge/discharge
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVTotalChargeDischarge();

  // HV capacity
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVCapacity();
}

// Custom dashboard
void requestMyDashboard(byte boardID) {
  // VIN
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendVIN();

  // SOC (BMS)
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendSOCBMS();

  // Car operation mode
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendCarMode();

  // HV battery voltage
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVBatteryV();

  // HV battery current
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVBatteryA();

  // ODOMETER
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendODOMETER();

  // HV total charge/discharge
  checkBTReceive();
  if (boardID != g_requestDashboard) return; // Bluetooth client changed dashboard
  readAndSendHVTotalChargeDischarge();
}

// Only for testing
void requestDashboardTest() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  checkBTReceive();
  if (sendUDSRequest(0x7A0,UDS_ReadDataByIdentifier_0x22,0xC0,0x0B)) {
    /* Speed
     * 
     * Example:
     * 22:38:34.646 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 F4 0D 00 00 00 00 
     * 22:38:34.646 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 F4 0D 00 AA AA AA 
     */
    Serial.print("Test : ");
   // Serial.println(g_dataBuffer[0]);
    Serial.println(((g_dataBuffer[8]<<24)+(g_dataBuffer[9]<<16)+(g_dataBuffer[10]<<8)+g_dataBuffer[11])/8583.07123641215);
    Serial.println(((g_dataBuffer[12]<<24)+(g_dataBuffer[13]<<16)+(g_dataBuffer[14]<<8)+g_dataBuffer[15])/8583.07123641215);

    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idSPEED,(g_dataBuffer[0]<<8) +g_dataBuffer[1]);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idSPEED);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
}
