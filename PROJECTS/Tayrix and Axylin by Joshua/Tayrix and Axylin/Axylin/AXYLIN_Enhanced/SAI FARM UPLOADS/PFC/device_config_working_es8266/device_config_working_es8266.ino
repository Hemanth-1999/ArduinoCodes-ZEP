//**********************LIBRARY DECLARATION**********************// 
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "millisDelay.h"
WiFiUDP ntpUDP;
#include <DNSServer.h> 
#include <ESP8266WebServer.h>
//********************** DECLARATION**********************// 
//****Wifi manager and spiffs declaration
 String combined_string;
#define TRIGGER_PIN 2
bool wm_nonblocking = false;
WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
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
//****delays declaration
millisDelay getvalue;
millisDelay rlvalue;
//**** declaration
 const char* data;
uint32_t chipid;
const char* eventogo;
const char* eventogo1;
const char* eventogo2;
const char* alertmode1;
const char* alertlevel;
const char* senddatato;
char tempstring1[1024];
char tempstring2[1024];
char tempstring11[1024];
char RELAYSTRING[1024];
int Manualstatus_FAN1, Manualstatus_FAN2, Manualstatus_FAN3, Manualstatus_FAN4, Manualstatus_FAN5, Manualstatus_FAN6, Manualstatus_FAN7, Manualstatus_FAN8, Manualstatus_CP, Manualstatus_FC , Manualstatus_FP , Manualstatus_CO2, Manualstatus_IRR, Manualstatus_DO, Manualstatus_DRIP, Manualstatus_LIGHT;
int Manualstatus_f1 ,Manualstatus_f2 ,Manualstatus_f3 ,Manualstatus_f4 ,Manualstatus_f5 ,Manualstatus_f6 ,Manualstatus_f7 ,Manualstatus_f8,Manualstatus_cp ,Manualstatus_co ,Manualstatus_fp , Manualstatus_fs ,Manualstatus_ir,Manualstatus_do ,Manualstatus_dr,Manualstatus_pb  ;
float txcc_es_ES_rtd_AT_v ;
double txcc_es_ES_rtd_RH_v ;
double txcc_es_ES_rtd_VPD_v ;
float txcc_es_ES_rtd_HI_v ;
int txcc_es_ES_rtd_Co2_v ;
int txcc_es_ES_rtd_ET_v ;
double txcc_es_ES_rtd_BP_v ;
double txcc_es_ES_rtd_SR_v ;
double txcc_es_ES_rtd_DP_v ;
int txcc_es_ES_rtd_SL_v ;
double Ws_AT_v;
double Ws_RH_v;
double Ws_VPD_v ;
double Ws_HI_v;
float Ws_ET_v ;
double Ws_BP_v ;
double Ws_SR_v;
double Ws_DP_v;
int Ws_SL_v ;
/////alerts from mega
int Alerts_Mode;
int Alerts_level;
int MANUALRELAY;
////events from mega
int Events_TEMP ;
int Events_VPD ;
int Events_IRR ;
///************* Publishing topics
char MQTT_PUBTOPIC[100]="/PFCDATA";
char MQTT_PUBTOPIC1[100]="/PFCSTATUS";
char MQTT_SUBTOPIC[50] = "/NODEDATA1";
char MQTT_SUBTOPIC1[50] = "Saifarm/GH2/PFC/ZoneA/PFCCONTROL";

void startup()
{
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  int customFieldLength = 90;
  new (&Mqttaddress) WiFiManagerParameter("MQTTADD", "MQTT IP", "", customFieldLength);
  new (&MqttPort) WiFiManagerParameter("MQTTPORT", "PORT", "", customFieldLength );
  new (&Mqttusername) WiFiManagerParameter("MQTTUSERNAME", "MQTTUSERNAME", "", customFieldLength);
  new (&Mqttpassword) WiFiManagerParameter("MQTTPASSWORD", "MQTTPASSWORD", "", customFieldLength);
  new (&devicename) WiFiManagerParameter("DEVICENAME", "DEVICENAME", "", customFieldLength);
  new (&farm) WiFiManagerParameter("FARM", "FARM", "", customFieldLength);
  new (&field) WiFiManagerParameter("FIELD", "FIELD", "", customFieldLength);
  new (&zone) WiFiManagerParameter("ZONE", "ZONE", "", customFieldLength);

  wm.addParameter(&Mqttaddress);
  wm.addParameter(&MqttPort);
  wm.addParameter(&Mqttusername);
  wm.addParameter(&Mqttpassword);
  wm.addParameter(&devicename);
  wm.addParameter(&farm);
  wm.addParameter(&field);
  wm.addParameter(&zone);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi", "param", "sep", "restart", "exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");
  wm.setConfigPortalTimeout(120); // auto close configportal after n seconds



  bool res;

  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect or hit timeout");

  }
  else {

    Serial.println("connected...yeey :)");
  }
}
void setup() {
  // SPIFFS.format();
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(3000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS file system");
    return;
  }
  startup();
  readsavedvalues();
  client.setServer(broker, mqttports);
  mqttconnection();


  client.setCallback(callback);
}

void checkButton() {
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if ( digitalRead(TRIGGER_PIN) == LOW ) {
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if ( digitalRead(TRIGGER_PIN) == LOW ) {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }

      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);

      if (!wm.startConfigPortal("OnDemandAP", "password")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}


String getParam(String name) {
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println(" mqtt address = " + getParam("MQTTADD"));
  Serial.println(" mqtt port = " + getParam("MQTTPORT"));
  Serial.println(" mqtt uername = " + getParam("MQTTUSERNAME"));
  Serial.println(" mqtt password = " + getParam("MQTTPASSWORD"));
  Serial.println(" device name = " + getParam("DEVICENAME"));
  Serial.println(" farm = " + getParam("FARM"));
  Serial.println(" field = " + getParam("FIELD"));
  Serial.println(" zone = " + getParam("ZONE"));

  strcpy(hostnamee, Mqttaddress.getValue());
  strcpy(portt, MqttPort.getValue());
  strcpy(userr, Mqttusername.getValue());
  strcpy(passwordd, Mqttpassword.getValue());
  strcpy(deviceidd, devicename.getValue());
  strcpy(farmm, farm.getValue());
  strcpy(fieldd, field.getValue());
  strcpy(zonee, zone.getValue());
  Serial.println(hostnamee);
  Serial.println(portt);
  Serial.println(userr);
  Serial.println(passwordd);
  Serial.println(deviceidd);
  Serial.println(farmm);
  Serial.println(fieldd);
  Serial.println(zonee);



  DynamicJsonDocument doc(1024);
  doc["hostnamee"] = hostnamee;
  doc["portt"] = portt;
  doc["userr"] = userr;
  doc["passwordd"] = passwordd;
  doc["deviceidd"] = deviceidd;
  doc["farmm"] = farmm;
  doc["fieldd"] = fieldd;
  doc["zonee"] = zonee;

  // Open a file in SPIFFS for writing
  File file = SPIFFS.open("/params.json", "w");

  // Serialize the JSON object to a string and write it to the file
  serializeJson(doc, file);

  // Close the file
  file.close();

}
void readsavedvalues()
{
  File file = SPIFFS.open("/params.json", "r");
  if (!file) {
    Serial.println("Failed to open params file");
    return;
  }

  // Parse the JSON data
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return;
  }


  String hostnameefromf = doc["hostnamee"];
  String porttfromf = doc["portt"];
  String userrfromf = doc["userr"];
  String passworddfromf = doc["passwordd"];
  String devicesa = doc["deviceidd"];
  String farmq = doc["farmm"];
  String fieldq = doc["fieldd"];
  String zonew = doc["zonee"];


  file.close();

  hostnameefromf.toCharArray(broker, 100);
  mqttports = porttfromf.toInt();
  userrfromf.toCharArray(usernames, 100);
  passworddfromf.toCharArray(passwords, 100);
  devicesa.toCharArray(dev, 100);
  farmq.toCharArray(frm, 100);
  fieldq.toCharArray(fld, 100);
  zonew.toCharArray(zne, 100);


  Serial.print("broker:  ");
  Serial.println(broker);
  Serial.print("port:  ");
  Serial.println(mqttports);
  Serial.print("user:  ");
  Serial.println(usernames);
  Serial.print("pass:  ");
  Serial.println(passwords);
  Serial.print("device:  ");
  Serial.println(dev);
  Serial.print("farm:  ");
  Serial.println(frm);
  Serial.print("field:  ");
  Serial.println(fld);
  Serial.print("zone:  ");
  Serial.println(zne);

   combined_string = devicesa + "/" + farmq + "/" + fieldq + "/" + zonew;
  Serial.println(combined_string);
   String combined_string1;
   String combined_string2;
   String combined_string3;
   
  combined_string1 = combined_string+MQTT_PUBTOPIC;
  combined_string2 = combined_string+MQTT_PUBTOPIC1;
 combined_string3 = combined_string+MQTT_SUBTOPIC;
  combined_string1.toCharArray(MQTT_PUBTOPIC, 100);
  combined_string2.toCharArray(MQTT_PUBTOPIC1, 100);  
  combined_string3.toCharArray(MQTT_SUBTOPIC, 100);  
  Serial.println(MQTT_PUBTOPIC);
  Serial.println(MQTT_PUBTOPIC1);
   Serial.println(MQTT_SUBTOPIC);

  
}

unsigned long SCpreviousMillis = 0;
unsigned long SendtomegapreviousMillis = 0;
unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
void loop() 
{
//*************Continuous Running Commands
  if (wm_nonblocking)
  {
    wm.process(); 
  }
   if (!client.connected()) 
  {
    mqttconnection();
  }
  checkButton();
  client.loop();
//****************************************

  unsigned long megacurrentMillis = millis();
  if (megacurrentMillis - SendtomegapreviousMillis >= 3000)
  {
   
    SendtomegapreviousMillis = megacurrentMillis;    
    getvalue.start(100);
  }
  if (getvalue.isRunning())
  {
   client.subscribe(MQTT_SUBTOPIC);
    sendnodedata();
  }
  if (getvalue.justFinished())
  {
  }
 //****************************************
  unsigned long torecievecurrentMillis = millis();
  if (torecievecurrentMillis - torecievestatus >= 5000)
  {
    torecievestatus = torecievecurrentMillis;
   alert();
   delay(15);
   eventstatus();   
  }
  //****************************************
  unsigned long torecievecurrentMillism = millis();
   if (torecievecurrentMillism - torecievestatusm >= 4000)
  {
    torecievestatusm = torecievecurrentMillism;
    manualstatus();
   
  }
  //****************************************
  
  unsigned long cloudcurrentMillis = millis();
  if (cloudcurrentMillis - tocloudstatus >= 8000)
  {
    tocloudstatus = cloudcurrentMillis;
    tocloud();
  }
//*******************************
}
