void saveConfig() {
  File configFile = SPIFFS.open("/config.json", "FILE_WRITE");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // Create a JSON object and write to the file
  DynamicJsonDocument json(1024);
  json["clientID"] = clientId;
  json["mqttServer"] = hostname;
  json["mqttPort"] = port_str;
  json["mqttUserName"] = user;
  json["mqttPwd"] = password;

  serializeJson(json, configFile);
  configFile.close();
  Serial.println("Configuration saved to SPIFFS");
}

void loadConfig() {
  File configFile = SPIFFS.open("/config.json", "FILE_READ");
  if (!configFile) {
    Serial.println("Config file not found.");
    return;
  }

  DynamicJsonDocument json(1024);
  DeserializationError error = deserializeJson(json, configFile);
  if (error) {
    Serial.println("Failed to parse config file.");
    return;
  }

  String testValue = json["test"] | "default";
  Serial.println("Test value loaded: " + testValue);
  configFile.close();
}

void wifiInfo() {
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}
