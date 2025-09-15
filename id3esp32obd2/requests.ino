/* ----------- Data requests  ----------
 * License: 2-Clause BSD License
 * Copyright (c) 2023 codingABI
 */

/* Car operation mode
 *  
 * Example:
 * 22:38:35.656 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 74 48 00 00 00 00 
 * 22:38:35.656 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 74 48 00 AA AA AA 
 */
void readAndSendCarMode() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007b,UDS_ReadDataByIdentifier_0x22,0x74,0x48)) {
    Serial.print("Car operation mode: ");
    switch (g_dataBuffer[0]) {
      case 0:
        Serial.println("Standby");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|Standby",idCARMODE);
        break;
      case 1:
        Serial.println("Driving");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|Driving",idCARMODE);
        break;
      case 4:
        Serial.println("AC charging");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|AC charging",idCARMODE);
        break;
      case 6:
        Serial.println("DC charging");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|DC charging",idCARMODE);
        break;
      default:
        Serial.println(g_dataBuffer[0]);
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idCARMODE,g_dataBuffer[0]);
    }
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idCARMODE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Speed
 * 
 * Example:
 * 22:38:34.646 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 F4 0D 00 00 00 00 
 * 22:38:34.646 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 F4 0D 00 AA AA AA 
 */
void readAndSendSpeed() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007b,UDS_ReadDataByIdentifier_0x22,0xF4,0x0D)) {
    Serial.print("Speed in km/h: ");
    Serial.println(g_dataBuffer[0]); 
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idSPEED,g_dataBuffer[0]);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idSPEED);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Driving mode position (P-N-D-B)
 *  
 * Example:
 * 10:54:44.614 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:03 22 21 0E 00 00 00 00 
 * 10:54:44.661 -> Receive ID: 17FE0076 Flags:1 Length:8 Data:05 62 21 0E 00 08 AA AA 
 */
void readAndSendDrivingMode() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17FC0076,UDS_ReadDataByIdentifier_0x22,0x21,0x0E)) {
    Serial.print("Driving mode position: ");
    switch (g_dataBuffer[1]) {
      case 0x8:
        Serial.println("P");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|P",idDRIVINGMODE);
        break;
      case 0x5:
        Serial.println("D");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|D",idDRIVINGMODE);
        break;
      case 0xc:
        Serial.println("B");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|B",idDRIVINGMODE);
        break;
      case 0x7:
        Serial.println("R");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|R",idDRIVINGMODE);
        break;
      case 0x6:
        Serial.println("N");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|N",idDRIVINGMODE);
        break;
      default:
        Serial.println(g_dataBuffer[1]);
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idDRIVINGMODE,g_dataBuffer[1]);
    }
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idDRIVINGMODE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* HV auxilary consumer power
 *  
 * Example:
 * 22:38:36.662 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:03 22 03 64 00 00 00 00 
 * 22:38:36.662 -> Receive ID: 17FE0076 Flags:1 Length:8 Data:05 62 03 64 00 00 AA AA 
 */
void readAndSendHVAuxilaryPower() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17FC0076,UDS_ReadDataByIdentifier_0x22,0x03,0x64)) {
    Serial.print("HV auxilary consumer power in kW: ");
    Serial.println(buffer2unsignedLong()/10.0f);  
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.2f",idAUXPOWER,buffer2unsignedLong()/10.0f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idAUXPOWER);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* HV battery main temperature
 *  
 * Example:
 * 10:58:19.521 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 2A 0B 00 00 00 00 
 * 10:58:19.573 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 2A 0B 74 AA AA AA 
 */
void readAndSendHVMainTemperature() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  
  if (sendUDSRequest(0x17fc007b,UDS_ReadDataByIdentifier_0x22,0x2A,0x0B)) {
    Serial.print("HV battery main temperature in °C: ");
    Serial.println((g_dataBuffer[0]/2.0f) - 40);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idBATTEMPERATURE,(g_dataBuffer[0]/2.0f) - 40);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idBATTEMPERATURE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Circulation pump HV battery
 *
 * Example:
 * 22:03:18.361 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 74 3B 00 00 00 00 
 * 22:03:18.361 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 74 3B 00 AA AA AA 
 */
void readAndSendHVCirculationPump() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  
  if (sendUDSRequest(0x17fc007B,UDS_ReadDataByIdentifier_0x22,0x74,0x3B)) {
    Serial.print("Circulation pump in %: ");
    Serial.println(g_dataBuffer[0]);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idHVCIRCULATIONPUMP,g_dataBuffer[0]);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCIRCULATIONPUMP);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Dynamic limit for charging in ampere
 *  
 * Example:
 * 22:03:19.907 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 1B 00 00 00 00 
 * 22:03:19.907 -> Receive ID: 17FE007B Flags:1 Length:8 Data:05 62 1E 1B 08 4D AA AA 
 */
void readAndSendHVDynamicChargeLimit() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007B,UDS_ReadDataByIdentifier_0x22,0x1E,0x1B)) {
    Serial.print("Dynamic limit for charging in A: ");
    Serial.println(buffer2unsignedLong()/5.0f);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVCHARGINGLIMIT,buffer2unsignedLong()/5.0f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCHARGINGLIMIT);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* HV total charge/discharge
 *  
 * Example:
 * 21:59:19.931 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 32 00 00 00 00 
 * 21:59:19.978 -> Receive ID: 17FE007B Flags:1 Length:8 Data:10 13 62 1E 32 00 15 C8 
 * 21:59:19.978 -> Multi frame with size 19
 * 21:59:19.978 -> Send    ID: 17FC007B Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
 * 21:59:19.978 -> Expect 2 frames
 * 21:59:19.978 -> Next receive ID: 17FE007B Flags:1 Length:8 Data:21 C1 FF EA D9 E6 00 7C 
 * 21:59:19.978 -> Next receive ID: 17FE007B Flags:1 Length:8 Data:22 33 68 FF 8A 00 1B AA 
 */
void readAndSendHVTotalChargeDischarge() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007B,UDS_ReadDataByIdentifier_0x22,0x1E,0x32)) {
    Serial.print("Total charge/discharge : ");
    Serial.print(buffer2unsignedLong(8,4)/8583.07f);
    Serial.print(" ");
    Serial.println(fabs((long) buffer2unsignedLong(12,4)/8583.07f));
    
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVTOTALCHARGE,buffer2unsignedLong(8,4)/8583.07f);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVTOTALDISCHARGE,fabs((long) buffer2unsignedLong(12,4)/8583.07f));
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVTOTALCHARGE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVTOTALDISCHARGE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);     
  }
}

/* HV battery capacity
 *  
 * Example:
 * 17:23:11.247 -> Send    ID: 710 Flags:0 Length:8 Data:03 22 2A B2 00 00 00 00 
 * 17:23:11.247 -> Receive ID: 77A Flags:0 Length:8 Data:07 62 2A B2 06 15 04 00 
 */
void readAndSendHVCapacity() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x710,UDS_ReadDataByIdentifier_0x22,0x2A,0xB2)) {
    Serial.print("HV battery capacity in kWh: ");
    Serial.println(buffer2unsignedLong()/1310.77f/1000.0f);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVCAPACITY,buffer2unsignedLong()/1310.77f/1000.0f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCAPACITY);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* VIN number of car (from engine ECU)
 * 
 * Example:
 * 11:55:44.316 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:03 22 F8 02 00 00 00 00 
 * 11:55:44.316 -> Receive ID: 17FE0076 Flags:1 Length:8 Data:10 14 62 F8 02 57 56 57 
 * 11:55:44.316 -> Multi frame with size 20
 * 11:55:44.316 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
 * 11:55:44.316 -> Expect 2 frames
 * 11:55:44.316 -> Next receive ID: 17FE0076 Flags:1 Length:8 Data:22 5A 5A 5A ... 
 * 11:55:44.316 -> Next receive ID: 17FE0076 Flags:1 Length:8 Data:29 50 32 33 ...
 */
void readAndSendVIN() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (!g_VINdone) { // Only once per bluetooth sessions
    if (sendUDSRequest(0x17FC0076,UDS_ReadDataByIdentifier_0x22,0xF8,0x02)) {
      Serial.print("VIN: ");
      g_dataBuffer[MAXBUFFER-1]=0; // Make sure that strings terminate
      Serial.println((char*)g_dataBuffer);
      snprintf(strData,MAXSTRDATALENGTH+1,"%i|%s",idVIN,(char*)g_dataBuffer);
      g_VINdone = true;
    } else {
      snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idVIN);
    }
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
}

/* SOC (BMS)
 *  
 * Example:
 * 22:38:33.639 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 02 8C 00 00 00 00 
 * 22:38:33.639 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 02 8C 9A AA AA AA 
 */
void readAndSendSOCBMS() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  
  if (sendUDSRequest(0x17fc007b,UDS_ReadDataByIdentifier_0x22,0x02,0x8c)) {
    Serial.print("SOC (BMS) in %: ");
    Serial.println(g_dataBuffer[0]/2.5f);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idSOCBMS,g_dataBuffer[0]/2.5f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idSOCBMS);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* HV battery voltage
 *  
 * Example:
 * 22:03:18.876 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 3B 00 00 00 00 
 * 22:03:18.876 -> Receive ID: 17FE007B Flags:1 Length:8 Data:05 62 1E 3B 05 A6 AA AA 
 */
void readAndSendHVBatteryV() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007B,UDS_ReadDataByIdentifier_0x22,0x1E,0x3B)) {
    Serial.print("HV battery voltage in V: ");
    Serial.println(buffer2unsignedLong()/4.0f);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVBATV,buffer2unsignedLong()/4.0f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVBATV);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* HV battery current
 *  
 * Example:
 * 22:09:13.118 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 3D 00 00 00 00 
 * 22:09:13.165 -> Receive ID: 17FE007B Flags:1 Length:8 Data:10 08 62 1E 3D 00 02 49 
 * 22:09:13.165 -> Multi frame with size 8
 * 22:09:13.165 -> Send    ID: 17FC007B Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
 * 22:09:13.165 -> Expect 1 frames
 * 22:09:13.165 -> Next receive ID: 17FE007B Flags:1 Length:8 Data:21 C8 3F AA AA AA AA AA 
 */
void readAndSendHVBatteryA() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17fc007B,UDS_ReadDataByIdentifier_0x22,0x1E,0x3D)) {
    Serial.print("HV battery current in A: ");
    Serial.println((buffer2unsignedLong(0,4)- 150000)/100.0f);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idHVBATA,((long)buffer2unsignedLong(0,4)- 150000)/100.0f);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVBATA);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* ODOMETER
 *  
 * Example:
 * 10:49:02.544 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:03 22 29 5A 00 00 00 00 
 * 10:49:02.544 -> Receive ID: 17FE0076 Flags:1 Length:8 Data:06 62 29 5A 00 11 D1 AA 
 */
void readAndSendODOMETER() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17FC0076,UDS_ReadDataByIdentifier_0x22,0x29,0x5A)) {
    Serial.print("ODOMETER in km: ");
    Serial.println(buffer2unsignedLong());   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idODOMETER,buffer2unsignedLong());
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idODOMETER);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* PTC heater battery current
 *  
 * Example:
 * 17:56:08.165 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 16 20 00 00 00 00 
 * 17:56:08.165 -> Receive ID: 17FE007B Flags:1 Length:8 Data:04 62 16 20 00 AA AA AA 
 */
void readAndSendPTCHeaterCurrent() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x17FC007B,UDS_ReadDataByIdentifier_0x22,0x16,0x20)) {
    Serial.print("PTC heater battery current in A: ");
    Serial.println(buffer2unsignedLong());   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idPTCHEATERCURRENT,buffer2unsignedLong());
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idPTCHEATERCURRENT);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Outside temperature
 *  
 * Example:
 * 17:56:07.649 -> Send    ID: 746 Flags:0 Length:8 Data:03 22 26 09 00 00 00 00 
 * 17:56:07.696 -> Receive ID: 7B0 Flags:0 Length:8 Data:04 62 26 09 A6 AA AA AA 
 */
void readAndSendTemperatureOutside() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x746,UDS_ReadDataByIdentifier_0x22,0x26,0x09)) {
    Serial.print("Outside temperature in °C: ");
    Serial.println((g_dataBuffer[0]/2.0f) - 50);   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idTEMPERATUREOUTSIDE,(g_dataBuffer[0]/2.0f) - 50);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idTEMPERATUREOUTSIDE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* Inside temperature
 *  
 * Example:
 * 17:56:07.133 -> Send    ID: 746 Flags:0 Length:8 Data:03 22 26 13 00 00 00 00 
 * 17:56:07.180 -> Receive ID: 7B0 Flags:0 Length:8 Data:05 62 26 13 00 C8 AA AA 
 */
void readAndSendTemperatureInside() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x746,UDS_ReadDataByIdentifier_0x22,0x26,0x13)) {
    Serial.print("Inside temperature in °C: ");
    Serial.println((buffer2unsignedLong()/5.0f)-40);   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idTEMPERATUREINSIDE,((long)buffer2unsignedLong()/5.0f)-40);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idTEMPERATUREINSIDE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1);    
}

/* Cruising range
 *  
 * Example:
 * 17:48:32.688 -> Send    ID: 710 Flags:0 Length:8 Data:03 22 2A B6 00 00 00 00 
 * 17:48:32.688 -> Receive ID: 77A Flags:0 Length:8 Data:10 0A 62 2A B6 00 B2 00 
 * 17:48:32.688 -> Multi frame with size 10
 * 17:48:32.688 -> Send    ID: 710 Flags:0 Length:8 Data:30 00 00 00 00 00 00 00 
 * 17:48:32.688 -> Expect 1 frames
 * 17:48:32.688 -> Next receive ID: 77A Flags:0 Length:8 Data:21 B2 00 00 00 AA AA AA 
 */
void readAndSendCruisingRange() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x710,UDS_ReadDataByIdentifier_0x22,0x2A,0xB6)) {
    Serial.print("Cruising range in km: ");
    Serial.println(buffer2unsignedLong(0,2));   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idCRUISINGRANGE,buffer2unsignedLong(0,2));
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idCRUISINGRANGE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* Charging state
 *  
 * Example:
 * 17:48:23.031 -> Send    ID: 710 Flags:0 Length:8 Data:03 22 15 D5 00 00 00 00 
 * 17:48:23.078 -> Receive ID: 77A Flags:0 Length:8 Data:06 62 15 D5 02 00 00 AA 
 */
void readAndSendChargingState() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x710,UDS_ReadDataByIdentifier_0x22,0x15,0xD5)) {
    Serial.print("Charing state: ");
    switch (g_dataBuffer[0]) {
      case 0x1:
        Serial.println("Precondition fulfilled");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|Standby charging",idCHARGINGSTATE);
        break;
      case 0x2:
        Serial.println("None");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|None",idCHARGINGSTATE);
        break;
      case 0x3:
        Serial.println("Error");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|Error",idCHARGINGSTATE);
        break;
      case 0x5:
        Serial.println("End charging");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|End charging",idCHARGINGSTATE);
        break;
      case 0x16:
        Serial.println("AC");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|AC",idCHARGINGSTATE);
        break;
      case 0x18:
        Serial.println("Trickle charge");
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|Trickle charge",idCHARGINGSTATE);
        break;
      default:
        Serial.println(g_dataBuffer[1]);
        snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idCHARGINGSTATE,g_dataBuffer[1]);
    }
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idCHARGINGSTATE);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}


/* CO2 content interior
 *  
 * Example:
 * 17:56:08.681 -> Send    ID: 746 Flags:0 Length:8 Data:03 22 42 DB 00 00 00 00 
 * 17:56:08.681 -> Receive ID: 7B0 Flags:0 Length:8 Data:05 62 42 DB 00 05 AA AA 
 */
void readAndSendCO2contentInterior() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  if (sendUDSRequest(0x746,UDS_ReadDataByIdentifier_0x22,0x42,0xDB)) {
    Serial.print("CO2 content interior in ppm: ");
    Serial.println(buffer2unsignedLong()*100);   
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i",idCO2CONTENTINTERIOR,buffer2unsignedLong()*100);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idCO2CONTENTINTERIOR);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* GPS-Time
 *  
 * Example:
 * 17:48:31.610 -> Send    ID: 767 Flags:0 Length:8 Data:03 22 22 B3 00 00 00 00 
 * 17:48:31.657 -> Receive ID: 7D1 Flags:0 Length:8 Data:10 09 62 22 B3 01 00 5D 
 * 17:48:31.657 -> Multi frame with size 9
 * 17:48:31.657 -> Send    ID: 767 Flags:0 Length:8 Data:30 00 00 00 00 00 00 00 
 * 17:48:31.657 -> Expect 1 frames
 * 17:48:31.657 -> Next receive ID: 7D1 Flags:0 Length:8 Data:21 D6 FC 1F AA AA AA AA 
 */
void readAndSendGPSTime() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  char strTime[MAXSTRDATALENGTH+1];
  char strHex[4];
  
  if (sendUDSRequest(0x767,UDS_ReadDataByIdentifier_0x22,0x22,0xB3)) {
    Serial.print("GPS time: ");
    strData[0] ='\0';
    for (int i=0;i<g_dataBufferLength;i++) {
      if (i*3 < MAXSTRDATALENGTH) {
        snprintf(strHex,MAXSTRDATALENGTH+1,"%02X ",g_dataBuffer[i]);
        strncat(strData,strHex,3);
      }
    }
    Serial.println(strData);
    unsigned long timeValue = buffer2unsignedLong(2,4);

    snprintf(strTime,MAXSTRDATALENGTH+1,"%04i-%02i-%02i %02i:%02i:%02i",
      ((timeValue >> 26) & 0b11111)+2000,
      (timeValue >> 22) & 0b1111,
      (timeValue >> 17) & 0b11111,
      (timeValue >> 12) & 0b11111,
      (timeValue >> 6) & 0b111111, 
      timeValue & 0b111111);

    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%s", idGPSTIME,strTime);

    // Set time for ESP32 to GPS time
    if (((timeValue >> 26) & 0b11111)+2000 > 2022) {
      setEspTime(strTime);
    }
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idGPSTIME);
  }
  g_SerialBT.write((byte*)strData,strlen(strData)+1); 
}

/* GPS data (lat/lon/ele)
 *  
 * Example:
 * 12:15:25.110 -> Send    ID: 767 Flags:0 Length:8 Data:03 22 24 30 00 00 00 00 
 * 12:15:25.156 -> Receive ID: 7D1 Flags:0 Length:8 Data:10 21 62 24 30 31 31 B0 
 * 12:15:25.156 -> Multi frame with size 33
 * 12:15:25.156 -> Send    ID: 767 Flags:0 Length:8 Data:30 00 00 00 00 00 00 00 
 * 12:15:25.156 -> Expect 4 frames
 * 12:15:25.156 -> Next receive ID: 7D1 Flags:0 Length:8 Data:21 31 31 27 34 30 2E 37 
 * 12:15:25.203 -> Next receive ID: 7D1 Flags:0 Length:8 Data:22 22 45 00 00 34 38 B0 
 * 12:15:25.203 -> Next receive ID: 7D1 Flags:0 Length:8 Data:23 33 37 27 31 2E 34 22 
 * 12:15:25.203 -> Next receive ID: 7D1 Flags:0 Length:8 Data:24 4E 00 00 00 03 EB AA 
 */
void readAndSendGPSData() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  char cToStr[2];

  #define DELTA 503
  /*
   * Byte 28 and 29 of the UDS response seem to be the gps elevation with a delta of about ~503. 
   * The delta was calculated by comparing the UDS response values with some official altitude values in Bavaria/Germany. 
   * Perhaps this delta is dependent on the location, ID.3 firmware ... Feel free to change the DELTA, if the delta does not fit for you
   * Some additional details can be found at https://github.com/codingABI/id3esp32obd2/issues/1
   * 
   * UDS raw  | Official/real  | Delta
   * values   | altitude in m  |
   * ---------+----------------+--------------
   * 971      | 467            | 504
   * 951      | 449            | 502
   * 1221     | 715            | 506
   * 1223     | 718            | 505
   * 979      | 472            | 507
   * 967      | 460            | 507
   * 1015     | 513            | 502
   * 1175     | 673            | 502
   * 1259     | 757            | 502
   * 963      | 459            | 504
   * 1291     | 787            | 504
   * 1497     | 998            | 499
   * 1901     | 1400           | 501
   * 1915     | 1413           | 502
   * 1565     | 1065           | 500
   * => Average Delta 503
   */
  cToStr[1] = '\0';
  if (sendUDSRequest(0x767,UDS_ReadDataByIdentifier_0x22,0x24,0x30)) {
    // elevation in meters
    Serial.print("GPS ele:");
    Serial.println(buffer2unsignedLong(28,2)-DELTA);
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|%i", idGPSELE,buffer2unsignedLong(28,2)-DELTA);    
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 

    // longitude in DDD°MM'SS.S"
    Serial.print("GPS lon:");
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|", idGPSLON);
    if (g_dataBufferLength >= 14) {
      for (int i=0;i<14;i++){
        if (g_dataBuffer[i]==0) break;
        Serial.print(char(g_dataBuffer[i]));
        cToStr[0]=char(g_dataBuffer[i]);
        strncat(strData,cToStr,1);
      }
    }
    Serial.println();
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 

    // latitude in DDD°MM'SS.S"
    Serial.print("GPS lat:");
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|", idGPSLAT);
    if (g_dataBufferLength >= 28) {
      for (int i=14;i<28;i++){
        if (g_dataBuffer[i]==0) break;
        Serial.print(char(g_dataBuffer[i]));
        cToStr[0]=char(g_dataBuffer[i]);
        strncat(strData,cToStr,1);
      }
    }
    Serial.println();
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idGPSELE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idGPSLAT);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idGPSLON);
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  }
}

// Voltage for 12V battery from analog pin
void readAndSendBAT12V() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  snprintf(strData,MAXSTRDATALENGTH+1,"%i|%.1f",idBAT12V,map(analogRead(MEASURE12V_PIN),0,4095,0,1990)/100.0f);
  g_SerialBT.write((byte*)strData,strlen(strData)+1);
}

// Compile time
void readAndSendCompileTime() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  
  snprintf(strData,MAXSTRDATALENGTH+1,"%i|%s %s",idCOMPILETIME,__DATE__,__TIME__);
  g_SerialBT.write((byte*)strData,strlen(strData)+1);  
}
