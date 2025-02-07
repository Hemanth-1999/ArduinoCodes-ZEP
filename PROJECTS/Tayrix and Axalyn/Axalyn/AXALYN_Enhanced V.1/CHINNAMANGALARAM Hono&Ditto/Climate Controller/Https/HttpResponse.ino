void SendDatatoHttps() {
  HTTPClient https;  // Use the correct HTTPClient class

  https.begin(client, httpServer);
  https.setAuthorization(httpUserName, httpPwd);
  https.addHeader("Content-Type", "application/json");

  String payload = "{\"temperature\": 25.5, \"humidity\": 60}";

  int httpResponseCode = https.POST(payload);

  if (httpResponseCode > 0) {
    String response = https.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on HTTP request");
  }

  https.end();
}
