void sendAcknowledgment() {
  LoRa.beginPacket();
  LoRa.print(SenderIDStr);  // Send acknowledgment message with slave node ID
  LoRa.endPacket();
}

bool waitForAcknowledgment() {
  unsigned long startTime = millis();
  while (millis() - startTime < 60000) {  // Wait for 3 seconds
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String receivedAck = "";
      while (LoRa.available()) {
        receivedAck += (char)LoRa.read();
      }

      Serial.println();
      Serial.println("Received ACK: " + receivedAck);

      // Construct the expected acknowledgment format
      String expectedAckPrefix = "ACK from Master to Node: ";
      String expectedAck = expectedAckPrefix + String(SenderID, HEX);

      Serial.println("Expected ACK: " + expectedAck);

      // Check if the received acknowledgment matches the expected format
      if (receivedAck.startsWith(expectedAckPrefix) && receivedAck.indexOf(String(SenderID, HEX)) != -1) {
        Serial.println("Valid acknowledgment received.");
        return true;  // Acknowledgment received
      } else {
        Serial.println("Invalid acknowledgment: " + receivedAck);
      }
    }
  }  return false;  // No acknowledgment received
}
