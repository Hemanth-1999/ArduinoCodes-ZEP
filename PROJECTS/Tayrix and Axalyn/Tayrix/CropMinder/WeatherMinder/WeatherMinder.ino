/**************************************************************
   Project: [SOIL MINDER]
   Description: [Sensor code to read soil PH EC NPK TEMP HUMIDITY readings]
   Author: [Joshua Jonathan Joseph]
   Date: [03/06/2023]
 **************************************************************/
#include <SPI.h>
#include <ETH.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//********Declaration Part
#define SEALEVELPRESSURE_HPA 1013.25 // Adjust this value for your location's sea level pressure
Adafruit_BME280 bme;
#define ss 17
#define rst 22
#define dio0 39
byte MonolineMaster = 0xFF;
byte WeatherMinder = 0xCC;
String Mymessage = "";
String SenderNode = "";
String outgoing;
byte msgCount = 0;
String incoming = "";
#define TRIGGER_PIN 34
bool wm_nonblocking = false;
WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
static bool eth_connected = false;
WiFiManagerParameter Mqttaddress;
WiFiManagerParameter MqttPort;
WiFiManagerParameter Mqttusername;
WiFiManagerParameter Mqttpassword;
WiFiManagerParameter devicename;
WiFiManagerParameter farm;
WiFiManagerParameter field;
WiFiManagerParameter zone;
char hostnamee[40] ;
char portt[6] ;
char userr[20];
char passwordd[40];
char deviceidd[20];
char farmm[20];
char fieldd[20];
char zonee[20];
char broker[40] ;
int mqttports;
char usernames [40];
char passwords[40] ;
char dev[40] ;
char frm[40] ;
char fld[40] ;
char zne[40] ;
char charbuffer[200];
float AT, VPD, WS, ET,WD,RH,RP,CH,BP,SR,SL, DP;
#define Time_To_Sleep 900
#define S_To_uS_Factor 1000000ULL
RTC_DATA_ATTR int bootCount = 0;
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);
  delay(2000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS file system");
    return;
  }
  startup();
  readsavedvalues();
  client.setServer(broker, mqttports);
  client.setCallback(callback);
  reconnect();
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  Serial.println("WeatherMinder LORA");
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor. Check wiring!");
    while (1);
  }
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(Time_To_Sleep * S_To_uS_Factor);
}
unsigned long Autoandmanual = 0;
unsigned long Sensor = 0;
void loop()
{
 /*LOOPs which need to be itterated frequently*/
 /////***********Continuos loop 
  onReceive(LoRa.parsePacket());
  if (wm_nonblocking) wm.process();
  checkButton();
  if (!client.connected()) {
    reconnect();
  }
/////***********Sleep Mode loop
  unsigned long currentMillis = millis();
  if (currentMillis - Autoandmanual >= 300000)
  {
    Serial.println("Time to take a nap");
    esp_deep_sleep_start();
    Autoandmanual = currentMillis;
  }
 /////***********Sending data to cloud loop
 unsigned long sensorcurrentMillis = millis();
  if (sensorcurrentMillis - Sensor >= 3000)
  {
   Sensorrun();
    Sensor = sensorcurrentMillis;
  }
}
