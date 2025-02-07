//#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include <SPIFFS.h>
#include <FS.h>
#include<EEPROM.h>
#include<Wire.h>
#include<WiFi.h>
#include "MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#define MQ2pin 34

float sensorValue;
float sensorValue1;
const char* ddata;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);

const size_t bufferSize = 100; // Adjust buffer size as per your requirement
char mqttServer[40] = "164.52.223.248"; //"pag.axalyn.com"; //
char port_str[40] = "1883"; //"30883"; // string representation of the port
uint16_t mqttPort = atoi(port_str); // convert port string to integer
char mqttUserName[60] = "axalyn"; // "DEVICE_USERNAME@6c458335-0fdc-40d9-8ab1-7606dabdb887"; //"Zeptogreens";//"zepto";//"Zeptogreens";
char mqttPwd[40] = "cenaura@2024"; // "DEVICE_PASSWORD"; // "12345";
char clientID[40] = "username0001";
String timestamp;

WiFiClient espClient;
PubSubClient client(espClient);

float ES_BP, ES_AT, ES_RH, RH, ETo, HI;
float t4, h4, p4, alti, dewpt, uvIntensity;

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

String msgStr = "";
const char* topic1 = "esp/test"; //publish topic
const char* Rectopic = "esp/test"; //publish topic

const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT =  "Saifarm/GH2/PFC/ZoneA/PFCCONTROL/nodeside";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
#define batteryvolt 34

int p;
int co2 = 0;

bool a;
bool shouldSaveConfig = false;

void saveConfig();
void loadConfig();
void saveConfigCallback();
void initializeWifiManager();
void wifiInfo();

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

}

void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["did"] = ESP.getChipModel();
  TXCC["ts"] = formattedTime;
  JsonObject TXCC_ES_rtd = TXCC.createNestedObject("ES_rtd");

  // int E_AT = random(100, 200);
  // TXCC_ES_rtd["AT_v"] = "{temp: 905, hum: 20}";
  String E_AT = String(ES_AT, 2);
  TXCC_ES_rtd["AT_V"] = "546";
  String E_RH = String(ES_RH, 2);
  TXCC_ES_rtd["RH_v"] = "546";
//  String E_VPD = String(VPDcal(), 2);
//  TXCC_ES_rtd["VPD_v"] = E_VPD;
  String E_HI = String(HI, 2);
  TXCC_ES_rtd["HI_v"] = "546";
  String cco2 = String(co2, 2);
  TXCC_ES_rtd["Co2_v"] = "546";
  String E_ETo = String(ETo, 2);
  TXCC_ES_rtd["ET_v"] = "546";
  String E_BP = String(ES_BP, 2);
  TXCC_ES_rtd["BP_v"] = "546";
//  String SR = String(solarRadiation, 2);
  TXCC_ES_rtd["SR_v"] = "546";
  String E_DP = String(dewpt, 2);
  TXCC_ES_rtd["DP_v"] = "546";
  TXCC_ES_rtd["SL_v"] = "546";

  char buffer[1024];

  serializeJson(doc, buffer);
  client.publish("axalyn", buffer);
  client.publish(sendtopic_CT, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

void reconnect() {
  while (!client.connected()) {
    // ESP.restart();
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe("api/command");
      client.subscribe("axalyn");

      a = true;

    }
    else {
      a = false;

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print("Reason: ");
      switch (client.state()) {
        case -4: Serial.println("Connection timeout"); break;
        case -3: Serial.println("Connection lost"); break;
        case -2: Serial.println("Connect failed"); break;
        case -1: Serial.println("Disconnected"); break;
        case 1: Serial.println("Bad protocol"); break;
        case 2: Serial.println("Bad client ID"); break;
        case 3: Serial.println("Unavailable"); break;
        case 4: Serial.println("Bad credentials"); break;
        case 5: Serial.println("Unauthorized"); break;
        default: Serial.println("Unknown error");
      }
      delay(5000);  // wait 5sec and retry
      ESP.restart();

    }
  }
}

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}

void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();
  pinMode(34, INPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    // Handle the failure appropriately, possibly halt or retry
    while (true) {
      delay(1000); // Delay indefinitely if SPIFFS fails to mount
    }
  }

  // Save configuration if needed
  if (shouldSaveConfig) {
    Serial.println("Saving updated configuration...");
    saveConfig();
  }

  loadConfig();   // Load configuration settings from SPIFFS or EEPROM

  initializeWifiManager();   // Set up WiFiManager and MQTT configuration

  wifiManager.setTimeout(180);

  wifiInfo();
  
  if (WiFi.status() == WL_CONNECTED) {

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    client.setServer(mqttServer, mqttPort); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
  }
  else {
    Serial.println("No Wifi");

  }

}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop() {
  // Retry loop if WiFi connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to reconnect to WiFi...");

    // Attempt to reconnect to Wi-Fi
    WiFi.reconnect();  // Try to reconnect to the last connected Wi-Fi network
    Serial.println("Reconnection failed, retrying...");
    delay(5000);  // Wait 5 seconds before retrying
    return;  // Skip the rest of the loop if not connected

    // If connected
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to WiFi."); \
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();

    if (!client.connected()) {
      reconnect();
    }
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;

    }
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      if (client.connected()) {
        sendDataMQTT();
      }

    }

    client.loop();
  }
  unsigned long rstresp1 = millis();
  if (rstresp1 - rstresp > 500000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }
}
