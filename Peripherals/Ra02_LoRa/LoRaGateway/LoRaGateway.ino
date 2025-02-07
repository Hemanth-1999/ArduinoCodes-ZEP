#include <LoRa.h>
#include <U8x8lib.h>  // Make sure you have the U8x8 OLED library included
#include <ArduinoJson.h>
#include <LoraMessage.h>
#include "esp_task_wdt.h"  // Include the Watchdog Timer library

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

// OLED Setup
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

int SN;

int battry = 100;

String receivedRssi;
String receivedMessage;

bool newData = false;
bool initializeLoRa();
bool loRaInitialized = false;

float  AT, RH, BP, VPD, HI, Co2, ET, DP, SL, SR;

void sl_page1();
void sl_page2();
void sl_page3();
void sl_page4();
void LoRafailed();
void processData();
void sendAcknowledgment();
void processAcknowledgment();
void displayData(String message);
void processSlave1Data(String data[11]);
void processSlave2Data(String data[11]);
void processReceivedMessage(String message);

const int WATCHDOG_TIMEOUT = 30;  // Timeout in seconds

const long interval = 3000;  // Interval at which to perform action (in milliseconds)
const long pageInterval = 5000;  // Interval to switch pages (5 seconds)

unsigned long previousMillis = 0;
unsigned long pageChangeMillis = 0;

int currentPage = 1;

const unsigned long RETRY_INTERVAL = 5000;  // Interval between retries (5 seconds)

unsigned long initStartTime;

void setup() {
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(9600);
  while (!Serial); //if just the the basic function, must connect to a computer

  // Initialize the Watchdog Timer
  //  esp_task_wdt_init(WATCHDOG_TIMEOUT, true);  // Timeout in seconds, panic on reset
  //  esp_task_wdt_add(NULL);  // Add the current task (main loop) to the watchdog

  //  Serial.println("Watchdog Timer initialized.");

  // Initialize the OLED
  u8x8.begin();
  u8x8.setPowerSave(0);

  initStartTime = millis();
  loRaInitialized = initializeLoRa();

  if (loRaInitialized) {
    Serial.println("LoRa initialization successful!");
    // Proceed with other setup tasks
  } else {
    Serial.println("Failed to initialize LoRa within the timeout period.");
    // Handle failure, maybe enter a low-power mode or perform alternative actions
  }

  // Set LoRa parameters
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  // put the radio into receive mode
  LoRa.receive();

}

void loop() {

  unsigned long currentMillis = millis();

  // Check for incoming LoRa packet
  if (loRaInitialized) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      receivedMessage = "";  // Clear the previous message
      while (LoRa.available()) {
        char c = (char)LoRa.read();
        receivedMessage += c;
      }

      Serial.print("Received from Node: ");
      Serial.println(SN, HEX);  // Print the sender ID in hexadecimal format
      Serial.print("Received Message Length: ");
      Serial.println(receivedMessage.length());  // Print the length of the received message
      Serial.print("Received Message: ");
      Serial.println(receivedMessage);  // Print the raw received message

      newData = true;  // Flag to indicate new data has been received
    }
  } else {
    unsigned long currentTime = millis();
    if (currentTime - initStartTime >= RETRY_INTERVAL) {
      loRaInitialized = initializeLoRa();
      initStartTime = currentTime;  // Reset retry timer

      if (loRaInitialized) {
        Serial.println("LoRa initialization successful!");
      } else {
        Serial.println("Retrying LoRa initialization...");
      }
    }
  }

  // Process and display data if new data is received and the interval has passed
  if (newData && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;  // Update the time of the last data processing

    // Process the received data
    processData();

    // Display the data on OLED
    displayData();

    // sendAcknowledgment to slave
    sendAcknowledgment();

    newData = false;  // Reset the newData flag
  }

  // Update the page every 5 seconds
  if (currentMillis - pageChangeMillis >= pageInterval) {
    pageChangeMillis = currentMillis;
    currentPage = (currentPage % 5) + 1;  // Cycle through pages 1, 2, 3, 4, 5

    // Display data for the current page
    displayData();
  }

  unsigned long rstresp = 0;
  unsigned long rstresp1 = millis();

  if (rstresp1 - rstresp > 500000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }

}
