//************* https://www.instructables.com/Coding-Timers-and-Delays-in-Arduino/
//*************** https://www.viralsciencecreativity.com/post/arduino-relay-trigger-at-specific-time-ds3231-rtc-module

///////************* LIBRARY *************///////

#include <Arduino.h>
#include <ArduinoJson.h>
#include <MemoryFree.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SparkFunTSL2561.h>
#include <SoftwareSerial.h>

#include <ML8511.h>
#define UVOUT 15
#define REF_3V3  4

///////************* DECLERATION *************///////

boolean gain;
unsigned int ms;

bool a;
char Sensordata[1024];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";
const char* mqtt_server = "192.168.0.173";
const char* mqttUsername = "Zeptogreens";
const char* mqttpsw = "123456";
const char* msg = "Sensor Values";
/* Example code for the Adafruit TCS34725 breakout library */

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

SFE_TSL2561 light;

StaticJsonDocument <500> doc1;
StaticJsonDocument<500> manualdoc;
StaticJsonDocument<500> doc3;
char tempString[1024];
//int c = 20;
RTC_DS3231 rtc;
long seconds = 00;
long minutes = 00;
long hours = 00;

uint16_t hourss;
uint16_t minrc;
uint16_t secsrc;
uint16_t Monthrc;
uint16_t Daterc;

uint16_t hourrc;
uint16_t minutess;
uint16_t secondss;

int hourupg;
int minupg;
uint8_t secslive;
int Monthlive;
int Datelive;
int Yearlive;
int val;

int Yearrc;
int hourok;
int minok;
int secok;
//BH1750 lightMeter;
String endchar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));
unsigned long asyncDelay = 50;
int delayLength = 50;
long duration = 0;
int readlux = 0;
int readlux1 = 0;
int readlux2 = 0;
int readlux3 = 0;
int readlux4 = 0;
int readlux5 = 0;

String data_from_display = "";
String dfd;
String str;
String feedBack;
char buff[2000];
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftHr;
bool status1;
bool status2;
bool status3;
bool status4;
bool status5;

bool zone1;
bool zone2;
bool zone3;
bool zone4;
bool zone5;

unsigned long currentMillis  = millis();

void sendData(String );

//////////////////////////////////////////////// ZONE-1 RELAY ////////////////////////////////////////////////

#define light1 2
#define light2 3
#define light3 4
#define light4 5

//////////////////////////////////////////////// ZONE-2 RELAY ////////////////////////////////////////////////

#define light5 6
#define light6 7
#define light7 8
#define light8 9

//////////////////////////////////////////////// ZONE-3 RELAY ////////////////////////////////////////////////

#define light9 10
#define light10 11
#define light11 12
#define light12 39

//////////////////////////////////////////////// ZONE-4 RELAY ////////////////////////////////////////////////

#define light13 40
#define light14 41
#define light15 42
#define light16 43

//////////////////////////////////////////////// ZONE-5 RELAY ////////////////////////////////////////////////

#define light17 44
#define light18 45
#define light19 46
#define light20 47

#define light21 48
#define light22 49
#define light23 50
#define light24 51
#define light25 22

StaticJsonDocument<200> doc;
bool okk = true;
bool checkflag;
bool checkflag1;
bool checkflag2;
bool checkflag3;

///////////////////////////////////////////////////// Auto ZONE1 /////////////////////////////////////////////////////

uint8_t EepromZone1OnHr   = 200;
uint8_t EepromZone1OnMin  = 202;
uint8_t EepromZone1OffHr  = 204;
uint8_t EepromZone1OffMin = 206;

///////////////////////////////////////////////////// Auto ZONE2 /////////////////////////////////////////////////////

uint8_t EepromZone2OnHr   = 300;
uint8_t EepromZone2OnMin  = 302;
uint8_t EepromZone2OffHr  = 304;
uint8_t EepromZone2OffMin = 306;

///////////////////////////////////////////////////// Auto ZONE3 /////////////////////////////////////////////////////

uint8_t EepromZone3OnHr   = 308;
uint8_t EepromZone3OnMin  = 310;
uint8_t EepromZone3OffHr  = 312;
uint8_t EepromZone3OffMin = 314;

///////////////////////////////////////////////////// Auto ZONE4 /////////////////////////////////////////////////////

uint8_t EepromZone4OnHr   = 316;
uint8_t EepromZone4OnMin  = 318;
uint8_t EepromZone4OffHr  = 320;
uint8_t EepromZone4OffMin = 322;

///////////////////////////////////////////////////// Auto ZONE5 /////////////////////////////////////////////////////

uint8_t EepromZone5OnHr   = 324;
uint8_t EepromZone5OnMin  = 326;
uint8_t EepromZone5OffHr  = 328;
uint8_t EepromZone5OffMin = 330;

/////////////////////////////////////////// Schedule ZONE1 slot1 ///////////////////////////////////////////

uint8_t Eepromsch1OnHr    = 506;
uint8_t Eepromsch1OnMin   = 516;
uint8_t Eepromsch1OffHr   = 526;
uint8_t Eepromsch1OffMin  = 546;

/////////////////////////////////////////// Schedule ZONE1 slot2 ///////////////////////////////////////////

uint8_t Eepromsch2OnHr    = 606;
uint8_t Eepromsch2OnMin   = 616;
uint8_t Eepromsch2OffHr   = 626;
uint8_t Eepromsch2OffMin  = 636;

/////////////////////////////////////////// Schedule ZONE1 slot3 ///////////////////////////////////////////

uint8_t Eepromsch3OnHr    = 646;
uint8_t Eepromsch3OnMin   = 656;
uint8_t Eepromsch3OffHr   = 666;
uint8_t Eepromsch3OffMin  = 676;

/////////////////////////////////////////// Schedule ZONE2 slot1 ///////////////////////////////////////////

uint8_t Eepromsch4OnHr    = 686;
uint8_t Eepromsch4OnMin   = 696;
uint8_t Eepromsch4OffHr   = 706;
uint8_t Eepromsch4OffMin  = 716;

/////////////////////////////////////////// Schedule ZONE2 slot2 ///////////////////////////////////////////

uint8_t Eepromsch5OnHr    = 726;
uint8_t Eepromsch5OnMin   = 728;
uint8_t Eepromsch5OffHr   = 738;
uint8_t Eepromsch5OffMin  = 748;

/////////////////////////////////////////// Schedule ZONE2 slot3 ///////////////////////////////////////////

uint8_t Eepromsch6OnHr    = 750;
uint8_t Eepromsch6OnMin   = 752;
uint8_t Eepromsch6OffHr   = 754;
uint8_t Eepromsch6OffMin  = 756;

/////////////////////////////////////////// Schedule ZONE3 slot1 ///////////////////////////////////////////

uint8_t Eepromsch7OnHr    = 758;
uint8_t Eepromsch7OnMin   = 760;
uint8_t Eepromsch7OffHr   = 762;
uint8_t Eepromsch7OffMin  = 764;

/////////////////////////////////////////// Schedule ZONE3 slot2 ///////////////////////////////////////////

uint8_t Eepromsch8OnHr    = 766;
uint8_t Eepromsch8OnMin   = 768;
uint8_t Eepromsch8OffHr   = 770;
uint8_t Eepromsch8OffMin  = 772;

/////////////////////////////////////////// Schedule ZONE3 slot3 ///////////////////////////////////////////

uint8_t Eepromsch9OnHr    = 774;
uint8_t Eepromsch9OnMin   = 776;
uint8_t Eepromsch9OffHr   = 778;
uint8_t Eepromsch9OffMin  = 780;

/////////////////////////////////////////// Schedule ZONE4 slot1 ///////////////////////////////////////////

uint8_t Eepromsch10OnHr    = 780;
uint8_t Eepromsch10OnMin   = 782;
uint8_t Eepromsch10OffHr   = 784;
uint8_t Eepromsch10OffMin  = 788;

/////////////////////////////////////////// Schedule ZONE4 slot2 ///////////////////////////////////////////

uint8_t Eepromsch11OnHr    = 790;
uint8_t Eepromsch11OnMin   = 792;
uint8_t Eepromsch11OffHr   = 794;
uint8_t Eepromsch11OffMin  = 796;

/////////////////////////////////////////// Schedule ZONE4 slot3 ///////////////////////////////////////////

uint8_t Eepromsch12OnHr    = 798;
uint8_t Eepromsch12OnMin   = 800;
uint8_t Eepromsch12OffHr   = 802;
uint8_t Eepromsch12OffMin  = 804;

/////////////////////////////////////////// Schedule ZONE5 slot1 ///////////////////////////////////////////

uint8_t Eepromsch13OnHr    = 806;
uint8_t Eepromsch13OnMin   = 808;
uint8_t Eepromsch13OffHr   = 810;
uint8_t Eepromsch13OffMin  = 812;

/////////////////////////////////////////// Schedule ZONE5 slot2 ///////////////////////////////////////////

uint8_t Eepromsch14OnHr    = 814;
uint8_t Eepromsch14OnMin   = 816;
uint8_t Eepromsch14OffHr   = 818;
uint8_t Eepromsch14OffMin  = 820;

/////////////////////////////////////////// Schedule ZONE5 slot3 ///////////////////////////////////////////

uint8_t Eepromsch15OnHr    = 822;
uint8_t Eepromsch15OnMin   = 824;
uint8_t Eepromsch15OffHr   = 826;
uint8_t Eepromsch15OffMin  = 828;

/////////////auto/////////////

uint8_t Zone1HrOn;
uint8_t Zone1MinOn;
uint8_t Zone1HrOff;
uint8_t Zone1MinOff;

uint8_t Zone2HrOn;
uint8_t Zone2MinOn;
uint8_t Zone2HrOff;
uint8_t Zone2MinOff;

uint8_t Zone3HrOn;
uint8_t Zone3MinOn;
uint8_t Zone3HrOff;
uint8_t Zone3MinOff;

uint8_t Zone4HrOn;
uint8_t Zone4MinOn;
uint8_t Zone4HrOff;
uint8_t Zone4MinOff;

uint8_t Zone5HrOn;
uint8_t Zone5MinOn;
uint8_t Zone5HrOff;
uint8_t Zone5MinOff;

//////////////////////////////////////////////////// Schedule ZONE1 Slot1

uint8_t sch1OnHr;
uint8_t sch1OnMin;
uint8_t sch1OffHr;
uint8_t sch1OffMin;

//////////////////////////////////////////////////// Schedule ZONE2 Slot2

uint8_t sch2OnHr;
uint8_t sch2OnMin;
uint8_t sch2OffHr;
uint8_t sch2OffMin;

//////////////////////////////////////////////////// Schedule ZONE3 Slot3

uint8_t sch3OnHr;
uint8_t sch3OnMin;
uint8_t sch3OffHr;
uint8_t sch3OffMin;

//////////////////////////////////////////////////// Schedule ZONE4 Slot2

uint8_t sch4OnHr;
uint8_t sch4OnMin;
uint8_t sch4OffHr;
uint8_t sch4OffMin;

//////////////////////////////////////////////////// Schedule ZONE5 Slot2

uint8_t sch5OnHr;
uint8_t sch5OnMin;
uint8_t sch5OffHr;
uint8_t sch5OffMin;

//////////////////////////////////////////////////// Schedule ZONE1 Slot1

uint8_t sch6OnHr;
uint8_t sch6OnMin;
uint8_t sch6OffHr;
uint8_t sch6OffMin;

//////////////////////////////////////////////////// Schedule ZONE2 Slot2

uint8_t sch7OnHr;
uint8_t sch7OnMin;
uint8_t sch7OffHr;
uint8_t sch7OffMin;

//////////////////////////////////////////////////// Schedule ZONE3 Slot3

uint8_t sch8OnHr;
uint8_t sch8OnMin;
uint8_t sch8OffHr;
uint8_t sch8OffMin;

//////////////////////////////////////////////////// Schedule ZONE4 Slot2

uint8_t sch9OnHr;
uint8_t sch9OnMin;
uint8_t sch9OffHr;
uint8_t sch9OffMin;

//////////////////////////////////////////////////// Schedule ZONE5 Slot2

uint8_t sch10OnHr;
uint8_t sch10OnMin;
uint8_t sch10OffHr;
uint8_t sch10OffMin;

//////////////////////////////////////////////////// Schedule ZONE1 Slot1

uint8_t sch11OnHr;
uint8_t sch11OnMin;
uint8_t sch11OffHr;
uint8_t sch11OffMin;

//////////////////////////////////////////////////// Schedule ZONE2 Slot2

uint8_t sch12OnHr;
uint8_t sch12OnMin;
uint8_t sch12OffHr;
uint8_t sch12OffMin;

///////////////////////////////////////////////////// Schedule ZONE3 Slot3

uint8_t sch13OnHr;
uint8_t sch13OnMin;
uint8_t sch13OffHr;
uint8_t sch13OffMin;

//////////////////////////////////////////////////// Schedule ZONE4 Slot2

uint8_t sch14OnHr;
uint8_t sch14OnMin;
uint8_t sch14OffHr;
uint8_t sch14OffMin;

//////////////////////////////////////////////////// Schedule ZONE5 Slot2

uint8_t sch15OnHr;
uint8_t sch15OnMin;
uint8_t sch15OffHr;
uint8_t sch15OffMin;

//void rtcupdte()
//{
//  DateTime now = rtc.now();
//  hourupg = now.hour();
//  minupg = now.minute();
//  secslive = now.second();
//  Datelive = now.day();
//  Monthlive = now.month();
//  Yearlive = now.year();
//}

//void readalldata() {
//  EEPROM.write(200, hourrc);
//  EEPROM.write(206, minrc);
//  EEPROM.write(209, secsrc);
//  EEPROM.write(212, Daterc);
//  EEPROM.write(215, Monthrc);
//  EEPROM.write(218, Yearrc);
//
//  EEPROM.read(200);
//  EEPROM.read(206);
//  EEPROM.read(209);
//  EEPROM.read(212);
//  EEPROM.read(215);
//  EEPROM.read(218);
//}

///////////////////////////////////////////////////   AUTO SAVE TIME   ///////////////////////////////////////////////////

void Zone1savetime()
{
  Zone1HrOn  = othr();
  Zone1MinOn = otmin();
  EEPROM.update(EepromZone1OnHr, Zone1HrOn);
  EEPROM.update(EepromZone1OnMin, Zone1MinOn);

}

void Zone1savedue()
{
  Zone1HrOff  = ofthr();
  Zone1MinOff = oftmin();

  EEPROM.update(EepromZone1OffHr, Zone1HrOff);
  EEPROM.update(EepromZone1OffMin, Zone1MinOff);
}

void EepromReadlght1()
{

  Zone1HrOn   = EEPROM.read(EepromZone1OnHr);
  Zone1MinOn  = EEPROM.read(EepromZone1OnMin);
  Zone1HrOff  = EEPROM.read(EepromZone1OffHr);
  Zone1MinOff = EEPROM.read(EepromZone1OffMin);
}

void Zone2savetime()
{
  Zone2HrOn  = othr();
  Zone2MinOn = otmin();
  EEPROM.update(EepromZone2OnHr, Zone2HrOn);
  EEPROM.update(EepromZone2OnMin, Zone2MinOn);
}

void Zone2savedue()
{
  Zone2HrOff  = ofthr();
  Zone2MinOff = oftmin();

  EEPROM.update(EepromZone2OffHr, Zone2HrOff);
  EEPROM.update(EepromZone2OffMin, Zone2MinOff);
}

void EepromReadlght2()
{

  Zone2HrOn   = EEPROM.read(EepromZone2OnHr);
  Zone2MinOn  = EEPROM.read(EepromZone2OnMin);
  Zone2HrOff  = EEPROM.read(EepromZone2OffHr);
  Zone2MinOff = EEPROM.read(EepromZone2OffMin);
}

void Zone3savetime()
{
  Zone3HrOn  = othr();
  Zone3MinOn = otmin();
  EEPROM.update(EepromZone3OnHr, Zone3HrOn);
  EEPROM.update(EepromZone3OnMin, Zone3MinOn);

}

void Zone3savedue()
{
  Zone3HrOff  = ofthr();
  Zone3MinOff = oftmin();

  EEPROM.update(EepromZone3OffHr, Zone3HrOff);
  EEPROM.update(EepromZone3OffMin, Zone3MinOff);
}

void EepromReadlght3()
{

  Zone3HrOn   = EEPROM.read(EepromZone3OnHr);
  Zone3MinOn  = EEPROM.read(EepromZone3OnMin);
  Zone3HrOff  = EEPROM.read(EepromZone3OffHr);
  Zone3MinOff = EEPROM.read(EepromZone3OffMin);
}

void Zone4savetime()
{
  Zone4HrOn  = othr();
  Zone4MinOn = otmin();
  EEPROM.update(EepromZone4OnHr, Zone4HrOn);
  EEPROM.update(EepromZone4OnMin, Zone4MinOn);

}

void Zone4savedue()
{
  Zone4HrOff  = ofthr();
  Zone4MinOff = oftmin();

  EEPROM.update(EepromZone4OffHr, Zone4HrOff);
  EEPROM.update(EepromZone4OffMin, Zone4MinOff);
}

void EepromReadlght4()
{

  Zone4HrOn   = EEPROM.read(EepromZone4OnHr);
  Zone4MinOn  = EEPROM.read(EepromZone4OnMin);
  Zone4HrOff  = EEPROM.read(EepromZone4OffHr);
  Zone4MinOff = EEPROM.read(EepromZone4OffMin);
}

void Zone5savetime()
{
  Zone5HrOn  = othr();
  Zone5MinOn = otmin();
  EEPROM.update(EepromZone5OnHr, Zone5HrOn);
  EEPROM.update(EepromZone5OnMin, Zone5MinOn);

}

void Zone5savedue()
{
  Zone5HrOff  = ofthr();
  Zone5MinOff = oftmin();

  EEPROM.update(EepromZone5OffHr, Zone5HrOff);
  EEPROM.update(EepromZone5OffMin, Zone5MinOff);
}

void EepromReadlght5()
{

  Zone5HrOn   = EEPROM.read(EepromZone5OnHr);
  Zone5MinOn  = EEPROM.read(EepromZone5OnMin);
  Zone5HrOff  = EEPROM.read(EepromZone5OffHr);
  Zone5MinOff = EEPROM.read(EepromZone5OffMin);
}

//////////////////////////////////////////  SCHEDULE MODE TIME  //////////////////////////////////////////

void sch1savetime() {
  sch1OnHr  = othr();
  sch1OnMin = otmin();
  EEPROM.update(Eepromsch1OnHr, sch1OnHr);
  EEPROM.update(Eepromsch1OnMin, sch1OnMin);
}

void sch1savedue() {
  sch1OffHr  = ofthr();
  sch1OffMin = oftmin();
  EEPROM.update(Eepromsch1OffHr, sch1OffHr);
  EEPROM.update(Eepromsch1OffMin, sch1OffMin);

}

void EepromReadsch1()
{
  sch1OnHr   = EEPROM.read(Eepromsch1OnHr);
  sch1OnMin  = EEPROM.read(Eepromsch1OnMin);
  sch1OffHr  = EEPROM.read(Eepromsch1OffHr);
  sch1OffMin = EEPROM.read(Eepromsch1OffMin);
}

void sch2savetime() {
  sch2OnHr  = othr();
  sch2OnMin = otmin();
  EEPROM.update(Eepromsch2OnHr, sch2OnHr);
  EEPROM.update(Eepromsch2OnMin, sch2OnMin);
}

void sch2savedue() {
  sch2OffHr  = ofthr();
  sch2OffMin = oftmin();
  EEPROM.update(Eepromsch2OffHr, sch2OffHr);
  EEPROM.update(Eepromsch2OffMin, sch2OffMin);

}

void EepromReadsch2()
{
  sch2OnHr   = EEPROM.read(Eepromsch2OnHr);
  sch2OnMin  = EEPROM.read(Eepromsch2OnMin);
  sch2OffHr  = EEPROM.read(Eepromsch2OffHr);
  sch2OffMin = EEPROM.read(Eepromsch2OffMin);
}

////////////////////////////////////////////////

void sch3savetime() {
  sch3OnHr  = othr();
  sch3OnMin = otmin();
  EEPROM.update(Eepromsch3OnHr, sch3OnHr);
  EEPROM.update(Eepromsch3OnMin, sch3OnMin);
}

void sch3savedue() {
  sch3OffHr  = ofthr();
  sch3OffMin = oftmin();
  EEPROM.update(Eepromsch3OffHr, sch3OffHr);
  EEPROM.update(Eepromsch3OffMin, sch3OffMin);

}

void EepromReadsch3() {
  sch3OnHr   = EEPROM.read(Eepromsch3OnHr);
  sch3OnMin  = EEPROM.read(Eepromsch3OnMin);
  sch3OffHr  = EEPROM.read(Eepromsch3OffHr);
  sch3OffMin = EEPROM.read(Eepromsch3OffMin);
}

/////////////////////////////////////////////////

void sch4savetime() {
  sch4OnHr  = othr();
  sch4OnMin = otmin();
  EEPROM.update(Eepromsch4OnHr, sch4OnHr);
  EEPROM.update(Eepromsch4OnMin, sch4OnMin);
}

void sch4savedue() {
  sch4OffHr  = ofthr();
  sch4OffMin = oftmin();
  EEPROM.update(Eepromsch4OffHr, sch4OffHr);
  EEPROM.update(Eepromsch4OffMin, sch4OffMin);

}

void EepromReadsch4()
{
  sch4OnHr   = EEPROM.read(Eepromsch4OnHr);
  sch4OnMin  = EEPROM.read(Eepromsch4OnMin);
  sch4OffHr  = EEPROM.read(Eepromsch4OffHr);
  sch4OffMin = EEPROM.read(Eepromsch4OffMin);
}

/////////////////////////////////////////////////

void sch5savetime() {
  sch5OnHr  = othr();
  sch5OnMin = otmin();
  EEPROM.update(Eepromsch5OnHr, sch5OnHr);
  EEPROM.update(Eepromsch5OnMin, sch5OnMin);
}

void sch5savedue() {
  sch5OffHr  = ofthr();
  sch5OffMin = oftmin();
  EEPROM.update(Eepromsch5OffHr, sch5OffHr);
  EEPROM.update(Eepromsch5OffMin, sch5OffMin);

}

void EepromReadsch5()
{
  sch5OnHr   = EEPROM.read(Eepromsch5OnHr);
  sch5OnMin  = EEPROM.read(Eepromsch5OnMin);
  sch5OffHr  = EEPROM.read(Eepromsch5OffHr);
  sch5OffMin = EEPROM.read(Eepromsch5OffMin);
}

//////////////////////////////////////////  SCHEDULE MODE TIME  //////////////////////////////////////////

void sch6savetime() {
  sch6OnHr  = othr();
  sch6OnMin = otmin();
  EEPROM.update(Eepromsch6OnHr, sch6OnHr);
  EEPROM.update(Eepromsch6OnMin, sch6OnMin);
}

void sch6savedue() {
  sch6OffHr  = ofthr();
  sch6OffMin = oftmin();
  EEPROM.update(Eepromsch6OffHr, sch6OffHr);
  EEPROM.update(Eepromsch6OffMin, sch6OffMin);

}

void EepromReadsch6()
{
  sch6OnHr   = EEPROM.read(Eepromsch6OnHr);
  sch6OnMin  = EEPROM.read(Eepromsch6OnMin);
  sch6OffHr  = EEPROM.read(Eepromsch6OffHr);
  sch6OffMin = EEPROM.read(Eepromsch6OffMin);
}

void sch7savetime() {
  sch7OnHr  = othr();
  sch7OnMin = otmin();
  EEPROM.update(Eepromsch7OnHr, sch7OnHr);
  EEPROM.update(Eepromsch7OnMin, sch7OnMin);
}

void sch7savedue() {
  sch7OffHr  = ofthr();
  sch7OffMin = oftmin();
  EEPROM.update(Eepromsch7OffHr, sch7OffHr);
  EEPROM.update(Eepromsch7OffMin, sch7OffMin);

}

void EepromReadsch7()
{
  sch7OnHr   = EEPROM.read(Eepromsch7OnHr);
  sch7OnMin  = EEPROM.read(Eepromsch7OnMin);
  sch7OffHr  = EEPROM.read(Eepromsch7OffHr);
  sch7OffMin = EEPROM.read(Eepromsch7OffMin);
}

////////////////////////////////////////////////

void sch8savetime() {
  sch8OnHr  = othr();
  sch8OnMin = otmin();
  EEPROM.update(Eepromsch8OnHr, sch8OnHr);
  EEPROM.update(Eepromsch8OnMin, sch8OnMin);
}

void sch8savedue() {
  sch8OffHr  = ofthr();
  sch8OffMin = oftmin();
  EEPROM.update(Eepromsch8OffHr, sch8OffHr);
  EEPROM.update(Eepromsch8OffMin, sch8OffMin);

}

void EepromReadsch8() {
  sch8OnHr   = EEPROM.read(Eepromsch8OnHr);
  sch8OnMin  = EEPROM.read(Eepromsch8OnMin);
  sch8OffHr  = EEPROM.read(Eepromsch8OffHr);
  sch8OffMin = EEPROM.read(Eepromsch8OffMin);
}

/////////////////////////////////////////////////

void sch9savetime() {
  sch9OnHr  = othr();
  sch9OnMin = otmin();
  EEPROM.update(Eepromsch9OnHr, sch9OnHr);
  EEPROM.update(Eepromsch9OnMin, sch9OnMin);
}

void sch9savedue() {
  sch9OffHr  = ofthr();
  sch9OffMin = oftmin();
  EEPROM.update(Eepromsch9OffHr, sch9OffHr);
  EEPROM.update(Eepromsch9OffMin, sch9OffMin);

}

void EepromReadsch9()
{
  sch9OnHr   = EEPROM.read(Eepromsch9OnHr);
  sch9OnMin  = EEPROM.read(Eepromsch9OnMin);
  sch9OffHr  = EEPROM.read(Eepromsch9OffHr);
  sch9OffMin = EEPROM.read(Eepromsch9OffMin);
}

/////////////////////////////////////////////////

void sch10savetime() {
  sch10OnHr  = othr();
  sch10OnMin = otmin();
  EEPROM.update(Eepromsch10OnHr, sch10OnHr);
  EEPROM.update(Eepromsch10OnMin, sch10OnMin);
}

void sch10savedue() {
  sch10OffHr  = ofthr();
  sch10OffMin = oftmin();
  EEPROM.update(Eepromsch10OffHr, sch10OffHr);
  EEPROM.update(Eepromsch10OffMin, sch10OffMin);

}

void EepromReadsch10()
{
  sch10OnHr   = EEPROM.read(Eepromsch10OnHr);
  sch10OnMin  = EEPROM.read(Eepromsch10OnMin);
  sch10OffHr  = EEPROM.read(Eepromsch10OffHr);
  sch10OffMin = EEPROM.read(Eepromsch10OffMin);
}

void sch11savetime() {
  sch11OnHr  = othr();
  sch11OnMin = otmin();
  EEPROM.update(Eepromsch11OnHr, sch11OnHr);
  EEPROM.update(Eepromsch11OnMin, sch11OnMin);
}

void sch11savedue() {
  sch11OffHr  = ofthr();
  sch11OffMin = oftmin();
  EEPROM.update(Eepromsch11OffHr, sch11OffHr);
  EEPROM.update(Eepromsch11OffMin, sch11OffMin);

}

void EepromReadsch11()
{
  sch11OnHr   = EEPROM.read(Eepromsch11OnHr);
  sch11OnMin  = EEPROM.read(Eepromsch11OnMin);
  sch11OffHr  = EEPROM.read(Eepromsch11OffHr);
  sch11OffMin = EEPROM.read(Eepromsch11OffMin);
}

void sch12savetime() {
  sch12OnHr  = othr();
  sch12OnMin = otmin();
  EEPROM.update(Eepromsch12OnHr, sch12OnHr);
  EEPROM.update(Eepromsch12OnMin, sch12OnMin);
}

void sch12savedue() {
  sch12OffHr  = ofthr();
  sch12OffMin = oftmin();
  EEPROM.update(Eepromsch12OffHr, sch12OffHr);
  EEPROM.update(Eepromsch12OffMin, sch12OffMin);

}

void EepromReadsch12()
{
  sch12OnHr   = EEPROM.read(Eepromsch12OnHr);
  sch12OnMin  = EEPROM.read(Eepromsch12OnMin);
  sch12OffHr  = EEPROM.read(Eepromsch12OffHr);
  sch12OffMin = EEPROM.read(Eepromsch12OffMin);
}

void sch13savetime() {
  sch13OnHr  = othr();
  sch13OnMin = otmin();
  EEPROM.update(Eepromsch13OnHr, sch13OnHr);
  EEPROM.update(Eepromsch13OnMin, sch13OnMin);
}

void sch13savedue() {
  sch13OffHr  = ofthr();
  sch13OffMin = oftmin();
  EEPROM.update(Eepromsch13OffHr, sch13OffHr);
  EEPROM.update(Eepromsch13OffMin, sch13OffMin);

}

void EepromReadsch13() {
  sch13OnHr   = EEPROM.read(Eepromsch13OnHr);
  sch13OnMin  = EEPROM.read(Eepromsch13OnMin);
  sch13OffHr  = EEPROM.read(Eepromsch13OffHr);
  sch13OffMin = EEPROM.read(Eepromsch13OffMin);
}

void sch14savetime() {
  sch14OnHr  = othr();
  sch14OnMin = otmin();
  EEPROM.update(Eepromsch14OnHr, sch14OnHr);
  EEPROM.update(Eepromsch14OnMin, sch14OnMin);
}

void sch14savedue() {
  sch14OffHr  = ofthr();
  sch14OffMin = oftmin();
  EEPROM.update(Eepromsch14OffHr, sch14OffHr);
  EEPROM.update(Eepromsch14OffMin, sch14OffMin);

}

void EepromReadsch14()
{
  sch14OnHr   = EEPROM.read(Eepromsch14OnHr);
  sch14OnMin  = EEPROM.read(Eepromsch14OnMin);
  sch14OffHr  = EEPROM.read(Eepromsch14OffHr);
  sch14OffMin = EEPROM.read(Eepromsch14OffMin);
}

void sch15savetime() {
  sch15OnHr  = othr();
  sch15OnMin = otmin();
  EEPROM.update(Eepromsch15OnHr, sch15OnHr);
  EEPROM.update(Eepromsch15OnMin, sch15OnMin);
}

void sch15savedue() {
  sch15OffHr  = ofthr();
  sch15OffMin = oftmin();
  EEPROM.update(Eepromsch15OffHr, sch15OffHr);
  EEPROM.update(Eepromsch15OffMin, sch15OffMin);

}

void EepromReadsch15()
{
  sch15OnHr   = EEPROM.read(Eepromsch15OnHr);
  sch15OnMin  = EEPROM.read(Eepromsch15OnMin);
  sch15OffHr  = EEPROM.read(Eepromsch15OffHr);
  sch15OffMin = EEPROM.read(Eepromsch15OffMin);
}

/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  val = dfd.indexOf("s") + 1;
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}
void timeMin() {

  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  val = cSmn.indexOf("n") + 1;
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
}
void timeHr() {
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1;
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt();
  hourrc = hrRc;


}
void timeDt() {
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;

  val = cSmnt.indexOf("m") + 1;
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt();
  Monthrc = mntRc;

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt();
  Yearrc = yrRc;
}

int othr() {
  int val = dfd.indexOf("g") + 1;
  dfd.remove(0, val);
  return (otHr);
}

int otmin() {
  char *nt = strtok(buff, "g");//off time hr val
  str = nt;
  int val = str.indexOf("f") + 1;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);
  //  ////Serial.println(otMin);
}

int ofthr() {
  char *fr = strtok(buff, "f");
  str = fr;
  int val = str.indexOf("e") + 1; //  on time min val
  str.remove(0, val);
  oftHr = str.toInt();
  return (oftHr);
}

int oftmin() {
  char *ui = strtok(buff, "e");//on time hr val
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}

class Switcher
{
    // class member variables
    byte relayPin; // number of pin for relay
    long OnTime;
    long OffTime;

    int zone1; // set relay state (active HIGH)
    int zone2; // set relay state (active HIGH)
    int zone3; // set relay state (active HIGH)
    int zone4; // set relay state (active HIGH)
    int zone5; // set relay state (active HIGH)

    unsigned long Z1previousMillis;// set time since last update
    unsigned long Z2previousMillis;// set time since last update
    unsigned long Z3previousMillis;// set time since last update
    unsigned long Z4previousMillis;// set time since last update
    unsigned long Z5previousMillis;// set time since last update
  public:
    Switcher(byte pin)
    {
      relayPin = pin;
    }

    void begin(long on, long off)
    {
      pinMode(relayPin, OUTPUT);
      OnTime = on;
      OffTime = off;

      zone1 = HIGH;
      zone2 = HIGH;
      zone3 = HIGH;
      zone4 = HIGH;
      zone5 = HIGH;

      Z1previousMillis = 0;
      status1 = true;

      Z2previousMillis = 0;
      status2 = true;

      Z3previousMillis = 0;
      status3 = true;

      Z4previousMillis = 0;
      status4 = true;

      Z5previousMillis = 0;
      status5 = true;
    }

    void Z1Update() {
      unsigned long Z1currentMillis  = millis();
      if ((Z1currentMillis < OffTime) && (status1 == true))
      {
        Serial.println("");
        Z1currentMillis = OffTime;
        status1 = false;
        zone1 = true;
      }

      if ((zone1 == HIGH) && (Z1currentMillis - Z1previousMillis >= OffTime))
      {
        unsigned long Z1currentMillis = millis();
        zone1 = LOW; // Turn it off
        Serial.println("relay1 ON");
        Z1previousMillis = Z1currentMillis; // Remember the time
        digitalWrite(light1, zone1);
        digitalWrite(light2, zone1);
        digitalWrite(light3, zone1);
        digitalWrite(light4, zone1);
      }
      else if ((zone1 == LOW) && (Z1currentMillis - Z1previousMillis >= OnTime))
      {
        zone1 = HIGH ; // turn it on
        Z1previousMillis = Z1currentMillis;
        Serial.println("relay1 OFF");
        digitalWrite(light1, zone1);
        digitalWrite(light2, zone1);
        digitalWrite(light3, zone1);
        digitalWrite(light4, zone1);
      }
    }

    void Z2Update() {
      unsigned long Z2currentMillis  = millis();
      if ((Z2currentMillis < OffTime) && (status2 == true))
      {
        Serial.println("");
        Z2currentMillis = 5000 - OffTime;
        status2 = false;
        zone2 = true;
      }

      if ((zone2 == HIGH) && (Z2currentMillis - Z2previousMillis >= OffTime))
      {
        unsigned long Z2currentMillis = millis();
        zone2 = LOW; // Turn it off
        Serial.println("relay2 ON");
        Z2previousMillis = Z2currentMillis; // Remember the time
        digitalWrite(light5, zone2);
        digitalWrite(light6, zone2);
        digitalWrite(light7, zone2);
        digitalWrite(light8, zone2);
      }
      else if ((zone2 == LOW) && (Z2currentMillis - Z2previousMillis >= OnTime))
      {
        zone2 = HIGH ; // turn it on
        Z2previousMillis = Z2currentMillis;
        Serial.println("relay2 OFF");
        digitalWrite(light5, zone2);
        digitalWrite(light6, zone2);
        digitalWrite(light7, zone2);
        digitalWrite(light8, zone2);
      }
    }

    void Z3Update() {
      unsigned long Z3currentMillis  = millis();
      if ((Z3currentMillis < OffTime) && (status3 == true))
      {
        Serial.println("");
        Z3currentMillis = 5000 - OffTime;
        status3 = false;
        zone3 = true;
      }

      if ((zone3 == HIGH) && (Z3currentMillis - Z3previousMillis >= OffTime))
      {
        unsigned long Z3currentMillis = millis();
        zone3 = LOW; // Turn it off
        Serial.println("relay3 ON");
        Z3previousMillis = Z3currentMillis; // Remember the time
        digitalWrite(light9, zone3);
        digitalWrite(light10, zone3);
        digitalWrite(light11, zone3);
        digitalWrite(light12, zone3);
      }
      else if ((zone3 == LOW) && (Z3currentMillis - Z3previousMillis >= OnTime))
      {
        zone3 = HIGH ; // turn it on
        Z3previousMillis = Z3currentMillis;
        Serial.println("relay3 OFF");
        digitalWrite(light9, zone3);
        digitalWrite(light10, zone3);
        digitalWrite(light11, zone3);
        digitalWrite(light12, zone3);
      }
    }

    void Z4Update() {
      unsigned long Z4currentMillis  = millis();
      if ((Z4currentMillis < OffTime) && (status4 == true))
      {
        Serial.println("");
        Z4currentMillis = 5000 - OffTime;
        status4 = false;
        zone4 = true;
      }

      if ((zone4 == HIGH) && (Z4currentMillis - Z4previousMillis >= OffTime))
      {
        unsigned long Z4currentMillis = millis();
        zone4 = LOW; // Turn it off
        Serial.println("relay4 ON");
        Z4previousMillis = Z4currentMillis; // Remember the time
        digitalWrite(light13, zone4);
        digitalWrite(light14, zone4);
        digitalWrite(light15, zone4);
        digitalWrite(light16, zone4);
      }
      else if ((zone4 == LOW) && (Z4currentMillis - Z4previousMillis >= OnTime))
      {
        zone4 = HIGH ; // turn it on
        Z4previousMillis = Z4currentMillis;
        Serial.println("relay4 OFF");
        digitalWrite(light13, zone4);
        digitalWrite(light14, zone4);
        digitalWrite(light15, zone4);
        digitalWrite(light16, zone4);
      }
    }

    void Z5Update() {
      unsigned long Z5currentMillis  = millis();
      if ((Z5currentMillis < OffTime) && (status5 == true))
      {
        Serial.println("");
        Z5currentMillis = 5000 - OffTime;
        status5 = false;
        zone5 = true;
      }

      if ((zone5 == HIGH) && (Z5currentMillis - Z5previousMillis >= OffTime))
      {
        unsigned long Z5currentMillis = millis();
        zone5 = LOW; // Turn it off

        Serial.println("relay5 ON");
        Z5previousMillis = Z5currentMillis; // Remember the time
        digitalWrite(light17, zone5);
        digitalWrite(light18, zone5);
        digitalWrite(light19, zone5);
        digitalWrite(light20, zone5);
      }
      else if ((zone5 == LOW) && (Z5currentMillis - Z5previousMillis >= OnTime))
      {
        zone5 = HIGH ; // turn it on
        Z5previousMillis = Z5currentMillis;
        Serial.println("relay5 OFF");
        digitalWrite(light17, zone5);
        digitalWrite(light18, zone5);
        digitalWrite(light19, zone5);
        digitalWrite(light20, zone5);
      }
    }
};

Switcher ZONE1(light1);
Switcher ZONE2(light5);
Switcher ZONE3(light9);
Switcher ZONE4(light13);
Switcher ZONE5(light17);

bool ZONE1bool;
bool ZONE2bool;
bool ZONE3bool;
bool ZONE4bool;
bool ZONE5bool;

void SerialCom() {
  String data_from_display = "";

  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }

  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());

  ///////////////////////////////////////// AUTOMODE ZONE-1 ////////////////////////////

  if ((data_from_display.substring(0, 5) == "glz1"))
  {

    Zone1savetime();
    Zone1savedue();

    Serial.print("otHr: ");
    Serial.println(Zone1HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone1MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone1HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone1MinOff);
    feedBack = "saved";
  }

  ///////////////////////////////////////// AUTOMODE ZONE-2 ////////////////////////////

  if ((data_from_display.substring(0, 5) == "glz2"))
  {

    Zone2savetime();
    Zone2savedue();

    Serial.print("otHr: ");
    Serial.println(Zone2HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone2MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone2HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone2MinOff);
    feedBack = "saved";
  }

  ///////////////////////////////////////// AUTOMODE ZONE-3 ////////////////////////////

  if ((data_from_display.substring(0, 5) == "glz3"))
  {

    Zone3savetime();
    Zone3savedue();

    Serial.print("otHr: ");
    Serial.println(Zone3HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone3MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone3HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone3MinOff);
    feedBack = "saved";
  }

  ///////////////////////////////////////// AUTOMODE ZONE-4 ////////////////////////////

  if ((data_from_display.substring(0, 5) == "glz4"))
  {

    Zone4savetime();
    Zone4savedue();

    Serial.print("otHr: ");
    Serial.println(Zone4HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone4MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone4HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone4MinOff);
    feedBack = "saved";
  }

  ///////////////////////////////////////// AUTOMODE ZONE-5 ////////////////////////////

  if ((data_from_display.substring(0, 5) == "glz5"))
  {

    Zone5savetime();
    Zone5savedue();

    Serial.print("otHr: ");
    Serial.println(Zone5HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone5MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone5HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone5MinOff);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone1 /////////////

  if ((data_from_display.substring(0, 4) == "Zn11"))
  {
    sch1savetime();
    sch1savedue();
    checkflag = true;
    Serial.println("Zone1 Slot1 Schedule Time");

    Serial.println(sch1OnHr);                  //Shift1Clockhr();

    Serial.println(sch1OnMin);                //Shift1Clockmin();

    Serial.println(sch1OffHr);                  //Shift1Clockhr();

    Serial.println(sch1OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone1 /////////////

  if ((data_from_display.substring(0, 4) == "Zn12"))
  {
    sch2savetime();
    sch2savedue();
    checkflag1 = true;
    Serial.println("Zone1 Slot2 Schedule Time");

    Serial.println(sch2OnHr);                  //Shift1Clockhr();

    Serial.println(sch2OnMin);                //Shift1Clockmin();

    Serial.println(sch2OffHr);                  //Shift1Clockhr();

    Serial.println(sch2OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone1 /////////////

  if ((data_from_display.substring(0, 4) == "Zn13"))
  {
    sch3savetime();
    sch3savedue();
    checkflag1 = true;
    Serial.println("Zone1 Slot3 Schedule Time");

    Serial.println(sch3OnHr);                  //Shift1Clockhr();

    Serial.println(sch3OnMin);                //Shift1Clockmin();

    Serial.println(sch3OffHr);                  //Shift1Clockhr();

    Serial.println(sch3OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone2 /////////////

  if ((data_from_display.substring(0, 4) == "Zn21"))
  {
    sch4savetime();
    sch4savedue();
    checkflag1 = true;
    Serial.println("Zone2 Slot1 Schedule Time");

    Serial.println(sch4OnHr);                  //Shift1Clockhr();

    Serial.println(sch4OnMin);                //Shift1Clockmin();

    Serial.println(sch4OffHr);                  //Shift1Clockhr();

    Serial.println(sch4OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone2 /////////////

  if ((data_from_display.substring(0, 4) == "Zn22"))
  {
    sch5savetime();
    sch5savedue();
    checkflag1 = true;
    Serial.println("Zone2 Slot2 Schedule Time");

    Serial.println(sch5OnHr);                  //Shift1Clockhr();

    Serial.println(sch5OnMin);                //Shift1Clockmin();

    Serial.println(sch5OffHr);                  //Shift1Clockhr();

    Serial.println(sch5OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone2 /////////////

  if ((data_from_display.substring(0, 4) == "Zn23"))
  {
    sch6savetime();
    sch6savedue();
    checkflag = true;
    Serial.println("Zone2 Slot3 Schedule Time");

    Serial.println(sch6OnHr);                  //Shift1Clockhr();

    Serial.println(sch6OnMin);                //Shift1Clockmin();

    Serial.println(sch6OffHr);                  //Shift1Clockhr();

    Serial.println(sch6OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone3 /////////////

  if ((data_from_display.substring(0, 4) == "Zn31"))
  {
    sch7savetime();
    sch7savedue();
    checkflag1 = true;
    Serial.println("Zone3 Slot1 Schedule Time");

    Serial.println(sch7OnHr);                  //Shift1Clockhr();

    Serial.println(sch7OnMin);                //Shift1Clockmin();

    Serial.println(sch7OffHr);                  //Shift1Clockhr();

    Serial.println(sch7OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone3 /////////////

  if ((data_from_display.substring(0, 4) == "Zn32"))
  {
    sch8savetime();
    sch8savedue();
    checkflag1 = true;
    Serial.println("Zone3 Slot2 Schedule Time");

    Serial.println(sch8OnHr);                  //Shift1Clockhr();

    Serial.println(sch8OnMin);                //Shift1Clockmin();

    Serial.println(sch8OffHr);                  //Shift1Clockhr();

    Serial.println(sch8OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone3 /////////////

  if ((data_from_display.substring(0, 4) == "Zn33"))
  {
    sch9savetime();
    sch9savedue();
    checkflag1 = true;
    Serial.println("Zone3 Slot3 Schedule Time");

    Serial.println(sch9OnHr);                  //Shift1Clockhr();

    Serial.println(sch9OnMin);                //Shift1Clockmin();

    Serial.println(sch9OffHr);                  //Shift1Clockhr();

    Serial.println(sch9OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone4 /////////////

  if ((data_from_display.substring(0, 4) == "Zn41"))
  {
    sch10savetime();
    sch10savedue();
    checkflag1 = true;
    Serial.println("Zone4 Slot1 Schedule Time");

    Serial.println(sch10OnHr);                  //Shift1Clockhr();

    Serial.println(sch10OnMin);                //Shift1Clockmin();

    Serial.println(sch10OffHr);                  //Shift1Clockhr();

    Serial.println(sch10OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone4 /////////////

  if ((data_from_display.substring(0, 4) == "Zn42"))
  {
    sch11savetime();
    sch11savedue();
    checkflag = true;
    Serial.println("Zone4 Slot2 Schedule Time");

    Serial.println(sch11OnHr);                  //Shift1Clockhr();

    Serial.println(sch11OnMin);                //Shift1Clockmin();

    Serial.println(sch11OffHr);                  //Shift1Clockhr();

    Serial.println(sch11OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone4 /////////////

  if ((data_from_display.substring(0, 4) == "Zn43"))
  {
    sch12savetime();
    sch12savedue();
    checkflag1 = true;
    Serial.println("Zone4 Slot3 Schedule Time");

    Serial.println(sch12OnHr);                  //Shift1Clockhr();

    Serial.println(sch12OnMin);                //Shift1Clockmin();

    Serial.println(sch12OffHr);                  //Shift1Clockhr();

    Serial.println(sch12OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone5 /////////////

  if ((data_from_display.substring(0, 4) == "Zn51"))
  {
    sch13savetime();
    sch13savedue();
    checkflag1 = true;
    Serial.println("Zone5 Slot1 Schedule Time");

    Serial.println(sch13OnHr);                  //Shift1Clockhr();

    Serial.println(sch13OnMin);                //Shift1Clockmin();

    Serial.println(sch13OffHr);                  //Shift1Clockhr();

    Serial.println(sch13OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone5 /////////////

  if ((data_from_display.substring(0, 4) == "Zn52"))
  {
    sch14savetime();
    sch14savedue();
    checkflag1 = true;
    Serial.println("Zone5 Slot2 Schedule Time");

    Serial.println(sch14OnHr);                  //Shift1Clockhr();

    Serial.println(sch14OnMin);                //Shift1Clockmin();

    Serial.println(sch14OffHr);                  //Shift1Clockhr();

    Serial.println(sch14OffMin);
    feedBack = "saved";
  }

  /////////////// Schedule Mode Zone5 /////////////

  if ((data_from_display.substring(0, 4) == "Zn53"))
  {
    sch15savetime();
    sch15savedue();
    checkflag1 = true;
    Serial.println("Zone5 Slot3 Schedule Time");

    Serial.println(sch15OnHr);                  //Shift1Clockhr();

    Serial.println(sch15OnMin);                //Shift1Clockmin();

    Serial.println(sch15OffHr);                  //Shift1Clockhr();

    Serial.println(sch15OffMin);
    feedBack = "saved";
  }


  sendData(data_from_display);
}

//////////////////////////////////////////////////////  MANUAL MODE  //////////////////////////////////////////////////////

void sendData(String dfd) {
    
  if (dfd == "z1mOn")
  {
    pinMode(light1, OUTPUT);
    digitalWrite(light1, LOW);
    feedBack = "z1mOn";
    Serial.println("On");

  }
  if (dfd == "z1mOff")
  {
    pinMode(light1, OUTPUT);
    digitalWrite(light1, HIGH);
    feedBack = "z1mOff";
    Serial.println("Off");
  }

  if (dfd == "z2mOn")
  {
    pinMode(light2, OUTPUT);
    digitalWrite(light2, LOW);
    feedBack = "z2mOn";
    Serial.println("On");

  }
  if (dfd == "z2mOff")
  {
    pinMode(light2, OUTPUT);
    digitalWrite(light2, HIGH);
    feedBack = "z2mOff";
    Serial.println("Off");
  }

  if (dfd == "z3mOn")
  {
    pinMode(light3, OUTPUT);
    digitalWrite(light3, LOW);
    feedBack = "z3mOn";
    Serial.println("On");

  }
  if (dfd == "z3mOff")
  {
    pinMode(light3, OUTPUT);
    digitalWrite(light3, HIGH);
    feedBack = "z3mOff";
    Serial.println("Off");
  }

  if (dfd == "z4mOn")
  {
    pinMode(light4, OUTPUT);
    digitalWrite(light4, LOW);
    feedBack = "z4mOn";
    Serial.println("On");

  }
  if (dfd == "z4mOff")
  {
    pinMode(light4, OUTPUT);
    digitalWrite(light4, HIGH);
    feedBack = "z4mOff";
    Serial.println("Off");
  }

  if (dfd == "z5mOn")
  {
    pinMode(light5, OUTPUT);
    digitalWrite(light5, LOW);
    feedBack = "z5mOn";
    Serial.println("On");

  }
  if (dfd == "z5mOff")
  {
    pinMode(light5, OUTPUT);
    digitalWrite(light5, HIGH);
    feedBack = "z5mOff";
    Serial.println("Off");
  }

  if (dfd == "z6mOn")
  {
    pinMode(light6, OUTPUT);
    digitalWrite(light6, LOW);
    feedBack = "z6mOn";
    Serial.println("On");

  }
  if (dfd == "z6mOff")
  {
    pinMode(light6, OUTPUT);
    digitalWrite(light6, HIGH);
    feedBack = "z6mOff";
    Serial.println("Off");
  }

  if (dfd == "z7mOn")
  {
    pinMode(light7, OUTPUT);
    digitalWrite(light7, LOW);
    feedBack = "z7mOn";
    Serial.println("On");

  }
  if (dfd == "z7mOff")
  {
    pinMode(light7, OUTPUT);
    digitalWrite(light7, HIGH);
    feedBack = "z7mOff";
    Serial.println("Off");
  }

  if (dfd == "z8mOn")
  {
    pinMode(light8, OUTPUT);
    digitalWrite(light8, LOW);
    feedBack = "z8mOn";
    Serial.println("On");

  }
  if (dfd == "z8mOff")
  {
    pinMode(light8, OUTPUT);
    digitalWrite(light8, HIGH);
    feedBack = "z8mOff";
    Serial.println("Off");
  }

  if (dfd == "z9mOn")
  {
    pinMode(light9, OUTPUT);
    digitalWrite(light9, LOW);
    feedBack = "z9mOn";
    Serial.println("On");

  }
  if (dfd == "z9mOff")
  {
    pinMode(light9, OUTPUT);
    digitalWrite(light9, HIGH);
    feedBack = "z9mOff";
    Serial.println("Off");
  }

  if (dfd == "z10mOn")
  {
    pinMode(light10, OUTPUT);
    digitalWrite(light10, LOW);
    feedBack = "z10mOn";
    Serial.println("On");

  }
  if (dfd == "z10mOff")
  {
    pinMode(light10, OUTPUT);
    digitalWrite(light10, HIGH);
    feedBack = "z10mOff";
    Serial.println("Off");
  }

  if (dfd == "z11mOn")
  {
    pinMode(light11, OUTPUT);
    digitalWrite(light11, LOW);
    feedBack = "z11mOn";
    Serial.println("On");

  }
  if (dfd == "z11mOff")
  {
    pinMode(light11, OUTPUT);
    digitalWrite(light11, HIGH);
    feedBack = "z11mOff";
    Serial.println("Off");
  }

  if (dfd == "z12mOn")
  {
    pinMode(light12, OUTPUT);
    digitalWrite(light12, LOW);
    feedBack = "z12mOn";
    Serial.println("On");

  }
  if (dfd == "z12mOff")
  {
    pinMode(light12, OUTPUT);
    digitalWrite(light12, HIGH);
    feedBack = "z12mOff";
    Serial.println("Off");
  }

  if (dfd == "z13mOn")
  {
    pinMode(light13, OUTPUT);
    digitalWrite(light13, LOW);
    feedBack = "z13mOn";
    Serial.println("On");

  }
  if (dfd == "z13mOff")
  {
    pinMode(light13, OUTPUT);
    digitalWrite(light13, HIGH);
    feedBack = "z13mOff";
    Serial.println("Off");
  }

  if (dfd == "z14mOn")
  {
    pinMode(light14, OUTPUT);
    digitalWrite(light14, LOW);
    feedBack = "z14mOn";
    Serial.println("On");

  }
  if (dfd == "z14mOff")
  {
    pinMode(light14, OUTPUT);
    digitalWrite(light14, HIGH);
    feedBack = "z14mOff";
    Serial.println("Off");
  }

  if (dfd == "z15mOn")
  {
    pinMode(light15, OUTPUT);
    digitalWrite(light15, LOW);
    feedBack = "z15mOn";
    Serial.println("On");

  }
  if (dfd == "z15mOff")
  {
    pinMode(light15, OUTPUT);
    digitalWrite(light15, HIGH);
    feedBack = "z15mOff";
    Serial.println("Off");
  }

  if (dfd == "z16mOn")
  {
    pinMode(light16, OUTPUT);
    digitalWrite(light16, LOW);
    feedBack = "z16mOn";
    Serial.println("On");

  }
  if (dfd == "z16mOff")
  {
    pinMode(light16, OUTPUT);
    digitalWrite(light16, HIGH);
    feedBack = "z16mOff";
    Serial.println("Off");
  }

  if (dfd == "z17mOn")
  {
    pinMode(light17, OUTPUT);
    digitalWrite(light17, LOW);
    feedBack = "z17mOn";
    Serial.println("On");

  }
  if (dfd == "z17mOff")
  {
    pinMode(light17, OUTPUT);
    digitalWrite(light17, HIGH);
    feedBack = "z17mOff";
    Serial.println("Off");
  }

  if (dfd == "z18mOn")
  {
    pinMode(light18, OUTPUT);
    digitalWrite(light18, LOW);
    feedBack = "z18mOn";
    Serial.println("On");

  }
  if (dfd == "z18mOff")
  {
    pinMode(light18, OUTPUT);
    digitalWrite(light18, HIGH);
    feedBack = "z18mOff";
    Serial.println("Off");
  }

  if (dfd == "z19mOn")
  {
    pinMode(light19, OUTPUT);
    digitalWrite(light19, LOW);
    feedBack = "z19mOn";
    Serial.println("On");

  }
  if (dfd == "z19mOff")
  {
    pinMode(light19, OUTPUT);
    digitalWrite(light19, HIGH);
    feedBack = "z19mOff";
    Serial.println("Off");
  }

  if (dfd == "z20mOn")
  {
    pinMode(light20, OUTPUT);
    digitalWrite(light20, LOW);
    feedBack = "z20mOn";
    Serial.println("On");

  }
  if (dfd == "z20mOff")
  {
    pinMode(light20, OUTPUT);
    digitalWrite(light20, HIGH);
    feedBack = "z20mOff";
    Serial.println("Off");
  }

  if (dfd == "z21mOn")
  {
    pinMode(light21, OUTPUT);
    digitalWrite(light21, LOW);
    feedBack = "z21mOn";
    Serial.println("On");

  }
  if (dfd == "z21mOff")
  {
    pinMode(light21, OUTPUT);
    digitalWrite(light21, HIGH);
    feedBack = "z21mOff";
    Serial.println("Off");
  }

  if (dfd == "z22mOn")
  {
    pinMode(light22, OUTPUT);
    digitalWrite(light22, LOW);
    feedBack = "z22mOn";
    Serial.println("On");

  }
  if (dfd == "z22mOff")
  {
    pinMode(light22, OUTPUT);
    digitalWrite(light22, HIGH);
    feedBack = "z22mOff";
    Serial.println("Off");
  }

  if (dfd == "z23mOn")
  {
    pinMode(light23, OUTPUT);
    digitalWrite(light23, LOW);
    feedBack = "z23mOn";
    Serial.println("On");

  }
  if (dfd == "z23mOff")
  {
    pinMode(light23, OUTPUT);
    digitalWrite(light23, HIGH);
    feedBack = "z23mOff";
    Serial.println("Off");
  }

  if (dfd == "z24mOn")
  {
    pinMode(light24, OUTPUT);
    digitalWrite(light24, LOW);
    feedBack = "z24mOn";
    Serial.println("On");

  }
  if (dfd == "z24mOff")
  {
    pinMode(light24, OUTPUT);
    digitalWrite(light24, HIGH);
    feedBack = "z24mOff";
    Serial.println("Off");
  }

#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000
  ////////////////////////////////////// ZONE-1 /////////////////////
  if (dfd == "z1on")
  {
    Serial.print("othr: ");
    Serial.println(Zone1HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone1MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone1HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone1MinOff);

    long Zone1HrOnconvOn  = Zone1HrOn * ms_per_hour;
    long Zone1MinOnconvOn = Zone1MinOn * ms_per_min;
    long Zone1TotalOnTime = Zone1HrOnconvOn + Zone1MinOnconvOn;

    long Zone1HrOnconvOff  = Zone1HrOff * ms_per_hour;
    long Zone1MinOnconvOff = Zone1MinOff * ms_per_min;
    long Zone1TotalOffTime = Zone1HrOnconvOff + Zone1MinOnconvOff;

    ZONE1.begin(Zone1TotalOnTime, Zone1TotalOffTime );
    ZONE1bool = true;

    Serial.println("ZONE1 On");

    EepromReadlght1();

    if (dfd == "z1on" ) {
      feedBack = "z1on";
      pinMode(light1, OUTPUT);
      digitalWrite(light1, LOW);
      Serial.println("light1 on");
    }
    if (dfd == "z1off") {
      feedBack = "z1off";
      digitalWrite(light1, HIGH);
      Serial.println("light1 Off");
    }
    if (dfd == "z1on" ) {
      feedBack = "z1on";
      pinMode(light2, OUTPUT);
      digitalWrite(light2, LOW);
      Serial.println("light2 on");
    }
    if (dfd == "z1off") {
      feedBack = "z1off";
      digitalWrite(light2, HIGH);
      Serial.println("light2 Off");
    }
    if (dfd == "z1on" ) {
      feedBack = "z1on";
      pinMode(light3, OUTPUT);
      digitalWrite(light3, LOW);
      Serial.println("light3 on");
    }
    if (dfd == "z1off") {
      feedBack = "z1off";
      digitalWrite(light3, HIGH);
      Serial.println("light3 Off");
    }
    if (dfd == "z1on" ) {
      feedBack = "z1on";
      pinMode(light4, OUTPUT);
      digitalWrite(light4, LOW);
      Serial.println("light4 on");
    }
    if (dfd == "z1off") {
      feedBack = "z1off";
      digitalWrite(light4, HIGH);
      Serial.println("light4 Off");
    }

    zone1off();
    ZONE1bool = false;
    Serial.println("ZONE1 Off");
    feedBack = "z1off";
  }

  ////////////////////////////////////// ZONE-2 /////////////////////
  if (dfd == "z2on")
  {
    Serial.print("othr: ");
    Serial.println(Zone2HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone2MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone2HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone2MinOff);

    long Zone2HrOnconvOn  = Zone2HrOn * ms_per_hour;
    long Zone2MinOnconvOn = Zone2MinOn * ms_per_min;
    long Zone2TotalOnTime = Zone2HrOnconvOn + Zone2MinOnconvOn;

    long Zone2HrOnconvOff  = Zone2HrOff * ms_per_hour;
    long Zone2MinOnconvOff = Zone2MinOff * ms_per_min;
    long Zone2TotalOffTime = Zone2HrOnconvOff + Zone2MinOnconvOff;

    Serial.println("Zone2TotalOnTime");

    Serial.println(Zone2TotalOnTime);

    Serial.println("Zone2TotalOffTime");

    Serial.println(Zone2TotalOffTime);

    ZONE2.begin(Zone2TotalOnTime, Zone2TotalOffTime );
    ZONE2bool = true;

    Serial.println("ZONE2 On");

    EepromReadlght2();

    if (dfd == "z2on" ) {
      feedBack = "z2on";
      pinMode(light5, OUTPUT);
      digitalWrite(light5, LOW);
      Serial.println("light5 on");
    }
    if (dfd == "z2off") {
      feedBack = "z2off";
      digitalWrite(light5, HIGH);
      Serial.println("light5 Off");
    }
    if (dfd == "z2on" ) {
      feedBack = "z2on";
      pinMode(light6, OUTPUT);
      digitalWrite(light6, LOW);
      Serial.println("light6 on");
    }
    if (dfd == "z2off") {
      feedBack = "z2off";
      digitalWrite(light6, HIGH);
      Serial.println("light6 Off");
    }
    if (dfd == "z2on" ) {
      feedBack = "z2on";
      pinMode(light7, OUTPUT);
      digitalWrite(light7, LOW);
      Serial.println("light7 on");
    }
    if (dfd == "z2off") {
      feedBack = "z2off";
      digitalWrite(light7, HIGH);
      Serial.println("light7 Off");
    }
    if (dfd == "z2on" ) {
      feedBack = "z2on";
      pinMode(light8, OUTPUT);
      digitalWrite(light8, LOW);
      Serial.println("light8 on");
    }
    if (dfd == "z2off") {
      feedBack = "z2off";
      digitalWrite(light8, HIGH);
      Serial.println("light8 Off");
    }

    zone2off();
    ZONE2bool = false;
    Serial.println("ZONE2 Off");
    feedBack = "z2off";
  }

  ////////////////////////////////////// ZONE-3 /////////////////////
  if (dfd == "z3on")
  {
    Serial.print("othr: ");
    Serial.println(Zone3HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone3MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone3HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone3MinOff);

    long Zone3HrOnconvOn  = Zone3HrOn * ms_per_hour;
    long Zone3MinOnconvOn = Zone3MinOn * ms_per_min;
    long Zone3TotalOnTime = Zone3HrOnconvOn + Zone3MinOnconvOn;

    long Zone3HrOnconvOff  = Zone3HrOff * ms_per_hour;
    long Zone3MinOnconvOff = Zone3MinOff * ms_per_min;
    long Zone3TotalOffTime = Zone3HrOnconvOff + Zone3MinOnconvOff;

    Serial.println("Zone3TotalOnTime");

    Serial.println(Zone3TotalOnTime);

    Serial.println("Zone3TotalOffTime");

    Serial.println(Zone3TotalOffTime);

    ZONE3.begin(Zone3TotalOnTime, Zone3TotalOffTime );
    ZONE3bool = true;

    Serial.println("ZONE3 On");

    EepromReadlght3();

    if (dfd == "z3on" ) {
      feedBack = "z3on";
      pinMode(light9, OUTPUT);
      digitalWrite(light9, LOW);
      Serial.println("light9 on");
    }
    if (dfd == "z3off") {
      feedBack = "z3off";
      digitalWrite(light9, HIGH);
      Serial.println("light9 Off");
    }
    if (dfd == "z3on" ) {
      feedBack = "z3on";
      pinMode(light10, OUTPUT);
      digitalWrite(light10, LOW);
      Serial.println("light10 on");
    }
    if (dfd == "z3off") {
      feedBack = "z3off";
      digitalWrite(light10, HIGH);
      Serial.println("light10 Off");
    }
    if (dfd == "z3on" ) {
      feedBack = "z3on";
      pinMode(light11, OUTPUT);
      digitalWrite(light11, LOW);
      Serial.println("light11 on");
    }
    if (dfd == "z3off") {
      feedBack = "z3off";
      digitalWrite(light11, HIGH);
      Serial.println("light11 Off");
    }
    if (dfd == "z3on" ) {
      feedBack = "z3on";
      pinMode(light12, OUTPUT);
      digitalWrite(light12, LOW);
      Serial.println("light12 on");
    }
    if (dfd == "z3off") {
      feedBack = "z3off";
      digitalWrite(light12, HIGH);
      Serial.println("light12Off");
    }

    zone3off();
    ZONE3bool = false;
    Serial.println("ZONE3 Off");
    feedBack = "z3off";
  }

  ////////////////////////////////////// ZONE-4 /////////////////////
  if (dfd == "z4on")
  {
    Serial.print("othr: ");
    Serial.println(Zone4HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone4MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone4HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone4MinOff);

    long Zone4HrOnconvOn  = Zone4HrOn * ms_per_hour;
    long Zone4MinOnconvOn = Zone4MinOn * ms_per_min;
    long Zone4TotalOnTime = Zone4HrOnconvOn + Zone4MinOnconvOn;

    long Zone4HrOnconvOff  = Zone4HrOff * ms_per_hour;
    long Zone4MinOnconvOff = Zone4MinOff * ms_per_min;
    long Zone4TotalOffTime = Zone4HrOnconvOff + Zone4MinOnconvOff;

    Serial.println("Zone4TotalOnTime");

    Serial.println(Zone4TotalOnTime);

    Serial.println("Zone4TotalOffTime");

    Serial.println(Zone4TotalOffTime);

    ZONE4.begin(Zone4TotalOnTime, Zone4TotalOffTime );
    ZONE4bool = true;

    Serial.println("ZONE4 On");

    EepromReadlght4();

    if (dfd == "z4on" ) {
      feedBack = "z4on";
      pinMode(light13, OUTPUT);
      digitalWrite(light13, LOW);
      Serial.println("light13 on");
    }
    if (dfd == "z4off") {
      feedBack = "z4off";
      digitalWrite(light13, HIGH);
      Serial.println("light13 Off");
    }
    if (dfd == "z4on" ) {
      feedBack = "z4on";
      pinMode(light14, OUTPUT);
      digitalWrite(light14, LOW);
      Serial.println("light4 on");
    }
    if (dfd == "z4off") {
      feedBack = "z4off";
      digitalWrite(light14, HIGH);
      Serial.println("light14 Off");
    }
    if (dfd == "z4on" ) {
      feedBack = "z4on";
      pinMode(light15, OUTPUT);
      digitalWrite(light15, LOW);
      Serial.println("light15 on");
    }
    if (dfd == "z4off") {
      feedBack = "z4off";
      digitalWrite(light15, HIGH);
      Serial.println("light15 Off");
    } if (dfd == "z4on" ) {
      feedBack = "z4on";
      pinMode(light16, OUTPUT);
      digitalWrite(light16, LOW);
      Serial.println("light16 on");
    }
    if (dfd == "z4off") {
      feedBack = "z4off";
      digitalWrite(light16, HIGH);
      Serial.println("light16 Off");
    }

    zone4off();
    ZONE4bool = false;
    Serial.println("ZONE4 Off");
    feedBack = "z4off";
  }

  ////////////////////////////////////// ZONE-5 /////////////////////

  if (dfd == "z5on")
  {
    Serial.print("othr: ");
    Serial.println(Zone5HrOn);

    Serial.print("otMin: ");
    Serial.println(Zone5MinOn);

    Serial.print("oftHr: ");
    Serial.println(Zone5HrOff);

    Serial.print("oftMin: ");
    Serial.println(Zone5MinOff);

    long Zone5HrOnconvOn  = Zone5HrOn * ms_per_hour;
    long Zone5MinOnconvOn = Zone5MinOn * ms_per_min;
    long Zone5TotalOnTime = Zone5HrOnconvOn + Zone5MinOnconvOn;

    long Zone5HrOnconvOff  = Zone5HrOff * ms_per_hour;
    long Zone5MinOnconvOff = Zone5MinOff * ms_per_min;
    long Zone5TotalOffTime = Zone5HrOnconvOff + Zone5MinOnconvOff;

    Serial.println("Zone5TotalOnTime");

    Serial.println(Zone5TotalOnTime);

    Serial.println("Zone5TotalOffTime");

    Serial.println(Zone5TotalOffTime);

    ZONE5.begin(Zone5TotalOnTime, Zone5TotalOffTime );
    ZONE5bool = true;

    Serial.println("ZONE5 On");

    EepromReadlght5();

    if (dfd == "z5on" ) {
      feedBack = "z5on";
      pinMode(light17, OUTPUT);
      digitalWrite(light17, LOW);
      Serial.println("light17 on");
    }
    if (dfd == "z5off") {
      feedBack = "z5off";
      digitalWrite(light17, HIGH);
      Serial.println("light17 Off");
    }
    if (dfd == "z5on" ) {
      feedBack = "z5on";
      pinMode(light18, OUTPUT);
      digitalWrite(light18, LOW);
      Serial.println("light18 on");
    }
    if (dfd == "z5off") {
      feedBack = "z5off";
      digitalWrite(light18, HIGH);
      Serial.println("light18 Off");
    }
    if (dfd == "z5on" ) {
      feedBack = "z5on";
      pinMode(light19, OUTPUT);
      digitalWrite(light19, LOW);
      Serial.println("light19 on");
    }
    if (dfd == "z5off") {
      feedBack = "z5off";
      digitalWrite(light19, HIGH);
      Serial.println("light19 Off");
    } if (dfd == "z5on" ) {
      feedBack = "z5on";
      pinMode(light20, OUTPUT);
      digitalWrite(light20, LOW);
      Serial.println("light20 on");
    }
    if (dfd == "z5off") {
      feedBack = "z5off";
      digitalWrite(light20, HIGH);
      Serial.println("light20 Off");
    }

    zone5off();
    ZONE5bool = false;
    Serial.println("ZONE5 Off");
    feedBack = "z5off";
  }
}

void ok()
{
  deserializeJson(doc, Serial3);
  DeserializationError error = deserializeJson(doc, Serial3);

  if (error) {

    return;
  }
  const char* dayy = doc["dayy"]; // "Wednesday"
  hourss = doc["hourss"]; // 12
  minutess = doc["minutess"]; // 58
  secondss = doc["secondss"]; // 5

  if ((secondss > 0) && (okk == true))
  {
    rtc.begin();
    rtc.adjust(DateTime(2022, 9, 17, hourss, minutess, secondss));
    okk == false;
  }
}

void callSerialCom() {
  if (Serial2.available()) {
    SerialCom();
  }
}

void setup()
{
  Serial.println(msg);
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  Wire.begin();

  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);
  pinMode(light5, OUTPUT);
  pinMode(light6, OUTPUT);
  pinMode(light7, OUTPUT);
  pinMode(light8, OUTPUT);
  pinMode(light9, OUTPUT);
  pinMode(light10, OUTPUT);
  pinMode(light11, OUTPUT);
  pinMode(light12, OUTPUT);
  pinMode(light13, OUTPUT);
  pinMode(light14, OUTPUT);
  pinMode(light15, OUTPUT);
  pinMode(light16, OUTPUT);
  pinMode(light17, OUTPUT);
  pinMode(light18, OUTPUT);
  pinMode(light19, OUTPUT);
  pinMode(light20, OUTPUT);
  digitalWrite(light1, HIGH);
  digitalWrite(light2, HIGH);
  digitalWrite(light3, HIGH);
  digitalWrite(light4, HIGH);
  digitalWrite(light5, HIGH);
  digitalWrite(light6, HIGH);
  digitalWrite(light7, HIGH);
  digitalWrite(light8, HIGH);
  digitalWrite(light9, HIGH);
  digitalWrite(light10, HIGH);
  digitalWrite(light11, HIGH);
  digitalWrite(light12, HIGH);
  digitalWrite(light13, HIGH);
  digitalWrite(light14, HIGH);
  digitalWrite(light15, HIGH);
  digitalWrite(light16, HIGH);
  digitalWrite(light17, HIGH);
  digitalWrite(light18, HIGH);
  digitalWrite(light19, HIGH);
  digitalWrite(light20, HIGH);
}

void printError(byte error) {
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

unsigned long SCpreviousMillis = 0;
const long period = 1000;
unsigned long LSpreviousMillis = 0;
unsigned long schpreviousMillis = 0;

void zone1on()
{
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);

  digitalWrite(light1, LOW);
  digitalWrite(light2, LOW);
  digitalWrite(light3, LOW);
  digitalWrite(light4, LOW);
}

void zone1off()
{
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);

  digitalWrite(light1, HIGH);
  digitalWrite(light2, HIGH);
  digitalWrite(light3, HIGH);
  digitalWrite(light4, HIGH);
}

void zone2on()
{
  pinMode(light5, OUTPUT);
  pinMode(light6, OUTPUT);
  pinMode(light7, OUTPUT);
  pinMode(light8, OUTPUT);

  digitalWrite(light5, LOW);
  digitalWrite(light6, LOW);
  digitalWrite(light7, LOW);
  digitalWrite(light8, LOW);
}

void zone2off()
{
  pinMode(light5, OUTPUT);
  pinMode(light6, OUTPUT);
  pinMode(light7, OUTPUT);
  pinMode(light8, OUTPUT);

  digitalWrite(light5, HIGH);
  digitalWrite(light6, HIGH);
  digitalWrite(light7, HIGH);
  digitalWrite(light8, HIGH);
}

void zone3on()
{
  pinMode(light9, OUTPUT);
  pinMode(light10, OUTPUT);
  pinMode(light11, OUTPUT);
  pinMode(light12, OUTPUT);

  digitalWrite(light9, LOW);
  digitalWrite(light10, LOW);
  digitalWrite(light11, LOW);
  digitalWrite(light12, LOW);
}

void zone3off()
{
  pinMode(light9, OUTPUT);
  pinMode(light10, OUTPUT);
  pinMode(light11, OUTPUT);
  pinMode(light12, OUTPUT);

  digitalWrite(light9, HIGH);
  digitalWrite(light10, HIGH);
  digitalWrite(light11, HIGH);
  digitalWrite(light12, HIGH);
}

void zone4on()
{
  pinMode(light13, OUTPUT);
  pinMode(light14, OUTPUT);
  pinMode(light15, OUTPUT);
  pinMode(light16, OUTPUT);

  digitalWrite(light13, LOW);
  digitalWrite(light14, LOW);
  digitalWrite(light15, LOW);
  digitalWrite(light16, LOW);
}

void zone4off()
{
  pinMode(light13, OUTPUT);
  pinMode(light14, OUTPUT);
  pinMode(light15, OUTPUT);
  pinMode(light16, OUTPUT);

  digitalWrite(light13, HIGH);
  digitalWrite(light14, HIGH);
  digitalWrite(light15, HIGH);
  digitalWrite(light16, HIGH);
}

void zone5on()
{
  pinMode(light17, OUTPUT);
  pinMode(light18, OUTPUT);
  pinMode(light19, OUTPUT);
  pinMode(light20, OUTPUT);

  digitalWrite(light17, LOW);
  digitalWrite(light18, LOW);
  digitalWrite(light19, LOW);
  digitalWrite(light20, LOW);
}

void zone5off()
{
  pinMode(light17, OUTPUT);
  pinMode(light18, OUTPUT);
  pinMode(light19, OUTPUT);
  pinMode(light20, OUTPUT);

  digitalWrite(light17, HIGH);
  digitalWrite(light18, HIGH);
  digitalWrite(light19, HIGH);
  digitalWrite(light20, HIGH);
}

void scheduleFun()
{
  /////////////////// ZONE1S SCHEDULE MODE SLOTS
  ////slot1
  if (checkflag == true)
  {
    Serial.println(sch1OnHr);                  //Shift1Clockhr();

    Serial.println(sch1OnMin);                //Shift1Clockmin();

    Serial.println(sch1OffHr);                  //Shift1Clockhr();

    Serial.println(sch1OffMin);

    Serial.println(hourok);
    Serial.println(minok);

    int S1Hr  = sch1OnHr;
    int S1Min = sch1OnMin;
    int S2Hr  = sch1OffHr;
    int S2Min = sch1OffMin;

//    rtcupdte();

    if ((hourok == S1Hr)  &&  (minok  ==  S1Min))
    {
      zone1on();
    }
    else if ((hourok == S2Hr)  &&  (minok  ==  S2Min))
    {
      zone1off();
    }
  }

  ///////slot2
  if (checkflag1 == true)
  {

    Serial.println(sch2OnHr);                  //Shift1Clockhr();

    Serial.println(sch2OnMin);                //Shift1Clockmin();

    Serial.println(sch2OffHr);                  //Shift1Clockhr();

    Serial.println(sch2OffMin);

    int S3Hr  = sch2OnHr;
    int S3Min = sch2OnMin;
    int S4Hr  = sch2OffHr;
    int S4Min = sch2OffMin;

//    rtcupdte();

    if ((hourok == S3Hr)  &&  (minok  ==  S3Min))
    {
      zone2on();

    }
    else if ((hourok == S4Hr)  &&  (minok  ==  S4Min))
    {
      zone2off();
    }
  }

  if (checkflag2 == true)
  {
    int S5Hr  = sch3OnHr;
    int S5Min = sch3OnMin;
    int S6Hr  = sch3OffHr;
    int S6Min = sch3OffMin;

//    rtcupdte();

    if ((hourok == S5Hr)  &&  (minok  ==  S5Min))
    {
      zone3on();
    }
    else if ((hourok == S6Hr)  &&  (minok  ==  S6Min))
    {
      zone3off();
    }
  }

  /////////////////slot2
  if (checkflag3 == true)
  {

    int S7Hr  = sch4OnHr;
    int S7Min = sch4OnMin;
    int S8Hr  = sch4OffHr;
    int S8Min = sch4OffMin;

//    rtcupdte();

    if ((hourok == S7Hr)  &&  (minok  ==  S7Min)) {
      zone4on();
    }
    else if ((hourok == S8Hr)  &&  (minok  ==  S8Min)) {
      zone4off();
    }
  }

  if (checkflag3 == true)
  {

    int S9Hr   = sch5OnHr;
    int S9Min  = sch5OnMin;
    int S10Hr  = sch5OffHr;
    int S10Min = sch5OffMin;

//    rtcupdte();

    if ((hourok == S9Hr)  &&  (minok  ==  S9Min)) {

      zone5on();
    }
    else if ((hourok == S10Hr)  &&  (minok  ==  S10Min)) {

      zone5off();
    }
  }
}

void loop()
{
  uint16_t r, g, b, c, colorTemp, RGBlux;
  double lux;    // Resulting lux value
  boolean good;  // True if neither sensor is saturated
  unsigned int data0, data1;

  int uvLevel  = analogRead(UVOUT);
  int refLevel = analogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity;
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = currentMillis;

    callSerialCom();
  }

  unsigned long LScurrentMillis = millis();
  if (LScurrentMillis - LSpreviousMillis >= 10000) {
//    rtcupdte();

    LSpreviousMillis = LScurrentMillis;
  }
  delay(3000);
  delay(ms);

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  RGBlux = tcs.calculateLux(r, g, b);

  if (Serial2.available()) {
    dfd += char(Serial2.read());
  }
  if (millis() > asyncDelay + delayLength) {
    if (asyncDelay > (4294967295 - delayLength)) {
      asyncDelay = (4294967295 - asyncDelay) + (delayLength - (4294967295 - asyncDelay));
    } else {
      asyncDelay += delayLength;
    }
    readlux  = RGBlux * 0.034 / 2;
    readlux1 = r * 0.034 / 2;
    readlux2 = g * 0.034 / 2;
    readlux3 = b * 0.034 / 2;
    readlux4 = data0 * 0.034 / 2;
    readlux5 = b * 0.034 / 2;
    
    Serial2.print("add 18,0," + String(readlux * 60) + endchar);
    Serial2.print("lux.val=" + String(readlux * 60) + endchar);
    Serial2.print("add 18,1," + String(readlux1 * 60) + endchar);
    Serial2.print("add 18,2," + String(readlux2 * 60) + endchar);
    Serial2.print("add 18,3," + String(readlux3 * 60) + endchar);

    Serial2.print("add 21,0," + String(readlux4 * 60) + endchar);
    Serial2.print("lux1.val=" + String(readlux4 * 60) + endchar);

    Serial2.print("add 21,1," + String(readlux5 * 60) + endchar);
    Serial2.print("lux2.val=" + String(readlux5 * 60) + endchar);
  }

  unsigned long schcurrentMillis = millis();
  if (schcurrentMillis - schpreviousMillis >= 5000) {
//    rtcupdte();

    schpreviousMillis = schcurrentMillis;
    scheduleFun();

  }

  ////////////////////////////////// SWITCHER //////////////////////////////////

  if (ZONE1bool == true) {
    ZONE1.Z1Update();
  }

  if (ZONE2bool == true) {
    ZONE2.Z2Update();
  }

  if (ZONE3bool == true) {
    ZONE3.Z3Update();
  }

  if (ZONE4bool == true) {
    ZONE4.Z4Update();
  }

  if (ZONE5bool == true) {
    ZONE5.Z5Update();
  }
}
