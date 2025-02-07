String res;

void MqttATCommand() {
  Serial.println("Initiating MQTT");
  modem.sendAT("+CMQTTSTART"); //init MQTT
  if (modem.waitResponse(10000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  Serial.println();
  delay(5000);

  Serial.println("Acquire a client");
  modem.sendAT("AT+CMQTTACCQ=0,Esp32,0"); //Configure the check UTF8 flag of the specified MQTT client context
  if (modem.waitResponse(10000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  Serial.println();
  delay(5000);

  Serial.println("Connecting to MQTT");
  String serverAddress = "tcp://pag.tayrix.com:30883";
  String command = "+CMQTTSSLCFG=0,\"" + serverAddress + "\",60,1,\"" + String("DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033") + "\",\"" + String("DEVICE_PASSWORD") + "\"";
  modem.sendAT(command); //init MQTT
  if (modem.waitResponse(10000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    // Check if the response indicates a successful connection
    if (res.indexOf("OK") != -1) {
      Serial.println("Successfully connected to MQTT broker");
    } else {
      Serial.println("Failed to connect to MQTT broker: " + res);
    }
  } else {
    Serial.println("No response from modem. Connection failed.");
  }
  Serial.println();
  delay(5000);

  // Define the topic string
  const char* topicString = "telemetry";
  // Get the length of the topic string
  size_t topicLength = strlen(topicString);
  Serial.println("Publishing to mqtt");
  // Construct the AT command string
  String topicCommand = "AT+CMQTTTOPIC=0," + String(topicLength);

  // Send the AT command to set the MQTT topic
  modem.sendAT(topicCommand);
  if (modem.waitResponse(10000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  Serial.println();
  delay(5000);

  Serial.println("Other topics");
  modem.sendAT("+CMQTTSUBTOPIC=\"Saifarm/GH2/PFC/ZoneA/PFCCONTROL\",\"account/Zepto/clientid/appid/climatedata/WS\""); //init MQTT
  if (modem.waitResponse(10000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  Serial.println();
  delay(5000);
}
