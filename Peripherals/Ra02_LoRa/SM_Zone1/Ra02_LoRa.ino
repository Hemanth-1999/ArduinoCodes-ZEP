void Radiotransmit() {
  // Format the data as a string
  message1 = "AT: " + String(ES_AT, 2) + " Deg C";
  message2 = "RH: " + String(ES_RH, 2) + "%";
  message3 = "BP: " + String(ES_BP, 2) + " hPa";
  message4 = "HI: " + String(HI, 2) + " Deg C";
  message5 = "Dep: " + String(dewpt) + " Deg C";
  message6 = "Sl: " + String(sl) + " hPa";
  message7 = "Co2: " + String(co2, 2) + " ppm";
  message8 = "ET: " + String(ETo, 2) + " mm";
  message9 = "SR: " + String(solarRadiation, 2) + " Wh/m²";
  message10 = "VPD: " + String(VPDcal(), 2) + " kPa";

  String fullMessage = SenderIDStr + "|" + message1 + "|" + message2 + "|" + message3 + "|" + message4 + "|" + message5 + "|" + message6 + "|" + message7 + "|" + message8 + "|" + message9 + "|" + message10;

  Serial.println();
  // Send the message via LoRa
  Serial.println("Sending packet: ");
  Serial.println(fullMessage);
  Serial.println(message1);
  Serial.println(message2);
  Serial.println(message3);
  Serial.println(message4);
  Serial.println(message5);
  Serial.println(message6);
  Serial.println(message7);
  Serial.println(message8);
  Serial.println(message9);
  Serial.println(message10);

  int state = radio.transmit(fullMessage);

  if (sendState == 0) {
    radio.transmit(fullMessage);
    sendState = 1;
  } else {
    radio.transmit(fullMessage);
    sendState = 0;
  }

  // Check if transmission was successful
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Transmission successful!");
    u8x8.clearDisplay();
    RadioData1();
    delay(5000);
    u8x8.clearDisplay();
    RadioData2();
    // Wait for acknowledgment from the master
    if (waitForAcknowledgment()) {
      Serial.println("Acknowledgment received!");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.drawString(0, 4, "Ack received");
      ackReceived = true;
    } else {
      Serial.println("No acknowledgment received!");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.drawString(0, 4, "No Ack received");
    }

  } else {
    Serial.print("Transmission failed, error code: ");
    Serial.println(state);

    // Additional debugging information
    switch (state) {
      case 0:  // RADIOLIB_ERR_NONE
        Serial.println("Transmission successful!");
        break;
      case -16:  // RADIOLIB_ERR_CHANNEL_BUSY
        Serial.println("Channel is busy.");
        break;
      case -12:  // RADIOLIB_ERR_PACKET_TOO_LONG
        Serial.println("Packet too long.");
        break;
      // Add more cases as necessary based on the error codes defined in RadioLib
      default:
        Serial.print("Unknown error. Code: ");
        Serial.println(state);
        break;
    }

    // Wait 10 seconds before sending the next message
    delay(10000);
  }
}
