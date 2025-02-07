#include <stdio.h>
#include <string.h>
#include <esp_task_wdt.h>  // Include watchdog library

#define DEBUG true

#define DTR_PIN 9
#define RI_PIN 8

#define LTE_PWRKEY_PIN 5
#define LTE_RESET_PIN 6
#define LTE_FLIGHT_PIN 7

// Variables to store user input and LTE module communication
String from_usb = "";
bool flightMode = false; // Change this based on your requirement

void setup() {
  // Initialize serial communication
  Serial.begin(115200);    // Serial monitor
  Serial1.begin(115200);   // Serial communication with LTE module

  // Initialize the Watchdog Timer (10 seconds timeout)
  esp_task_wdt_init(10, true);  // Enable panic on timeout
  esp_task_wdt_add(NULL);       // Add current task to watchdog

  // Check LTE module state
  if (moduleStateCheck()) {
    Serial.println("LTE module is ready for commands.");
  } else {
    Serial.println("LTE module communication failed.");
  }
}

void loop() {
  // Reset the watchdog timer to prevent reset
  esp_task_wdt_reset();

  // Read data from LTE module and print to serial monitor
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }

  // Read user input from Serial and process commands
  while (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
  }
}

// Function to handle long-running tasks with watchdog resets
void longRunningFunction() {
  for (int i = 0; i < 100000; i++) {
    esp_task_wdt_reset();  // Feed the watchdog during long operations
    // Simulate long work (e.g., processing data)
    delay(1);  // Simulate a small task delay
  }
}

// Function to check LTE module state
bool moduleStateCheck() {
  int attempts = 5;
  for (int i = 0; i < attempts; i++) {
    String msg = sendData("AT", 1000, DEBUG);
    if (msg.indexOf("OK") >= 0) {
      Serial.println("SIM7600 Module is powered on.");
      return true;
    }
    delay(1000);
    esp_task_wdt_reset();  // Feed the watchdog during the check
  }
}

// Function to send AT commands to the LTE module and handle responses
String sendData(String command, const int timeout, boolean debug) {
  String response = "";

  // Handle Ctrl+Z command (0x1A)
  if (command.equals("1A") || command.equals("1a")) {
    Serial.println("Sending Ctrl+Z...");
    Serial1.write(26);  // Ctrl+Z (0x1A)
    return "";
  } else {
    Serial1.println(command);  // Send command to LTE module
    Serial1.flush();           // Ensure all bytes are sent
  }

  // Read response with timeout
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read();
      response += c;
    }
    esp_task_wdt_reset();  // Feed watchdog during response waiting
  }

  // Print response if debugging is enabled
  if (debug) {
    Serial.print(response);
  }

  return response;
}
