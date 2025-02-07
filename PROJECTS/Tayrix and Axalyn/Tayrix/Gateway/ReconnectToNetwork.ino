void ReconnectToNetwork() {
  // Optional: Reset or reinitialize the modem if necessary
  modem.restart(); // This is hypothetical; check your modem's library for the correct command

  // Define the network modes to try
  uint8_t networkModes[] = {
    2,  // Automatic
    13, // GSM only
    38, // LTE only
    51  // GSM and LTE only
  };

  for (int i = 0; i < sizeof(networkModes) / sizeof(networkModes[0]); i++) {
    Serial.printf("Trying to reconnect using network mode %d\n", networkModes[i]);
    modem.setNetworkMode(networkModes[i]);
    delay(3000); // Allow time for the modem to switch modes

    bool isConnected = false;
    int tryCount = 60; // Set a maximum number of attempts

    while (tryCount--) {
      String networkOperator = modem.getOperator();
      Serial.print("Operator: ");
      Serial.println(networkOperator);

      int16_t signal = modem.getSignalQuality();
      Serial.print("Signal Quality: ");
      Serial.println(signal);

      isConnected = modem.isNetworkConnected();
      Serial.print("Network Connection Status: ");
      Serial.println(isConnected ? "CONNECTED" : "NOT CONNECTED YET");

      if (isConnected) {
        Serial.println("Reconnected successfully.");
        return; // Exit the function if connected
      }

      delay(1000); // Wait before the next check
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Toggle LED
    }

    Serial.println("Failed to reconnect using this network mode.");
  }

  Serial.println("All attempts to reconnect failed.");
}
