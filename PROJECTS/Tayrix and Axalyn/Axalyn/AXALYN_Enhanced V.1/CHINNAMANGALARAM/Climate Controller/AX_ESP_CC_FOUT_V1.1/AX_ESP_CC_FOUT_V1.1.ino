                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /*//README...NOTE.........
   Removed CS node
   ES node and WS Node Rec. Data
   Sending Data To MQTT - Kura

   Issus - WM Connection

   Serial Output------E.g
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
#include <WiFiManager.h>
#include "FS.h"
//#include "SPIFFS.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>

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

WiFiManager wm;
WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);
WiFiClient espClient;
PubSubClient client(espClient);

///MQTT SETUP
char hostname[40] = "164.52.223.248"; // or "cea.axalyn.com"
char port_str[40] = "1883"; // string representation of the port
uint16_t port = atoi(port_str); // convert port string to integer
char clientId[40] = "ESP32Client";
char user[40] = "axalyn";
char password[40] = "cenaura@2024";
String timestamp;
bool shouldSaveConfig = false;
char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/CLIMATECONTROL";
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

String lastData = "";
String receivedData;

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
int Manualstatus_f1 , Manualstatus_f2 , Manualstatus_f3 , Manualstatus_f4 , Manualstatus_f5 , Manualstatus_f6 , Manualstatus_f7 , Manualstatus_f8, Manualstatus_cp , Manualstatus_co , Manualstatus_fp , Manualstatus_fs , Manualstatus_ir, Manualstatus_do , Manualstatus_dr, Manualstatus_pb;

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

bool MQTTFlag;
float avg(float, float);
void SendDataToMega();
void sendDataMQTT();

uint32_t chipid;

void sendDataMQTT() {

  DynamicJsonDocument doc(1024);
  DynamicJsonDocument doc3(1024);

  chipid = ESP.getChipId();
  JsonObject AvgDoc = doc.createNestedObject("AvgData");
  int Tavg_AT = 0;
  Tavg_AT = (int)(Ax_es_ES_rtd_AT_v_Avg);
  AvgDoc["AT_V"] = (float)(Tavg_AT);

  int Havg_RH = 0;
  Havg_RH = (int)(Ax_es_ES_rtd_RH_v_Avg);
  AvgDoc["RH_v"] = (float)(Havg_RH);
  int VPDavg_VPD = 0;
  VPDavg_VPD = (int)(Ax_es_ES_rtd_VPD_v_Avg);
  AvgDoc["VPD_v"] = (float)(VPDavg_VPD);
  int  HIavg_HI = 0;
  HIavg_HI = (int)( Ax_es_ES_rtd_HI_v_Avg);
  AvgDoc["HI_v"] = (float)(HIavg_HI);

  AvgDoc["Co2_v"] = Ax_es_ES_rtd_Co2_v;

  int Ax_es_ES_rtd_ET_v_ET = 0;
  Ax_es_ES_rtd_ET_v_ET = (int)(Ax_es_ES_rtd_ET_v);
  AvgDoc["ET_v"] = (float)(Ax_es_ES_rtd_ET_v_ET);
  int Ax_es_ES_rtd_BP_v_BP = 0;
  Ax_es_ES_rtd_BP_v_BP = (int)(Ax_es_ES_rtd_BP_v);
  AvgDoc["BP_v"] = (float)(Ax_es_ES_rtd_BP_v_BP);
  int Ax_es_ES_rtd_SR_v_SR = 0;
  Ax_es_ES_rtd_SR_v_SR = (int)(Ax_es_ES_rtd_SR_v);
  AvgDoc["SR_v"] = (float) (Ax_es_ES_rtd_SR_v_SR);
  int Ax_es_ES_rtd_DP_v_DP = 0;
  Ax_es_ES_rtd_DP_v_DP = (int)(Ax_es_ES_rtd_DP_v);
  AvgDoc["DP_v"] = (float)(Ax_es_ES_rtd_DP_v_DP);
  AvgDoc["SL_v"] = Ax_es_ES_rtd_SL_v;

  //**Weather Station-------------------------------------------------------------
  int Ax_ws_ws_rtd_AT_AT = 0;
  Ax_ws_ws_rtd_AT_AT = (int)(Ax_ws_ws_rtd_AT);
  doc["WAT"] = (float)(Ax_ws_ws_rtd_AT_AT);
  int Ax_ws_ws_rtd_RH_RH = 0;

  Ax_ws_ws_rtd_RH_RH = (int)(Ax_ws_ws_rtd_RH);
  doc["WRH"] = (float) (Ax_ws_ws_rtd_RH_RH);
  int Ax_ws_ws_rtd_VPD_VPD = 0;

  Ax_ws_ws_rtd_VPD_VPD = (int)(Ax_ws_ws_rtd_VPD);
  doc["WVPD"] = (float)( Ax_ws_ws_rtd_VPD_VPD);
  int Ax_ws_ws_rtd_HI_HI = 0;
  Ax_ws_ws_rtd_HI_HI = (int)(Ax_ws_ws_rtd_HI);
  doc["WHI"] = (float)(Ax_ws_ws_rtd_HI_HI);

  int Ax_ws_ws_rtd_ET_ET = 0;
  Ax_ws_ws_rtd_ET_ET = (int)(Ax_ws_ws_rtd_ET);
  doc["WEv"] = (float)(Ax_ws_ws_rtd_ET_ET);

  int Ax_ws_ws_rtd_BP_BP = 0;
  Ax_ws_ws_rtd_BP_BP = (int)(Ax_ws_ws_rtd_BP);
  doc["WBaPa"] = (float) (Ax_ws_ws_rtd_BP_BP);
  int Ax_ws_ws_rtd_SR_SR = 0;
  Ax_ws_ws_rtd_SR_SR = (int)(Ax_ws_ws_rtd_SR);
  doc["WSolar"] = (float)(Ax_ws_ws_rtd_SR_SR);
  int Ax_ws_ws_rtd_DP_DP = 0;
  Ax_ws_ws_rtd_DP_DP = (int)(Ax_ws_ws_rtd_DP);
  doc["WDew"] = (float)(Ax_ws_ws_rtd_DP_DP);
  doc["WSeaL"] = Ax_ws_ws_rtd_SL;

  doc["WSclk"] = Ax_ws_ws_rtd_WSclk;
  doc["WSaclk"] = Ax_ws_ws_rtd_WSaclk;
  doc["WDclk"] = Ax_ws_ws_rtd_WDclk;
  doc["WDaclk"] = Ax_ws_ws_rtd_WDaclk;

  doc3["AT_V"] = (float)(Tavg_AT);
  doc3["RH_v"] = (float)(Havg_RH);
  VPDavg_VPD = (int)(VPDavg);
  doc3["VPD_v"] = (float)(VPDavg_VPD);
  doc3["HI_v"] = (float)(HIavg_HI);
  doc3["Co2_v"] = Ax_es_ES_rtd_Co2_v;
  doc3["ET_v"] = (float)(Ax_es_ES_rtd_ET_v_ET);
  doc3["BP_v"] = (float)(Ax_es_ES_rtd_BP_v_BP);
  Ax_es_ES_rtd_SR_v_SR = (int)(Ax_es_ES_rtd_SR_v);
  doc3["SR_v"] = (float) (Ax_es_ES_rtd_SR_v_SR);
  doc3["DP_v"] = (float)(Ax_es_ES_rtd_DP_v_DP);
  doc3["SL_v"] = Ax_es_ES_rtd_SL_v;

  doc3["SR_v"] = (float) (Ax_es_ES_rtd_SR_v_SR);
  doc3["DP_v"] = (float)(Ax_es_ES_rtd_DP_v_DP);
  doc3["SL_v"] = Ax_es_ES_rtd_SL_v;

  char Node[1024];

  memset(Node, 0, 1024);

}

void SendDataToMega() {
  sendDataMQTT();
}

void eventstatus()
{
  String DataFromMega = Serial.readStringUntil('\n');
  DynamicJsonDocument event(500);
  deserializeJson(event, Serial);
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

void alert()
{
  String DataFromMega = Serial.readStringUntil('\n');
  DynamicJsonDocument alertfrom(500);
  deserializeJson(alertfrom, Serial);
  DeserializationError error = deserializeJson(alertfrom, Serial);
  if (error) {
    return;
  }

  Alerts_Mode = alertfrom["Mode"];
  Alerts_level = alertfrom["level"];

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

DynamicJsonDocument doc(1024);

double ES_rtd_AT_v = 0;
double ES_rtd_RH_v = 0;
double ES_rtd_VPD_v = 0;
double ES_rtd_HI_v = 0;
int ES_rtd_Co2_v = 0;
long ES_rtd_ET_v = 0;
double ES_rtd_BP_v = 0;
double ES_rtd_SR_v = 0;
double ES_rtd_DP_v = 0;
int ES_rtd_SL_v = 0;

bool newData = false;
String receivedPayload;

char avgData[1024];
DynamicJsonDocument avgDoc(1024);

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);  // For communication with Mega 2560
  RAT.clear();
  RAH.clear();
  RAHI.clear();
  RAVPD.clear();
  RACO2.clear();
  RAET.clear();
  RABP.clear();
  RASOL.clear();
  RADEW.clear();

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

unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
void loop()
{
  // Retry loop if WiFi connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to reconnect to WiFi...");

    // Attempt to reconnect to Wi-Fi
    WiFi.reconnect();  // Try to reconnect to the last connected Wi-Fi network
    Serial.println("Reconnection failed, retrying...");
    delay(5000);  // Wait 5 seconds before retrying
    return;  // Skip the rest of the loop if not connected

    //    WiFi.begin(); // Retry connection to the previously saved network
    //    int retryCount = 0;
    //
    //    // Wait for a short period for the connection to establish
    //    while (WiFi.status() != WL_CONNECTED && retryCount < 10) {
    //      delay(1000);
    //      Serial.print(".");
    //      retryCount++;
    //    }
    //
    // If connected
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to WiFi."); \
    }

    // If not connected after retries, reset the device
    //    if (retryCount >= 10) {
    //      Serial.println("Failed to reconnect. Restarting...");
    //      ESP.restart();
    //    }
  }

  HandleMqttState();
  timeClient.update();
  client.loop();

  unsigned long now1 = millis();
  if (now1 - lastMsg > 3000) {
    lastMsg = now1;
    processData();
  }

  unsigned long now2 = millis();
  if (now2 - lastMsg01 > 1000) {
    lastMsg01 = now2;
    sendsetpointmega();
  }

  unsigned long torecievecurrentMillis = millis();
  if (torecievecurrentMillis - torecievestatus >= 5000)
  {
    torecievestatus = torecievecurrentMillis;
    alert();
    delay(15); // Consider removing this if possible
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
    //    tocloud();
  }

  // Send the latest received payload to Mega 2560
  if (receivedPayload.length() > 0) {
    serializeJson(avgDoc, avgData);
    // Send the averaged data to Mega 2560
    Serial.println("Publish mega:");
    serializeJson(avgDoc, Serial);
    Serial.println(avgData);
    tocloud();
  } else {
    // Keep sending the last received data
    if (strlen(avgData) > 0) {
      serializeJson(avgDoc, Serial);
      Serial.println(avgData);
    } else {
      Serial.println("Data didn't receive yet");
    }
  }
}
