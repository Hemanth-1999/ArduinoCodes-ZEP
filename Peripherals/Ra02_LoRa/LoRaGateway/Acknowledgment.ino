void sendAcknowledgment() {
  if (SN != 0) {  // Only send acknowledgment if SN is valid
    LoRa.beginPacket();
    LoRa.print(SN, HEX);  // Send acknowledgment message with sender node ID in hexadecimal
    LoRa.endPacket();

    Serial.print("Acknowledgment sent to Node: ");
    Serial.println(SN, HEX);
  } else {
    Serial.println("Invalid Sender Node (SN). No ACK sent.");
  }
}
