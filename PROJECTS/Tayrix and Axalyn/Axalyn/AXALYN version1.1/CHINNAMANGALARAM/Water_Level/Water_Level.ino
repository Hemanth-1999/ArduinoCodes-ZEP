#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include <EEPROM.h>
#include <Wire.h>
#include"MapFloat.h"
#include <NTPClient.h>
#include <FS.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define echoPin 18 // attach pin GPIO18 to pin Echo of JSN-SR04
#define trigPin 5  // attach pin GPIO5 ESP32 to pin Trig of JSN-SR04                     

float sensorvalue;
float sensorValue1;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);
WiFiClient espClient;
PubSubClient client(espClient);
char hostname[40] = "broker.hivemq.com";//"164.52.223.248";//"cea.axalyn.com";
char port[40] = "1883";
char clientId[40] = "ESP32Client";
char user[40] = "axalyn";
char password[40] = "cenaura@2024";
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
float voltage, outputvoltage;
int battry;
const int pagecount = 4;
int p;

bool a;

long duration; // Time taken for the pulse to reach the receiver
int distance;

void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void Display();
void dummy();
void batterypercent();
int averageAnalogRead(int);

void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
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

}

void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);

  StaticJsonDocument<500> doc;

  doc["dn"] = "irr_aqua";
  doc["d-id"] = ESP.getChipModel();
  doc["zone-id"] = 1;
  doc["ts"] = epochTime;

  JsonObject metrics = doc.createNestedObject("  metrics");

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5); 

  digitalWrite(trigPin, HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10); // keep the trigger "ON" for 10 ms to generate pulse
  digitalWrite(trigPin, LOW); // Turn off the pulse trigger to stop pulse

  // If pulse reached the receiver echoPin
  // become high Then pulseIn() returns the
  // time taken by the pulse to reach the receiver
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034/ 2; 

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  metrics["d"] = distance;
  
  char tempstring[1024];

  serializeJson(doc, tempstring);
  client.publish("axalyn", tempstring);
  memset(tempstring, 0, 500);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

void setup()
{
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  Serial.begin(115200);
  Serial.println("Distance measurement using JSN-SR04T");
  pinMode(34, INPUT);

  initializeWifiManager();

    timeClient.begin();
    timeClient.setTimeOffset(19800);
    timeClient.update();

    client.setServer(hostname, atoi(port)); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.  
    
  delay(500);
}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();

    if (!client.connected()) {
      reconnect();
    }
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;
      Display();
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
//    ESP.restart();
  }
}
