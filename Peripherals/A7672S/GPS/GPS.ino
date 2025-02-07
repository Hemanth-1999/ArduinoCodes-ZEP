#include <SoftwareSerial.h>

// Define the software serial object
SoftwareSerial mySerial(RXD, TXD); // RX, TX

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  mySerial.begin(9600);

  // Other setup code goes here
}

void loop() {
  // Read from the hardware serial (connected to the computer)
  if (Serial.available()) {
    char data = Serial.read();
    // Process the data as needed
  }

  // Read from the software serial (connected to other device)
  if (mySerial.available()) {
    char data = mySerial.read();
    // Process the data as needed
  }

  // Other loop code goes here
}
