
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include "millisDelay.h"
#include <FS.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>

int chipid;
//****************************************************NTP SERVER***************************************
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


//**********************************************MQTT CREDENTIALS**********************************************

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
const char* sendtopic_app = "irrgation/app";
void callback(char* topic, byte * payload, unsigned int length);

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

//***************************************************MESSAGE FROM APP**********************************
char APP_DATA[500]={0};//this contains json message from app

//************************************************FILE SYSTEM CREDENTIALS**********************************
void FileSystemInitialize();
void readsavedvalues();
bool read_from_file_status=true;
