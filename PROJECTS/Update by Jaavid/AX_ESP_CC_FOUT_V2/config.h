#include <PubSubClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <RunningAverage.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FS.h>
//*******************************************NTP TIME CREDENTIALS****************************
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};



//******************************************RUNNING AVERAGE VARIABLES**********************
RunningAverage RAT(5);
RunningAverage RAH(5);
RunningAverage RAHI(5);
RunningAverage RAVPD(5);
RunningAverage RACO2(5);
RunningAverage RAET(5);
RunningAverage RABP(5);
RunningAverage RASOL(5);
RunningAverage RADEW(5);
RunningAverage RAUV(5);
void clearRunningAverage();
float _AT;//INSIDE NODE AVERAGE TEMPERATURE FROM BME SENSOR
float _HUM;//INSIDE NODE AVERAGE HUMIDITY
float _HI;//INSIDE NODE AVERAGE HEAT INDEX
float _VPD;//INSIDE NODE AVERAGE VPD
float _CO2;//INSIDE NODE AVERAGE CO2 
float _ET;//INSIDE NODE AVERAGE ENVIRONMENTAL TEMPERATURE CALCULATION
float _BP;//INSIDE NODE AVERAGE PRESSURE
float _SL;//INSIDE NODE AVERAGE SEA LEVEL PRESSURE
float _DP;//INSIDE NODE AVERAGE DWE POINT
float _UV;

float O_AT;//OUTSIDE NODE AVERAGE TEMPERATURE FROM BME SENSOR
float O_HUM;//OUTSIDE NODE AVERAGE HUMIDITY
float O_HI;//OUTSIDE NODE AVERAGE HEAT INDEX
float O_VPD;//OUTSIDE NODE AVERAGE VPD
float O_CO2;//OUTSIDE NODE AVERAGE CO2 
float O_ET;//OUTSIDE NODE AVERAGE ENVIRONMENTAL TEMPERATURE CALCULATION
float O_BP;//OUTSIDE NODE AVERAGE PRESSURE
float O_SL;//OUTSIDE NODE AVERAGE SEA LEVEL PRESSURE
float O_DP;//OUTSIDE NODE AVERAGE DWE POINT
float O_UV;
//**************************************MILLIS CREDENTIALS********************************
unsigned long tocloudstatus = 0;
unsigned long torecievestatus = 0;
unsigned long torecievestatusm = 0;
unsigned long lastMsg   = 0;
unsigned long lastMsg01 = 0;
unsigned long lastMsg02 = 0;
unsigned long lastMsg03 = 0;
unsigned long lastMsg04 = 0;
unsigned long lastMsg05 = 0;
unsigned long lastMsg06 = 0;
//*************************************MQTT CREDENTIALS*******************************
WiFiClient espClient;
PubSubClient client(espClient);
struct mqtt
{
  char* Broker=NULL;
  int Port;
  char* Username=NULL;
  char* Password=NULL;
  char* Clientid=NULL;
  char* Topic=NULL;
  char* Farm=NULL;
  char* Zone=NULL;
  char*Field=NULL;
}mqttcredentials;
//char MQTT_SUBTOPIC[50] = "Saifarm/GH2/PFC/ZoneA/CLIMATECONTROL";
const char* sendtopic_ESMesh = "account/Zepto/clientid/appid/climatedata/ES";//nodes that are inside farm
const char* sendtopic_WSMesh = "account/Zepto/clientid/appid/climatedata/WS";//outside node sends data to this topic
const char* sendtopic_app = "climate/app";
void callback(char* topic, byte * payload, unsigned int length);

//******************************************MANUAL RELAY VARIABLES ***********************
int Manualstatus_f1;
int Manualstatus_f2;
int Manualstatus_f3; 
int Manualstatus_f4; 
int Manualstatus_f5; 
int Manualstatus_f6; 
int Manualstatus_f7; 
int Manualstatus_f8; 
int Manualstatus_cp; //cp means cooling pads
int Manualstatus_co; 
int Manualstatus_fp; 
int Manualstatus_fs; 
int Manualstatus_ir; 
int Manualstatus_do; 
int Manualstatus_dr; 
int Manualstatus_pb;

//*********************************************ALERT MESSAGE FROM ATMEGA******************
int Alerts_Mode;
int Alerts_level;
int Events_TEMP ;
int Events_VPD ;
int Events_IRR ;
const char* eventogo;
const char* eventogo1;
const char* eventogo2;
const char* alertmode1;
const char* alertlevel;
void alert();
void eventstatus();
//********************************************EVENT MESSAGE FROM AT MEGA******************
void eventstatus();
//SOMETHING FISHY. THIS IS USING VARIABLES OF ALERT AND void eventstatus() TO SET ALERT LEVELS
//SO WHAT MY DOUBT IS ALREADY ALERTS ARE DOING THIS THEY WHY AGAIN EVENTS?

//**************************************************ROGUE GLOBAL VARIABLES*********************
uint32_t chipid;

//******************************************CONFIGUR WIFI PORTAL************************************
WiFiManager wm;
bool wm_nonblocking = false;
  WiFiManagerParameter custom_mqtt_client_id;
  WiFiManagerParameter custom_mqtt_server;
  WiFiManagerParameter custom_mqtt_port;
  WiFiManagerParameter custom_mqtt_user;
  WiFiManagerParameter custom_mqtt_pass;
  WiFiManagerParameter custom_mqtt_topic;
  WiFiManagerParameter custom_mqtt_farm;
  WiFiManagerParameter custom_mqtt_field;
  WiFiManagerParameter custom_mqtt_zone;
void saveConfigCallback ();
void configure_wifi_portal();
//************************************************FILE SYSTEM CREDENTIALS**********************************
void FileSystemInitialize();
void readsavedvalues();
bool read_from_file_status=true;
//***************************************************MESSAGE FROM APP**********************************
char APP_DATA[500]={0};//this contains json message from app
