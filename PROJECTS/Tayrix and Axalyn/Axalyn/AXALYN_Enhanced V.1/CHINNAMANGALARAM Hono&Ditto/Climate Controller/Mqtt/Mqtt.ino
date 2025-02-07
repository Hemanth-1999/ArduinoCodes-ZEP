#include<WiFi.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <SSLClient.h>
#include "certificates.h" // This file must be regenerated
#include <SPIFFS.h>
#include <FS.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include"MapFloat.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CO2Sensor.h"
#include <AsyncTCP.h>
#include <U8x8lib.h>
#include<EEPROM.h>

const char* ddata;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);
WiFiManager wifiManager;
WiFiClient espClient;
//WiFiClientSecure espClient; //This ClientSecure is for Gateway (Eg.pag.tayrix.com).
PubSubClient client(espClient);

void saveConfig();
void loadConfig();
void saveConfigCallback();
void initializeWifiManager();
void wifiInfo();
//Topics
const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT =  "Saifarm/GH2/PFC/ZoneA/PFCCONTROL/nodeside";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
const char* send_topic = "telemetry"; //publish 
// const char* send_topic = "axalyn"; //publish 

//// Mqtt Credentials ////
char mqttServer[40] = "ditto.tayrix.com"; //"164.52.223.248"; //"pag.axalyn.com"; //
// string representation of the port
char port_str[40] = "31872"; //"30883"; //"31872"; //  ditto.tayrix.com port
                             //"30883"; //"32337"; //  ditto.axalyn.com port
                                        //"1883";  //  cea.axalyn.com port       
// convert port string to integer
uint16_t mqttPort = atoi(port_str);
char mqttUserName[60] = "DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033"; //"axalyn";   
//"DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033"; // Tayrix user
//"DEVICE_USERNAME@6c458335-0fdc-40d9-8ab1-7606dabdb887"; // Axalyn user

char mqttPwd[40] = "DEVICE_PASSWORD"; //"cenaura@2024";  //
char clientID[40] = "ESP32Client";

String timestamp;

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout
bool shouldSaveConfig = false;
bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button
bool WMISBLOCKING    = false; // use blocking or non blocking mode

String msgStr = "";

int p;
bool a;

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

  wifiInfo();

  wifiManager.setTimeout(180);

  if (WiFi.status() == WL_CONNECTED) {

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    // Enable mutual TLS with SSLClient
//    espClient.setCACert(rootCACertificate); // Use the CA certificates for the client

    client.setServer(mqttServer, mqttPort); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    
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
