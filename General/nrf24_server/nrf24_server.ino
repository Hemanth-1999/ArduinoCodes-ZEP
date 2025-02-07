#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Instantiate RF24 class with CE and CSN values
RF24 radio(7, 8);
// Address to devices comunicate each other (same in both)
const uint64_t pipe = 0xE8E8F0F0E1LL;
// A variable to hold some info
boolean info = false;

void setup() {
  // Setup serial output
  Serial.begin(9600);
  // Start RF
  radio.begin();
  // Setup the channel to work within, number 100
  radio.setChannel(100);
  // Open recept pipe
  radio.openReadingPipe(1, pipe);
  // Start to listen
  radio.startListening();
}

void loop() {
  // Wait until some data
  if (radio.available()) {
    // Read payload, and check if it finished
    radio.read(&info, sizeof(info));
    // Manage info
    if (info) {
      Serial.println("We received positive!");
    } else {
      Serial.println("We received negative!");
    }
  }
  // Wait a bit
  delay(50);
}
