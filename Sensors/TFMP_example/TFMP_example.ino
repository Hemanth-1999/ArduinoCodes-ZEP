
#include <SoftwareSerial.h>     // Include ESP32 SoftwareSerial library
#include <atomic>               // Include atomic library for atomic bool
#include <TFMPlus.h>            // Include TFMini Plus Library
#include "printf.h"             // Modified printf for ESP32

//2, 4, 25 26, 27, 32, 33

#define RX 16                    // Define RX pin
#define TX 17                    // Define TX pin

#define BAUD_RATE 115200        // Define baud rate
#define MAX_FRAMEBITS (1 + 8 + 1 + 2)  // Define max frame bits

SoftwareSerial testSerial(RX, TX); // Replace RX and TX with the appropriate pin numbers
std::atomic<bool> rxPending(false);  // Atomic bool for receive handler
std::atomic<bool> m_isrOverflow;

void IRAM_ATTR receiveHandler() {
  rxPending.store(true);
}

#include <TFMPlus.h>  // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;         // Create a TFMini Plus object

#include "printf.h"   // Modified to support Intel based Arduino
// devices such as the Galileo. Download from:
// https://github.com/spaniakos/AES/blob/master/printf.h

// The Software Serial library is an alternative for devices that
// have only one hardware serial port. Delete the comment slashes
// on lines 37 and 38 to invoke the library, and be sure to choose
// the correct RX and TX pins: pins 10 and 11 in this example. Then
// in the 'setup' section, change the name of the hardware 'Serial2'
// port to match the name of your software serial port, such as:
// 'mySerial.begin(115200); etc.

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 4);

void setup() {
  Serial.begin(115200);   // Initialize terminal serial port
  Serial.setDebugOutput(false);
  testSerial.begin(BAUD_RATE, SWSERIAL_8N1, RX, TX);
  testSerial.enableIntTx(false);
  testSerial.onReceive(receiveHandler);

  testSerial.println(PSTR("\nSoftware serial onReceive() event test started"));

  for (char ch = ' '; ch <= 'z'; ch++) {
    testSerial.write(ch);
  }
  testSerial.println();
  delay(20);               // Give port time to initialize
  printf_begin();          // Initialize printf.
  printf("\r\nTFMPlus Library Example - 10SEP2021\r\n");  // say 'hello'

  Serial2.begin(115200);  // Initialize TFMPLus device serial port.
  delay(20);              // Give port time to initialize
  tfmP.begin(&Serial2);   // Initialize device library object and...
  // pass device serial port to the object.

  // Send some example commands to the TFMini-Plus
  // - - Perform a system reset - - - - - - - - - - -
  printf("Soft reset: ");
  if (tfmP.sendCommand(SOFT_RESET, 0)) {
    printf("passed.\r\n");
  }
  else tfmP.printReply();

  delay(500);  // added to allow the System Rest enough time to complete

  // - - Display the firmware version - - - - - - - - -
  printf("Firmware version: ");
  if (tfmP.sendCommand(GET_FIRMWARE_VERSION, 0)) {
    printf("%1u.", tfmP.version[0]); // print three single numbers
    printf("%1u.", tfmP.version[1]); // each separated by a dot
    printf("%1u\r\n", tfmP.version[2]);
  }
  else tfmP.printReply();
  // - - Set the data frame-rate to 20Hz - - - - - - - -
  printf("Data-Frame rate: ");
  if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_1000)) {
    printf("%2uHz.\r\n", FRAME_1000);
  }
  else tfmP.printReply();
  // - - - - - - - - - - - - - - - - - - - - - - - -

  delay(500);            // And wait for half a second.
}

// Initialize variables
int16_t tfDist = 0;    // Distance to object in centimeters
int16_t tfFlux = 0;    // Strength or quality of return signal
int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip

// Use the 'getData' function to pass back device data.
void loop() {
  delay(50);   // Loop delay to match the 20Hz data frame rate

  if (tfmP.getData(tfDist, tfFlux, tfTemp)) { // Get data from the device
    
    bool isRxPending = m_isrOverflow.exchange(false);
    auto avail = testSerial.available();
    
    if (isRxPending && !avail) {
      // event fired on start bit, wait until first stop bit of longest frame
      delayMicroseconds(1 + MAX_FRAMEBITS * 1000000 / BAUD_RATE);
      avail = testSerial.available();
    }
    // try to force to half-duplex
    decltype(avail) prev_avail;
    do {
      delayMicroseconds(1 + MAX_FRAMEBITS * 1000000 / BAUD_RATE);
      prev_avail = avail;
    } while (prev_avail != (avail = testSerial.available()));
    while (avail > 0) {
      testSerial.write(testSerial.read());
      avail = testSerial.available();
    }
    testSerial.println();

    printf("Dist:%04icm ", tfDist);    // display distance,
    printf("Flux:%05i ", tfFlux);      // display signal strength/quality,
    printf("Temp:%2i%s", tfTemp, "C"); // display temperature,
    printf("\r\n");                    // end-of-line.
  } else { // If the command fails...
    tfmP.printFrame();  // display the error and HEX data
  }
}
