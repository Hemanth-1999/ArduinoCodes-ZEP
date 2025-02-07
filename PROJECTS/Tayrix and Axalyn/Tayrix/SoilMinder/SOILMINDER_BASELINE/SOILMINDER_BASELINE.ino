
/*
  Soil minder
  Send Data Formate
  {"dn":"highline","d-id":"ESP32-D0WDQ6","zone-id":"A","ts":1709384128,"  metrics":{"at":30.15999985,"rh":41.65917969,
  "vpd":2.400000095,"hi":30.07944679,"co2_v":0,"et_v":168.6000061,"bp":957.7346191,"uv":5470.092285,"dp":15.70949268,
  "sl":1149.726318,"sh":19.60000038,"st":26.29999924,"ec":143,"ph":6.900000095,"(n)":10,"(p)":13,"(k)":34}}

  NPK, Mois, Temp, EC and PH define for their index in the dataMod array..
  Which is a multi-dimensional array.. it has 7 arrays of 8 bytes each..
  It was easier for me to use the array this way..
  each 8 byte array
  byte  1 address
  byte  2 function code
  bytes 3-4 starting address
  bytes 5-6 QTY how many regs to read
  bytes 7-8 Checksum

  Here 2 arrays are used in this code, one with sensor address 1 the other _B uses address 255
  which is defined as the broadcast address,all sensors should respond,
  else sensor only responds to its address, which may not be 1

*/
#include"config.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CO2Sensor.h"
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include "MapFloat.h"
#include <NTPClient.h>
#include <FS.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <cstring>

#define MQ2pin 34
#include <Arduino.h>
#define TAG      "RS485_MODB_APP"
#define mod      Serial2
//#define DI       17 //TX
//#define RO       16 //RX
#define RE       5
#define DE       4
#define BUF_SIZE 20
#define TIMEOUT  500
#define USE_CAST false

#define MOIST   0
#define TEMP    1
#define EC      2
#define PH      3
#define NITRO   4
#define PHOS    5
#define POTA    6

float sensorValue;
float sensorValue1;

//all together.. 7 in 1, first 7 registers from device address 1..
const byte dataMod[7][8] = {
  {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A},//MOIST
  {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA},//temp
  {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA},//ec
  {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A},//ph
  {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xc5, 0xcb},//nitro
  {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0b},//phos
  {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b} //pot
};

//same but address 255 (broadcast) any connected sensor
//should respond, the sensor will replace the 255 with their
//currently configured address, change USE_CAST to true to use

const byte dataMod_B[7][8] = {
  {0xFF, 0x03, 0x00, 0x00, 0x00, 0x01, 0x91, 0xD4},//MOIST
  {0xFF, 0x03, 0x00, 0x01, 0x00, 0x01, 0xC0, 0x14},//temp
  {0xFF, 0x03, 0x00, 0x02, 0x00, 0x01, 0x30, 0x14},//ec
  {0xFF, 0x03, 0x00, 0x03, 0x00, 0x01, 0x61, 0xD4},//ph
  {0xFF, 0x03, 0x00, 0x04, 0x00, 0x01, 0xD0, 0x15},//nitro
  {0xFF, 0x03, 0x00, 0x05, 0x00, 0x01, 0x81, 0xD5},//phos
  {0xFF, 0x03, 0x00, 0x06, 0x00, 0x01, 0x71, 0xD5} //pot
};

//incoming buffer
byte buf[BUF_SIZE];

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiClientSecure espClient;
PubSubClient client(espClient);

struct MqttSettings
{
  char* Broker = "164.52.223.248"; //"pag.tayrix.com";
  int Port = 1883; //"30883";
  char* ClientId = "ESP32";
  char* Username = "axalyn"; //"DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033";
  char* Password = "cenaura@2024"; //"DEVICE_PASSWORD";
  char* Topic1 = "axalyn"; //"telemetry";
  char* Farm = NULL;
  char* Field = NULL;
  char* Zone = NULL;
} MqttCredentials;

String timestamp;
const char* valuedata;
bool shouldSaveConfig = false;

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout

bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button
bool WMISBLOCKING    = false; // use blocking or non blocking mode

String msgStr = "";
const char* topic1 = "esp/test"; //publish topic
const char* Rectopic = "esp/test"; //publish topic

char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCCONTROL";
char MQTT_PUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCDATA";
char MQTT_PUBTOPIC1[50] = "Saifarm/GH2/PFC/ZoneA/PFCSTATUS";
//const char* sendtopic_ESMesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_WSMesh = "account/Zepto/clientid/appid/climatedata/WS";
const char* sendtopic_ESMesh = "axalyn";
#define batteryvolt 34
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position

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

#define UVOUT 32
#define  REF_3V3 33
CO2Sensor co2Sensor(15, 0.99, 100);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int pagecount = 4;
int p;
//Scheduler r;
int uvLevel;
int refLevel;
int sensorvalue;
float voltage, outputvoltage;
int battry;
float t4, h4, p4, alti, dewpt, HI, uvIntensity;
void UVsensor();
int co2;//=co2Sensor.read();
int sl, ET;
bool a;

float Tme, dn1, dn, num1, enum1, enum2, expon, pdenom, powercal, top1, top2, num2, num3, denom2, powercalc2, expnum1, expnum2, expcal;
float expnum3, expnum4, expcal2, ETcal, Es, RHcal, RHcal2, num5, num6, numcalc, fdenom1, fdenom2, fdenom3, fdenom4, fnum1, fnum2, fnum3, fnum4, fnum5, fnum6, fnum7, fnum8, fnum9, fnum10;
float ETo, ES_BP, ES_AT, ES_RH;
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

void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void Display();
void dummy();
float VPDcal();
void page1();
void page2();
void page3();
void page4();
void ETcalcul();
void UVA();
void batterypercent();
int averageAnalogRead(int);

//void (*pages[])() = { page1, page2, page3, page4};
//int duration [pagecount] = { 1000, 1000, 1000, 1000};

byte EaddrNode = 10;
byte EaddrNodetemp = 12;
byte EaddrNodeMOISTi = 14;
byte EaddrNodepre = 16;
byte EaddrNodeHI = 18;
byte EaddrNodedewpt = 20;
byte EaddrNodesl = 22;
byte EaddrNodeco2 = 24;
byte EaddrNodeVPD = 26;
byte EaddrNodeET = 28;
int  Nco2, Nsl;
float Ntemp, NMOISTi, Npre, Ndewpt, NVPD, NET, NHI;
float tem;
float RH, p1, p2;
float AirTemp;
float LeafTemp;
float VPD, outputVoltage;

#define TINY_GSM_MODEM_SIM7600 //TINY_GSM_MODEM compatible for 7672 as well
#define TINY_GSM_RX_BUFFER 1024

#define TINY_GSM_TEST_SMS true
#define SMS_TARGET1  "7702826670" //Enter you phone number to which you would like to recevied SMS
//You can add multiple phone number to get SMS
//#define SMS_TARGET2  "xxxxxxxxxx" //Enter you phone number to which you would like to recevied SMS
//#define SMS_TARGET3  "xxxxxxxxxx" //Enter you phone number to which you would like to recevied SMS
#include <TinyGsmClient.h>
#include <SPI.h>
#include <Ticker.h>
#define SerialAT Serial1
#define SerialMon Serial

#define GSM_PIN "" //In case if you have a password protection for your simcard

//Set APN as per your sim card:
//Airtel -> "airtelgprs.com"
//BSNL -> "bsnlnet"
//Voda -> portalnmms
//jio -> jionet
const char apn[]  = "airtelgprs.com";
const char gprsUser[] = "";
const char gprsPass[] = "";

//API/Callback url to send your data to cloud. In this example we have used
//Pipedream which will allow to create a dummy call back URL and send data for testing purpose.
//Create your account in Pipedream and deploy a call back URL and assign that url to this variable.
//String send_data_to_url = "https://eogas6eaag50nu2.m.pipedream.net";
String gpsLatLong = ""; //Get gps lat long
String tempstring;
int timeout = 60; //GPS latch timeout duration. You can increase this value based on your application/need.

StaticJsonDocument<500> doc; //for testing MQTT request

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define UART_BAUD   115200
#define PIN_DTR     25
#define PWR_PIN     32

#define LED_PIN 2

//Get module manufacturer details
String modemName = modem.getModemName();
String modemInfo = modem.getModemInfo();

void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
}

void ETcalcul()
{
  Tmax = ES_AT;
  Tmin = ES_AT - 0.3;
  Tme = Tmax + Tmin;

  Tmean = Tme / 2;
  h = ES_RH;
  dn1 = 67.8 * h;
  dn = dn1 - 5.42;
  num1 = 4.87 / dn;
  U2 = Un * num1;
  //U2=(Un*(4.87/(67.8*h-5.42)));

  enum1 = 17.27 * Tmean;
  enum2 = Tmean + 237.3;
  expon = exp(enum1 / enum2);
  pdenom = Tmean + 237.3;
  powercal = pow(pdenom, 2);
  top1 = 0.6108 * expon;
  top2 = top1 * 4098;
  delta = top2 / powercal;


  //delta=4098*0.6108*exp(17.27*Tmean/Tmean+237.3)/pow(Tmean+237.3,2);

  P = 101.3 * pow(((293 - 0.0065 * z) / 293), 5.26);
  num2 = 0.0065 * z;
  num3 = 293 - num2;
  denom2 = num3 / 293;
  powercalc2 = pow(denom2, 5.26);
  P = 101.3 * powercalc2;
  gama = (0.000665 * P);
  expnum1 = 17.27 * Tmax;
  expnum2 = Tmax + 237.3;
  expcal = exp(expnum1 / expnum2);
  ETmax = 0.6108 * expcal;
  //ETmax=0.6108*exp(17.27*Tmax/Tmax+237.3);

  expnum3 = 17.27 * Tmin;
  expnum4 = Tmin + 237.3;
  expcal2 = exp(expnum3 / expnum4);
  ETmin = 0.6108 * expcal2;
  //ETmin=0.6108*exp(17.27*Tmin/Tmin+237.3);

  ETcal = ETmax + ETmin;
  Es = ETcal / 2;
  //Es=(ETmax+ETmin)/2;

  RHmax = ES_RH;
  RHmin = ES_RH - 0.3;
  RHcal = RHmax / 100;
  RHcal2 = RHmin / 100;
  num5 = ETmin * RHcal;
  num6 = ETmax * RHcal2;
  numcalc = num5 + num6;
  Ea = numcalc / 2;
  //Ea=ETmin*(RHmax/100)+ETmax*(RHmin/100)/2;

  T = ES_AT;
  Rn = uvIntensity;
  G = HI;
  fdenom1 = 0.34 * U2;
  fdenom2 = 1 + fdenom1;
  fdenom3 = gama * fdenom2;
  fdenom4 = delta + fdenom3;
  fnum1 = 0.408 * delta;
  fnum2 = Rn - G;
  fnum3 = fnum1 * fnum2;
  fnum4 = Es - Ea;
  fnum5 = U2 * fnum4;
  fnum6 = T + 273;
  fnum7 = 900 / fnum6;
  fnum8 = fnum7 * fnum5;
  fnum9 = gama * fnum8;
  fnum10 = fnum3 + fnum9;
  ETo = fnum10 / fdenom4;
  //ETo=0.408*delta*(Rn-G)+gama*(900/T+273)*U2*(Es-Ea)/delta+gama*(1+0.34*U2);
}

float tempdec;

void nodesave()
{
  //  EEPROM.write(EaddrNode4, nodenumber);
  EEPROM.write(EaddrNodetemp, ES_AT);
  EEPROM.write(EaddrNodeMOISTi, ES_RH);
  EEPROM.write(EaddrNodepre, ES_BP);
  EEPROM.write(EaddrNodeHI, HI);
  EEPROM.write(EaddrNodedewpt, dewpt);
  EEPROM.write(EaddrNodesl, sl);
  EEPROM.write(EaddrNodeco2, co2);
  EEPROM.write(EaddrNodeVPD, VPD);
  EEPROM.write(EaddrNodeET, ETo);

  //N4 = EEPROM.read(EaddrNode4);
  Ntemp = EEPROM.read(EaddrNodetemp);
  NMOISTi = EEPROM.read(EaddrNodeMOISTi);
  Npre = EEPROM.read(EaddrNodepre);
  NHI = EEPROM.read(EaddrNodeHI);
  Ndewpt = EEPROM.read(EaddrNodedewpt);
  Nsl = EEPROM.read(EaddrNodesl);
  Nco2 = EEPROM.read(EaddrNodeco2);
  NVPD = EEPROM.read(EaddrNodeVPD);
  NET = EEPROM.read(EaddrNodeET);
}

float VPDcal()
{

  AirTemp = ES_AT;
  LeafTemp = ES_AT;
  RH = ES_RH;
  p1 = pow ( 10 , ((float)(7.5 * AirTemp) / (float)(237.3 + AirTemp)));
  p2 = pow ( 10 , ((float)(7.5 * LeafTemp) / (float)(237.3 + LeafTemp)));
  VPD = (((float)610.7 * (float) p1) / 1000) - (((float)610.7 * (float)p2) / 1000) * (((float)RH) / 100);
  return VPD;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  float refanalog = analogRead(pinToRead);
  float DCvalue = mapFloat( refanalog, 0, 1023, 0, 5);

  runningValue /= numberOfReadings;
  return (runningValue);
}

void Display()
{

  batterypercent();
  UVA();
  ETcalcul();
  co2 = 00;

}

void UVA() {

  uvLevel = averageAnalogRead(UVOUT);
  refLevel = averageAnalogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  outputvoltage = 3.3 / refLevel * uvLevel;
  //float dcvoltage=mapFloat(refLevel,0,1023,0,5);

  uvIntensity = mapFloat( outputvoltage, 0.0, 31.0, 0.0, 99.0) * 10; //Convert the voltage to a UV intensity level
  //float uvIntensity = mapFloat(outputVoltage,0.99, 2.8, 0.0, 15.0)* 10;
  //uvIntensitynm =uvIntensity*10;

}

void printHexByte(byte b)
{

}

uint16_t GetModVal(byte val) {
  uint32_t startTime = 0;
  uint8_t  byteCount = 0;
  memset(buf, 0, sizeof(buf)); //empty incoming..
  //send request..
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (USE_CAST)
    mod.write(dataMod[val], sizeof(dataMod_B[val])); else
    mod.write(dataMod[val], sizeof(dataMod[val]));
  mod.flush();//wait for outgoing to be sent..
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  startTime = millis();
  while (millis() - startTime <= TIMEOUT) {
    if (mod.available() && byteCount < sizeof(buf)) {
      buf[byteCount++] = mod.read();
      printHexByte(buf[byteCount - 1]);
    }
  }
  Serial.println();
  //combine 2 byte into word..
  return (uint16_t)(buf[3] << 8 | buf[4]);
}

uint16_t val1;
uint16_t val2;
uint16_t val3;
uint16_t val4;
uint16_t val5;
uint16_t val6;
uint16_t val7;

float Val1;
float Val2;
float Val4;

void sendDataMQTT() {

  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
  uint64_t chipId = 0;

  doc["dn"] = "sm/b";
  doc["d-id"] = ESP.getChipModel();
  doc["zone-id"] = "A";
  doc["ts"] = epochTime;

  JsonObject NPK_ES = doc.createNestedObject("  metrics");

  float temp, hum, pres;

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  Serial.print("Temp: ");
  Serial.print(temp);
  ES_AT = temp;
  Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\t\tHumidity: ");
  Serial.print(hum);
  Serial.print("% RH");
  ES_RH = hum;
  Serial.print("\t\tPressure: ");
  Serial.print(pres);
  Serial.print(String(presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only
  ES_BP = pres;

  EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
  EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

  /// To get correct local altitude/height (QNE) the reference Pressure
  ///    should be taken from meteorologic messages (QNH or QFF)
  float altitude = EnvironmentCalculations::Altitude(pres, envAltUnit, referencePressure, outdoorTemp, envTempUnit);

  float dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);

  /// To get correct seaLevel pressure (QNH, QFF)
  ///    the altitude value should be independent on measured pressure.
  /// It is necessary to use fixed altitude point e.g. the altitude of barometer read in a map
  float seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(barometerAltitude, temp, pres, envAltUnit, envTempUnit);

  float absHum = EnvironmentCalculations::AbsoluteHumidity(temp, hum, envTempUnit);

  Serial.print("\t\tAltitude: ");
  Serial.print(altitude);
  Serial.print((envAltUnit == EnvironmentCalculations::AltitudeUnit_Meters ? "m" : "ft"));
  alti = altitude;
  Serial.print("\t\tDew point: ");
  Serial.print(dewPoint);
  dewpt = dewPoint;
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\t\tEquivalent Sea Level Pressure: ");
  Serial.print(seaLevel);
  Serial.print(String( presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only
  sl = (int)seaLevel;
  Serial.print("\t\tHeat Index: ");
  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);
  Serial.print(heatIndex);
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));
  HI = heatIndex;
  Serial.print("\t\tAbsolute Humidity: ");
  Serial.println(absHum);

  int E_AT = (int)(ES_AT * 10);
  NPK_ES["at"] = temp;

  int E_RH = (int)(ES_RH * 10);
  NPK_ES["rh"] = hum;

  int E_VPD = (int)(VPDcal() * 10);
  NPK_ES["vpd"] = (float)(E_VPD) / 10;

  int E_HI = (int)(HI * 10);
  NPK_ES["hi"] = heatIndex;

  NPK_ES["co2_v"] = sensorValue1;

  int E_ETo = (int)(ETo * 10);
  NPK_ES["et_v"] = (float)(E_ETo) / 10;

  int E_BP = (int)(ES_BP * 10);
  NPK_ES["bp"] = pres;
  NPK_ES["uv"] = uvIntensity * 10;

  int E_DP = (int)(dewpt * 10);
  NPK_ES["dp"] = dewPoint;

  NPK_ES["sl"] = seaLevel;
  NPK_ES["sh"] = Val1;
  NPK_ES["st"] = Val2;
  NPK_ES["ec"] = val3;
  NPK_ES["ph"] = Val4;
  NPK_ES["(n)"] = val5;
  NPK_ES["(p)"] = val6;
  NPK_ES["(k)"] = val7;

  serializeJson(doc, tempstring);
  //  client.publish("telemetry", tempstring);
  //  memset(tempstring, 0, 500);
  //  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

void modemPowerOn() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);
  digitalWrite(PWR_PIN, HIGH);
}

void modemRestart() {
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}

void SIMCOM() {
  // Set console baud rate
  SerialMon.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(100);

  modemPowerOn();
  SerialAT.begin(UART_BAUD, SERIAL_8N1, 16, 17);
  Serial.clearWriteError();
  Serial.println();
  Serial.println("Tayrix...");
  Serial.println();

  delay(2000);

  String res;
  Serial.println("Initializing Modem...");

  if (!modem.init()) {
    digitalWrite(LED_PIN, HIGH);
    modemRestart();
    delay(2000);
    Serial.println("Failed to restart modem, attempting to continue without restarting");
    digitalWrite(LED_PIN, LOW);
    return;
  }

  //Blue LED on the board use as an indicator
  //If blinking: Modem not able to boot
  //If turned ON: connected to network
  //If turned OFF: Modem booted successfully but not connected to network, check your SIM, network coverage etc.

  digitalWrite(LED_PIN, LOW);

  Serial.println("Running SIMCOMATI command...");
  modem.sendAT("+SIMCOMATI"); //Get the module information
  modem.waitResponse(1000L, res);
  res.replace(GSM_NL "OK" GSM_NL, "");
  //  Serial.println(res);
  res = "";
  //  Serial.println();

  //  Serial.println("Preferred mode selection (GSM/LTE)...");
  modem.sendAT("+CNMP?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    //    Serial.println(res);
  }
  res = "";
  Serial.println();

  //This section is only applicable for testing modules with NBIoT,
  //for other modules the command doesnt return anything and can be ignored while testing
  Serial.println("Preferred selection between CAT-M and NB-IoT...");
  modem.sendAT("+CMNB?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  res = "";
  Serial.println();
  sendDataMQTT();
  // Unlock your SIM card with a PIN if needed (not applicable for regular sim testing)
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }

  Serial.println("Network mode connectivity testing (GSM, LTE or GSM/LTE)...");

  DeviceNetwork();

  digitalWrite(LED_PIN, HIGH); //Modem connected to network

  Serial.println();
  Serial.println("Device is connected to Sim network.");
  Serial.println();

  delay(1000);
  Serial.println("Checking UE (User Equipment) system information...");
  Serial.println();
  modem.sendAT("+CPSI?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }

  delay(1000);
  Serial.println("Testing the a sample MQTTS Call to pipe drive server...");
  //  Serial.println("NOTE: Please ensure to deploy an end point on Pipe Dream (pipedream.com) to test. Example: " + send_data_to_url);
  Serial.println();
  Serial.println("MODEM TESTING IN PROGRESS....");
  Serial.println();
  delay(5000);

  MqttATCommand();  
  Serial.println();
  Serial.print("Paylog length: ");
  Serial.println(tempstring.length());
  Serial.println("Setting MQTT call back");
  Serial.println();
  delay(2000);
  Serial.print("Data sent to MQTT: ");
  Serial.println(tempstring);
  Serial.print("Post call response:");
  delay(2000);

  Serial.println("Sending Test SMS to number(s):");
  Serial.println(SMS_TARGET1);

#if TINY_GSM_TEST_SMS && defined TINY_GSM_MODEM_HAS_SMS && defined SMS_TARGET1
  res = modem.sendSMS(SMS_TARGET1, String(modemInfo));
  DBG("SMS sent status:", res ? "SUCCESS" : "FAILED");
  delay(2000);

#endif

  Serial.println("END OF MODEM TESTING");

  //******************************************************************************************
  //
  //    GPS/GNSS TESTING SAMPLE CODE.
  //    The below code run only if the 4G module supports GPS/GNSS
  //    so it is safe to ignore if you are testing only 4G modem
  //    Bharat Pi 4G boards are available with and wihtout GPS.
  //    Kindly check which one you have bought before testing GPS/GNSS code
  //******************************************************************************************

  //GPS/GNSS Configuration. Below AT commands will power ON the GNSS
  //Switch GNSS Ports and Assisted GPS, once successfully ready then it make a
  //get GPS co-ordinates call.
  // For full set of GPS/GNSS AT commands refer the documents/manual for Simcom A7672S AT Commands here: MQTTs://www.simcom.com/product/A7672X.html

  Serial.println("Enabling GPS/GNSS");
  Serial.println("Checking if modem supports GPS/GNSS...");
  if (modemName == "A7672S-LASC") { //FASE->With GPS/GNSS module
    Serial.println("Modem doesnt support GPS/GNSS, so no need of GPS testing.");
    return;
  } else if (modemName == "A7672S-FASE") {
    Serial.println("GPS/GNSS is supported!");
    Serial.println("Powering on GPS/GNSS...");
    modem.sendAT("+CGNSSPWR=1"); //Power on the GNSS
    while (modem.waitResponse(1000UL, "+CGNSSPWR: READY!") != 1) {
      Serial.print(".");
    }
    Serial.println("GPS/GNSS Power ON READY!");
    Serial.println("Switching GPS/GNSS Port");
    modem.sendAT("+CGNSSPORTSWITCH=1,1"); //Port switch for GNSS. For more details on the AT commands refer the AT command manual for SimCom A7276S 4G Module
    while (modem.waitResponse(1000UL, "OK") != 1) {
      Serial.print(".");
    }
    Serial.println("GPS/GNSS port switch success");
    Serial.println("Enabling Assisted GPS");
    modem.sendAT("+CAGPS"); //Assisted GPS
    while (modem.waitResponse(1000UL, "+AGPS: success.") != 1) {
      Serial.print(".");
    }
    Serial.println("GNSS assisted GPS success");
    Serial.println("Fetching GPS co-ordinates...");
    modem.sendAT("+CGNSSINFO");
    modem.waitResponse(1000UL, gpsLatLong); //Wait and get the latlong response from the CGNSSINFO AT command. (NOTE: Do not attempt to use cold/hot boot AT commands for GPS/GNSS as this might send the GNSS into shutdown and will require you to do a hard power ON. Hard power ON of GNSS is currently not supported on this board so take care before using Cold/Hot boot AT commands.)
    Serial.print("CGPS INFO: ");
    Serial.println(gpsLatLong);
    Serial.println("Awaiting for GPS latch: "); //Awaiti until the GPS latches as we get the Lat Long info

    //Try GPS latch until timeout
    for (timeout = 30; timeout != 0; timeout--) {
      modem.sendAT("+CGNSSINFO");
      modem.waitResponse(1000UL, gpsLatLong);
      if (gpsLatLong.indexOf("+CGNSSINFO: ,,,,,,,,") > 0) {
        Serial.print(".");
        Serial.print(timeout);
      } else {
        Serial.print("**** GPS Latched ****");
        modem.sendAT("+CGNSSINFO");
        modem.waitResponse(1000UL, gpsLatLong); //Wait and get the latlong response from the CGNSSINFO AT command. (NOTE: Do not attempt to use cold/hot boot AT commands for GPS/GNSS as this might send the GNSS into shutdown and will require you to do a hard power ON. Hard power ON of GNSS is currently not supported on this board so take care before using Cold/Hot boot AT commands.)
        Serial.println(gpsLatLong);
        return;
      }
      delay(1000);
    }
    if (timeout == 0) {
      Serial.println(">>>>> TIME OUT: GPS Latch timedout. <<<<<<");
      Serial.println("Please ensure you are outside and have sky visibility in order for GPS to latch initially. Without this the GPS wont latch and latlong will not be available.");
    }
    Serial.println("GPS testing ended.");
  } else {
    Serial.println("######### Could not find SIMCOM A7672S Module! ##########");
  }
  Serial.println(">>>>> End of 4G Module Testing <<<<<<");
}

bool ConnectedToSIMNetwork = true; // Track SIM network connection status

void setup() {

  SerialMon.begin(115200);
  mod.begin(4800);
  mod.begin(4800, SERIAL_8N1, 12, 14);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  EEPROM.begin(512);
  co2Sensor.calibrate();
  Wire.begin();
  pinMode(34, INPUT);
  SIMCOM();

  //  while (!bme.begin())
  //  {
  //    Serial.println("Could not find BME280 sensor!");
  //  }

  Serial.println("SIM Network connected");

  if (ConnectedToSIMNetwork) {

    switch (bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        //      Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        //      Serial.println("Found BMP280 sensor! No humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
    }

    timeClient.begin();
    timeClient.setTimeOffset(19800);
    timeClient.update();

    client.setServer("pag.tayrix.com", atoi("30883")); //setting MQTT server
//    client.setCallback(callback); //defining function which will be called when message is recieved.
    espClient.setInsecure();
  }
  else {
    Serial.println("No Network");

  }

}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop() {
  while (1) {
    while (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
  }    
}
