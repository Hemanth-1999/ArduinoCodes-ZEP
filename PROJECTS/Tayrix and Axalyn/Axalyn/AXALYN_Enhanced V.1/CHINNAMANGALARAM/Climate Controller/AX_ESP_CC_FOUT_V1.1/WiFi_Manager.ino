void initializeWifiManager()
{
  WiFiManagerParameter custom_mqtt_client_id("client_id", "mqtt client id", clientId, 40);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", hostname, 40);

  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", port_str, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", user, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", password, 20);

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_mqtt_client_id);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);

  //  Remove saved WiFi credentials
  //    wifiManager.resetSettings();

  if (!WiFi.SSID()) {
    Serial.println("No stored WiFi credentials, opening Config Portal...");
    wifiManager.startConfigPortal("CLIMATECONTROLLER");
  } else {
    Serial.println("Connecting to stored WiFi credentials...");
    if (!wifiManager.autoConnect("CLIMATECONTROLLER")) {
      Serial.println("Failed to connect and hit timeout, restarting...");
      ESP.restart();
    }
  }

  // Read updated parameters
  strcpy(clientId, custom_mqtt_client_id.getValue());
  strcpy(hostname, custom_mqtt_server.getValue());
  strcpy(port_str, custom_mqtt_port.getValue());
  strcpy(user, custom_mqtt_user.getValue());
  strcpy(password, custom_mqtt_pass.getValue());
}

void saveConfigCallback ()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
