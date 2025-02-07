#include <WiFi.h>

const char* ssid = "Zeptogreens";
const char* password = "Zepto@23";

// Replace with your desired lock key
const char* unlockKey = "Your_Unlock_Key";

bool serialPortLocked = true;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  if (serialPortLocked) {
    unlockSerialPort();
  }
  // Your main code here (if needed)
}

void unlockSerialPort() {
  Serial.println("Enter the unlock key:");
  while (Serial.available() == 0) {
    // Wait for input
  }

  String inputKey = Serial.readStringUntil('\n');
  inputKey.trim();

  if (inputKey == unlockKey) {
    Serial.println("Serial port unlocked.");
    serialPortLocked = false;
  } else {
    Serial.println("Incorrect key. Try again.");
    Serial.flush();
  }
}
