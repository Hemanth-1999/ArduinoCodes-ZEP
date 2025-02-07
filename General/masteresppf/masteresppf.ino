/*//README...NOTE.........
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
   Last Upadte...06/07/22  --PFC Master Esp V1.1

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
unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


///MQTT SETUP
const char* ssid = "Zeptogreens";//"Zeptogreens";//"Zepto";//"Zeptogreens"; // "Zeptogreens";//replace this with your wifi access point
//name
const char* password = "Zeptogreens@20"; //"Zeptogreens@20";// "Zepto@2022";//"Zepto@2022";//"Zeptogreens20"; //replace with your wifi password
const char* broker = "192.168.1.102";// // "192.168.1.183"; //"192.168.1.102";//IP address of machine on which broker is installed
const int port = 1883;
const char* mqttUser = "Zeptogreens";//"zepto"; //"zepto"; Zeptogreens
const char* mqttPassword = "123456";//"12345";
char mqtt_send[50] = "account-name/PD1/W1/A1/ClimateData";

char MQTT_PUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/PFCData"; //working
char MQTT_MESHPUBTOPIC[50] = "Chinnamangalaram/ZoneA/RawData";
//char MQTT_SUBTOPIC[50] = "Saifarm/GH1/ZoneA/Node";
char MQTT_SUBTOPIC[50] = "account/Zepto/clientid/appid/climatedata";
#define MQTT_PUBTOPIC_TEST "Saifarm/GH1/appid/test/testdata"
#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];
char tempString[1024];
char tempString2[512];
char tempString3[1024];
int samples = 0;
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
float As_ZA_WIF_v , As_ZA_WOF_v , As_ZA_WTL_v ,As_ZA_WTSP_v ;
float avg(float, float);

unsigned long previousMillis = 0;
const long interval = 5000;

void sendsensordata();
void sendWSsensordata();
RunningAverage RAT(5);
RunningAverage RAH(5);
RunningAverage RAHI(5);
RunningAverage RAVPD(5);

WiFiClient espClient;
PubSubClient client(espClient);
bool MQTTFlag;
float avg(float, float);
void SendDataToMega();
void sendDataMQTT();
//Task t3(3000, TASK_FOREVER, &SendDataToMega, &r);
//Task sendDataMQTTTask(10000, TASK_FOREVER, &sendDataMQTT, &r);

uint32_t chipid;

DynamicJsonDocument megadoc(1024);
DynamicJsonDocument megadoc1(1024);
DynamicJsonDocument doc(1024);

void sendDataMQTT() {

//  time_t epochTime = timeClient.getEpochTime();/

//  String formattedTime = timeClient.getFormattedTime();/

//  if (MQTTFlag == true) {
chipid = ESP.getChipId();

DynamicJsonDocument doc3(512);
DynamicJsonDocument doc4(512);
//    doc["Time"] = formattedTime;/
//    doc["ID"] = chipid;/
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

//    doc3["Time"] = formattedTime;/
//    doc3["ID"] = chipid;/

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

doc3["WIF_v"] = As_ZA_WIF_v ; // 12
doc3["WOF_v"] =   As_ZA_WOF_v ; // 52
doc3["WTL_v"] =   As_ZA_WTL_v; // 31
doc3["WTSP_v"] =    As_ZA_WTSP_v; // 82

DynamicJsonDocument doc2(1024);
DynamicJsonDocument doc5(1024);
DynamicJsonDocument doc6(1024);
megadoc = doc;
//megadoc = doc3;
// megadoc1 = doc4;
//    doc2["metrics"] = doc;  //IN and OUT Data
doc5["metrics"] = doc3; //Inside Greenhouse Data
doc6["metrics"] = doc4;  //Outside Greenhouse Data (Weather Station)

//        Serial.println("Publish msg tO Pi:");
//        serializeJson(doc2, tempString);
//        Serial.println("Doc2 message: ");
//        serializeJson(doc2, Serial);
//    Serial.print("\n");

serializeJson(doc5, tempString2);
Serial.println("Doc5 message: ");
serializeJson(doc5, Serial);
Serial.print("\n");

//    client.publish(MQTT_PUBTOPIC, tempString); //MQTT_MESHPUBTOPIC
client.publish(MQTT_PUBTOPIC, tempString2); //MQTT_MESHPUBTOPIC
client.publish(MQTT_PUBTOPIC_TEST, tempString2); //MQTT_MESHPUBTOPIC
//    client.publish(MQTT_PUBTOPIC, tempString3); //MQTT_MESHPUBTOPIC/


//    client.publish(MQTT_MESHPUBTOPIC, tempString);
client.publish(MQTT_MESHPUBTOPIC, tempString2);
//    client.publish(MQTT_MESHPUBTOPIC, tempString3);/

memset(tempString, 0, 1024);
memset(tempString2, 0, 1024);
memset(tempString3, 0, 1024);

}

void SendDataToMega() {
Serial.println("Publish mega:");
serializeJson(megadoc, Serial);
Serial.print("\n");
//serializeJson(megadoc1, Serial);
//Serial.print("\n");
}

void callback(char* topic, byte * payload, unsigned int length) {
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
if (JsonObject Es = doc["Es"]) {

long Es_did = Es["did"]; // 11839312
const char* Es_ts = Es["ts"]; // "11:35:40"
int Ax_es_ES_rtd_AT_v = Es["AT_v"]; // 27
double Ax_es_ES_rtd_RH_v = Es["RH_v"]; // 65.59999847
double Ax_es_ES_rtd_VPD_v = Es["VPD_v"]; // 1.200000048
double Ax_es_ES_rtd_HI_v = Es["HI_v"]; // 28.60000038
Ax_es_ES_rtd_Co2_v = Es["Co2_v"]; // 2147483647
Ax_es_ES_rtd_ET_v = Es["ET_v"]; // -0.300000012
Ax_es_ES_rtd_BP_v = Es["BP_v"]; // 938.7000122
Ax_es_ES_rtd_SR_v = Es["SR_v"]; // 273.5689392
Ax_es_ES_rtd_DP_v = Es["DP_v"]; // 20
Ax_es_ES_rtd_SL_v = Es["SL_v"]; // 1129
RAT.addValue(Ax_es_ES_rtd_AT_v);
RAH.addValue(Ax_es_ES_rtd_RH_v);
RAHI.addValue(Ax_es_ES_rtd_HI_v);
RAVPD.addValue(Ax_es_ES_rtd_VPD_v);

//    Serial.print("ES Station");
//    Serial.print(Es_did);
//    Serial.print("ES AT");
//    Serial.println(Ax_es_ES_rtd_AT_v);

Ax_es_ES_rtd_AT_v_Avg = RAT.getAverage();
Ax_es_ES_rtd_RH_v_Avg = RAH.getAverage();
Ax_es_ES_rtd_HI_v_Avg = RAHI.getAverage();
Ax_es_ES_rtd_VPD_v_Avg = RAVPD.getAverage();
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
//  int Ws_ntips = Ws["ntips"]; // 0

//    Serial.print("Weather Station");
//    Serial.print(Ws_did);
//    Serial.print("Weather AT");
//    Serial.println(Ax_ws_ws_rtd_AT);
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

  if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
    //      Serial.println("connected");
    // Once connected, publish an announcement...
    //      client.publish("outTopic", "hello world");
    // ... and resubscribe
    client.subscribe(MQTT_SUBTOPIC);
    // client.setCallback(callback);
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
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    yield();
  }
  Serial.println("Connected to the WiFi network");

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);

  //MQTT-SetUP
  client.setServer(broker, port);
  //  client.subscribe("account/Zepto/clientid/appid/climatedata");
  client.setCallback(callback);


}



void loop() {
  if (!client.connected()) {
    reconnect();
  }
  timeClient.update();
  client.loop();

  unsigned long now1 = millis();
  if (now1 - lastMsg > 8000) {
    lastMsg = now1;
    sendDataMQTT();

  }
  unsigned long now2 = millis();
  if (now2 - lastMsg01 > 3000) {
    lastMsg01 = now2;
    // client.subscribe("account/Zepto/clientid/appid/climatedata");
    // Serial.print("Publish message: ");
    SendDataToMega();

  }

}
