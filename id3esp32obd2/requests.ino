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
    g_SerialBT.write((byte*)strData,strlen(strData)+1);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idCARMODE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idSPEED);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idDRIVINGMODE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1);  
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idAUXPOWER);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idBATTEMPERATURE);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCIRCULATIONPUMP);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCHARGINGLIMIT);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
}

/* HV total charge/discharge
 *  
 * Example:
 * 21:59:19.931 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 32 00 00 00 00 
 * 21:59:19.978 -> Receive ID: 17FE007B Flags:1 Length:8 Data:10 13 62 1E 32 00 15 C8 
 * 21:59:19.978 -> Multi frame with size 19
 * 21:59:19.978 -> Send ID: 17FC007B Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVCAPACITY);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
}

/* VIN number of car (from engine ECU)
 * 
 * Example:
 * 11:55:44.316 -> Send    ID: 17FC0076 Flags:1 Length:8 Data:03 22 F8 02 00 00 00 00 
 * 11:55:44.316 -> Receive ID: 17FE0076 Flags:1 Length:8 Data:10 14 62 F8 02 57 56 57 
 * 11:55:44.316 -> Multi frame with size 20
 * 11:55:44.316 -> Send ID: 17FC0076 Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
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
      g_SerialBT.write((byte*)strData,strlen(strData)+1); 
      g_VINdone = true;
    } else {
      snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idVIN);
      g_SerialBT.write((byte*)strData,strlen(strData)+1);    
    }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1);
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idSOCBMS);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVBATV);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
}

/* HV battery current
 *  
 * Example:
 * 22:09:13.118 -> Send    ID: 17FC007B Flags:1 Length:8 Data:03 22 1E 3D 00 00 00 00 
 * 22:09:13.165 -> Receive ID: 17FE007B Flags:1 Length:8 Data:10 08 62 1E 3D 00 02 49 
 * 22:09:13.165 -> Multi frame with size 8
 * 22:09:13.165 -> Send ID: 17FC007B Flags:1 Length:8 Data:30 00 00 00 00 00 00 00 
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idHVBATA);
    g_SerialBT.write((byte*)strData,strlen(strData)+1);    
  }
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
    g_SerialBT.write((byte*)strData,strlen(strData)+1); 
  } else {
    snprintf(strData,MAXSTRDATALENGTH+1,"%i|ERR",idODOMETER);
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
