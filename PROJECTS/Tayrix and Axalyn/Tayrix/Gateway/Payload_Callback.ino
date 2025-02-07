void connectToMqtt(String broker, int port, String username, String password, String topic) {
  while (!mqtt.connected()) { // Loop until connected to MQTT server

    DeviceNetwork();

    // Check if GPRS is connected
    if (modem.isGprsConnected()) {
      Serial.println("GPRS connected!");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.setCursor(0, 4);
      u8x8.print("GPRS connected!");
      delay(5000);
      if (modem.gprsConnect(apn)) {
        Serial.println("Connected to apn");
        u8x8.clearDisplay();
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        u8x8.setCursor(0, 4);
        u8x8.print("Connected to apn");
        delay(5000);
      }
    }

    // Check if GPRS is connected
    if (!modem.isGprsConnected()) {
      Serial.println("GPRS disconnected!");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.setCursor(0, 4);
      u8x8.print("GPRS disconnected!");
      delay(5000);
      if (!modem.gprsConnect(apn)) {
        Serial.println("Failed to reconnect GPRS");
        u8x8.clearDisplay();
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        u8x8.setCursor(0, 4);
        u8x8.print("Failed to reconnect GPRS");
        delay(5000);
      }
    }

    Serial.println();
    Serial.println("Attempting MQTT connection...");
    Serial.println("Host: " + String(broker));
    Serial.println("Port: " + String(port));
    Serial.println("Username: " + String(username));
    Serial.println("Password: " + String(password));

    // Start MQTT service
    SerialAT.println("ATE0");
    delay(2000);
    String Echo = waitForResponse(5000);
    Serial.println("Got: " + Echo);
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    u8x8.setCursor(0, 4);
    u8x8.print("Got: " + Echo);
    delay(5000);

    if (Echo.indexOf("OK") == -1) {
      Serial.println("Failed to clear echo");
      delay(2000);
      continue;
    }

    SerialAT.println("AT+CMQTTSTART"); //Establishing MQTT Connection
    delay(15000);
    String response = waitForResponse(15000);
    Serial.println("Got: " + response);
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    u8x8.setCursor(0, 4);
    u8x8.print("Got: " + response);
    delay(5000);

    if (response.indexOf("OK") == -1) {
      Serial.println("Failed to start MQTT service");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.setCursor(0, 4);
      u8x8.print("Failed to start MQTT service");
      delay(5000);
      continue;
    }

    // Set client ID
    SerialAT.println("AT+CMQTTACCQ=0,\"username0001\""); // Simplified client ID
    delay(2000);
    String response1 = waitForResponse(5000);
    Serial.println("Got: " + response1);
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    u8x8.setCursor(0, 4);
    u8x8.print("Got: " + response1);
    delay(5000);

    if (response1.indexOf("OK") == -1) {
      Serial.println("Error setting client ID");
      u8x8.clearDisplay();
      u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
      u8x8.setCursor(0, 4);
      u8x8.print("Error setting client ID");
      delay(5000);
      continue;
    }

    // Connect to MQTT Broker

    // Build the entire AT command as one string
    String mqttConnectCommand = "AT+CMQTTCONNECT=0,\"tcp://";
    mqttConnectCommand += broker;      // Add MQTT server
    mqttConnectCommand += ":";
    mqttConnectCommand += port;        // Add MQTT port
    mqttConnectCommand += "\",90,0,\"";
    mqttConnectCommand += username;    // Add MQTT username
    mqttConnectCommand += "\",\"";
    mqttConnectCommand += password;      // Add MQTT password (converted from uint16_t to string)
    mqttConnectCommand += "\"";

    // Send the entire command at once
    SerialAT.println(mqttConnectCommand);

    // Optionally, print the command to Serial for debugging purposes
    Serial.println("Sent command: " + mqttConnectCommand);

    String response2 = waitForResponse(5000);
    Serial.println("Got: " + response2);
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
    u8x8.setCursor(0, 4);
    u8x8.print("Got: " + response2);
    delay(5000);

    if (response2.indexOf("OK") == -1) {
      Serial.println("Error connecting to mqtt");
      delay(5000);
      continue;
    }

    // Check connection status
    if (mqtt.connected()) {
      Serial.println("Connected to Broker");
      SerialAT.println("AT+CMQTTSUBTOPIC=0,9,1"); //AT Command for Setting up the Subscribe Topic Name
      delay(2000);
      SerialAT.println(topicName); //Topic Name
      delay(2000);
      SerialAT.println("AT+CMQTTSUB=0,4,1,1"); //Length of message
      delay(2000);
      SerialAT.println("HAII"); //message
      delay(2000);
      Serial.println("Done");
      mqtt.subscribe(topicName);   //Subscribe to Learning Mode Topic
      break; // Exit loop if connected
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqtt.state());
      Serial.print("Reason: ");
      switch (mqtt.state()) {
        case -4: Serial.println("Connection timeout"); break;
        case -3: Serial.println("Connection lost"); break;
        case -2: Serial.println("Connect failed"); break;
        case -1: Serial.println("Disconnected"); break;
        case 1: Serial.println("Bad protocol"); break;
        case 2: Serial.println("Bad client ID"); break;
        case 3: Serial.println("Unavailable"); break;
        case 4: Serial.println("Bad credentials"); break;
        case 5: Serial.println("Unauthorized"); break;
        default: Serial.println("Unknown error");
      }
      delay(5000);  // Will attempt connection again in 5 seconds
      Serial.println("Resetting device...");

      // Reset the device before attempting MQTT connection
      ESP.restart(); // Reset the device
      delay(5000);  // Wait for modem to restart
      String rst = waitForResponse(5000);
      Serial.println("Got: " + rst);

    }
  }
}

String waitForResponse(unsigned long timeout) {
  String response = "";
  unsigned long startTime = millis();
  while (millis() - startTime < timeout) {
    while (SerialAT.available()) {
      char c = SerialAT.read();
      response += c;
      delay(5); // Small delay to allow buffer to fill
    }
  }
}

void reconnect() {
  connectToMqtt(MqttCredentials.Broker, MqttCredentials.Port, MqttCredentials.Username, MqttCredentials.Password, MqttCredentials.Topic);
}

void callback(char* topic, byte* payload, unsigned int length) { // MQTT callback function
  if (String(topic) == topicName) {
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
    Serial.print("ledStatus:");
    Serial.println(ledStatus);
    mqtt.publish(topicName, ledStatus ? "1" : "0");
    Serial.println("Incoming message");
  } else {
    Serial.println("No matching topic");
  }
}
