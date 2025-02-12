#include "time.h"
#include <ESP32Time.h>
ESP32Time rtc;
//
hw_timer_t * timer = NULL;
// the number of the LED pin
const int ledPin = 5;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);


  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}

void loop() {
  // increase the LED brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(10);
  }

  // decrease the LED brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(10);
  }
}
