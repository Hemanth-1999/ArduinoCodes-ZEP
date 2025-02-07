#include <LoRa.h>
#include <U8x8lib.h>  // Make sure you have the U8x8 OLED library included
#include <ArduinoJson.h>
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

int battry = 100;

String receivedRssi;
String receivedMessage;

bool newData = false;

int  AT, RH, BP, VPD, HI, Co2, ET, DP, SL, SR;
String  AT_l, RH_l, BP_l, VPD_l, HI_l, Co2_l, ET_l, DP_l, SL_l, SR_l;

void page1();
void page2();
void page3();
void page4();
void LoRafailed();
void processData();
void displayData(String message);

const int WATCHDOG_TIMEOUT = 30;  // Timeout in seconds

unsigned long previousMillis = 0;
const long interval = 3000;  // Interval at which to perform action (in milliseconds)

void setup() {
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(115200);
  while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);

  // Initialize the Watchdog Timer
  esp_task_wdt_init(WATCHDOG_TIMEOUT, true);  // Timeout in seconds, panic on reset
  esp_task_wdt_add(NULL);  // Add the current task (main loop) to the watchdog

  Serial.println("Watchdog Timer initialized.");

  // Initialize the OLED
  u8x8.begin();
  u8x8.setPowerSave(0);

  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.println("LoRa Receiver");
  u8x8.drawString(0, 1, "LoRa Receiver");

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    while (LoRa.available()) {
      receivedMessage = (char)LoRa.read();
    }

    newData = true; // Indicate new data has been received
    processData();
    displayData();
  }

  unsigned long currentMillis = millis();
  esp_task_wdt_reset();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Received Message: ");
    Serial.println(receivedMessage);  // Print the raw received message
  }
}
