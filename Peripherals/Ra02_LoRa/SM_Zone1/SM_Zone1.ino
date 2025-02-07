#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include"MapFloat.h"
#include "CO2Sensor.h"
#include <AsyncTCP.h>
#include <U8x8lib.h>
#include<EEPROM.h>
#include <cmath> // Include cmath for mathematical functions
#include <RadioLib.h>

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

// LoRa
#define LORA_MOSI 27 // 23
#define LORA_MISO 19 // 19
#define LORA_SCK 5   // 18
#define LORA_CS 18   // 21

#define LORA_RST 14  // 22
#define LORA_DIO0 26  // 32
#define LORA_DIO1 14 // 35

#define FREQUENCY 915.0
#define BANDWIDTH 250.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1278 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1, SPI, SPISettings());

// Assumed environmental values:
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
float temp(NAN), hum(NAN), pres(NAN);

BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_hPa);
EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_16,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);

////// Function prototype declaration //////
void scanI2CDevices();
void BMEData();

void displayData();
void page1();
void page2();
void page3();
void page4();

void ETcalcul();
void displayDataJson();

void Radiotransmit();
void RadioData1();
void RadioData2();
void LoRafailed();

void sendAcknowledgment();

/// Total environmental calculation declerations
float t4, h4, p4, alti, dewpt, uvIntensity;
float Tme, dn1, dn, num1, enum1, enum2, expon, pdenom, powercal, top1, top2, num2, num3, denom2, powercalc2, expnum1, expnum2, expcal;
float expnum3, expnum4, expcal2, ETcal, Es, RHcal, RHcal2, num5, num6, numcalc, fdenom1, fdenom2, fdenom3, fdenom4, fnum1, fnum2, fnum3, fnum4, fnum5, fnum6, fnum7, fnum8, fnum9, fnum10;
float Tmean;
float U2, P, Ea, delta, gama, ETmax, ETmin;
float Tmax, Tmin;
float h;//h=8.2;//height of the measurement above the ground surface
float T;//t=7.7;//air temp
float z = 840; //3.6;//elevation above sealevel//
//The elevation of the land surface varies from the low point of −418 m at the Dead Sea, to a maximum altitude of 8,848 m at the top of Mount Everest.
//The mean Height of land above sea level is 840 m.”

float Un = 2.4; ////measured wind speed z m above the ground surface
float RHmax, RHmin;
float Rn;//UV//=5.9;//net radiation flux
float G;//=HI//4.5;//sensible heat flux into the soil
float AirTemp;
float LeafTemp;
float voltage, outputvoltage;

int battry = 100;

uint8_t x;
uint8_t y;

float radiation;
float altitudeAngle;
float solarRadiation;
float averageDailyRadiation;

const float latitude = 17.388333;  // Your latitude
const float longitude = 78.230630; // Your longitude
const float maxRadiation = 1000.0; // Max solar radiation at zenith (clear sky conditions)

// Define M_PI if it's not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float ES_BP, ES_AT, ES_RH, RH, ETo, HI;

CO2Sensor co2Sensor(15, 0.99, 100);
const char* co2 = "0.0";

int sl, ET;
float VPD, p1, p2;

int uvLevel;
int refLevel;

float VPDcal();

String message1;
String message2;
String message3;
String message4;
String message5;
String message6;
String message7;
String message8;
String message9;
String message10;
String receivedRssi;
String receivedMessage;

String SenderIDStr = "1";  // String containing the integer value
int SenderID = SenderIDStr.toInt();  // Convert String to int

bool newData = false;
bool initializeLoRa();
bool ackReceived = false;
bool loRaInitialized = false;
bool waitForAcknowledgment();

// Global variables for I2C scanning
unsigned long lastI2CScan = 0;
unsigned long I2CScanInterval = 30000; // Interval for I2C scan (30 seconds)

const long interval = 3000;  // Interval at which to perform action (in milliseconds)
const long pageInterval = 5000;  // Interval to switch pages (5 seconds)

unsigned long previousMillis = 0;
unsigned long pageChangeMillis = 0;

int sendState = 0;
int currentPage = 1;

const unsigned long RETRY_INTERVAL = 5000;  // Interval between retries (5 seconds)

unsigned long initStartTime;

// displayData used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

/*
-2: The module is not responding.
-3: The selected frequency is invalid.
-4: The bandwidth is invalid.
-5: The spreading factor is invalid.
-6: The coding rate is invalid.*/

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // initialize SX1276 with default settings
  Serial.print(F("[SX1278] Initializing ... "));

  int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, OUTPUT_POWER, PREAMBLE_LEN, GAIN);
  // int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);

  }
  Serial.print(F("Waiting for incoming transmission ... "));

  Wire.begin();

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }

  u8x8.begin();
}

unsigned long lastMsg = 0;     // Declare outside loop to preserve value
unsigned long lastMsg01 = 0;   // Declare outside loop to preserve value
unsigned long rstresp = 0;     // Declare outside loop to preserve value

void loop()
{
  unsigned long currentMillis = millis();
  // Check for incoming LoRa packet if LoRa is initialized
  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print data of the packet
    Serial.print(F("Data:\t\t\t"));
    Serial.println(str);

    // print RSSI (Received Signal Strength Indicator) of the last received packet
    Serial.print(F("RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print SNR (Signal-to-Noise Ratio) of the last received packet
    Serial.print(F("SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error of the last received packet
    Serial.print(F("Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));
    Serial.println();

    delay(5000);

  }

  unsigned long nowon = millis();
  if (nowon - lastMsg > 1000) {
    lastMsg = nowon;
    Serial.print("VPD: ");
    Serial.print(VPDcal());
    Serial.println(" kPa");
    delay(2000);  // Adjust delay if necessary
  }

  unsigned long now1 = millis();
  if (now1 - lastMsg01 > 5000) {
    lastMsg01 = now1;

    // sendAcknowledgment to master
    sendAcknowledgment();

  }

  // Perform I2C scan at intervals
  if (millis() - lastI2CScan > I2CScanInterval) {
    lastI2CScan = millis();
    scanI2CDevices();
  }

  // Update the page every 5 seconds
  if (currentMillis - pageChangeMillis >= pageInterval) {
    pageChangeMillis = currentMillis;
    currentPage = (currentPage % 5) + 1;  // Cycle through pages 1, 2, 3, 4, 5

    // Display data for the current page
    displayData();
  }

  unsigned long rstresp1 = millis();
  if (rstresp1 - rstresp > 600000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }
}
