                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /*//README...NOTE.........
   Removed CS node
   ES node and WS Node Rec. Data
   Sending Data To MQTT - Kura

   Issus - WM Connection

   Serial Output------E.g
   Doc5 message:
   {"metrics":{"AT":27,"RH":63.40000153,"VPD":0,"HI":29.29999924,"Co2":420,"Ev":-0.600000024,"BaPa":938.0999756,"Solar":273.7999878,"Dew":20.10000038,"SeaL":1127}}
   Publish mega:
   {"AT":27,"RH":63.40000153,"VPD":1.299999952,"HI":29.29999924,"Co2":420,"Ev":-0.600000024,"BaPa":938.0999756,"Solar":273.7999878,"Dew":20.10000038,"SeaL":1127,"WAT":26.89999962,"WRH":62.70000076,"WVPD":1.299999952,"WHI":28.10000038,"WEv":0,"WBaPa":938.7000122,"WSolar":280.5,"WDew":19.20000076,"WSeaL":1129,"WSclk":0,"WSaclk":0,"WDclk":0,"WDaclk":0}
   --------
   Last Upadte...  --PFC Master Esp V1.1

   Irrigation Node ZA and ZB Node Added
   added new topic for sai farm
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long lastMsg   = 0;
unsigned long lastMsg01 = 0;
unsigned long lastMsg02 = 0;
unsigned long lastMsg03 = 0;
unsigned long lastMsg04 = 0;
unsigned long lastMsg05 = 0;
unsigned long lastMsg06 = 0;
//int toperform;
const char* eventogo;
const char* eventogo1;
const char* eventogo2;
const char* alertmode1;
const char* alertlevel;

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


///MQTT SETUP
const char* ssid = "Zeptogreens";//"Zeptogreens";//"Zepto";//"Zeptogreens"; // "Zeptogreens";//replace this with your wifi access point
const char* password = "Zeptogreens@2023"; //"Zeptogreens@20";// "Zepto@2022";//"Zepto@2022";//"Zeptogreens20"; //replace with your wifi password
const char* broker = "192.168.0.199";// // "192.168.1.183"; //"192.168.1.102";//IP address of machine on which broker is installed
const int port = 1883;
const char* mqttUser = "Zeptogreens";//"zepto"; //"zepto"; Zeptogreens
const char* mqttPassword = "123456";//"12345";
char mqtt_send[50] = "account-name/PD1/W1/A1/ClimateData";

char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCCONTROL";
char MQTT_PUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/CLIMATEDATA";
char MQTT_PUBTOPIC1[50] = "Saifarm/GH2/PFC/ZoneA/CLIMATESTATUS";
const char* sendtopic_ESMesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_WSMesh = "account/Zepto/clientid/appid/climatedata/WS";

#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];
char tempString[1024];
char tempString2[512];
char tempString3[1024];
char tempstring4[1024];
char tempstring5[1024];
char tempstring6[1024];
char tempstring7[1024];
char tempstring11[1024];
char tempstring22[1024];
char RELAYSTRING[1024];
int samples = 0;
int Manualstatus_FAN1;
int Manualstatus_FAN2;
int Manualstatus_FAN3;
int Manualstatus_FAN4;
int Manualstatus_FAN5;
int Manualstatus_FAN6;
int Manualstatus_FAN7;
int Manualstatus_FAN8;
int Manualstatus_CP;
int Manualstatus_FC;
int Manualstatus_FP;
int Manualstatus_CO2;
int Manualstatus_IRR;
int Manualstatus_DO;
int Manualstatus_DRIP;
int Manualstatus_LIGHT;
int Manualstatus_f1 , Manualstatus_f2 , Manualstatus_f3 , Manualstatus_f4 , Manualstatus_f5 , Manualstatus_f6 , Manualstatus_f7 , Manualstatus_f8, Manualstatus_cp , Manualstatus_co , Manualstatus_fp , Manualstatus_fs , Manualstatus_ir, Manualstatus_do , Manualstatus_dr, Manualstatus_pb  ;
/////alerts from mega
int Alerts_Mode;
int Alerts_level;
int MANUALRELAY;
////events from mega
int Events_TEMP ;
int Events_VPD ;
int Events_IRR ;
const char* valuedata;
float Tavg, HIavg, VPDavg, Havg;
long Ax_es_did, Ax_cs_did ;
const char* Ax_es_ts;
const char* Ax_cs_ts;

float Ax_es_ES_rtd_AT_v, Ax_es_ES_rtd_RH_v, Ax_es_ES_rtd_VPD_v, Ax_es_ES_rtd_HI_v, Ax_es_ES_rtd_ET_v, Ax_es_ES_rtd_BP_v, Ax_es_ES_rtd_SR_v, Ax_es_ES_rtd_DP_v;
int Ax_es_ES_rtd_Co2_v, Ax_es_ES_rtd_SL_v , Ax_es_ES_rtd_nt_v, Ax_cs_cs_rtd_nt_v ;
float Ax_cs_cs_rtd_AT_v, Ax_cs_cs_rtd_RH_v, Ax_cs_cs_rtd_VPD_v, Ax_cs_cs_rtd_HI_v;
float Ax_cs_cs_rtd_AT_v_Avg, Ax_es_ES_rtd_AT_v_Avg, Ax_cs_cs_rtd_RH_v_Avg, Ax_es_ES_rtd_RH_v_Avg, Ax_cs_cs_rtd_VPD_v_Avg, Ax_es_ES_rtd_VPD_v_Avg, Ax_cs_cs_rtd_HI_v_Avg, Ax_es_ES_rtd_HI_v_Avg;

float WSTavg, WSHavg, WSVPDavg, WSHIavg, Ax_WS_WS_rtd_ET_v, Ax_WS_WS_rtd_BP_v, Ax_WS_WS_rtd_SR_v, Ax_WS_WS_rtd_DP_v, Ax_WS_WS_rtd_wsclk_v, Ax_WS_ES_rtd_wsaclk_v, Ax_WS_ES_rtd_wdclk_v, Ax_WS_ES_rtd_wdaclk_v, Ax_WS_ES_rtd_clohei_v;
int Ax_WS_ES_rtd_SL_v, Ax_WS_ES_rtd_ntips_v;

float   Ax_ws_ws_rtd_AT, Ax_ws_ws_rtd_RH, Ax_ws_ws_rtd_VPD, Ax_ws_ws_rtd_ET, Ax_ws_ws_rtd_BP, Ax_ws_ws_rtd_SR, Ax_ws_ws_rtd_DP, Ax_ws_ws_rtd_HI, Ax_ws_ws_rtd_WSclk, Ax_ws_ws_rtd_WSaclk, Ax_ws_ws_rtd_WDclk, Ax_ws_ws_rtd_WDaclk;
int  Ax_ws_ws_rtd_ntips,  Ax_ws_ws_rtd_SL ;
float As_ZA_WIF_v , As_ZA_WOF_v , As_ZA_WTL_v , As_ZA_WTSP_v ;
float avg(float, float);

unsigned long previousMillis = 0;
const long interval = 5000;

void sendsensordata();
void sendWSsensordata();
RunningAverage RAT(5);
RunningAverage RAH(5);
RunningAverage RAHI(5);
RunningAverage RAVPD(5);
RunningAverage RACO2(5);
RunningAverage RAET(5);
RunningAverage RABP(5);
RunningAverage RASOL(5);
RunningAverage RADEW(5);

WiFiClient espClient;
PubSubClient client(espClient);
bool MQTTFlag;
float avg(float, float);
void SendDataToMega();
void sendDataMQTT();


uint32_t chipid;

DynamicJsonDocument megadoc(1024);
DynamicJsonDocument megadoc1(1024);
DynamicJsonDocument doc(1024);


void sendDataMQTT() {


  chipid = ESP.getChipId();

  DynamicJsonDocument doc3(512);
  DynamicJsonDocument doc4(512);

  int Tavg_AT = 0;
  Tavg_AT = (int)(Ax_es_ES_rtd_AT_v_Avg * 10);
  doc["AT"] = (float)(Tavg_AT) / 10;

  int Havg_RH = 0;
  Havg_RH = (int)(Ax_es_ES_rtd_RH_v_Avg * 10);
  doc["RH"] = (float)(Havg_RH) / 10;
  int VPDavg_VPD = 0;
  VPDavg_VPD = (int)(Ax_es_ES_rtd_VPD_v_Avg * 10);
  doc["VPD"] = (float)(VPDavg_VPD) / 10;
  int  HIavg_HI = 0;
  HIavg_HI = (int)( Ax_es_ES_rtd_HI_v_Avg * 10);
  doc["HI"] = (float)(HIavg_HI) / 10;

  doc["Co2"] = Ax_es_ES_rtd_Co2_v;

  int Ax_es_ES_rtd_ET_v_ET = 0;
  Ax_es_ES_rtd_ET_v_ET = (int)(Ax_es_ES_rtd_ET_v * 10);
  doc["Ev"] = (float)(Ax_es_ES_rtd_ET_v_ET) / 10;
  int Ax_es_ES_rtd_BP_v_BP = 0;
  Ax_es_ES_rtd_BP_v_BP = (int)(Ax_es_ES_rtd_BP_v * 10);
  doc["BaPa"] = (float)(Ax_es_ES_rtd_BP_v_BP) / 10;
  int Ax_es_ES_rtd_SR_v_SR = 0;
  Ax_es_ES_rtd_SR_v_SR = (int)(Ax_es_ES_rtd_SR_v * 10);
  doc["Solar"] = (float) (Ax_es_ES_rtd_SR_v_SR) / 10;
  int Ax_es_ES_rtd_DP_v_DP = 0;
  Ax_es_ES_rtd_DP_v_DP = (int)(Ax_es_ES_rtd_DP_v * 10);
  doc["Dew"] = (float)(Ax_es_ES_rtd_DP_v_DP) / 10 ;
  doc["SeaL"] = Ax_es_ES_rtd_SL_v;







  //**Weather Station-------------------------------------------------------------
  int Ax_ws_ws_rtd_AT_AT = 0;
  Ax_ws_ws_rtd_AT_AT = (int)(Ax_ws_ws_rtd_AT * 10);
  doc["WAT"] = (float)(Ax_ws_ws_rtd_AT_AT) / 10;
  int Ax_ws_ws_rtd_RH_RH = 0;

  Ax_ws_ws_rtd_RH_RH = (int)(Ax_ws_ws_rtd_RH * 10);
  doc["WRH"] = (float) (Ax_ws_ws_rtd_RH_RH) / 10;
  int Ax_ws_ws_rtd_VPD_VPD = 0;

  Ax_ws_ws_rtd_VPD_VPD = (int)(Ax_ws_ws_rtd_VPD * 10);
  doc["WVPD"] = (float)( Ax_ws_ws_rtd_VPD_VPD) / 10;
  int Ax_ws_ws_rtd_HI_HI = 0;
  Ax_ws_ws_rtd_HI_HI = (int)(Ax_ws_ws_rtd_HI * 10);
  doc["WHI"] = (float)(Ax_ws_ws_rtd_HI_HI) / 10;

  int Ax_ws_ws_rtd_ET_ET = 0;
  Ax_ws_ws_rtd_ET_ET = (int)(Ax_ws_ws_rtd_ET * 10);
  doc["WEv"] = (float)(Ax_ws_ws_rtd_ET_ET) / 10;

  int Ax_ws_ws_rtd_BP_BP = 0;
  Ax_ws_ws_rtd_BP_BP = (int)(Ax_ws_ws_rtd_BP * 10);
  doc["WBaPa"] = (float) (Ax_ws_ws_rtd_BP_BP) / 10;
  int Ax_ws_ws_rtd_SR_SR = 0;
  Ax_ws_ws_rtd_SR_SR = (int)(Ax_ws_ws_rtd_SR * 10);
  doc["WSolar"] = (float)(Ax_ws_ws_rtd_SR_SR) / 10;
  int Ax_ws_ws_rtd_DP_DP = 0;
  Ax_ws_ws_rtd_DP_DP = (int)(Ax_ws_ws_rtd_DP * 10);
  doc["WDew"] = (float)(Ax_ws_ws_rtd_DP_DP) / 10 ;
  doc["WSeaL"] = Ax_ws_ws_rtd_SL;

  doc["WSclk"] = Ax_ws_ws_rtd_WSclk;
  doc["WSaclk"] = Ax_ws_ws_rtd_WSaclk;
  doc["WDclk"] = Ax_ws_ws_rtd_WDclk;
  doc["WDaclk"] = Ax_ws_ws_rtd_WDaclk;


  doc3["AT"] = (float)(Tavg_AT) / 10;
  doc3["RH"] = (float)(Havg_RH) / 10;
  VPDavg_VPD = (int)(VPDavg * 10);
  doc3["VPD"] = (float)(VPDavg_VPD) / 10;
  doc3["HI"] = (float)(HIavg_HI) / 10;
  doc3["Co2"] = Ax_es_ES_rtd_Co2_v;
  doc3["Ev"] = (float)(Ax_es_ES_rtd_ET_v_ET) / 10;
  doc3["BaPa"] = (float)(Ax_es_ES_rtd_BP_v_BP) / 10;
  Ax_es_ES_rtd_SR_v_SR = (int)(Ax_es_ES_rtd_SR_v * 10);
  doc3["Solar"] = (float) (Ax_es_ES_rtd_SR_v_SR) / 10;
  doc3["Dew"] = (float)(Ax_es_ES_rtd_DP_v_DP) / 10 ;
  doc3["SeaL"] = Ax_es_ES_rtd_SL_v;

  doc3["Solar"] = (float) (Ax_es_ES_rtd_SR_v_SR) / 10;
  doc3["Dew"] = (float)(Ax_es_ES_rtd_DP_v_DP) / 10 ;
  doc3["SeaL"] = Ax_es_ES_rtd_SL_v;



  DynamicJsonDocument doc2(1024);
  DynamicJsonDocument doc5(1024);
  DynamicJsonDocument doc6(1024);
  megadoc = doc;


  doc5["metrics"] = doc3; //Inside Greenhouse Data
  doc6["metrics"] = doc4;  //Outside Greenhouse Data (Weather Station)


  serializeJson(doc5, tempString2);
  Serial.println("Doc5 message: ");
  serializeJson(doc5, Serial);
  Serial.print("\n");


  client.publish(MQTT_PUBTOPIC, tempString2); //MQTT_MESHPUBTOPIC



  memset(tempString, 0, 1024);
  memset(tempString2, 0, 1024);
  memset(tempString3, 0, 1024);

}

void SendDataToMega() {
  Serial.println("Publish mega:");
  serializeJson(megadoc, Serial);
  Serial.print("\n");
   sendDataMQTT();
}

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

  if (JsonObject txcc_es = doc["txcc_es"])
  {
    long txcc_es_did = txcc_es["did"]; // 11607560
    const char* txcc_es_ts = txcc_es["ts"]; // "12:07:01"

    JsonObject txcc_es_ES_rtd = txcc_es["ES_rtd"];
    double ES_rtd_AT_v = txcc_es_ES_rtd["AT_v"]; // 27.79999924
    double ES_rtd_RH_v = txcc_es_ES_rtd["RH_v"]; // 42.09999847
    double ES_rtd_VPD_v = txcc_es_ES_rtd["VPD_v"]; // 2.099999905
    double ES_rtd_HI_v = txcc_es_ES_rtd["HI_v"]; // 27.60000038
    int ES_rtd_Co2_v = txcc_es_ES_rtd["Co2_v"]; // 0
    long ES_rtd_ET_v = txcc_es_ES_rtd["ET_v"]; // 214748368
    double ES_rtd_BP_v = txcc_es_ES_rtd["BP_v"]; // 946.2999878
    double ES_rtd_DP_v = txcc_es_ES_rtd["DP_v"]; // 13.80000019
    int ES_rtd_SL_v = txcc_es_ES_rtd["SL_v"]; // 1137



    RAT.addValue(ES_rtd_AT_v);
    RAH.addValue(ES_rtd_RH_v);
    RAHI.addValue(ES_rtd_HI_v);
    RAVPD.addValue(ES_rtd_VPD_v);
    RACO2.addValue(ES_rtd_Co2_v);
    RAET.addValue(ES_rtd_ET_v);
    RABP.addValue(ES_rtd_BP_v);
    RASOL.addValue(ES_rtd_SL_v);
    RADEW.addValue(ES_rtd_DP_v);


    Ax_es_ES_rtd_AT_v_Avg = RAT.getAverage();
    Ax_es_ES_rtd_RH_v_Avg = RAH.getAverage();
    Ax_es_ES_rtd_HI_v_Avg = RAHI.getAverage();
    Ax_es_ES_rtd_VPD_v_Avg = RAVPD.getAverage();
    Ax_es_ES_rtd_Co2_v = RACO2.getAverage();
    Ax_es_ES_rtd_ET_v = RAET.getAverage();
    Ax_es_ES_rtd_BP_v = RABP.getAverage();
    Ax_es_ES_rtd_SR_v = RASOL.getAverage();
    Ax_es_ES_rtd_DP_v = RADEW.getAverage();

  }
  else if (JsonObject Ws = doc["Ws"]) {

    long Ws_did = Ws["did"]; // 3608632
    const char* Ws_ts = Ws["ts"]; // "11:35:38"
    Ax_ws_ws_rtd_AT = Ws["AT_v"]; // 26.70000076
    Ax_ws_ws_rtd_RH = Ws["RH_v"]; // 64
    Ax_ws_ws_rtd_VPD = Ws["VPD_v"]; // 1.200000048
    Ax_ws_ws_rtd_HI = Ws["HI_v"]; // 28
    Ax_ws_ws_rtd_ET = Ws["ET_v"]; // 0
    Ax_ws_ws_rtd_BP = Ws["BP_v"]; // 939.2999878
    Ax_ws_ws_rtd_SR = Ws["SR_v"]; // 280.5175476
    Ax_ws_ws_rtd_DP = Ws["DP_v"]; // 19.29999924
    Ax_ws_ws_rtd_SL = Ws["SL_v"]; // 1129
    Ax_ws_ws_rtd_WSclk = Ws["WSclk"]; // 0
    Ax_ws_ws_rtd_WSaclk = Ws["WSaclk"]; // 0
    Ax_ws_ws_rtd_WDclk = Ws["WDclk"]; // 0
    Ax_ws_ws_rtd_WDaclk = Ws["WDaclk"]; // 0



  }
  else if (JsonObject As = doc["As"]) {

    if (JsonObject As_ZA = doc["As"]["ZA"]) {
      long As_ZA_Zdid = As_ZA["Zdid"]; // 14255503
      const char* As_ZA_ts = As_ZA["ts"]; // "16:25:10"
      As_ZA_WIF_v = As_ZA["WIF_v"]; // 12
      As_ZA_WOF_v = As_ZA["WOF_v"]; // 52
      As_ZA_WTL_v = As_ZA["WTL_v"]; // 31
      As_ZA_WTSP_v = As_ZA["WTSP_v"]; // 82
    }
    else if (JsonObject As_ZB = doc["As"]["ZB"]) {
      long As_ZB_did = As_ZB["did"]; // 14255503
      const char* As_ZB_ts = As_ZB["ts"]; // "16:25:10"
      int As_ZB_WIF_v = As_ZB["WIF_v"]; // 12
      int As_ZB_WOF_v = As_ZB["WOF_v"]; // 52
      int As_ZB_WTL_v = As_ZB["WTL_v"]; // 31
      int As_ZB_WTSP_v = As_ZB["WTSP_v"]; // 82
    }

  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      client.subscribe(MQTT_SUBTOPIC);
      client.subscribe(sendtopic_WSMesh);
      client.subscribe(sendtopic_ESMesh);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {

  Serial.begin(115200);
  RAT.clear();
  RAH.clear();
  RAHI.clear();
  RAVPD.clear();
  RACO2.clear();
  RAET.clear();
  RABP.clear();
  RASOL.clear();
  RADEW.clear();
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


unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  timeClient.update();
  client.loop();

  unsigned long now1 = millis();
  if (now1 - lastMsg > 1000) {
    lastMsg = now1;
    sendsetpointmega();
  }
  unsigned long now2 = millis();
  if (now2 - lastMsg01 > 4000) {
    lastMsg01 = now2;
    SendDataToMega();
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
