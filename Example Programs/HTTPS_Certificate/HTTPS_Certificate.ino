/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

float sensorValue;
float sensorValue1;
const char* ddata;

String E_AT = "{temp: 905, hum: 20}";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);

const char* httpUserName = "DEVICE_USERNAME@6c458335-0fdc-40d9-8ab1-7606dabdb887";
const char* httpPwd = "DEVICE_PASSWORD";

//Your Domain name with URL path or IP address with path
const char* serverName = "https://api.axalyn.com:30883/telemetry";
const char* clientID = "username0001"; // client id username+0001

String timestamp;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

WiFiClient espClient;

PubSubClient client(espClient);

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

const char* sendtopic_Mesh = "telemetry";
const char* sendtopic_CT =  "Saifarm/GH2/PFC/ZoneA/PFCCONTROL/nodeside";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";

#define batteryvolt 34

const int pagecount = 4;
int p;
int chipId;
int sensorvalue;
float voltage, outputvoltage;
int battry;
bool a;

void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void batterypercent();
int averageAnalogRead(int);

void saveWifiCallback() {
  Serial.println("[CALLBACK] saveCallback fired");
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");

}

void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
}

float tempdec;

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

void sendDataHTTP() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  uint32_t ES_did = chipId;

  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["did"] = ES_did;
  TXCC["ts"] = formattedTime;
  JsonObject TXCC_ES_rtd = TXCC.createNestedObject("ES_rtd");

  TXCC_ES_rtd["AT_v"] = E_AT;

  char buffer[1024];

  serializeJson(doc, buffer);
  client.publish(sendtopic_Mesh, buffer);
  client.publish(sendtopic_CT, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

void wifiInfo() {
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}

void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();
  pinMode(34, INPUT);

  WiFiManager wifiManager;

  wifiManager.setTimeout(180);

  if (!wifiManager.autoConnect("Axalyn Sensor Wi-Fi")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);

    ESP.restart();
    delay(5000);
  }

  Serial.println("WiFi connected");

  wifiInfo();
  if (WiFi.status() == WL_CONNECTED) {

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);   //
  }
  else {
    Serial.println("No Wifi");

  }

}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop() {
  //Send an HTTP POST request every 10 minutes
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();
    WiFiClient client;

    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      if (client.connected()) {
        sendDataHTTP();
      }

      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(clientID, serverName);
      
      // If you need Node-RED/server authentication, insert user and password below
      http.setAuthorization(httpUserName, httpPwd);

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      
      // Data to send with HTTP POST
      String httpRequestData = "{temp: 905, hum: 20}";
      
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
  }
  else {
    Serial.println("Do not Respond ");
  }
  lastTime = millis();

}
