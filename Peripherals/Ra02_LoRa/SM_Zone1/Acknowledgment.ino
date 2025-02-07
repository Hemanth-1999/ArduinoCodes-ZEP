void sendAcknowledgment() {
  // Construct acknowledgment message
  String ackMessage = SenderIDStr;  // This holds the acknowledgment message, typically the node ID
  
  // Transmit the acknowledgment message
  int state = radio.transmit(ackMessage);
  
  // Check if the transmission was successful
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("ACK sent successfully!"));
  } else {
    Serial.print(F("ACK failed to send, error code: "));
    Serial.println(state);
  }
}

bool waitForAcknowledgment() {
  unsigned long startTime = millis();
  
  while (millis() - startTime < 60000) {  // Wait for up to 60 seconds
    String receivedAck;
    
    // Try to receive a LoRa packet
    int state = radio.receive(receivedAck);
    
    if (state == RADIOLIB_ERR_NONE) {
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
    } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
      // Timeout occurred, no acknowledgment received yet
      Serial.println("Acknowledgment wait timed out.");
    } else {
      // Some other reception error
      Serial.print(F("Failed to receive ACK, error code: "));
      Serial.println(state);
    }
  }

  return false;  // No acknowledgment received within timeout
}
