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
