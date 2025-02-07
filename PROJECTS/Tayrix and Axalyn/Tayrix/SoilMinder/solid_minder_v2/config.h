#include <BME280I2C.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <MapFloat.h>
#include <ArduinoJson.h>
#include <EnvironmentCalculations.h>
#include <SPIFFS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* valuedata;

uint32_t chipId;

//*******MQTT CREDENTION********************

//WiFiClientSecure espClient;
WiFiClient espClient;
PubSubClient client(espClient);
struct MqttSettings
{
   char Broker[20] = "164.52.223.248"; //"pag.tayrix.com";
   int Port = 1883; //"30883";
   char ClientId[20] = "ESP32";
   char Username[60] =  "axalyn"; //"DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033";
   char Password[20] = "cenaura@2024"; //"DEVICE_PASSWORD";
   char Topic1[20] = "axalyn"; //"telemetry";
   char Farm[20]={'\0',};
   char Field[20]={'\0',};
   char Zone[20]={'\0',};
}MqttCredentials;

//**********BME SENSOR CREDENTIALS *************

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

float ES_AT;//used to store temperature from bme sensor
float tempdec;//used to store temperature from bme sensor but only 2 decimal points.
float ES_RH;//used to store humidity from bme sensor
float ES_BP;//used to store pressure from bme sensor
float alti;//used to store Altitude in meters  from bme sensore
float dewpt;//used to store DewPoint from bme sensore
float sl;//used to store Equivalent Sea LevelPressure from bme sensore
float HI;//used to store HeatIndex from bme sensore
void printBME280Data( Stream* client);

//******************************************************UVA*****************************************
int uvLevel;
int refLevel;
float voltage; 
float outputvoltage;
float uvIntensity;
#define UVOUT 32
#define  REF_3V3 33
void UVA();
int averageAnalogRead(int);

//**************************************ENVIRONMENTAL TEMPERATURE CALCULATION****************************

float ETo;
void ETcalcul();
 
//*****************NPK SENSOR CREDENTIALS***************

#define mod      Serial1
#define RE       5
#define DE       4

#define MOIST    0
#define TEMP     1
#define EC       2
#define PH       3
#define NITRO    4
#define PHOS     5
#define POTA     6
#define TIMEOUT  500
#define USE_CAST false

#define BUF_SIZE 20

byte buf[BUF_SIZE];

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

uint16_t NPK_moist;
uint16_t NPK_temp;
uint16_t NPK_ec;
uint16_t NPK_ph;
uint16_t NPK_nitr;
uint16_t NPK_phosp;
uint16_t NPK_potass;

void getNPKpotass();
void getNPKphosp();
void getNPKnitr();
void getNPKph();
void getNPKelec();
void getNPKtemp();
void getNPKmoist();
void getNPKdata();

//**************ESP32 SLEEP CREDENTIALS**********
#define Time_To_Sleep 900
#define S_To_uS_Factor 1000000ULL
//************PORTAL CONFIGURATION****************
WiFiManager wm;
bool wm_nonblocking = false;
WiFiManagerParameter Mqttaddress;
WiFiManagerParameter MqttPort;
WiFiManagerParameter Mqttusername;
WiFiManagerParameter Mqttpassword;
WiFiManagerParameter devicename;
WiFiManagerParameter farm;
WiFiManagerParameter field;
WiFiManagerParameter zone;
bool ReadMqttCredentialsFromFile =true;

//****************************************************NTP CLIENT***********************************
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
