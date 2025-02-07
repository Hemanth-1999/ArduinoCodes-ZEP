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
PubSubClient client(espClient);
millisDelay getvalue;
millisDelay rlvalue;
///DECLARATION
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

///MQTT SETUP
const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";
const char* broker = "192.168.0.197";
const int port = 1883;
const char* mqttUser = "Zeptogreens";
const char* mqttPassword = "123456";
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

}
void manualstatus()
{
  DynamicJsonDocument relaystatus(500);

  DeserializationError error = deserializeJson(relaystatus, Serial);

  if (error) {

    return;
  }

 JsonObject Manualstatus = relaystatus["Manualstatus"];
 Manualstatus_f1 = Manualstatus["f1"];
 Manualstatus_f2 = Manualstatus["f2"]; 
 Manualstatus_f3 = Manualstatus["f3"]; 
 Manualstatus_f4 = Manualstatus["f4"]; 
 Manualstatus_f5 = Manualstatus["f5"]; 
 Manualstatus_f6 = Manualstatus["f6"]; 
 Manualstatus_f7 = Manualstatus["f7"]; 
 Manualstatus_f8 = Manualstatus["f8"]; 
 Manualstatus_cp = Manualstatus["cp"]; 
 Manualstatus_co = Manualstatus["co"]; 
 Manualstatus_fp = Manualstatus["fp"]; 
 Manualstatus_fs = Manualstatus["fs"]; 
 Manualstatus_ir = Manualstatus["ir"]; 
 Manualstatus_do = Manualstatus["do"]; 
 Manualstatus_dr = Manualstatus["dr"]; 
 Manualstatus_pb = Manualstatus["pb"]; 

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
  MANUALstatus["FAN1"] =  Manualstatus_f1;
  MANUALstatus["FAN2"] =  Manualstatus_f2;
  MANUALstatus["FAN3"] =  Manualstatus_f3;
  MANUALstatus["FAN4"] =  Manualstatus_f4;
  MANUALstatus["FAN5"] =  Manualstatus_f5;
  MANUALstatus["FAN6"] =  Manualstatus_f6;
  MANUALstatus["FAN7"] =  Manualstatus_f7;
  MANUALstatus["FAN8"] =  Manualstatus_f8;
  MANUALstatus["CP"] =    Manualstatus_cp;
  MANUALstatus["FP"] = Manualstatus_fp;
  MANUALstatus["FC"] = Manualstatus_fs;
  MANUALstatus["CO2"] = Manualstatus_co;
  MANUALstatus["IRR"] = Manualstatus_ir;
  MANUALstatus["DO"] = Manualstatus_do;
  MANUALstatus["DRIP"] =  Manualstatus_dr;
  MANUALstatus["LIGHT"] = Manualstatus_pb;
  JsonObject EVENTS = PFCDATA.createNestedObject("Events");
  EVENTS["TEMP"] =  eventogo ;
  EVENTS["VPD"] = eventogo1 ;
  EVENTS["IRR"] = eventogo2 ;

  serializeJson(mydoc, tempstring11);
  Serial.println("");
  client.publish( MQTT_PUBTOPIC1, tempstring11);
  memset(tempstring11, 0, 500);
}
void sendsetpointmega()
{
  Serial.println(senddatato);
  DynamicJsonDocument Externalcommand(50);
  JsonObject Fromapp = Externalcommand.createNestedObject("Fromapp"); 
  Fromapp["Setpoint"] =  senddatato ;
  Serial.println("");
  serializeJson(Externalcommand, Serial);
  Serial.print("\n");
 
 }
void sendnodedata()
{
  DynamicJsonDocument metricdoc(1024);
  DynamicJsonDocument Nodedoc(1024);
  JsonObject ESNODE = Nodedoc.createNestedObject("ESNODE");
  ESNODE["ES_AT"] =  txcc_es_ES_rtd_AT_v;
  ESNODE["ES_RH"] =  txcc_es_ES_rtd_RH_v;
  ESNODE["ES_VPD"] =  txcc_es_ES_rtd_VPD_v;
  ESNODE["ES_HI"] =  txcc_es_ES_rtd_HI_v;
  ESNODE["ES_CO2"] =  txcc_es_ES_rtd_Co2_v;
  ESNODE["ES_ET"] =  txcc_es_ES_rtd_ET_v;
  ESNODE["ES_SR"] =  txcc_es_ES_rtd_SR_v;
  ESNODE["ES_BP"] =  txcc_es_ES_rtd_BP_v;
  ESNODE["ES_DP"] =  txcc_es_ES_rtd_DP_v;
  ESNODE["ES_SL"] =  txcc_es_ES_rtd_SL_v;
  JsonObject WSNODE = Nodedoc.createNestedObject("WSNODE");
  WSNODE["WS_AT"] =  Ws_AT_v;
  WSNODE["WS_RH"] =  Ws_RH_v;
  WSNODE["WS_VPD"] =  Ws_VPD_v;
  WSNODE["WS_HI"] =  Ws_HI_v;
  WSNODE["WS_ET"] =  Ws_ET_v;
  WSNODE["WS_SR"] =  Ws_SR_v;
  WSNODE["WS_BP"] =  Ws_BP_v;
  WSNODE["WS_DP"] =  Ws_DP_v;
  WSNODE["WS_SL"] =  Ws_SL_v;
  serializeJson(Nodedoc, tempstring1);
  Serial.println("");
  serializeJson(Nodedoc, Serial);
  memset(tempstring1, 0, 500);

  JsonObject METRIC = metricdoc.createNestedObject("metrics");
  METRIC["AT"] =  txcc_es_ES_rtd_AT_v;
  METRIC["RH"] =  txcc_es_ES_rtd_RH_v;
  METRIC["VPD"] =  txcc_es_ES_rtd_VPD_v;
  METRIC["HI"] =  txcc_es_ES_rtd_HI_v;
  METRIC["Co2"] =  txcc_es_ES_rtd_Co2_v;
  METRIC["Ev"] =  txcc_es_ES_rtd_ET_v;
  METRIC["Solar"] =  txcc_es_ES_rtd_SR_v;
  METRIC["BaPa"] =  txcc_es_ES_rtd_BP_v;
  METRIC["Dew"] =  txcc_es_ES_rtd_DP_v;
  METRIC["SeaL"] =  txcc_es_ES_rtd_SL_v;
  serializeJson(metricdoc, tempstring2);
  Serial.println("");
  client.publish(MQTT_PUBTOPIC, tempstring2);
  memset(tempstring2, 0, 500);

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
   
 
 
  if (JsonObject txcc_es = doc["txcc_es"])
  {
    long txcc_es_did = txcc_es["did"];
    const char* txcc_es_ts = txcc_es["ts"];
    JsonObject txcc_es_ES_rtd = txcc_es["ES_rtd"];
    txcc_es_ES_rtd_AT_v = txcc_es_ES_rtd["AT_v"];
    txcc_es_ES_rtd_RH_v = txcc_es_ES_rtd["RH_v"];
    txcc_es_ES_rtd_VPD_v = txcc_es_ES_rtd["VPD_v"];
    txcc_es_ES_rtd_HI_v = txcc_es_ES_rtd["HI_v"];
    txcc_es_ES_rtd_Co2_v = txcc_es_ES_rtd["Co2_v"];
    txcc_es_ES_rtd_ET_v = txcc_es_ES_rtd["ET_v"];
    txcc_es_ES_rtd_BP_v = txcc_es_ES_rtd["BP_v"];
    txcc_es_ES_rtd_SR_v = txcc_es_ES_rtd["SR_v"];
    txcc_es_ES_rtd_DP_v = txcc_es_ES_rtd["DP_v"];
    txcc_es_ES_rtd_SL_v = txcc_es_ES_rtd["SL_v"];
  }
  if (JsonObject Ws = doc["Ws"])
  {
    long Ws_did = Ws["did"];
    const char* Ws_ts = Ws["ts"];
    Ws_AT_v = Ws["AT_v"];
    Ws_RH_v = Ws["RH_v"];
    Ws_VPD_v = Ws["VPD_v"];
    Ws_HI_v = Ws["HI_v"];
    Ws_ET_v = Ws["ET_v"];
    Ws_BP_v = Ws["BP_v"];
    Ws_SR_v = Ws["SR_v"];
    Ws_DP_v = Ws["DP_v"];
    Ws_SL_v = Ws["SL_v"];
  }

}
void reconnect() {
 
   while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

      if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("connected");
      client.subscribe(MQTT_SUBTOPIC);
      client.subscribe(sendtopic_WSMesh);
      client.subscribe(sendtopic_ESMesh);
      
    }
    else 
    {
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    yield();
  }
  Serial.println("Connected to the WiFi network");


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
void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  timeClient.update();
  client.loop();

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
