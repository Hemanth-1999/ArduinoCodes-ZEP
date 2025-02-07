bool initializeLoRa() {
  return LoRa.begin(915E6);
}

void Ra02LoRa() {
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

  LoRa.beginPacket();
  LoRa.print(fullMessage);
  LoRa.endPacket();

  // Wait for acknowledgment from the master
  if (waitForAcknowledgment()) {
    Serial.println("Acknowledgment received!");
    ackReceived = true;
  } else {
    Serial.println("No acknowledgment received!");
  }

  // Wait 10 seconds before sending the next message
  delay(10000);
}
