bool initializeLoRa() {
  return LoRa.begin(915E6);
}

void processData() {
  if (newData) {
    // Split string into parts based on '|'
    String data[100];  // Array to hold split values
    int index = 0;

    // Extract data using the pipe delimiter
    while (receivedMessage.length() > 0 && index < 100) {
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
    if (index == 22) {
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

      // Process data based on the sender node ID (SN)
      if (SN == 1) {
        // Data from Slave 1
        Serial.println("Processing data from Slave 1...");
        processSlave1Data(data);  // Call function to process Slave 1 data
      } else if (SN == 2) {
        // Data from Slave 2
        Serial.println("Processing data from Slave 2...");
        processSlave2Data(data);  // Call function to process Slave 2 data
      }

      // After processing, reset the receivedMessage
      newData = false;
    }
  }
}

// Function to process data from Slave 1
void processSlave1Data(String data[11]) {
  // Extract the float values from each part for Slave 1
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

  // Print data from Slave 1
  Serial.print("Slave 1 -> AT: "); Serial.print(AT);
  Serial.print("\tRH: "); Serial.print(RH);
  Serial.print("\tBP: "); Serial.print(BP);
  Serial.print("\tHI: "); Serial.print(HI);
  Serial.print("\tDP: "); Serial.print(DP);
  Serial.print("\tSL: "); Serial.print(SL);
  Serial.print("\tCo2: "); Serial.print(Co2);
  Serial.print("\tET: "); Serial.print(ET);
  Serial.print("\tSR: "); Serial.print(SR);
  Serial.print("\tVPD: "); Serial.println(VPD);
}

// Function to process data from Slave 2
void processSlave2Data(String data[11]) {
  // Extract the float values from each part for Slave 2
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

  // Print data from Slave 2
  Serial.print("Slave 2 -> AT: "); Serial.print(AT);
  Serial.print("\tRH: "); Serial.print(RH);
  Serial.print("\tBP: "); Serial.print(BP);
  Serial.print("\tHI: "); Serial.print(HI);
  Serial.print("\tDP: "); Serial.print(DP);
  Serial.print("\tSL: "); Serial.print(SL);
  Serial.print("\tCo2: "); Serial.print(Co2);
  Serial.print("\tET: "); Serial.print(ET);
  Serial.print("\tSR: "); Serial.print(SR);
  Serial.print("\tVPD: "); Serial.println(VPD);
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
