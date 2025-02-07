/*
   19/04/22
   Doser MQTT
   WIFI Manager

 * */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h> //library for MQTT
#include <ArduinoJson.h> //library for Parsing JSON
#include<TaskScheduler.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define USEOTA
// enable OTA
#ifdef USEOTA
#include <ArduinoOTA.h>
#endif
#include<EEPROM.h>
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };
//parameters for using non-blocking delay

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

//setting up wifi and mqtt client
WiFiClient espClient;
PubSubClient client(espClient);

WiFiManager wm;
WiFiManager wifiManager;

String timestamp;

// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout

bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button
bool WMISBLOCKING    = false; // use blocking or non blocking mode
bool shouldSaveConfig = false;

#define LED 2
bool MQTT_Flag;
Scheduler r;
///MQTT SETUP
char hostname[40] = "164.52.223.248"; // or "cea.axalyn.com"
char port_str[40] = "1883"; // string representation of the port
uint16_t port = atoi(port_str); // convert port string to integer
char clientId[40] = "CCESP8266";
char user[40] = "axalyn";
char password[40] = "cenaura@2024";
//TOPICS
const char* topic = "api/PD1/W1/A1/request"; //publish topic

const char* MQTT_PUBTOPIC ="Saifarm/GH2/PFC/ZoneA/NUTRIENTDATA";
const char* MQTT_SUBTOPIC = "Chinnamangalaram/GH1/ZoneA/Node";



DynamicJsonDocument megadoc(1024);
DynamicJsonDocument megadoc1(1024);
DynamicJsonDocument doc(1024);



#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];
char tempString[1024];
void sendDataMQTT();
void senddatatoKURA();
void SendDataToMega();
Task t1(10000, TASK_FOREVER, &sendDataMQTT, &r);

void bindServerCallback() {
  wm.server->on("/custom", handleRoute);
  // wm.server->on("/info",handleRoute); // you can override wm!
}

void handleRoute() {
  Serial.println("[HTTP] handle route");
  wm.server->send(200, "text/plain", "hello from user code");
}
//SendDataToMega
int  As_ZA_WIF_v ; 
int  As_ZA_WOF_v ; 
int  As_ZA_WTL_v ;
int  As_ZA_WTSP_v;
int  WIF_v ;
int  WOF_v ;
int  WTL_v ;
int  WTSP_v ;
bool  WifiSt ;
bool  MqttSt ;

void SendDataToMega() {

  DynamicJsonDocument espdata(250);
  //  doc["TIME"] = formattedTime;
  espdata["id"] = "WSnode";
  espdata["WIF_v"] = WIF_v;
  espdata["WOF_v"] = WOF_v;
  espdata["WTL_v"] = WTL_v;
  espdata["WTSP_v"] = WTSP_v;
  espdata["WifiSt"] = WifiSt;
  espdata["MqttSt"] = MqttSt;

  Serial.println("Publish mega:");
  serializeJson(espdata, Serial);
  Serial.print("\n");

}

void callback(char*topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);

  JsonObject As_ZA = doc["As"]["ZA"];
  const char* As_ZA_ts = As_ZA["ts"]; // "09:54:58"
  As_ZA_WIF_v = As_ZA["WIF_v"]; // 35
  As_ZA_WOF_v = As_ZA["WOF_v"]; // 36
  As_ZA_WTL_v = As_ZA["WTL_v"]; // 14
  As_ZA_WTSP_v = As_ZA["WTSP_v"]; // 11
  WIF_v = As_ZA_WIF_v;
  WOF_v = As_ZA_WOF_v;
  WTL_v = As_ZA_WTL_v;
  WTSP_v = As_ZA_WTSP_v;
  Serial.println();
  Serial.print("As_ZA_WTSP_v :");
  Serial.print(As_ZA_WTSP_v);
}

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(512);
  // Serial1.begin(115200);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
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

  wm.setConfigPortalBlocking(true);
  wm.setConfigPortalTimeout(60);

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);
  client.setServer(hostname, port);
  client.setCallback(callback);
  t1.enable();
  r.startNow();
}

float ph_value ;
float ec_value ;
float nt_value ;
float ndo_value;
float orp_value;
float ntl_value;
float ph_aspvalue ;
float ec_aspvalue ;
float nt_aspvalue ;
float ndo_aspvalue ;
float orp_aspvalue;
float ntl_aspvalue;
float ph_hysvalue ;
float ec_hysvalue ;
float nt_hysvalue ;
float ndo_hysvalue ;
float orp_hysvalue ;
float ntl_hysvalue;

int SubP ;
int Heater ;
int Mixer ;
int RO;

void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();
  String phval;
  String ecval;
  String formattedTime = timeClient.getFormattedTime();
  if (Serial.available())
  {
    
    StaticJsonDocument<500> doc;
    DeserializationError err = deserializeJson(doc, Serial);
    if (err == DeserializationError::Ok)
    {
      ph_value = doc["pH"].as<float>();
      String phval = String(ph_value, 1);
      ec_value = doc["EC"].as<float>();
      String ecval = String(ec_value, 1);
      nt_value = doc["NT"].as<float>();
      ndo_value = doc["DO"].as<float>();
      ntl_value = doc["NL"].as<int>();
      ph_aspvalue = doc["pHSetPA"].as<long>();
      ec_aspvalue = doc["ECSetPA"].as<long>();
      SubP = doc["Sampler"].as<long>();
      Heater = doc["Heater"].as<long>();
      Mixer = doc["Mixer"].as<long>();
      RO = doc["RO"].as<long>();

      Serial.print("phvalue   :");
      Serial.println(ph_value);
    }
    else
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
      while (Serial.available() > 0)
        Serial.read();
    }
  }

  DynamicJsonDocument doc(1024);
  doc["id"] = "TxDc8266";
  doc["ph"] = ph_value;
  doc["ec"] = ec_value;
  doc["ntemp"] = nt_value;
  doc["do"] = ndo_value;
  doc["ntl"] = ntl_value;
  doc["pHSp"] = ph_aspvalue;
  doc["EcSp"] = ec_aspvalue;
  doc["SubP"] = SubP;
  doc["Heater"] = Heater;
  doc["Mixer"] = Mixer;
  doc["RO"] = RO;

  DynamicJsonDocument doc2(1024);
  doc2["metrics"] = doc;
  serializeJson(doc2, Serial);
  serializeJson(doc2, tempString);
  client.publish("axalyn", tempString);
  memset(tempString, 0, 1024);

  delay(6000);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    if (client.connect("ESP32Client", user, password)) {
      Serial.println("connected");
      
      MqttSt=true;
      client.subscribe("axalyn");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      MqttSt=false;
     
      delay(5000);
    }
  }
}
unsigned long lastMsg = 0;
void loop() {
  r.execute();
  timeClient.update();


  unsigned long now1 = millis();
  if (now1 - lastMsg > 3000) {
    lastMsg = now1;
    if (!client.connected()) {
      reconnect();
    }
  }

  if (!WMISBLOCKING) {
    wm.process();
  }



  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    Serial.println("Sending Data");

    if (client.connected()) {
      sendDataMQTT();
    }

  }
  client.loop();
  delay(100);
  unsigned long megacurentmills = millis();
  unsigned long megaprevious = 0;


  if (megacurentmills - megaprevious >= 3000) {
    megaprevious = megacurentmills;

    Serial.println("Sending Data");

    if (client.connected()) {
      SendDataToMega();
    }

  }

}
