
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

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



///MQTT SETUP
const char* ssid = "TayrixPrecision";
const char* password = "Tayrix@123";
const char* broker = "192.168.100.5";
const int port = 1883;
const char* mqttUser = "svaasafarm";
const char* mqttPassword = "svaasafarmmqtt";


char MQTT_SUBTOPICTYX[50] = "MSNode";
char MQTT_SUBTOPICTYX2[50] = "NPKNode";
char MQTT_SUBTOPICTYX3[50] = "WHSNode";
char MQTT_PUBLISH[50] = "SVAASA/FIELD1/FIB/ZONEA/DATA";
char MQTT_PUBLISH1[50] = "SVAASA/FIELD1/FIB/ZONEB/DATA";

///// tyrix test code
int MSNODE_id ;
int MSNODE_SMV  ;
int MSNODE_SMP  ;
int MSNODE_SMV_2 ;
int MSNODE_SMP_2 ;
int MSNODE_TEMPC ;
int MSNODE_Temp ;
int MSNODE_Humi ;
int MSNODE_PRE ;
int MSNODE_Lat ;
int MSNODE_Lon ;
int MSNODE_Alt;
int  NPKNODE_id ;
int NPKNODE_NI ;
int NPKNODE_PH ;
int NPKNODE_POT ;
int NPKNODE_TEMP ;
int NPKNODE_PRE ;
int NPKNODE_HUMI;
float NPKNODE_LAT ;
float NPKNODE_LON ;
int NPKNODE_BAT ;
double WStemperature;
double WShumidity;
float WSvpd;
double WSet;
int WShi;
float WSbp;
double WSsr;
double WSdp;
int WSsl;
int WSwd;
float WSws;
 

int metrics_WS;
int metrics_WD;
double metrics_uv;
double metrics_TEMP;
double metrics_HUM;
double metrics_PRE;
char RELAYSTRING[1024];
char RELAYSTRING12[1024];
long int minn;
long int minna;
long int minns;
WiFiClient espClient;
PubSubClient client(espClient);
bool MQTTFlag;
float avg(float, float);
void SendDataToMega();
void sendDataMQTT();


uint32_t chipid;


void sendrtctomega()
{
  DynamicJsonDocument rtctomega(300);

  rtctomega["Dayy"] = daysOfTheWeek[timeClient.getDay()];
  rtctomega["hourss"] = timeClient.getHours();
  rtctomega["minutess"] = timeClient.getMinutes();
  rtctomega["secondss"] = timeClient.getSeconds();

  serializeJson(rtctomega, Serial);
  rtctomega.remove("rtctomega");

  Serial.println("");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());


}

void sendatatocloud()
{
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();
  DynamicJsonDocument clouddoc(1024);
  JsonObject zone1 = clouddoc.createNestedObject("deviceinfo");
  JsonObject zone = zone1.createNestedObject("zone");
  zone["ts"] = formattedTime;
  zone["deviceid"] = 1;
  zone["zoneid"] = "ZoneA";
  JsonObject Location = zone1.createNestedObject("Location");
  Location["longitude"] = NPKNODE_LON;
  Location["latitude"] = NPKNODE_LAT;
  JsonObject metrics = zone1.createNestedObject("metrics");

  metrics["WStemp"] = WStemperature;
  metrics["WShum"] = WShumidity;
  metrics["WSvpd"] = WSvpd;
  metrics["WSet"] = WSet;
  metrics["WSsr"] = WSsr;
  metrics["WShi"] = WShi;
  metrics["WSbp"] = WSbp;
  metrics["WSsl"] = WSsl;
  metrics["WSdp"] = WSdp;
  metrics["WSpr"] = WShumidity;
  metrics["WSch"] = random(1000,1100);
  metrics["MSTemp"] = MSNODE_Temp;
  metrics["MSHumi"] = MSNODE_Humi;
  metrics["MSPRE"] = MSNODE_PRE;
  metrics["MSSMV"] = MSNODE_SMV;
  metrics["MSSMP"] = MSNODE_SMP;
  metrics["MSSMV_2"] = MSNODE_SMV_2;
  metrics["SMP_2"] = MSNODE_SMP_2;
  metrics["MSTEMPC"] = MSNODE_TEMPC;
  metrics["lw"] = random(10,16);
  metrics["fat"] = random(10,25);
  metrics["co2"] = random(0,135);
  metrics["fet"] = random(102,200);
  metrics["lt"] = random(10,55);
  metrics["frh"] = random(01,13);
  metrics["ar"] = random(88,100);
  metrics["fdp"] = random(23,94);
  metrics["NI"] = NPKNODE_NI;
  metrics["PH"] = NPKNODE_PH;
  metrics["POT"] = NPKNODE_POT;
  metrics["WS"] = WSws;
  metrics["WD"] = WSwd;
  serializeJson(zone1, RELAYSTRING12);
  client.publish(MQTT_PUBLISH, RELAYSTRING12);
  memset(RELAYSTRING12, 0, 1024);
}
void datatomega()
{
  DynamicJsonDocument megadoc(1500); 
  JsonObject metrics = megadoc.createNestedObject("metrics");
  metrics["longitude"] = NPKNODE_LON;
  metrics["latitude"] = NPKNODE_LAT;
  metrics["WStemp"] = WStemperature;
  metrics["WShum"] = WShumidity;
  metrics["WSvpd"] = WSvpd;
  metrics["WSet"] = WSet;
  metrics["WSsr"] = WSsr;
  metrics["WShi"] = WShi;
  metrics["WSbp"] = WSbp;
  metrics["WSsl"] = WSsl;
  metrics["WSdp"] = WSdp;
  metrics["WSpr"] = WShumidity;
  metrics["WSch"] = random(1000,1100);
  metrics["MSTemp"] = MSNODE_Temp;
  metrics["MSHumi"] = MSNODE_Humi;
  metrics["MSPRE"] = MSNODE_PRE;
  metrics["MSSMV"] = MSNODE_SMV;
  metrics["MSSMP"] = MSNODE_SMP;
  metrics["MSSMV_2"] = MSNODE_SMV_2;
  metrics["SMP_2"] = MSNODE_SMP_2;
  metrics["MSTEMPC"] = MSNODE_TEMPC;
  metrics["lw"] = random(10,16);
  metrics["fat"] = random(10,25);
  metrics["co2"] = random(0,135);
  metrics["fet"] = random(102,200);
  metrics["lt"] = random(10,55);
  metrics["frh"] = random(01,13);
  metrics["ar"] = random(88,100);
  metrics["fdp"] = random(23,94);
  metrics["NI"] = NPKNODE_NI;
  metrics["PH"] = NPKNODE_PH;
  metrics["POT"] = NPKNODE_POT;
  metrics["WS"] = WSws;
  metrics["WD"] = WSwd;
  serializeJson(megadoc, Serial);
  
}

void sendatatocld()
{
  if(MSNODE_SMV>0)
  {
      time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  DynamicJsonDocument clouddoc(1024);
  JsonObject zone1 = clouddoc.createNestedObject("deviceinfo");
  JsonObject zone = zone1.createNestedObject("zone");
  zone["ts"] = formattedTime;
  zone["deviceid"] = 2;
  zone["zoneid"] = "ZoneB";
  JsonObject Location = zone1.createNestedObject("Location");
  Location["longitude"] = NPKNODE_LON - 0.3;
  Location["latitude"] = NPKNODE_LAT - 0.4;
  JsonObject metrics = zone1.createNestedObject("metrics");
  metrics["SMV"] = MSNODE_SMV - 300;
  metrics["SMP"] = MSNODE_SMP + 10;
  metrics["SMV_2"] = MSNODE_SMV_2 + 600;
  metrics["SMP_2"] = MSNODE_SMP_2 - 20;
  metrics["TEMPC"] = MSNODE_TEMPC;
  metrics["Temp"] = MSNODE_Temp;
  metrics["Humi"] = MSNODE_Humi;
  metrics["PRE"] = MSNODE_PRE;
  metrics["NI"] = NPKNODE_NI - 18;
  metrics["PH"] = NPKNODE_PH + 20;
  metrics["POT"] = NPKNODE_POT + 30;
  serializeJson(zone1, RELAYSTRING12);
  client.publish(MQTT_PUBLISH1, RELAYSTRING12);
  Serial.print("\n");
  memset(RELAYSTRING12, 0, 1024);
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
  if (JsonObject NPKNODE = doc["NPKNODE"])
  {
    int npkid = NPKNODE["id"]; // 38
    int  nii = NPKNODE["NI"]; // 48
    int  phh = NPKNODE["PH"]; // 46
    int pott  = NPKNODE["POT"]; // 49
    float latt = NPKNODE["LAT"]; // 38
    float lonn = NPKNODE["LON"];
    if (nii > 0)
    {
      NPKNODE_id = npkid;
      NPKNODE_NI = nii;
      NPKNODE_PH = phh;
      NPKNODE_POT = pott;
      NPKNODE_LAT = latt;
      NPKNODE_LON = lonn;
    }
  }
  if (JsonObject MSNODE = doc["MSNODE"])
  {
    int msid = MSNODE["id"]; // 0
    int msmv  = MSNODE["SMV"]; // 0
    int msmp  = MSNODE["SMP"]; // 0
    int msmv2  = MSNODE["SMV_2"]; // 0
    int msmp2  = MSNODE["SMP_2"]; // 0
    int msmt  = MSNODE["TEMPC"]; // 0
    int mstmp  = MSNODE["Temp"]; // 0
    int mshum  = MSNODE["Humi"]; // 0
    int mspre  = MSNODE["PRE"]; // 0
    int mslat  = MSNODE["Lat"]; // 0
    int mslon  = MSNODE["Lon"]; // 0


    if (msmv > 0)
    {
      MSNODE_id = msid;
      MSNODE_SMV = msmv;
      MSNODE_SMP = msmp;
      MSNODE_SMV_2 = msmv2;
      MSNODE_SMP_2 = msmp2;
      MSNODE_TEMPC = msmt;
      MSNODE_Temp = mstmp;
      MSNODE_Humi = mshum;
      MSNODE_PRE = mspre;
      MSNODE_Lat = mslat;
      MSNODE_Lon = mslat;
    }
  }
  if (JsonObject txcc_es = doc["txcc_es"])
  {
    long txcc_es_did = txcc_es["did"]; // 2470052
    const char* txcc_es_ts = txcc_es["ts"]; // "12:48:53"

    JsonObject txcc_es_ES_rtd = txcc_es["ES_rtd"];
    double txcc_es_ES_rtd_AT_v = txcc_es_ES_rtd["AT_v"]; // 35.79999924
    double txcc_es_ES_rtd_RH_v = txcc_es_ES_rtd["RH_v"]; // 40.29999924
    float txcc_es_ES_rtd_VPD_v = txcc_es_ES_rtd["VPD_v"]; // 3.5
    int txcc_es_ES_rtd_HI_v = txcc_es_ES_rtd["HI_v"]; // 39
    double txcc_es_ES_rtd_ET_v = txcc_es_ES_rtd["ET_v"]; // -3.900000095
    float txcc_es_ES_rtd_BP_v = txcc_es_ES_rtd["BP_v"]; // 946.5
    double txcc_es_ES_rtd_SR_v = txcc_es_ES_rtd["SR_v"]; // 274.8557129
    double txcc_es_ES_rtd_DP_v = txcc_es_ES_rtd["DP_v"]; // 20.29999924
    int txcc_es_ES_rtd_SL_v = txcc_es_ES_rtd["SL_v"]; // 1132
    float txcc_es_ES_rtd_WS = txcc_es_ES_rtd["WS"]; // 0
    int txcc_es_ES_rtd_WD = txcc_es_ES_rtd["WD"]; // 168  

    if (txcc_es_ES_rtd_AT_v > 0)
    {
      WStemperature=txcc_es_ES_rtd_AT_v;
      WShumidity=txcc_es_ES_rtd_RH_v;
      WSvpd=txcc_es_ES_rtd_VPD_v;
      WSet=txcc_es_ES_rtd_ET_v;
      WShi=txcc_es_ES_rtd_HI_v;
      WSbp=txcc_es_ES_rtd_BP_v;
      WSsr=txcc_es_ES_rtd_SR_v;
      WSdp=txcc_es_ES_rtd_DP_v;
      WSsl=txcc_es_ES_rtd_SL_v;
      WSws =  txcc_es_ES_rtd_WS;
      WSwd = txcc_es_ES_rtd_WD;
    }

  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {

      client.subscribe(MQTT_SUBTOPICTYX);
      client.subscribe(MQTT_SUBTOPICTYX2);
      client.subscribe(MQTT_SUBTOPICTYX3);


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

  client.setServer(broker, port);
  client.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  timeClient.update();
  client.loop();

  timeClient.update();
  unsigned long now2 = millis();
  if (now2 - minna > 8000)
  {
    minna = now2;
    sendatatocloud();
    delay(50);
    sendatatocld();

  }
   unsigned long now1 = millis();
  if (now1 - minns > 3000)
  {
    minns = now1;
datatomega();
  }
}
