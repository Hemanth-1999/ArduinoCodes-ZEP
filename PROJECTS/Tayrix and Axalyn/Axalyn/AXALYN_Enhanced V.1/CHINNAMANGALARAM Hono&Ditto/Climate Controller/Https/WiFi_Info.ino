void saveConfig() {
  File file = SPIFFS.open("/config.json", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  // Create a JSON object and write to the file
  DynamicJsonDocument json(1024);
  json["httpServer"] = httpServer;
  json["httpUserName"] = httpUserName;
  json["httpPwd"] = httpPwd;

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
