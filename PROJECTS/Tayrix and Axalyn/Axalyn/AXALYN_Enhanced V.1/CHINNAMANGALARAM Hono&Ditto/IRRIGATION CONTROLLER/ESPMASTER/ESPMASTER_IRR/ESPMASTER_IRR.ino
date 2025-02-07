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

millisDelay getvalue;
millisDelay rlvalue;
// Here you can pre-set the settings for the MQTT connection. The settings can later be changed via Wifi Manager.
struct MqttSettings {
  char clientId[20] = "ESP8266Client";
  char hostname[40] = "192.168.0.173";
  char port[6] = "1883";
  char user[20];
  char password[20];
  char wm_mqtt_client_id_identifier[15] = "mqtt_client_id";
  char wm_mqtt_hostname_identifier[14] = "mqtt_hostname";
  char wm_mqtt_port_identifier[10] = "mqtt_port";
  char wm_mqtt_user_identifier[10] = "mqtt_user";
  char wm_mqtt_password_identifier[14] = "mqtt_password";
};
PubSubClient client(espClient);
MqttSettings mqttSettings;
bool shouldSaveConfig = false;
void readSettingsFromConfig() {
  //clean FS for testing
  // SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        // Use arduinojson.org/v6/assistant to compute the capacity.
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, configFile);
        if (error) {
          Serial.println(F("Failed to read file, using default configuration"));
        } else {
          Serial.println("\nparsed json");

          strcpy(mqttSettings.clientId, doc[mqttSettings.wm_mqtt_client_id_identifier]);
          strcpy(mqttSettings.hostname, doc[mqttSettings.wm_mqtt_hostname_identifier]);
          strcpy(mqttSettings.port, doc[mqttSettings.wm_mqtt_port_identifier]);
          strcpy(mqttSettings.user, doc[mqttSettings.wm_mqtt_user_identifier]);
          strcpy(mqttSettings.password, doc[mqttSettings.wm_mqtt_password_identifier]);

        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void initializeWifiManager() {

  WiFiManagerParameter custom_mqtt_client_id("client_id", "mqtt client id", mqttSettings.clientId, 40);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqttSettings.hostname, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqttSettings.port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqttSettings.user, 20);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqttSettings.password, 20);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;


  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_client_id);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);



  //reset settings - for testing
  wifiManager.resetSettings();


  wifiManager.autoConnect("AutoConnectAP");


  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqttSettings.clientId, custom_mqtt_client_id.getValue());
  strcpy(mqttSettings.hostname, custom_mqtt_server.getValue());
  strcpy(mqttSettings.port, custom_mqtt_port.getValue());
  strcpy(mqttSettings.user, custom_mqtt_user.getValue());
  strcpy(mqttSettings.password, custom_mqtt_pass.getValue());

}
void saveConfig() {
  Serial.println("saving config");
  StaticJsonDocument<1024> doc;
  doc[mqttSettings.wm_mqtt_client_id_identifier] = mqttSettings.clientId;
  doc[mqttSettings.wm_mqtt_hostname_identifier] = mqttSettings.hostname;
  doc[mqttSettings.wm_mqtt_port_identifier] = mqttSettings.port;
  doc[mqttSettings.wm_mqtt_user_identifier] = mqttSettings.user;


  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  configFile.close();
}
void initializeMqttClient() {
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  client.setServer(mqttSettings.hostname, atoi(mqttSettings.port));
}

bool tryConnectToMqttServer() {
  if (strlen(mqttSettings.user) == 0) {
    return client.connect(mqttSettings.clientId);
  } else {
    return client.connect(mqttSettings.clientId, mqttSettings.user, mqttSettings.password);
  }
}
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
const char* ssid = "CENURA";
const char* password = "india@123";
const char* broker = "api.tayrix.com"; //"192.168.0.173";
const int port = 1883;
const char* mqttUser = "tayrix";
const char* mqttPassword = "zepto@2023";
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
  DynamicJsonDocument mydoc(1024);
  JsonObject PFCDATA = mydoc.createNestedObject("PFCDATA");
  JsonObject Alerts = PFCDATA.createNestedObject("Alerts");
  Alerts["Mode"] = alertmode1;
  Alerts["level"] = alertlevel;
  JsonObject MANUALstatus = PFCDATA.createNestedObject("Manualstatus");
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
  client.publish( MQTT_PUBTOPIC1, tempstring11);
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
    Serial.print("Attempting MQTT connection...");
    if (tryConnectToMqttServer()) {

      Serial.println("connected");
      client.subscribe(MQTT_SUBTOPIC);
      client.subscribe(sendtopic_WSMesh);
      client.subscribe(sendtopic_ESMesh);

    }
    else
    {
      Serial.print(mqttSettings.hostname);
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void handleMqttState() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
void setup()
{

  Serial.begin(115200);

  readSettingsFromConfig();
  initializeWifiManager();

  if (shouldSaveConfig) {
    saveConfig();
  }

  initializeMqttClient();

  timeClient.begin();
  timeClient.setTimeOffset(19800);

  //MQTT-SetUP
  client.setServer(broker, port);
  client.setCallback(callback);

}
unsigned long SCpreviousMillis = 0;
unsigned long SendtomegapreviousMillis = 0;
unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
void loop() {

  handleMqttState();
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
