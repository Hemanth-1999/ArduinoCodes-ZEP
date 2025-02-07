#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Updater.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Cenaura";
const char* password = "Centech#2024";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Setup mDNS
  if (!MDNS.begin("esp-ota")) {
    Serial.println("MDNS failed to start");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("MDNS started");

  // OTA Setup
  ArduinoOTA.setHostname("esp-ota");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    }
    Serial.printf("Start updating %s\n", type.c_str());
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    switch (error) {
      case OTA_AUTH_ERROR:
        Serial.println("Auth Failed");
        break;
      case OTA_BEGIN_ERROR:
        Serial.println("Begin Failed");
        break;
      case OTA_CONNECT_ERROR:
        Serial.println("Connect Failed");
        break;
      case OTA_RECEIVE_ERROR:
        Serial.println("Receive Failed");
        break;
      case OTA_END_ERROR:
        Serial.println("End Failed");
        break;
    }
  });
  ArduinoOTA.begin();
  Serial.println("OTA setup complete");

  // Web Server Setup for OTA
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "OK");
  }, [](AsyncWebServerRequest *request) {
    // Handle file upload
    HTTPUpload& upload = request->upload();
    if (upload.status == UPLOAD_FILE_START) {
      String filename = upload.filename;
      if (!filename.startsWith("/")) filename = "/" + filename;
      Serial.printf("Update: %s\n", filename.c_str());
      if (!Update.begin()) { // Start the update
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u\n", upload.totalSize);
        // Optionally, you can restart the ESP8266 here
        // ESP.restart();
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.begin();
}

void loop() {
  ArduinoOTA.handle(); // Handle OTA requests
}
