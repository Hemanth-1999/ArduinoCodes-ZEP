void initializeWifiManager()
{
  WiFiManagerParameter custom_http_server("server", "http server", httpServer, 40);
  WiFiManagerParameter custom_http_user("user", "http user", httpUserName, 20);
  WiFiManagerParameter custom_http_pass("pass", "http pass", httpPwd, 20);

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_http_server);
  wifiManager.addParameter(&custom_http_user);
  wifiManager.addParameter(&custom_http_pass);

  //  Remove saved WiFi credentials
  wifiManager.resetSettings();

  if (!WiFi.SSID()) {
    Serial.println("No stored WiFi credentials, opening Config Portal...");
    wifiManager.startConfigPortal("ES NODE");
  } else {
    Serial.println("Connecting to stored WiFi credentials...");
    if (!wifiManager.autoConnect("ES NODE")) {
      Serial.println("Failed to connect and hit timeout, restarting...");
      ESP.restart();
    }
  }

  // Read updated parameters
  strcpy(httpServer, custom_http_server.getValue());
  strcpy(httpUserName, custom_http_user.getValue());
  strcpy(httpPwd, custom_http_pass.getValue());

}
