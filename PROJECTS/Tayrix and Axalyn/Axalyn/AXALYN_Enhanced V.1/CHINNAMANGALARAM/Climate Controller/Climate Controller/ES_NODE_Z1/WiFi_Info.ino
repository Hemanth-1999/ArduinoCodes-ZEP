void saveConfig() {

  File file = SPIFFS.open("/config.json", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // Create a JSON object and write to the file
  DynamicJsonDocument json(1024);
  json["clientID"] = clientID;
  json["mqttServer"] = mqttServer;
  json["mqttPort"] = port_str;
  json["mqttUserName"] = mqttUserName;
  json["mqttPwd"] = mqttPwd;

  serializeJson(json, file);
  file.close();
  Serial.println("Configuration saved to SPIFFS");
}

void loadConfig() {
  File file = SPIFFS.open("/config.json", FILE_READ);
  if (!file) {
    Serial.println("Config file not found.");
    return;
  }

  DynamicJsonDocument json(1024);
  DeserializationError error = deserializeJson(json, file);
  if (error) {
    Serial.println("Failed to parse config file.");
    return;
  }

  String testValue = json["test"] | "default";
  Serial.println("Test value loaded: " + testValue);
  file.close();
}

void initializeWifiManager()
{
  WiFiManagerParameter custom_mqtt_client_id("client_id", "mqtt client id", clientID, 40);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqttServer, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", port_str, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqttUserName, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqttPwd, 20);

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_mqtt_client_id);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);

  //  Remove saved WiFi credentials
  //  wifiManager.resetSettings();

  if (!wifiManager.autoConnect("ES NODE")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    // restart and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // Read updated parameters
  strcpy(clientID, custom_mqtt_client_id.getValue());
  strcpy(mqttServer, custom_mqtt_server.getValue());
  strcpy(port_str, custom_mqtt_port.getValue());
  strcpy(mqttUserName, custom_mqtt_user.getValue());
  strcpy(mqttPwd, custom_mqtt_pass.getValue());

}

void saveConfigCallback ()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void wifiInfo() {
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}
