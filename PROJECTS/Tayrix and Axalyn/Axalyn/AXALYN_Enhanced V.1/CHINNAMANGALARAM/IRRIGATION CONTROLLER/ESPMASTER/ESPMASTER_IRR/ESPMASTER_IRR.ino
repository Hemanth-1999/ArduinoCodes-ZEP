#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "millisDelay.h"
WiFiUDP ntpUDP;
#include <FS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiClient espClient;
WiFiManager wm;
WiFiManager wifiManager;

millisDelay getvalue;
millisDelay rlvalue;
// Here you can pre-set the settings for the MQTT connection. The settings can later be changed via Wifi Manager.

PubSubClient client(espClient);
bool shouldSaveConfig = false;

///DECLARATION
uint32_t chipid;
const char* eventogo;
const char* eventogo1;
const char* eventogo2;
const char* alertmode1;
const char* alertlevel;
const char* valuedata;
char tempstring1[1024];
char tempstring2[1024];
char tempstring11[1024];
char RELAYSTRING[1024];

int Manualstatus_irz1a ;
int Manualstatus_irz1b ;
int Manualstatus_irz2a ;
int Manualstatus_irz2b ;
int Manualstatus_doa ;
int Manualstatus_dob;
/////alerts from mega
int Alerts_Mode;
int Alerts_level;
int MANUALRELAY;
////events from mega
int Events_TEMP ;
int Events_VPD ;
int Events_IRR ;

///MQTT SETUP
char hostname[40] = "164.52.223.248"; // or "cea.axalyn.com"
char port_str[40] = "1883"; // string representation of the port
uint16_t port = atoi(port_str); // convert port string to integer
char clientId[40] = "CCESP8266";
char user[40] = "axalyn";
char password[40] = "cenaura@2024";
char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCCONTROL";
char MQTT_PUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCDATA";
char MQTT_PUBTOPIC1[50] = "Saifarm/GH2/PFC/ZoneA/PFCSTATUS";
const char* sendtopic_ESMesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_WSMesh = "account/Zepto/clientid/appid/climatedata/WS";

void eventstatus()
{

  DynamicJsonDocument event(500);

  DeserializationError error = deserializeJson(event, Serial);

  if (error) {
    return;
  }

  Events_TEMP = event["Events"]["TEMP"];
  Events_VPD = event["Events"]["VPD"];
  Events_IRR = event["Events"]["IRR"];

  if (Events_TEMP == 3)
  {
    intializer(3);
    intializer(1);
  }

  if (Events_VPD == 4)
  {
    intializer(4);
    intializer(1);
  }
  if (Events_IRR == 5)
  {
    intializer(5);
  }
  if (Events_IRR == 6)
  {
    intializer(6);
  }
  if (Events_IRR == 21)
  {
    intializer(21);
  }

}

void manualstatus()
{
  DynamicJsonDocument relaystatus(500);

  DeserializationError error = deserializeJson(relaystatus, Serial);

  if (error) {

    return;
  }
  JsonObject Manualstatus = relaystatus["Manualstatus"];
  Manualstatus_irz1a = Manualstatus["irz1a"];
  Manualstatus_irz1b = Manualstatus["irz1b"];
  Manualstatus_irz2a = Manualstatus["irz2a"];
  Manualstatus_irz2b = Manualstatus["irz2b"];
  Manualstatus_doa = Manualstatus["doa"];
  Manualstatus_dob = Manualstatus["dob"];

}

void alert()
{
  DynamicJsonDocument alertfrom(500);
  DeserializationError error = deserializeJson(alertfrom, Serial);
  if (error) {
    return;
  }

  Alerts_Mode = alertfrom["Alerts"]["Mode"];
  Alerts_level = alertfrom["Alerts"]["level"];
  if (Alerts_Mode == 7)
  {
    intializer(7);
  }
  if (Alerts_Mode == 12)
  {
    intializer(12);
  }
  if (Alerts_level == 8)
  {
    intializer(8);

  }
  if (Alerts_level == 9)
  {
    intializer(9);
  }
  if (Alerts_level == 10)
  {
    intializer(10);
  }
  if (Alerts_level == 11)
  {
    intializer(11);

  }
}

void intializer(int toperform)
{
  switch (toperform)

  {
    case 1:
      if (Events_TEMP == 0)
      {
        eventogo = "null";
      }
      if (Events_VPD == 0)
      {
        eventogo1 = "null";
      }
      if (Events_IRR == 0)
      {
        eventogo2 = "null";
      }
      break;

    case 3:
      eventogo = "Autotemp";
      break;


    case 4:
      eventogo1 = "Autovpd";
      break;

    case 5:
      eventogo2 = "AutoIrr";
      break;

    case 6:
      eventogo2 = "ScheIrr";
      break;

    case 7:
      alertmode1 = "VPD";
      break;

    case 8:
      alertlevel = "Critical";
      break;

    case 9:
      alertlevel = "moderate";
      break;

    case 10:
      alertlevel = "low";
      break;
    case 11:
      alertlevel = "normal";
      break;

    case 12:
      alertmode1 = "Temperature";
      break;

    case 21:
      alertmode1 = "Auto and Sche";
      break;

  }
}

void tocloud()
{

  chipid = ESP.getChipId();
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  DynamicJsonDocument mydoc(1024);

  mydoc["dn"] = "IRR_ESP";
  mydoc["zone_id"] = "5805";
  mydoc["did"] = ESP.getChipId();
  mydoc["ts"] = epochTime;
  JsonObject PFCDATA = mydoc.createNestedObject("metrics");
  JsonObject Alerts = PFCDATA.createNestedObject("alerts");
  Alerts["Mode"] = alertmode1;
  Alerts["level"] = alertlevel;
  JsonObject MANUALstatus = Alerts.createNestedObject("Manualstatus");
  MANUALstatus["irz1a"] =  Manualstatus_irz1a;
  MANUALstatus["irz1b"] =  Manualstatus_irz1b;
  MANUALstatus["irz2a"] =  Manualstatus_irz2a;
  MANUALstatus["irz2b"] =  Manualstatus_irz2b;
  MANUALstatus["doa"] =  Manualstatus_doa;
  MANUALstatus["dob"] =  Manualstatus_dob;

  JsonObject EVENTS = PFCDATA.createNestedObject("Events");
  EVENTS["IRR"] = eventogo2 ;

  serializeJson(mydoc, tempstring11);
  Serial.println("");
  serializeJson(mydoc, Serial);
  client.publish("axalyn", tempstring11);
  memset(tempstring11, 0, 500);
}

void sendsetpointmega()
{
  DynamicJsonDocument relaydoc(1024);
  JsonObject CHANGERELAY = relaydoc.createNestedObject("Allrelays");
  if (valuedata != NULL)
  {
    String compare;
    String readd = valuedata;
    if (compare != readd)
    {
      compare = readd;
      CHANGERELAY["FAN1"] =  readd;
    }

    serializeJson(relaydoc, RELAYSTRING);
    Serial.println("");
    delay(50);
    serializeJson(relaydoc, Serial);
    Serial.print("\n");
    memset(RELAYSTRING, 0, 1024);
  }
}

void sendnodedata()
{
  DynamicJsonDocument metricdoc(1024);
  DynamicJsonDocument Nodedoc(1024);

}

void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
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

  valuedata = doc["data"];

}

void reconnect() {

  while (!client.connected()) {

    if (client.connect("ESP8266Client", user, password))
    {
      client.subscribe("axalyn");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      ESP.restart();
    }
  }
}
void HandleMqttState() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
void setup()
{

  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount Failed");
    // Handle the failure appropriately, possibly halt or retry
    while (true) {
      delay(1000); // Delay indefinitely if SPIFFS fails to mount
    }
  }

  //   Save configuration if needed
  if (shouldSaveConfig) {
    Serial.println("Saving updated configuration...");
    saveConfig();
  }

  loadConfig();   // Load configuration settings from SPIFFS or EEPROM

  initializeWifiManager();   // Set up WiFiManager and MQTT configuration

  wifiManager.setTimeout(180);

  wifiInfo();

  timeClient.begin();
  timeClient.setTimeOffset(19800);

  //MQTT-SetUP
  client.setServer(hostname, port);
  client.setCallback(callback);

}

unsigned long SCpreviousMillis = 0;
unsigned long SendtomegapreviousMillis = 0;
unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
void loop() {

  HandleMqttState();
  timeClient.update();
  client.loop();
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 1000)
  {
    SCpreviousMillis = currentMillis;
    sendsetpointmega();
  }
  unsigned long megacurrentMillis = millis();
  if (megacurrentMillis - SendtomegapreviousMillis >= 3000)
  {

    SendtomegapreviousMillis = megacurrentMillis;
    getvalue.start(100);
  }
  if (getvalue.isRunning())
  {
    sendnodedata();
  }
  if (getvalue.justFinished())
  {
  }
  unsigned long torecievecurrentMillis = millis();
  if (torecievecurrentMillis - torecievestatus >= 5000)
  {
    torecievestatus = torecievecurrentMillis;
    alert();
    delay(15);
    eventstatus();
  }
  unsigned long torecievecurrentMillism = millis();
  if (torecievecurrentMillism - torecievestatusm >= 4000)
  {
    torecievestatusm = torecievecurrentMillism;
    manualstatus();

  }

  unsigned long cloudcurrentMillis = millis();
  if (cloudcurrentMillis - tocloudstatus >= 8000)
  {
    tocloudstatus = cloudcurrentMillis;
    tocloud();
  }

}
