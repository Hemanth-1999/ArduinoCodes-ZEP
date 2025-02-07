#include "CO2Sensor.h"
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include<EEPROM.h>
#include"MapFloat.h"
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <U8g2lib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

uint32_t chipId;

//*************************************************MQTT CREDENTIALS********************************

WiFiClient espClient;
PubSubClient client(espClient);
struct mqtt
{
  char* Broker=NULL;
  int Port;
  char* Username=NULL;
  char* Password=NULL;
  char* Clientid=NULL;
  char* Topic=NULL;
  char* Farm=NULL;
  char* Zone=NULL;
  char*Field=NULL;
}mqttcredentials;
//char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/CLIMATECONTROL";
//const char* sendtopic_ESMesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_WSMesh = "account/Zepto/clientid/appid/climatedata/WS";//nodes outside farm must publish to this topic.
void callback(char* topic, byte * payload, unsigned int length);


//*******************************************************CAPTIVE PORTAL CONFIGURATION*****************

void saveConfigCallback ();
void configure_wifi_portal();
WiFiManager wm;
//*********************************************CO2 SENSOR PINS*****************************
int co2;
CO2Sensor co2Sensor(15, 0.99, 100);

//*************************************************BME SENSOR CREDENTIALS************************
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
//*********************************************BATTERY CREDENTIALS*****************************

#define batteryvolt 34//BATTERY IS CONNECTED TO PIN 32 OF ESP8266.
int sensorvalue;//THIS IS THE ANALOG VALUE FROM PIN 34.
int battry;//THIS ANALOG VALUE FROM PIN 32 IS MAPPED SO THAT IT LOOK BATTERY VALUE.
void batterypercent();

//*************************************************MILLIS CREDENTIALS****************************
unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
//***************************************************DISPLAY************************
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
const int pagecount = 4;
bool a;
void Display();
void page1();
void page2();
void page3();
void page4();

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
//*************************************VPD CALCULATION*******************************************
float VPDcal();
float VPD;
//****************************************************NTP CLIENT***********************************
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
