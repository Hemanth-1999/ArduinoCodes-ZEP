#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>  // Correct library for ESP32
#include <SPIFFS.h>
#include <FS.h>

char httpServer[40] = "https://ditto.axalyn.com";
char httpUserName[60] = "DEVICE_httpUserName@6c458335-0fdc-40d9-8ab1-7606dabdb887";
char httpPwd[40] = "DEVICE_httpPwd";

void saveConfig();

bool shouldSaveConfig = false;

WiFiManager wm;
WiFiManagerParameter custom_http_server("httpServer", "http server", "", 40);
WiFiManager wifiManager;
WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    // Handle the failure appropriately, possibly halt or retry
    while (true) {
      delay(1000); // Delay indefinitely if SPIFFS fails to mount
    }
  }

  // Save configuration if needed
  if (shouldSaveConfig) {
    Serial.println("Saving updated configuration...");
    saveConfig();
  }

  loadConfig();   // Load configuration settings from SPIFFS or EEPROM

  initializeWifiManager();   // Set up WiFiManager and http configuration

  wifiManager.setTimeout(180);

  wifiInfo();

  // Set time for certificate validation
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  client.setInsecure();  // Skip certificate validation for testing
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Attempting to reconnect to WiFi...");

    // Attempt to reconnect to Wi-Fi
    WiFi.reconnect();  // Try to reconnect to the last connected Wi-Fi network
    Serial.println("Reconnection failed, retrying...");
    delay(5000);  // Wait 5 seconds before retrying
    return;  // Skip the rest of the loop if not connected

    delay(10000);  // Send data every 10 seconds
  }

  // If connected
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Reconnected to WiFi."); \
  }

  SendDatatoHttps();

}
}
