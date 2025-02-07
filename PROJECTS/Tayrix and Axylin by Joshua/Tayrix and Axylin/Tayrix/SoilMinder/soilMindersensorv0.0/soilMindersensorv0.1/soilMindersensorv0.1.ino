/**************************************************************
   Project: [SOIL MINDER]
   Description: [Sensor to read soil PH EC NPK TEMP HUMIDITY readings]
   Author: [Joshua Jonathan Joseph]
   Date: [03/06/2023]
 **************************************************************/
#include <CAN.h>
#include <SPI.h>
#include <ETH.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <LoRa.h>
//********Declaration Part
#define ss 5
#define rst 25
#define dio0 35
byte MonolineMaster = 0xFF;
byte SoilMinder = 0xBB;
String Mymessage = "";
String SenderNode = "";
String outgoing;
byte msgCount = 0;
String incoming = "";
#define TRIGGER_PIN 0
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
#define TX_GPIO_NUM   21
#define RX_GPIO_NUM   22
int N, P, K;
float SoilM, SoilT, PH, EC;


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
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  Serial.println("SolarMinder LORA");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop()
{
  onReceive(LoRa.parsePacket());
  if (wm_nonblocking) wm.process();
  checkButton();
  if (!client.connected()) {
    reconnect();
  }
  N = random(32, 54);
  P = random(32, 54);
  K = random(32, 54);
  SoilM = random(32, 54);
  SoilT = random(32, 54);
  PH = random(32, 54);
  EC = random(32, 54);
}
