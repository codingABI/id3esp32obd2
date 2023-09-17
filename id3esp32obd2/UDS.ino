/* ----------- Stuff to create UDS Unified Diagnostic Services requests ----------
 * License: 2-Clause BSD License
 * Copyright (c) 2023 codingABI
 */
 
// Convert databuffer to usigned long
unsigned long buffer2unsignedLong(int startByte=0,int bytesCount=0) {
  unsigned long result = 0;
  int maxBytes;

  if (startByte >= g_dataBufferLength) {
    Serial.print("Start byte out of range ");
    Serial.print(startByte);
    Serial.print(">=");
    Serial.println(g_dataBufferLength);
    return 0;
  }
  if ((bytesCount > 0) && (startByte + bytesCount > g_dataBufferLength)) {
    Serial.print("Start byte and lenght out of range ");
    Serial.print(startByte);
    Serial.print("+");    
    Serial.print(bytesCount);
    Serial.print(">");
    Serial.println(g_dataBufferLength);
    return 0;
  }
  
  if (bytesCount == 0) maxBytes = g_dataBufferLength; else maxBytes = bytesCount;
  for (int i=0;i<maxBytes;i++) {
    result += (g_dataBuffer[i+startByte]<<((maxBytes-i-1)*8));
  }
  return result;
}

// Clear receive buffer
void clearBuffer() {
  g_dataBufferLength = 0;
  for (int i=0;i<MAXBUFFER;i++) {
    g_dataBuffer[i] = 0x00;
  }
}

// Show contens of received data buffer
void showBuffer() {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];

  Serial.print("Databuffer: ");
  for (int i=0;i<g_dataBufferLength;i++) {
    snprintf(strData,MAXSTRDATALENGTH+1,"%02X",g_dataBuffer[i]);
    Serial.print(strData);
    if (i < g_dataBufferLength-1) Serial.print(" ");
  }
  Serial.println();
}

// Show content of a CAN message
void showMessage(can_message_t *message) {
  #define MAXSTRDATALENGTH 80
  char strData[MAXSTRDATALENGTH+1];
  
  Serial.print("ID: ");
  Serial.print(message->identifier,HEX);
  Serial.print(" Flags:");
  Serial.print(message->flags);
  Serial.print(" Length:");
  Serial.print(message->data_length_code);

  if (!(message->flags & CAN_MSG_FLAG_RTR)) {
    Serial.print(" Data:");
    for (int i=0; i < message->data_length_code;i++) {
      snprintf(strData,MAXSTRDATALENGTH+1,"%02X",message->data[i]);
      Serial.print(strData);
      if (i < message->data_length_code) Serial.print(" ");
    }
  } 
}

// Swap first two bytes to swap source and target address for a 29 bit ISO-TP in "normal fixed addressing"
unsigned long swapIsoTpSourceTarget(unsigned long canID) {
  return (canID & 0xffff0000) + ((canID & 0xff) << 8) + ((canID & 0xff00) >> 8);
}

// Send a UDS request and write response to the receive buffer
bool sendUDSRequest(unsigned long canID, byte serviceID, byte parameterIDHighByte, byte parameterIDLowByte) {
  can_message_t txMessage, rxMessage, fcMessage;
  unsigned long lastSentMS;
  byte flags = 0;
  esp_err_t error;

  if (canID > 2047) flags = CAN_MSG_FLAG_EXTD; // 29 bit ID needed?
  
  // Clear receive buffer before requesting new data
  clearBuffer();

  delay(500); // Without delay my receivings were behind time
  
  txMessage.identifier = canID;
  txMessage.flags = flags;
  txMessage.data_length_code = 8; // Length of CAN packet
  txMessage.data[0] = 0x03; // Length of payload in CAN packet
  txMessage.data[1] = serviceID;
  txMessage.data[2] = parameterIDHighByte;
  txMessage.data[3] = parameterIDLowByte;
  txMessage.data[4] = 0x00; // Padding
  txMessage.data[5] = 0x00; // Padding
  txMessage.data[6] = 0x00; // Padding
  txMessage.data[7] = 0x00; // Padding
  Serial.print("Send    ");
  showMessage(&txMessage);  
  Serial.println();
  error = can_transmit(&txMessage, pdMS_TO_TICKS(CAN_SEND_TIMEOUT));
  if (error != ESP_OK) {
    Serial.print("Send error ");
    Serial.println(esp_err_to_name(error));
    return false; // Abort
  }
  lastSentMS = millis();

  do { // Receive frames until correct response, timeout or failure
    long remainingTimeoutMS;
    remainingTimeoutMS = CAN_RECEIVE_TIMEOUT-(millis()-lastSentMS);
    if (remainingTimeoutMS <= 0) {
      Serial.println("Timeout receiving message");
      return false; // Abort
    }
    error = can_receive(&rxMessage, pdMS_TO_TICKS(remainingTimeoutMS));
    if (error != ESP_OK) {
      Serial.print("Receive error ");
      Serial.println(esp_err_to_name(error));
      return false; // Abort
    }
    Serial.print("Receive ");
    showMessage(&rxMessage);
    Serial.println();
    if (rxMessage.data_length_code != 8) { // At the moment I expect always 8 bytes per frame
      Serial.print("Wrong frame lenght ");
      Serial.println(rxMessage.data_length_code); 
      continue; // Ignore frame and receive again    
    }
    if (((rxMessage.data[0]>>4) != ISOTP_singleFrame_0x0) && ((rxMessage.data[0]>>4) != ISOTP_firstFrame_0x1)) {
      Serial.print("Wrong frame type ");
      Serial.println((rxMessage.data[0]>>4));
      continue; // Ignore frame and receive again
    }

    // Single frame
    if ((rxMessage.data[0]>>4) == ISOTP_singleFrame_0x0) {
      if (rxMessage.data_length_code < 4) { // At least 4 bytes for a single frame needed
        Serial.print("Frame length to short ");
        Serial.println(rxMessage.data_length_code); 
        continue; // Ignore frame and receive again    
      }
      if (rxMessage.data[0] < 3) { // At least 3 bytes of payload needed
        Serial.println("Message too short");
        continue; // Ignore frame and receive again
      }
      if (rxMessage.data[1] == UDS_NegativeResponse_0x7f) {
        if (rxMessage.data[2] == txMessage.data[1]) { // Error seems to be related to my request
          Serial.print("Error received ");     
          switch(rxMessage.data[3]) {
            case UDS_RequestOutOfRange_0x31:
              Serial.println("0x31 request out of range"); 
              break;
            case UDS_ServiceNotSupported_0x11:
              Serial.println("0x11 service not supported");
              break;
            case UDS_ResponsePending_0x78:
              Serial.println("0x78 request correctly received but response pending");
              continue; // No error, response should be received later
              break;
            default:
              Serial.println(rxMessage.data[3],HEX); 
          }
          return false; // Abort
        } else continue; // Ignore and receive again, if error was not related to my request
      }
      if (rxMessage.data[1] != txMessage.data[1] + UDS_PositiveResponseOffset_0x40) {
        Serial.println("Service ID does not match to sent service ID");
        continue; // Ignore frame and receive again
      }
      if (rxMessage.data[2] != txMessage.data[2]) {
        Serial.println("Command does not match to sent command");
        continue; // Ignore frame and receive again
      }
      if (rxMessage.data[3] != txMessage.data[3]) {
        Serial.println("Command does not match to sent command");
        continue; // Ignore frame and receive again
      }

      g_dataBufferLength = rxMessage.data[0]-3;
      for (int i=0;i<g_dataBufferLength;i++) {
          g_dataBuffer[i] = rxMessage.data[4+i];
      }
      showBuffer();
      return true; // Receive message successfully parsed
    }

    // Multi frame
    if ((rxMessage.data[0]>>4) == ISOTP_firstFrame_0x1) {
      if (rxMessage.data_length_code < 5) { // At least 5 bytes for a multi frame needed
        Serial.print("Frame length to short ");
        Serial.println(rxMessage.data_length_code); 
        continue; // Ignore frame and receive again    
      }

      int frameSize = ((rxMessage.data[0] & 0xf) << 8) + rxMessage.data[1];
      g_dataBufferLength = 0;
      Serial.print("Multi frame with size ");
      Serial.println(frameSize);
      
      if (rxMessage.data[2] != txMessage.data[1] + 0x40) {
        Serial.println("Service ID does not match to sent service ID");
        continue; // Ignore frame and receive again
      }
      if (rxMessage.data[3] != txMessage.data[2]) {
        Serial.println("Command does not match to sent command");
        continue; // Ignore frame and receive again
      }
      if (rxMessage.data[4] != txMessage.data[3]) {
        Serial.println("Command does not match to sent command");
        continue; // Ignore frame and receive again
      }

      for (int j=0;j<3;j++) { // Only maximal three byte for data in first frame
        if (g_dataBufferLength >= frameSize - 3) break;
        g_dataBuffer[g_dataBufferLength] = rxMessage.data[5+j];
        g_dataBufferLength++;
      }
      if (rxMessage.identifier & 0xFFFF0000== 0x18DB0000) { // functional CAN id
        // Swap first two bytes to swap source and target address (A flow control frame can not be a functional requests, a.k. broadcast)
        fcMessage.identifier = swapIsoTpSourceTarget(rxMessage.identifier);
      } else {
        fcMessage.identifier = txMessage.identifier;
      }
      fcMessage.flags = flags;
      fcMessage.data_length_code = 8; // Length of CAN packet
      // Request all frames without delay 0x3000000000000000
      fcMessage.data[0] = (ISOTP_flowControlFrame_0x3 << 4);
      fcMessage.data[1] = 0x00;
      fcMessage.data[2] = 0x00;
      fcMessage.data[3] = 0x00;
      fcMessage.data[4] = 0x00;
      fcMessage.data[5] = 0x00;
      fcMessage.data[6] = 0x00;
      fcMessage.data[7] = 0x00;  
      Serial.print("Send    ");
      showMessage(&fcMessage);  
      Serial.println();
      error = can_transmit(&fcMessage, pdMS_TO_TICKS(CAN_SEND_TIMEOUT));
      if (error != ESP_OK) {
        Serial.print("Send error ");
        Serial.println(esp_err_to_name(error));
        return false; // Abort
      }

      // First frame has 6 byte payload and consecutive frames 7 bytes 
      int frames = (frameSize)/7;
      Serial.print("Expect ");
      Serial.print(frames);
      Serial.println(" frames");

      unsigned long originIdentifier = rxMessage.identifier;
      while (g_dataBufferLength < frameSize - 3) {
        error = can_receive(&rxMessage, pdMS_TO_TICKS(CAN_RECEIVE_TIMEOUT));
        if (error != ESP_OK) {
          Serial.print("Receive error ");
          Serial.println(esp_err_to_name(error));
          return false; // Abort
        }
        Serial.print("Next receive ");
        showMessage(&rxMessage);
        Serial.println();
        if ((rxMessage.data[0]>>4) != ISOTP_consecutiveFrame_0x2) {
          Serial.print("Wrong frame type ");
          Serial.println((rxMessage.data[0]>>4));
          continue; // Ignore frame
        }
        if (originIdentifier != rxMessage.identifier) {
          Serial.println("Wrong identifier");
          continue; // Ignore frame
        }

        for (int j=0;j<rxMessage.data_length_code-1;j++) { // Maximal seven byte for data in a consecutive frame
          if (g_dataBufferLength >= frameSize - 3) break;
          g_dataBuffer[g_dataBufferLength] = rxMessage.data[1+j];
          if (g_dataBufferLength >= MAXBUFFER) {
            Serial.println("Data sequence too long"); // Should/Must not happen
          } else g_dataBufferLength++;
        }
      }
      showBuffer();
      return true; // Receive message successfully parsed
    }
  } while (millis()-lastSentMS < CAN_RECEIVE_TIMEOUT);
  return false; // No matching receive message
}
