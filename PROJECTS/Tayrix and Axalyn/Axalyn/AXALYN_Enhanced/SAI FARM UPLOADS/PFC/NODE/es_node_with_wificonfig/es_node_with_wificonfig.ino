#include <WiFiManager.h>
#include <PubSubClient.h>
#include<WiFi.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "CO2Sensor.h"
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <U8g2lib.h>
#include<EEPROM.h>
#include<Wire.h>
#include"MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String timestamp;
#define TRIGGER_PIN 0
bool wm_nonblocking = false;
WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
#define batteryvolt 34
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_16,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);

#define UVOUT 32
#define  REF_3V3 33
CO2Sensor co2Sensor(15, 0.99, 100);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int pagecount = 4;
int p;
int chipId;
//Scheduler r;
int uvLevel;
int refLevel;
int sensorvalue;
float voltage, outputvoltage;
int battry;
float t4, h4, p4, alti, dewpt, HI, uvIntensity;
void UVsensor();
int co2;//=co2Sensor.read();
int sl, ET;
bool a;
float Tme, dn1, dn, num1, enum1, enum2, expon, pdenom, powercal, top1, top2, num2, num3, denom2, powercalc2, expnum1, expnum2, expcal;
float expnum3, expnum4, expcal2, ETcal, Es, RHcal, RHcal2, num5, num6, numcalc, fdenom1, fdenom2, fdenom3, fdenom4, fnum1, fnum2, fnum3, fnum4, fnum5, fnum6, fnum7, fnum8, fnum9, fnum10;

float ETo, ES_BP, ES_AT, ES_RH;
float Tmean;
float U2, P, Ea, delta, gama, ETmax, ETmin;
float Tmax, Tmin;
float h;//h=8.2;//height of the measurement above the ground surface
float T;//t=7.7;//air temp
float z = 840; //3.6;//elevation above sealevel//
//The elevation of the land surface varies from the low point of −418 m at the Dead Sea, to a maximum altitude of 8,848 m at the top of Mount Everest.
//The mean Height of land above sea level is 840 m.”

float Un = 2.4; ////measured wind speed z m above the ground surface
float RHmax, RHmin;
float Rn;//UV//=5.9;//net radiation flux
float G;//=HI//4.5;//sensible heat flux into the soil
// User stub
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void Display();
void dummy();
float VPDcal();
void page1();
void page2();
void page3();
void page4();
void ETcalcul();
void UVA();
void batterypercent();
int averageAnalogRead(int);
char sendtopic_Mesh[100] = "/NODEDATA1";
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

  res = wm.autoConnect("Axylin ES Node", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect or hit timeout");

  }
  else {

    Serial.println("connected...yeey :)");
  }
}
void setup() {
// SPIFFS.format();
Serial.begin(115200);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  EEPROM.begin(512);
  u8g2.begin();
  co2Sensor.calibrate();
  Wire.begin();
  pinMode(34, INPUT);
  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
   
  }
  WiFi.mode(WIFI_STA);
  
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

   hostnameefromf.toCharArray(broker,100);    
   mqttports = porttfromf.toInt();   
   userrfromf.toCharArray(usernames,100);
   passworddfromf.toCharArray(passwords,100);
    devicesa.toCharArray(dev,100);
    farmq.toCharArray(frm,100);
    fieldq.toCharArray(fld,100);
    zonew.toCharArray(zne,100);
  

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
  
  String combined_string = devicesa + "/" + farmq + "/" + fieldq + "/" + zonew;
  Serial.println(combined_string);
  String combined_string1;
  combined_string1 = combined_string+sendtopic_Mesh;
  combined_string1.toCharArray(sendtopic_Mesh, 100);
   Serial.println(sendtopic_Mesh);
}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;
void mqttconnection()
{

  while(!client.connected())
  {
    checkButton();
 
   if (client.connect("ESP32Client", usernames, passwords))
    {
    Serial.print("connected");
   

    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    
     
    }
}
}
void loop() {
  if (wm_nonblocking) wm.process(); 
  
  if(!client.connected())
  {
  mqttconnection();
  }
   client.loop();
   
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;
      Display();
    }
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      
        sendDataMQTT();
      

    }
unsigned long rstresp1 = millis();
    if (rstresp1 - rstresp >500000) {
      Serial.println("....");
      rstresp = rstresp1;
       ESP.restart();
      }
}
