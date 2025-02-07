bool initializeLoRa() {
  return LoRa.begin(915E6);
}

void processData() {
  if (newData) {
    // Split string into parts based on '|'
    String data[11];  // Array to hold split values
    int index = 0;

    // Extract data using the pipe delimiter
    while (receivedMessage.length() > 0 && index < 11) {
      int pipeIndex = receivedMessage.indexOf('|');  // Find the next pipe character
      if (pipeIndex != 0) {
        data[index++] = receivedMessage.substring(0, pipeIndex);  // Store substring
        receivedMessage = receivedMessage.substring(pipeIndex + 1);  // Update remaining string
      } else {
        data[index] = receivedMessage;  // Last element
        break;
      }
    }

    // Ensure that we received the correct number of values
    if (index == 11) {
      // Extract the numeric values from each part

      // After splitting the message
      Serial.print("Extracted Sender Node String (data[0]): ");
      Serial.println(data[0]);  // Print the first part of the message

      // Assuming SN is the first part of the message (index 0)
      String senderIDStr = data[0];
      SN = senderIDStr.toInt();  // Convert SenderID to integer

      // Add debug statement to verify the value of SN
      Serial.print("Extracted Sender Node ID (SN): ");
      Serial.println(SN, HEX);  // Print in hexadecimal format

      AT = extractFloat(data[1]);
      RH = extractFloat(data[2]);
      BP = extractFloat(data[3]);

      HI = extractFloat(data[4]);
      DP = extractFloat(data[5]);
      SL = extractFloat(data[6]);

      Co2 = extractFloat(data[7]);
      ET = extractFloat(data[8]);
      SR = extractFloat(data[9]);

      VPD = extractFloat(data[10]);

      Serial.print("AT: "); Serial.print(AT);
      Serial.print("\tRH: "); Serial.print(RH);
      Serial.print("\tBP: "); Serial.print(BP);
      Serial.print("\tHI: "); Serial.print(HI);
      Serial.print("\tDP: "); Serial.print(DP);
      Serial.print("\tSL: "); Serial.print(SL);
      Serial.print("\tCo2: "); Serial.print(Co2);
      Serial.print("\tET: "); Serial.print(ET);
      Serial.print("\tSR: "); Serial.print(SR);
      Serial.print("\tVPD: "); Serial.println(VPD);

      sendAcknowledgment();

    } else {
      Serial.println("Received data does not match expected format.");
    }
    // Reset flag
    newData = false;
  }
}

// Function to send acknowledgment back to the sender node
void sendAcknowledgment() {
  if (SN != 0) {  // Only send acknowledgment if SN is valid
    LoRa.beginPacket();
    LoRa.print("ACK from Master to Node: ");
    LoRa.print(SN, HEX);  // Send acknowledgment message with sender node ID in hexadecimal
    LoRa.endPacket();

    Serial.print("Acknowledgment sent to Node: ");
    Serial.println(SN, HEX);
  } else {
    Serial.println("Invalid Sender Node (SN). No ACK sent.");
  }
}

// Helper function to extract float values from strings with labels
float extractFloat(const String &str) {
  int colonIndex = str.indexOf(':');
  if (colonIndex != -1) {
    String value = str.substring(colonIndex + 1);
    Serial.print("Raw Value: ");
    Serial.println(value); // Debug print to check the raw value
    // Remove any non-numeric characters (including units and spaces)
    value.replace("Deg C", "");
    value.replace(" %", "");
    value.replace(" hPa", "");
    value.replace(" ppm", "");
    value.replace(" mm", "");
    value.replace(" Wh/m²", "");
    value.replace(" kPa", "");
    value.trim(); // Remove any leading or trailing spaces
    return value.toFloat();
  }
  return 0.0; // Default value if extraction fails
}
