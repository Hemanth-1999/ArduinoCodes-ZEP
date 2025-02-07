#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(16, 17); // RX, TX

void setup() {
  Serial.begin(9600); // Initialize serial monitor
  gsmSerial.begin(115200); // Initialize GSM serial communication
  delay(1000);

  Serial.println("Initializing A7672S module...");
  delay(1000);

  // Handle other initialization commands as needed
}

void loop() {
  // Send AT command to check if module is responsive
  gsmSerial.println("AT");
  delay(1000);

  if (gsmSerial.find("OK")) {
    Serial.println("A7672S module is responsive.");
  } else {
    Serial.println("A7672S module is not responding. Check connections and power.");
//    while (true); // Hang here if module is not responsive
  }

  // Example: Configure module for connecting to the cellular network
  gsmSerial.println("AT+CGDCONT=1,\"IP\",\"1123\""); // Replace 1234 with your SIM card PIN
  delay(1000);
}
