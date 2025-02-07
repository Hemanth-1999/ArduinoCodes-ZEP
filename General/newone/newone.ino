/*/
   irrigation Arduino MEGA with ESP8266 Board Code...
   Serial communication to esp8266
   ArduinoJson
   Auto Save
   Mode Save
   Audo Sheduler delay
   Json New Modification Doc
*/
#include <Arduino.h>
//Data
#include <MemoryFree.h>
#include <SD.h>
#include <TaskScheduler.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
char t[32];
#include "Countimer.h"
#include "Timer.h"
Timer timer;

//*******************************************************************************************
////Hardware pin definitions
int UVOUT = A2; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board


#define LEDPIN LED_BUILTIN
//#define USE_SSD1306

//EPROM
#include <EEPROMex.h>

//WatchDogTimer
//#include <avr/wdt.h>

//Define
#define BUZZER 37
#define LEDRED 40
#define LEDGREEN 44
#define LEDBLUE 58

//RELAY

#define Z1PUMPAtest 34 //36

#define Z2PUMPAtest 23 //36

#define Z1PUMPA 36 //36
#define Z1PUMPB 34 //34
#define Z1AIRPUMP 32 //49 //14
#define Z1SOLVALVE 42 //48 //15

#define Z2PUMPA 23//23
#define Z2PUMPB 25//25
#define Z2AIRPUMP 27
#define Z2SOLVALVE 29

//#define Z1PUMPA 38 //36
//#define Z1PUMPB 34 //34
//#define Z1AIRPUMP 32 //49 //14
//#define Z1SOLVALVE 36 //48 //15
//
//#define Z2PUMPA 23//23
//#define Z2PUMPB 29//25
//#define Z2AIRPUMP 27
//#define Z2SOLVALVE 25


int watsensor1 = 2;    // intterupt 0  pump 1
int watsensor2 = 3;   // interrupt 1  pump 2
int watsensor3 = 20;   // intterupt 3  pump 3
int watsensor4 = 21;  // intterupt 2  pump 4
float flowRate;
float flowRate2;
float flowRate3;
float flowRate4;
volatile int count;
volatile int count2;
volatile int count3;
volatile int count4;

//String data_from_display = "";
String dfd;
String str;
char buff[2000];
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftHr;
uint8_t pswd;
String feedBack;
int val;
String ssid;
String password;
bool waterlevel_sensor;
bool water_level_sensor;

int FirstState = 0;
int SecondState = 0;
int ThirdState = 0;
int MANUAL = 0;
int NC = 0;

uint8_t led = 0;
uint8_t led1 = 2;
uint8_t led2 = 3;
uint8_t led3 = 4;

//Relay Status
bool ZAApumpSt = true;
bool ZABpumpSt = true;
bool ZBApumpSt = true;
bool ZBBpumpSt = false;
bool ZAAirSPumpSt = false;
bool ZBAirSPumpSt = false;
bool ZAROSt = false;
bool ZBROSt = false;

unsigned long previousMillis = 0;
const long interval = 1000;

//COUNTER FOR ZONE 1
Countimer PUMPAON;
Countimer PUMPAOFF;
Countimer PUMPANone;

Countimer PUMPBON;
Countimer PUMPBOFF;
Countimer PUMPBNone;

Countimer AIRSTONEON;
Countimer AIRSTONEOFF;
Countimer AIRSTONEone;

//COUNTER FOR ZONE 2
Countimer Z2PUMPAON;
Countimer Z2PUMPAOFF;
Countimer Z2PUMPANone;

Countimer Z2PUMPBON;
Countimer Z2PUMPBOFF;
Countimer Z2PUMPBNone;

Countimer Z2AIRSTONEON;
Countimer Z2AIRSTONEOFF;
Countimer Z2AIRSTONEone;

uint8_t nhur = 436, nmin = 439;
uint8_t nhur1, nmin1;
uint8_t ledRED = LOW;
uint8_t ledGREEN = LOW;
uint8_t ledBLUE = LOW;

#define DEBUG 1                                // change value to 1 to enable debuging using serial monitor  
#define SERIAL_PORT_SPEED 115200                //9600, 19200, 38400, 57600, 115200

//****************************************** Water Level US Sensor **************************************************************
#define RORelay 19

#define trigPin (uint8_t)52
#define echoPin (uint8_t)53
int duration;
int distance;
float waterHeight;
uint8_t tankHeight ; //enter your water tank height here in [cm]
uint8_t totalVolume ; // volume here is in [cm^3], change depending on tank volume
float waterQuantity;
float volume;
float pi = 3.14159265359;
float offset = 245;
//Water distance
float Wtime = 0, Wdistance = 0;
uint8_t Wtemp = 0;
bool TankM;
uint8_t tanksetpoint;
///*******************UVA*********previousMillis**************************************
float solar_Value;



//******************************ZONE A PUMP B********TASK**************************************
//***********************************************************************************************************************
Scheduler r;
//Tasks
//void getNutrientTL();
void sensorjson();
void uva();
void nexVal();
void dateandtime();
//void readalldata();
void setTimeNex();
void runTimeNex();

Task t0(1000, TASK_FOREVER, &dateandtime, &r);
Task t1(2000, TASK_FOREVER, &nexVal, &r );
//Task t3(7000,  TASK_FOREVER, &uva, &r);
Task t4(10000,  TASK_FOREVER, &sensorjson, &r);
//Task t6(6000,  TASK_FOREVER, &getNutrientTL, &r);
//Task t7(6000,  TASK_FOREVER, &readalldata, &r);
Task t8(4000,  TASK_FOREVER, &setTimeNex, &r);
Task t9(5000,  TASK_FOREVER, &runTimeNex, &r);


String chipID;
String cloudIp;
String wifiStatus;
String userSsid;
String userPswd;

void wifireceived() {
  StaticJsonDocument<150> doc;
  DeserializationError err1 = deserializeJson(doc, Serial3);
  if (err1 == DeserializationError::Ok)
  {
    chipID = doc["Wifi"]["Cid"].as<String>();
    cloudIp = doc["Wifi"]["IpC"].as<String>();
    wifiStatus = doc["Wifi"]["WiSt"].as<String>();
    userSsid = doc["Wifi"]["Ssid"].as<String>();
    userPswd = doc["Wifi"]["Pswd"].as<String>();

    ////Serial.print("chipID: ");
    ////Serial.println(chipID);
    ////Serial.println("cloudIp: ");
    ////Serial.print(cloudIp);
    ////Serial.println("wifiStatus: ");
    ////Serial.print(wifiStatus);
    ////Serial.println("userSsid: ");
    ////Serial.print(userSsid);
    ////Serial.println("userPswd: ");
    ////Serial.print(userPswd);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
    }
  }
}

void ssidpwd() {//sendCred();
  DynamicJsonDocument doc(150);
  JsonObject SdPwd = doc.createNestedObject("SdPwd");
  SdPwd["ssid"] = ssid;
  SdPwd["password"] = password;
  serializeJsonPretty(doc, Serial3);
}


#define LEDPIN 13

//************SD Card ********///
#define SD_DETECT_PIN   49
#define SDSS            53
File sdcard_file;


RTC_DS3231 rtc; //for date time
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

uint16_t hourrc;
uint16_t minrc;
uint16_t secsrc;
uint16_t Monthrc;
uint16_t Daterc;
int Yearrc;

uint8_t hourupg;
uint8_t minupg;
uint8_t secslive;
uint8_t Monthlive;
uint8_t Datelive;
uint16_t Yearlive;

//int ledCtrl = LOW;

// TankM --Door
//*******************************TANK**********************************
//TOGGLE(TankM, Tankcontrol, "TANK MANAGEMENT : ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
//       , VALUE("STOP", true, doNothing, enterEvent)
//       , VALUE("START", false, doExit, noEvent)
//      );
//
//void TankMgStart() {
//  TankM = true;
//
//}
//void TankMgStop() {
//  TankM = false;
//
//}
///**********************Relay Control******************************

bool relayPA ;
bool relayPB ;
bool relayAir ;
bool relaySol ;

bool relayPAZ2 ;
bool relayPBZ2 ;
bool relayAirZ2 ;
bool relaySolZ2 ;

void switching() {

  if (relayPA == HIGH ) {

    relaySol == LOW;
  }
  else
  {
    relaySol == HIGH;
  };
  if (relayPB == HIGH) {
    relaySol == LOW;

  }
  else {

    relaySol == HIGH;
  }

}
//******************************************************************

//*****************************ZONE ONE AUTO ************************************
uint8_t Z1PASThr;//on time
uint8_t Z1PASTmin;//on time
uint8_t Z1PASPhr;//off time
uint8_t Z1PASPmin;//off time

uint8_t Z1ZPASThr;//sche z1
uint8_t Z1ZPASTmin;
uint8_t Z1ZPASPhr;
uint8_t Z1ZPASPmin;//sche z1


uint8_t Z1PBSThr;//pump b on time
uint8_t Z1PBSTmin;// pump b on time
uint8_t Z1PBSPhr;// pump b off time
uint8_t Z1PBSPmin;// pump b off time

uint8_t Z1ZPBSThr;
uint8_t Z1ZPBSTmin;
uint8_t Z1ZPBSPhr;
uint8_t Z1ZPBSPmin;

uint8_t Z1APSThr;// pump do on time
uint8_t Z1APSTmin;
uint8_t Z1APSPhr;
uint8_t Z1APSPmin;// pump do off time

uint8_t Z1ZAPSThr;
uint8_t Z1ZAPSTmin;
uint8_t Z1ZAPSPhr;
uint8_t Z1ZAPSPmin;
//********************************** zone 2 ********************************/
uint8_t Z2PASThr;
uint8_t Z2PASTmin;
uint8_t Z2PASPhr;
uint8_t Z2PASPmin;

uint8_t Z2ZPASThr;
uint8_t Z2ZPASTmin;
uint8_t Z2ZPASPhr;
uint8_t Z2ZPASPmin;

uint8_t Z2PBSThr;
uint8_t Z2PBSTmin;
uint8_t Z2PBSPhr;
uint8_t Z2PBSPmin;

uint8_t Z2ZPBSThr;
uint8_t Z2ZPBSTmin;
uint8_t Z2ZPBSPhr;
uint8_t Z2ZPBSPmin;

uint8_t Z2APSThr;
uint8_t Z2APSTmin;
uint8_t Z2APSPhr;
uint8_t Z2APSPmin;

uint8_t Z2ZAPSThr;
uint8_t Z2ZAPSTmin;
uint8_t Z2ZAPSPhr;
uint8_t Z2ZAPSPmin;


//////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t EepromZ1PASThr = 200;
uint8_t EepromZ1PASTmin = 202;
uint8_t EepromZ1PASPhr = 204;
uint8_t EepromZ1PASPmin = 206;

uint8_t EepromZ1ZPASThr = 300;
uint8_t EepromZ1ZPASTmin = 302;
uint8_t EepromZ1ZPASPhr = 304;
uint8_t EepromZ1ZPASPmin = 306;


uint8_t EepromZ1PBSThr = 208;
uint8_t EepromZ1PBSTmin = 210;
uint8_t EepromZ1PBSPhr = 212;
uint8_t EepromZ1PBSPmin = 214;

uint8_t EepromZ1ZPBSThr = 308;
uint8_t EepromZ1ZPBSTmin = 310;
uint8_t EepromZ1ZPBSPhr = 312;
uint8_t EepromZ1ZPBSPmin = 314;


uint8_t EepromZ1APSThr = 216;
uint8_t EepromZ1APSTmin = 214;
uint8_t EepromZ1APSPhr = 218;
uint8_t EepromZ1APSPmin = 220;

uint8_t EepromZ1ZAPSThr = 316;
uint8_t EepromZ1ZAPSTmin = 314;
uint8_t EepromZ1ZAPSPhr = 318;
uint8_t EepromZ1ZAPSPmin = 320;

uint8_t EepromZ2PASThr = 340;
uint8_t EepromZ2PASTmin = 342;
uint8_t EepromZ2PASPhr = 344;
uint8_t EepromZ2PASPmin = 346;

uint8_t EepromZ2ZPASThr = 348;
uint8_t EepromZ2ZPASTmin = 350;
uint8_t EepromZ2ZPASPhr = 352;
uint8_t EepromZ2ZPASPmin = 354;


uint8_t EepromZ2PBSThr = 356;
uint8_t EepromZ2PBSTmin = 358;
uint8_t EepromZ2PBSPhr = 360;
uint8_t EepromZ2PBSPmin = 362;

int EepromZ2ZPBSThr = 364;
int EepromZ2ZPBSTmin = 366;
int EepromZ2ZPBSPhr = 368;
int EepromZ2ZPBSPmin = 370;


int EepromZ2APSThr = 372;
int EepromZ2APSTmin = 374;
int EepromZ2APSPhr = 376;
int EepromZ2APSPmin = 378;

int EepromZ2ZAPSThr = 1000;
int EepromZ2ZAPSTmin = 1005;
int EepromZ2ZAPSPhr = 1010;
int EepromZ2ZAPSPmin = 1015;



//***SAVE TIME********************************************************///
bool saveZ1PASThr;

void Z1PAsavetime() {
  Z1PASThr = othr();
  Z1PASTmin = otmin();
  EEPROM.update(EepromZ1PASThr, Z1PASThr);
  EEPROM.update(EepromZ1PASTmin, Z1PASTmin);
}

void Z1PAsavedue() {
  Z1PASPhr = ofthr();
  Z1PASPmin = oftmin();
  EEPROM.update(EepromZ1PASPhr, Z1PASPhr);
  EEPROM.update(EepromZ1PASPmin, Z1PASPmin);
}

void EepromReadZ1AP()
{
  Z1PASThr = EEPROM.read(EepromZ1PASThr);
  Z1PASTmin = EEPROM.read(EepromZ1PASTmin);
  Z1PASPhr = EEPROM.read(EepromZ1PASPhr);
  Z1PASPmin = EEPROM.read(EepromZ1PASPmin);
}

//*****************************************************************
void Z1ZPAsavetime() {
  //  Z1ZPASThr = othr();
  //  Z1ZPASTmin = otmin();
  EEPROM.update(EepromZ1ZPASThr, Z1ZPASThr);
  EEPROM.update(EepromZ1ZPASTmin, Z1ZPASTmin);


}
void Z1ZPAsavedue() {
  //  Z1ZPASPhr = ofthr();
  //  Z1ZPASPmin = oftmin();
  EEPROM.update(EepromZ1ZPASPhr, Z1ZPASPhr);
  EEPROM.update(EepromZ1ZPASPmin, Z1ZPASPmin);

}
void EepromReadZ1ZAP()
{
  Z1ZPASThr = EEPROM.read(EepromZ1ZPASThr);
  Z1ZPASTmin = EEPROM.read(EepromZ1ZPASTmin);
  Z1ZPASPhr = EEPROM.read(EepromZ1ZPASPhr);
  Z1ZPASPmin = EEPROM.read(EepromZ1ZPASPmin);

}
//**********************************************************************************
void Z1PBsavetime() {
  Z1PBSThr = othr();
  Z1PBSTmin = otmin();
  EEPROM.update(EepromZ1PBSThr, Z1PBSThr);
  EEPROM.update(EepromZ1PBSTmin, Z1PBSTmin);
}
void Z1PBsavedue() {
  Z1PBSPhr = ofthr();
  Z1PBSPmin = oftmin();
  EEPROM.update(EepromZ1PBSPhr, Z1PBSPhr);
  EEPROM.update(EepromZ1PBSPmin, Z1PBSPmin);
}
void EepromReadZ1PB()
{
  Z1PBSThr = EEPROM.read(EepromZ1PBSThr);
  Z1PBSTmin = EEPROM.read(EepromZ1PBSTmin);
  Z1PBSPhr = EEPROM.read(EepromZ1PBSPhr);
  Z1PBSPmin = EEPROM.read(EepromZ1PBSPmin);

}
//*************************************************************************
void Z1ZPBsavetime() {
  EEPROM.update(EepromZ1ZPBSThr, Z1ZPBSThr);
  EEPROM.update(EepromZ1ZPBSTmin, Z1ZPBSTmin);


}
void Z1ZPBsavedue() {

  EEPROM.update(EepromZ1ZPBSPhr, Z1ZPBSPhr);
  EEPROM.update(EepromZ1ZPBSPmin, Z1ZPBSPmin);

}
void EepromReadZ1ZPB()
{
  Z1ZPBSThr = EEPROM.read(EepromZ1ZPBSThr);
  Z1ZPBSTmin = EEPROM.read(EepromZ1ZPBSTmin);
  Z1ZPBSPhr = EEPROM.read(EepromZ1ZPBSPhr);
  Z1ZPBSPmin = EEPROM.read(EepromZ1ZPBSPmin);

}
//*****************************************************************
void Z1APsavetime() {
  Z1APSThr = othr();
  Z1APSTmin = otmin();
  EEPROM.update(EepromZ1APSThr, Z1APSThr);
  EEPROM.update(EepromZ1APSTmin, Z1APSTmin);

}
void Z1APsavedue() {
  Z1APSPhr = ofthr();
  Z1APSPmin = oftmin();
  EEPROM.update(EepromZ1APSPhr, Z1APSPhr);
  EEPROM.update(EepromZ1APSPmin, Z1APSPmin);
}
void EepromReadZ1Air()
{
  Z1APSThr = EEPROM.read(EepromZ1APSThr);
  Z1APSTmin = EEPROM.read(EepromZ1APSTmin);
  Z1APSPhr = EEPROM.read(EepromZ1APSPhr);
  Z1APSPmin = EEPROM.read(EepromZ1APSPmin);

}
//*****************************************************************************
void Z1ZAPsavetime() {
  EEPROM.update(EepromZ1ZAPSThr, Z1ZAPSThr);
  EEPROM.update(EepromZ1ZAPSTmin, Z1ZAPSTmin);


}
void Z1ZAPsavedue() {

  EEPROM.update(EepromZ1ZAPSPhr, Z1ZAPSPhr);
  EEPROM.update(EepromZ1ZAPSPmin, Z1ZAPSPmin);

}
void EepromReadZ1ZAir()
{
  Z1ZAPSThr = EEPROM.read(EepromZ1ZAPSThr);
  Z1ZAPSTmin = EEPROM.read(EepromZ1ZAPSTmin);
  Z1ZAPSPhr = EEPROM.read(EepromZ1ZAPSPhr);
  Z1ZAPSPmin = EEPROM.read(EepromZ1ZAPSPmin);

}

///*********************************ZONE 2***************************************************************
void Z2PAsavetime() {
  EEPROM.update(EepromZ2PASThr, Z2PASThr);
  EEPROM.update(EepromZ2PASTmin, Z2PASTmin);
}
void Z2PAsavedue() {

  EEPROM.update(EepromZ2PASPhr, Z2PASPhr);
  EEPROM.update(EepromZ2PASPmin, Z2PASPmin);

}
void EepromReadZ2AP()
{
  Z2PASThr = EEPROM.read(EepromZ2PASThr);
  Z2PASTmin = EEPROM.read(EepromZ2PASTmin);
  Z2PASPhr = EEPROM.read(EepromZ2PASPhr);
  Z2PASPmin = EEPROM.read(EepromZ2PASPmin);

}
//*****************************************************************
void Z2ZPAsavetime() {
  EEPROM.update(EepromZ2ZPASThr, Z2ZPASThr);
  EEPROM.update(EepromZ2ZPASTmin, Z2ZPASTmin);


}
void Z2ZPAsavedue() {

  EEPROM.update(EepromZ2ZPASPhr, Z2ZPASPhr);
  EEPROM.update(EepromZ2ZPASPmin, Z2ZPASPmin);

}
void EepromReadZ2ZAP()
{
  Z2ZPASThr = EEPROM.read(EepromZ2ZPASThr);
  Z2ZPASTmin = EEPROM.read(EepromZ2ZPASTmin);
  Z2ZPASPhr = EEPROM.read(EepromZ2ZPASPhr);
  Z2ZPASPmin = EEPROM.read(EepromZ2ZPASPmin);

}
//**********************************************************************************
void Z2PBsavetime() {
  EEPROM.update(EepromZ2PBSThr, Z2PBSThr);
  EEPROM.update(EepromZ2PBSTmin, Z2PBSTmin);


}
void Z2PBsavedue() {

  EEPROM.update(EepromZ2PBSPhr, Z2PBSPhr);
  EEPROM.update(EepromZ2PBSPmin, Z2PBSPmin);

}
void EepromReadZ2PB()
{
  Z2PBSThr = EEPROM.read(EepromZ2PBSThr);
  Z2PBSTmin = EEPROM.read(EepromZ2PBSTmin);
  Z2PBSPhr = EEPROM.read(EepromZ2PBSPhr);
  Z2PBSPmin = EEPROM.read(EepromZ2PBSPmin);

}
//*************************************************************************
void Z2ZPBsavetime() {
  EEPROM.update(EepromZ2ZPBSThr, Z2ZPBSThr);
  EEPROM.update(EepromZ2ZPBSTmin, Z2ZPBSTmin);


}
void Z2ZPBsavedue() {

  EEPROM.update(EepromZ2ZPBSPhr, Z2ZPBSPhr);
  EEPROM.update(EepromZ2ZPBSPmin, Z2ZPBSPmin);

}
void EepromReadZ2ZPB()
{
  Z2ZPBSThr = EEPROM.read(EepromZ2ZPBSThr);
  Z2ZPBSTmin = EEPROM.read(EepromZ2ZPBSTmin);
  Z2ZPBSPhr = EEPROM.read(EepromZ2ZPBSPhr);
  Z2ZPBSPmin = EEPROM.read(EepromZ2ZPBSPmin);

}
//*****************************************************************
void Z2APsavetime() {
  EEPROM.update(EepromZ2APSThr, Z2APSThr);
  EEPROM.update(EepromZ2APSTmin, Z2APSTmin);


}
void Z2APsavedue() {

  EEPROM.update(EepromZ2APSPhr, Z2APSPhr);
  EEPROM.update(EepromZ2APSPmin, Z2APSPmin);

}
void EepromReadZ2Air()
{
  Z2APSThr = EEPROM.read(EepromZ2APSThr);
  Z2APSTmin = EEPROM.read(EepromZ2APSTmin);
  Z2APSPhr = EEPROM.read(EepromZ2APSPhr);
  Z2APSPmin = EEPROM.read(EepromZ2APSPmin);

}
//*****************************************************************************
void Z2ZAPsavetime() {
  EEPROM.update(EepromZ2ZAPSThr, Z2ZAPSThr);
  EEPROM.update(EepromZ2ZAPSTmin, Z2ZAPSTmin);


}
void Z2ZAPsavedue() {

  EEPROM.update(EepromZ2ZAPSPhr, Z2ZAPSPhr);
  EEPROM.update(EepromZ2ZAPSPmin, Z2ZAPSPmin);

}
void EepromReadZ2ZAir()
{
  Z2ZAPSThr = EEPROM.read(EepromZ2ZAPSThr);
  Z2ZAPSTmin = EEPROM.read(EepromZ2ZAPSTmin);
  Z2ZAPSPhr = EEPROM.read(EepromZ2ZAPSPhr);
  Z2ZAPSPmin = EEPROM.read(EepromZ2ZAPSPmin);

}

////**************************************************************************************


bool Autocnt;
//void relay1_Pan(){
//  PUMPAON.start();
//}
////void relay1_Paf(){
////  PUMPAON.start();
////}
//void relay1_Pbn(){
//  PUMPBON.start();
//}
////void relay1_Paf(){
////  PUMPBON.start();
////}
//void relay1_Pdn(){
//  AIRSTONEON.start();
//}
void runer() {
  if (Autocnt == true)
  {
    PUMPAON.start();
    PUMPBON.start();
    AIRSTONEON.start();
    // ////Serial.print ("auto start");


  }
}

bool Z2Autocnt;
void Z2runer() {
  if (Z2Autocnt == true)
  {
    Z2PUMPAON.start();
    Z2PUMPBON.start();
    Z2AIRSTONEON.start();
    // ////Serial.print ("auto start");
    //        digitalWrite (Z2PUMPA, LOW);
    //    digitalWrite (Z2PUMPB, LOW);
    //    digitalWrite (Z2AIRPUMP, LOW);
    //    digitalWrite (Z2SOLVALVE, LOW);
  }
}

void stoper() {//when automode is ON all relays off becoz manual mode shouldn't clash.
  if (Autocnt == false)
  {
    PUMPAON.stop();
    PUMPBON.stop();
    AIRSTONEON.stop();
    PUMPAOFF.stop();
    PUMPBOFF.stop();
    AIRSTONEOFF.stop();
    //    digitalWrite (Z1PUMPA, relayPA);
    //    digitalWrite (Z1PUMPB, relayPB);
    //    digitalWrite (Z1AIRPUMP, relayAir);
    //    digitalWrite (Z1SOLVALVE, relaySol);
  }
}
void Z2stoper() {
  if (Z2Autocnt == false) {
    Z2PUMPAON.stop();
    Z2PUMPBON.stop();
    Z2AIRSTONEON.stop();
    Z2PUMPAOFF.stop();
    Z2PUMPBOFF.stop();
    AIRSTONEOFF.stop();
    //    digitalWrite (Z2PUMPA, relayPAZ2);
    //    digitalWrite (Z2PUMPB, relayPBZ2);
    //    digitalWrite (Z2AIRPUMP, relayAirZ2);
    //    digitalWrite (Z2SOLVALVE, relaySolZ2);
  }
}
bool Autocntt;

//*********************************************************************************************************

int totalVolume1;
int EepromNTHset = 300;
int EepromNTVSet = 310;
///*********EPROM********************///
void NTHset() {
  EEPROM.write(EepromNTHset, tankHeight);

  //   EEPROM.read(EepromNTHset, tankHeight);



}

void NTVset() {

  EEPROM.write(EepromNTVSet, totalVolume1);
  // EEPROM.read(EepromNTVSet, totalVolume);

}
int EepromTSLSet = 350;
void TSLset() {

  EEPROM.write(EepromTSLSet, tanksetpoint);


}
void EepromReadNT()
{
  tankHeight = EEPROM.read(EepromNTHset);
  totalVolume1 = EEPROM.read(EepromNTVSet);
  tanksetpoint = EEPROM.read(EepromTSLSet);
}


int distanceincm;
int distanceinInch;

void readalldata() {
  EepromReadZ1AP();
  EepromReadZ1PB();
  EepromReadZ1Air();
  EepromReadNT();


}
void readalldataZ2() {
  EepromReadZ2AP();
  EepromReadZ2PB();
  EepromReadZ2Air();
}

void sensorjson() {
  DynamicJsonDocument doc(512);
  //  StaticJsonDocument<1024> doc;
  doc["CONTROL ID"] = "321654";
  doc["RTCNow"] = t;

  doc["ZAPumpA "] = random(0, 1); //ZAApumpSt;
  doc["ZAPumpB"] =  random(0, 1); //ZABpumpSt;
  doc["ZAAirSPump"] =  random(0, 1); //ZAAirSPumpSt;
  doc["ZARO"] =  random(0, 1); //ZAROSt;
  doc["ZAInletFlow"] = flowRate;
  doc["ZAOutletFlow"] = flowRate2;
  doc["ZANTankLevel"] = distanceincm;//waterconlevel
  doc["RADIATION"] = solar_Value;

  doc["ZBPumpA "] =  random(0, 1); //ZBApumpSt;
  doc["ZBPumpB"] =  random(0, 1); //ZBBpumpSt;
  doc["ZBAirSPump"] =  random(0, 1); //ZBAirSPumpSt;
  doc["ZBRO"] =  random(0, 1); //ZBROSt;
  doc["ZBInletFlow"] = flowRate3;
  doc["ZBOutletFlow"] = flowRate4;
  doc["ZBNTankLevel"] = random(0, 1); //waterconlevel


  serializeJsonPretty(doc, Serial3);
  serializeJsonPretty(doc, Serial);
  //Serial.print("test json ");
}


void dateandtime(void) {
  DateTime now = rtc.now();
  hourupg = now.hour();
  //  ////Serial.println("time callng");
  minupg = now.minute();
  secslive = now.second();
  Datelive = now.day();
  Monthlive = now.month();
  Yearlive = now.year();
  //  ////Serial.print("datefun");
}
bool switchst ;
long waterconlevel;
void getNutrientTL()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds (10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds (20);
  digitalWrite(trigPin, LOW);


  duration = pulseIn(echoPin, HIGH);
  distanceincm = duration * 0.034 / 2;
  distanceinInch = distanceincm / 2.54;
  volume = pi * 16 * distanceincm; //change based on the volume for the tank
  //waterHeight = tankHeight-distanceincm;
  waterQuantity = totalVolume - volume - offset;
  waterconlevel = tankHeight * 2.54 - waterHeight;

  ////Serial.print("Tank level: ");
  ////Serial.println(waterconlevel);
  ////Serial.print("Tank is: ");
  ////Serial.print(distanceincm);
  ////Serial.println(" Cm");
  ////Serial.print(distanceinInch);
  //////Serial.println(" Intch");
  //////Serial.print("Water level [cm]: ");
  //////Serial.println(waterHeight);
  //////Serial.print("Water Quantity [ml]: ");
  //////Serial.println(waterQuantity);

  int temp;
  if (distanceincm < tanksetpoint && temp == 0)
  {
    digitalWrite(Z1SOLVALVE, LOW);
    ////Serial.println(" PUMP OFF");

    temp = 1;
  }

  else if (distanceincm < tanksetpoint && temp == 1)
  {
    digitalWrite(Z1SOLVALVE, LOW);
    ////Serial.println(" PUMP OFF");
  }

  else if (distanceincm < tankHeight)
  {
    digitalWrite(Z1SOLVALVE, HIGH);
    ////Serial.println(" PUMP ON");

    temp = 0;
  }



  return distance;



}

class Switcher
{
    // class member variables
    byte relayPin; // number of pin for relay
    long OnTime;
    long OffTime;
    int relayState; // set relay state (active HIGH)
    unsigned long previousMillis; // set time since last update
  public:
    Switcher(byte pin)
    {
      relayPin = pin;
    }

    void begin(long on, long off)
    {
      pinMode(relayPin, OUTPUT);
      OnTime = on;
      OffTime = off;
      relayState = HIGH;
      previousMillis = 0;
    }
    void Update()
    {
      // check the time to see if relays need to be turned on
      // or off
      unsigned long currentMillis = millis();

      if ((relayState == HIGH) && (currentMillis - previousMillis >= OffTime))
      {
        relayState = LOW; // Turn it off
        Serial.println("relay off");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
      }
      else if ((relayState == LOW) && (currentMillis - previousMillis >= OnTime))
      {
        relayState = HIGH; // turn it on
        previousMillis = currentMillis;
        Serial.println("relay On");
        digitalWrite(relayPin, relayState);
      }
    }
};
Switcher PumpAZ1(Z1PUMPA);
Switcher PumpBZ1(Z1PUMPB);
Switcher PumpDOZ1(Z1AIRPUMP);

Switcher PumpAZ2(Z2PUMPA);
Switcher PumpBZ2(Z2PUMPB);
Switcher PumpDOZ2(Z2AIRPUMP);
bool PumpAZ1bool;
bool PumpBZ1bool;
bool PumpDOZ1bool;

bool PumpAZ2bool;
bool PumpBZ2bool;
bool PumpDOZ2bool;

long PumpAOnTime ;
long PumpAOffTime ;

long PumpBOnTime ;
long PumpBOffTime ;

long PumpDOOnTime;
long PumpDOOffTime ;
//***********************************************************************
void SerialCom() {

  if (Serial2.available()) {
    String data_from_display = "";
    //   String dfd;           //use for setpoints but will effect schedule mode
    delay(60);
    while (Serial2.available()) {
      data_from_display += char(Serial2.read());
    }
    //    ////Serial.println(data_from_display);
    dfd = data_from_display;
    dfd.toCharArray(buff, dfd.length());
    //    ////Serial.println(buff);

    /**************** AUTO MODE ******************/

    if ((data_from_display.substring(0, 5) == "saz1")) {//.substring(0, 5)

      Z1PAsavetime();
      Z1PAsavedue();
      Serial.println("ZONE A PUMP A");
      Serial.print("othr: ");
      Serial.println(Z1PASThr);

      Serial.print("otMin: ");
      Serial.println(Z1PASTmin);

      Serial.print("oftHr: ");
      Serial.println(Z1PASPhr);

      Serial.print("oftMin: ");
      Serial.println(Z1PASPmin);
      feedBack = "saved";


    }
    if ((data_from_display.substring(0, 5) == "apaz2")) {//.substring(0, 5)

      Z2PAsavetime();
      Z2PAsavedue();
      //Same
      
      Serial.println("ZONE B PUMP A");
      Serial.println(Z2PASThr);                  //Shift1Clockhr();

      Serial.println(Z2PASTmin);                //Shift1Clockmin();

      Serial.println(Z2PASPhr);                  //Shift1Clockhr();

      Serial.println(Z2PASPmin);
      feedBack = "saved";
    }

    if ((data_from_display.substring(0, 5) = "saz1")) {

      Z1PBsavetime();
      Z1PBsavedue();
      //PUMP B ZA in next
      Serial.println("ZONE A PUMP B");
       
      Serial.println(Z1PBSThr);                  //Shift1Clockhr();

      Serial.println(Z1PBSTmin);                //Shift1Clockmin();

      Serial.println(Z1PBSPhr);                  //Shift1Clockhr();

      Serial.println(Z1PBSPmin);

      feedBack = "saved";
    }
    if ((data_from_display.substring(0, 5) == "apbz2")) {
      Serial.println("ZONE B PUMP B");
      //same

      Z2PBsavetime();
      Z2PBsavedue();
      Serial.println(Z2PBSThr);                  //Shift1Clockhr();

      Serial.println(Z2PBSTmin);                //Shift1Clockmin();

      Serial.println(Z2PBSPhr);                  //Shift1Clockhr();

      ////Serial.println(Z2PBSPmin);//Z2PBSPmin
      feedBack = "saved";
    }
    if ((data_from_display.substring(0, 5) == "apdz1")) {

      Z1APsavetime();
      Z1APsavedue();
      //Airstone A Zone
      Serial.println("ZONE A PUMP AIR");
      Serial.println(Z1APSThr);                  //Shift1Clockhr();

      Serial.println(Z1APSTmin);                //Shift1Clockmin();

      Serial.println(Z1APSPhr);                  //Shift1Clockhr();

      Serial.println(Z1APSPmin);
      feedBack = "saved";
    }
    if ((data_from_display.substring(0, 5) == "apdz2")) {

      Z2APsavetime();
      Z2APsavedue();
      Serial.println("ZONE B PUMP AirStone");
      Serial.println(Z2APSThr);                  //Shift1Clockhr();

      Serial.println(Z2APSTmin);                //Shift1Clockmin();

      Serial.println(Z2APSPhr);                  //Shift1Clockhr();

      ////Serial.println(Z2APSPmin);
      feedBack = "saved";
    }

    ////************************************************** SCHEDULE MODE ******************************************//
    //    if ((dfd.substring(0, 5) == "az1s1")) {
    //      //////Serial.println("Pump A Z1 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s2")) {
    //      //////Serial.println("Pump A Z1 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "az1s3")) {
    //      //////Serial.println("Pump A Z1 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s4")) {
    //      //////Serial.println("Pump A Z1 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s5")) {
    //      //////Serial.println("Pump A Z1 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s6")) {
    //      //////Serial.println("Pump A Z1 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s7")) {
    //      //////Serial.println("Pump A Z1 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az1s8")) {
    //      //////Serial.println("Pump A Z1 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z1 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    ////
    //    if ((dfd.substring(0, 5) == "az2s1")) {
    //      //////Serial.println("Pump A Z2 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s2")) {
    //      //////Serial.println("Pump A Z2 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "az2s3")) {
    //      //////Serial.println("Pump A Z2 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s4")) {
    //      //////Serial.println("Pump A Z2 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s5")) {
    //      //////Serial.println("Pump A Z2 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s6")) {
    //      //////Serial.println("Pump A Z2 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s7")) {
    //      //////Serial.println("Pump A Z2 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      //////Serial.println("Pump A Z2 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "az2s8")) {
    //      //////Serial.println("Pump A Z2 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      // ////Serial.println("Pump A Z2 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    ///////
    //    if ((dfd.substring(0, 5) == "bz1s1")) {
    //      ////Serial.println("Pump B Z1 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s2")) {
    //      ////Serial.println("Pump B Z1 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "bz1s3")) {
    //      ////Serial.println("Pump B Z1 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s4")) {
    //      ////Serial.println("Pump B Z1 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s5")) {
    //      ////Serial.println("Pump B Z1 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s6")) {
    //      ////Serial.println("Pump B Z1 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s7")) {
    //      ////Serial.println("Pump B Z1 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz1s8")) {
    //      ////Serial.println("Pump B Z1 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z1 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    ////
    //    if ((dfd.substring(0, 5) == "bz2s1")) {
    //      ////Serial.println("Pump B Z2 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s2")) {
    //      ////Serial.println("Pump B Z2 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "bz2s3")) {
    //      ////Serial.println("Pump B Z2 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      // ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s4")) {
    //      ////Serial.println("Pump B Z2 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      // ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s5")) {
    //      ////Serial.println("Pump B Z2 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s6")) {
    //      ////Serial.println("Pump B Z2 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s7")) {
    //      ////Serial.println("Pump B Z2 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "bz2s8")) {
    //      ////Serial.println("Pump B Z2 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump B Z2 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    //////////////////
    //    if ((dfd.substring(0, 5) == "dz1s1")) {
    //      ////Serial.println("Pump DO Z1 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s2")) {
    //      ////Serial.println("Pump DO Z1 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "dz1s3")) {
    //      ////Serial.println("Pump DO Z1 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s4")) {
    //      ////Serial.println("Pump DO Z1 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      //////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      // ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      //////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s5")) {
    //      ////Serial.println("Pump DO Z1 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s6")) {
    //      ////Serial.println("Pump DO Z1 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s7")) {
    //      ////Serial.println("Pump DO Z1 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz1s8")) {
    //      ////Serial.println("Pump DO Z1 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z1 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    ////
    //    if ((dfd.substring(0, 5) == "dz2s1")) {
    //      ////Serial.println("Pump DO Z2 S1 /n On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S1 /n Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s2")) {
    //      ////Serial.println("Pump DO Z2 S2 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S2 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //
    //    if ((dfd.substring(0, 5) == "dz2s3")) {
    //      ////Serial.println("Pump DO Z2 S3 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S3 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s4")) {
    //      ////Serial.println("Pump DO Z2 S4 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S4 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s5")) {
    //      ////Serial.println("Pump DO Z2 S5 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S5 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s6")) {
    //      ////Serial.println("Pump DO Z2 S6 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S6 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s7")) {
    //      ////Serial.println("Pump DO Z2 S7 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S7 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    //    if ((dfd.substring(0, 5) == "dz2s8")) {
    //      ////Serial.println("Pump DO Z2 S8 / On Time");
    //      othr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(otHr);                  //Shift1Clockhr();
    //
    //      otmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(otMin);                //Shift1Clockmin();
    //
    //      ////Serial.println("Pump DO Z2 S8 / Off Time");
    //      ofthr();
    //      //S1Hr = shr;
    //      ////Serial.print("Hr = ");
    //      ////Serial.println(oftHr);                  //Shift1Clockhr();
    //
    //      oftmin();
    //      //S1Min = smn;
    //      ////Serial.print("Min = ");
    //      ////Serial.println(oftMin);
    //    }
    if ((dfd.substring(0, 5) == "tkmst")) {
      //      othr();
      ////Serial.print("Tank Level= ");
      totalVolume1 = othr();
      ////Serial.println(totalVolume1);

      //      tank();
      ////Serial.print("Tank Height= ");
      tankHeight = tank();
      ////Serial.println(tankHeight);

      //      oftmin();
      ////Serial.print("Set Level= ");
      tanksetpoint = oftmin();
      ////Serial.println(tanksetpoint);
      feedBack = "saved";

    }
    /**************************    WIFI   ********************/
    if ((dfd.substring(0, 4) == "sdpd")) {
      pwdname();
      ////Serial.print("User Name: ");
      ////Serial.println(str);
      str = ssid;

      pwd();
      ////Serial.print("Password: ");
      ////Serial.println(dfd);
      dfd = password;
      feedBack = "saved";
    }
    /**************************    CLOUD    ********************/
    if ((dfd.substring(0, 4) == "clod")) {
      pwdname();
      ////Serial.print("Cloud SSID: ");
      ////Serial.println(str);
      str = ssid;

      pwd();
      ////Serial.print("Cloud Password: ");
      ////Serial.println(dfd);
      dfd = password;
      feedBack = "saved";
    }
    /**************************    MESH    ********************/
    if ((dfd.substring(0, 4) == "mesh")) {
      meshpass();
      prefix();
      port();
      feedBack = "saved";
    }
    /**************************    NEXTION - RTC   ********************/
    if ((data_from_display.substring(0, 3) == "clk")) {
      timeMin();
      timeHr();
      timeSec();
      timeYr();
      timeMnt();
      timeDt();
      //      data_from_display = "";
      feedBack = "saved";
      dateandtime();
      readalldata();
      t8.enable();//setTimeNex
      t9.disable();
    }
    if ((data_from_display.substring(0, 6) == "setrtc")) {
      t8.disable();
      t9.enable();//runTimeNex
    }
    sendData(data_from_display);
  }
}

/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  //  ////Serial.println(dfd);
  val = dfd.indexOf("s") + 1; //clkd24m2y2022h2n58s30
  //  ////Serial.println(val);
  dfd.remove(0, val);
  //  Serial2.print("sec:");
  //  Serial2.println(dfd);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
  ////Serial.print("Seconds: ");
  ////Serial.println(secRc);
}
void timeMin() {
  //  ////Serial.println(buff);
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  //  ////Serial.println(cSmn);
  val = cSmn.indexOf("n") + 1; //    ////Serial.println(val);
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
  ////Serial.print("Minutes: ");
  ////Serial.println(minRc);
}
void timeHr() {
  //  ////Serial.println(buff);
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1; //    ////Serial.println(val);
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;
  ////Serial.print("Hours: ");
  ////Serial.println(hrRc);

}
void timeDt() {
  //  ////Serial.println(buff);
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  //  ////Serial.println(str);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
  ////Serial.print("Date: ");
  ////Serial.println(Daterc);

}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  //  ////Serial.println(cSmnt);
  val = cSmnt.indexOf("m") + 1; //    ////Serial.println(val);
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Monthrc = mntRc;
  ////Serial.print("Month: ");
  ////Serial.println(Monthrc);

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //    ////Serial.println(val);
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Yearrc = yrRc;
  ////Serial.print("Year: ");
  ////Serial.println(Yearrc);
}
int othr() {
  ////Serial.println(dfd);//off time min value
  int val = dfd.indexOf("g") + 1;//
  //  ////Serial.println(val);
  dfd.remove(0, val);
  //  ////Serial.println(dfd);
  otHr = dfd.toInt();
  return (otHr);
  //  ////Serial.println(otHr);
}

int otmin() {
  char *nt = strtok(buff, "g");//off time hr val
  str = nt;
  int val = str.indexOf("f") + 1;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);
  //  ////Serial.println(otMin);
}
int ofthr() {
  char *fr = strtok(buff, "f");
  str = fr;
  int val = str.indexOf("e") + 1; //  on time min val
  str.remove(0, val);
  oftHr = str.toInt();
  return (oftHr);
}
int oftmin() {
  char *ui = strtok(buff, "e");//on time hr val
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}

int tank() {
  char *nt = strtok(buff, "g");
  str = nt;
  int val = str.indexOf("e") + 1;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);
}

void pwd() {
  int val = dfd.indexOf("<") + 1;
  dfd.remove(0, val);
}
void pwdname() {
  char *pd = strtok(buff, "<");
  str = pd;
  str.remove(0, 4);
  str = str;
}
/**************************    MESH     ***********************/
void port() {
  int val = dfd.indexOf(">") + 1;
  dfd.remove(0, val);//port
  //  ////Serial.print("Port: ");
  //  ////Serial.println(dfd);
}
void meshpass() {
  char *pd = strtok(buff, ">");
  str = pd;
  int val = str.indexOf("<") + 1;
  str.remove(0, val);//meshpass
  String mstr = str;
  //  ////Serial.print("Mesh Pswd: ");
  //  ////Serial.println(mstr);
}

void prefix() {
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
  //  ////Serial.print("Prefix: ");
  //  ////Serial.println(str1);
}
void sendData(String dfd) {

  //////////////////////// MANUAL MODE //////////////////////////
  if (dfd == "pAz1On") {
    //    relayPA = LOW;
    digitalWrite(Z1PUMPA, LOW);
    Serial.println("Pump A / Zone 1 On");
    feedBack = "pAz1On";
    ZAApumpSt = "on";
  } if (dfd == "pAz1Off") {
    //    relayPA = HIGH;
    digitalWrite(Z1PUMPA, HIGH);
    Serial.println("Pump A / Zone 1 Off");
    feedBack = "pAz1Off";
    ZAApumpSt = "off";
  }
  if (dfd == "pAz2On") {
    relayPAZ2 = LOW;
    digitalWrite(Z2PUMPAtest, LOW);
    Serial.println("Pump A / Zone 2 On");
    feedBack = "pAz2On";
    ZBApumpSt = "on";
  } if (dfd == "pAz2Off") {
    relayPAZ2 = HIGH;
    digitalWrite(Z2PUMPAtest, HIGH);
    Serial.println("Pump A / Zone 2 Off");
    feedBack = "pAz2Off";
    ZBApumpSt = "off";
  }
  if (dfd == "pBz1On") {
    relayPB = LOW;
    digitalWrite(Z1PUMPB, LOW);
    //    digitalWrite(Z1PUMPA,LOW);
    Serial.println("Pump B / Zone 1 On");
    feedBack = "pBz1On";
    ZBApumpSt = "on";
  } if (dfd == "pBz1Off") {
    relayPB = HIGH;
    digitalWrite(Z1PUMPB, HIGH);
    //    digitalWrite(Z2PUMPA, HIGH);
    Serial.println("Pump B / Zone 1 Off");
    feedBack = "pBz1Off";
    ZBApumpSt = "off";
  }
  if (dfd == "pBz2On") {
    relayPBZ2 = LOW;
    digitalWrite(Z2PUMPB, LOW);
    Serial.println("Pump B / Zone 2 On");
    feedBack = "pBz2On";
    ZBBpumpSt = "on";
  } if (dfd == "pBz2Off") {
    relayPBZ2 = HIGH;
    digitalWrite(Z2PUMPB, HIGH);
    Serial.println("Pump B / Zone 2 Off");
    feedBack = "pBz2Off";
    ZBBpumpSt = "off";
  }
  if (dfd == "Doz1On") {
    relayAir = LOW;
    digitalWrite(Z1AIRPUMP, LOW);
    Serial.println("Do Pump / Zone 1 On");
    feedBack = "Doz1On";
    ZAAirSPumpSt = "on";
  }
  if (dfd == "Doz1Off") {
    relayAir = HIGH;
    digitalWrite(Z1AIRPUMP , HIGH);
    Serial.println("Do Pump / Zone 1 Off");
    feedBack = "Doz1Off";
    ZAAirSPumpSt = "off";
  }
  if (dfd == "Doz2On") {
    relayAirZ2 = LOW;
    digitalWrite(Z2AIRPUMP, LOW);
    Serial.println("Do Pump / Zone 2 On");
    feedBack = "Doz2On";
    ZBAirSPumpSt = "on";
  }
  if (dfd == "Doz2Off") {
    relayAirZ2 = HIGH;
    digitalWrite(Z2AIRPUMP , HIGH);
    Serial.println("Do Pump / Zone 2 Off");
    feedBack = "Doz2Off";
    ZBAirSPumpSt = "off";

  }
  if (dfd == "Svz1On") {
    relaySol = LOW;
    digitalWrite(Z1SOLVALVE, LOW);
    Serial.println("Sol valve / Zone 1 On");
    feedBack = "Svz1On";

    ZAROSt = "on";
  }
  if (dfd == "Svz1Off") {
    relaySol = HIGH;
    digitalWrite(Z1SOLVALVE, HIGH);
    Serial.println("Sol valve / Zone 1 Off");
    feedBack = "Svz1Off";

    ZAROSt = "off";
  }
  if (dfd == "Svz2On") {
    relaySolZ2 = LOW;
    digitalWrite(Z2SOLVALVE, LOW);
    Serial.println("Sol valve / Zone 2 On");
    feedBack = "Svz2On";

    ZBROSt = "on";
  }
  if (dfd == "Svz2Off") {
    relaySolZ2 = HIGH;
    digitalWrite(Z2SOLVALVE, HIGH);
    //Serial.println("Sol valve / Zone 2 Off");
    feedBack = "Svz2Off";
    ZBROSt = "off";
  }

  /************************* SETTINS ***************************/
  if (dfd == "TkMgOn") {
    ////Serial.println("Tank Management On");
    feedBack = "TkMgOn";
    water_level_sensor = true;
    waterlevel_sensor = true;

  } if (dfd == "TkMgOff") {
    ////Serial.println("Tank Management Off");
    feedBack = "TkMgOn";
    waterlevel_sensor = false;
  }

  ///////// AUTO AND SCHEDULE MODES////////////
  //  if (dfd == "amOn") {
  //    ////Serial.println("Z1 On");
  //    Autocnt = true;
  //    runer();
  //    Z2Autocnt = false;
  //    Z2stoper();
  //    feedBack = "amOn";
  //  }
  //  if (dfd == "amOff") {
  //    ////Serial.println("Auto Mode Z1  Off");
  //    Autocnt = false;
  //    stoper();
  //    feedBack = "amOff";
  //  }
  //
  //  if (dfd == "Z2amOn") {
  //    ////Serial.println("Auto Mode Z2 On");
  //    Z2Autocnt = true;
  //    Z2runer();
  //    Autocnt = false;
  //    stoper();
  //    feedBack = "Z2amOn";
  //  }
  //  if (dfd == "Z2amOff") {
  //    ////Serial.println("Auto Mode Z2 Off");//
  //    Z2Autocnt = false;
  //    Z2stoper();
  //    feedBack = "Z2amOff";
  //  }


#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000
  //***********************************************ZONE A PUMP A**********************************************************************
  if ((dfd = "z1aon"))  {
    Serial.print("othr: ");

    Serial.println(Z1PASThr);

    Serial.print("otMin: ");
    Serial.println(Z1PASTmin);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhr);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPmin);
    long Z1PASThrconvOn = Z1PASThr * ms_per_hour;
    long Z1PASTminconvOn = Z1PASTmin * ms_per_min;
    long PumpAOnTimetotelOn = Z1PASThrconvOn + Z1PASTminconvOn;

    Serial.print("final mills Hr: ");
    Serial.println(Z1PASThrconvOn);
    Serial.print("final mill min: ");
    Serial.println(Z1PASTminconvOn);
    Serial.print("Total  ");
    Serial.println(PumpAOnTimetotelOn);

    long Z1PASThrconvoff = Z1PASPhr * ms_per_hour;
    long Z1PASTminconvoff = Z1PASPmin * ms_per_min;
    long PumpAOnTimetoteloff = Z1PASThrconvoff + Z1PASTminconvoff;

    Serial.print("OFF final mills Hr: ");
    Serial.println(Z1PASThrconvoff);
    Serial.print("OFF final mill min: ");
    Serial.println(Z1PASTminconvoff);
    Serial.print("OFF Total  ");
    Serial.println(PumpAOnTimetoteloff);

    PumpAZ1.begin(PumpAOnTimetotelOn , PumpAOnTimetoteloff );

    Serial.println("PUMPA Z1 On");
    PumpAZ1bool = true;
    PumpAZ1.Update();

    //    PumpAZ1_Controller();
    EepromReadZ1AP();
    //    PUMPAON.start();
    feedBack = "saz1";
  }
  if (dfd == "z1aoff") {
    PumpAZ1bool = false;
    Serial.println("PUMPA Z1 Off");
    //    PUMPAON.stop();
    //    PUMPAOFF.stop();
    feedBack = "z1aoff";
  }

  //*************************************************ZONE A PUMP B**********************************
  if (dfd == "z1bon") {
     Serial.println("ZONE A PUMP B");
       
      Serial.println(Z1PBSThr);                  //Shift1Clockhr();

      Serial.println(Z1PBSTmin);                //Shift1Clockmin();

      Serial.println(Z1PBSPhr);                  //Shift1Clockhr();

      Serial.println(Z1PBSPmin);
    long Z1PBSThrconvOn = Z1PBSThr * ms_per_hour;
    long Z1PBSTminconvOn = Z1PBSTmin * ms_per_min;
    long PumpBOnTimetotelOn = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1PBSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1PBSPmin * ms_per_min;
    long PumpBOnTimetoteloff = Z1PBSThrconvoff + Z1PBSTminconvoff;

    PumpBZ1.begin(PumpBOnTimetotelOn , PumpBOnTimetoteloff);
    PumpBZ1bool = true;
    Serial.println("PUMP B ON");
    //    PumpBZ1_Controller();
    EepromReadZ1PB();
    //    PUMPBON.start();
    feedBack = "z1bon";
  }
  if (dfd == "z1boff") {
    PumpBZ1bool = false;
    Serial.println("PUMP B Off");
    //    PUMPBON.stop();
    //    PUMPBOFF.stop();
    feedBack = "z1boff";
  }

  //ZONE A PUMP DO
  if (dfd == "z1don") {
    Serial.println("ZONE A PUMP AIR");
      Serial.println(Z1APSThr);                  //Shift1Clockhr();

      Serial.println(Z1APSTmin);                //Shift1Clockmin();

      Serial.println(Z1APSPhr);                  //Shift1Clockhr();

      Serial.println(Z1APSPmin);
    long Z1PBSThrconvOn = Z1APSThr * ms_per_hour;
    long Z1PBSTminconvOn = Z1APSTmin * ms_per_min;
    long PumpDOOnTime = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1APSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1APSPmin * ms_per_min;
    long PumpDOOffTime = Z1PBSThrconvoff + Z1PBSTminconvoff;

    PumpDOZ1.begin(PumpDOOnTime , PumpDOOffTime);
    PumpDOZ1bool = true;
    Serial.println("PUMP DO ON");
    PumpDOZ1_Controller();
    EepromReadZ1Air();
    //    AIRSTONEON.start();
    feedBack = "z1don";
  }
  if (dfd == "z1doff") {
    PumpDOZ1bool = false;
    Serial.println("PUMP DO Off");
    //    AIRSTONEON.stop();
    //    AIRSTONEOFF.stop();
    feedBack = "z1doff";
  }
  /*************** ZONE 2 ******************/
  if (dfd == "z2aon") {
    PumpAZ2.begin(PumpAOnTime, PumpAOffTime);
    ////Serial.println("PUMPA Z2 On");//
    //    PumpAZ2_Controller();
    EepromReadZ2AP();
    //    Z2PUMPAON.start();
    feedBack = "z2aon";
  }
  if (dfd == "z2aoff") {
    ////Serial.println("PUMPA Z2 Off");
    //    Z2PUMPAON.stop();
    //    Z2PUMPAOFF.stop();
    feedBack = "z2aoff";
  }

  if (dfd == "z2bon") {
    PumpBZ2.begin(PumpAOnTime, PumpAOffTime);
    ////Serial.println("PUMPB ON");
    //    PumpBZ2_Controller();
    EepromReadZ2PB();
    //    Z2PUMPBON.start();
    feedBack = "z2bon";
  }
  if (dfd == "z2boff") {
    ////Serial.println("PUMPB Off");
    //    Z2PUMPBON.stop();
    //    Z2PUMPBOFF.stop();
    feedBack = "z2boff";
  }
  if (dfd == "z2don") {
    PumpDOZ2.begin(PumpDOOnTime , PumpDOOnTime);
    Serial.println("PUMPDO ON");
    //    PumpDOZ2_Controller();
    EepromReadZ2Air();
    //    Z2AIRSTONEON.start();
    feedBack = "z2don";
  }
  if (dfd == "z2doff") {
    Serial.println("PUMPDO Off");
    //    Z2AIRSTONEON.stop();
    //    Z2AIRSTONEOFF.stop();
    feedBack = "z2doff";
  }

}

void waterflow() {
  count = 0;
  count2 = 0;
  count3 = 0;      // Reset the counter so we start counting from 0 again
  count4 = 0;
  interrupts();   //Enables interrupts on the Arduino
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    noInterrupts(); //Disable the interrupts on the Arduino
  }
  //  noInterrupts(); //Disable the interrupts on the Arduino


  flowRate = (count * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  //  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute

  //Start the math
  flowRate2 = (count2 * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  //  flowRate2 = flowRate2 * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate2 = flowRate2 / 1000;       //Convert mL to Liters, giving you Liters / Minute

  flowRate3 = (count3 * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  //  flowRate3 = flowRate3 * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate3 = flowRate3 / 1000;

  flowRate4 = (count4 * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  //  flowRate4 = flowRate4 * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate4 = flowRate4 / 1000;

  //Serial.print("flowRate1 : ");
  //Serial.println(flowRate);
  //Serial.print("flowRate2 : ");
  //Serial.println(flowRate2);
  //Serial.print("flowRate3 : ");
  //Serial.println(flowRate3);         //Print the variable flowRate to Serial
  //Serial.print("flowRate4 : ");
  //Serial.println(flowRate4);
}
void Flow()
{
  count++; //Every time this function is called, increment "count" by 1
}
void Flow2()
{
  count2++; //Every time this function is called, increment "count" by 1
}
void Flow3()
{
  count3++; //Every time this function is called, increment "count" by 1
}
void Flow4()
{
  count4++; //Every time this function is called, increment "count" by 1
}

void nexVal() {
  dateandtime();
  Serial2.print("fdbk.txt=\"");
  Serial2.print(feedBack);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("solar.txt=\"");
  Serial2.print(solar_Value);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("inlet.txt=\"");
  Serial2.print(flowRate);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("tank.txt=\"");
  Serial2.print(totalVolume1);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //Serial.print(nutrientTemperature);
  //  Serial2.print("WaterTemp.txt=\"");
  //  Serial2.print(nutrientTemperature);
  //  Serial2.print("\"");
  //  Serial2.write(0x03ff);
  //  Serial2.write(0x03ff);
  //  Serial2.write(0x03ff);

  /**************** DATE & TIME ***********************/
  //Serial.print(hourupg);
  Serial2.print("hour.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("hourr.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //Serial.print(minupg);
  Serial2.print("min.txt=\"");
  Serial2.print(minupg);//minupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //  //Serial.print(secslive);
  Serial2.print("sec.txt=\"");
  Serial2.print(secslive);//secslive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  ////////////  YEAR ////////

  ////Serial.print(Datelive);
  Serial2.print("day.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("dayy.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  // //Serial.print(Monthlive);
  Serial2.print("month.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("monthh.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  ////Serial.print(Yearlive);
  Serial2.print("year.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  //    SerialU.println(yearch);

  Serial2.print("yearr.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  ////////////////// wifi status /////////////
  Serial2.print("hstatus.txt=\"");
  Serial2.print(wifiStatus);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("readtank.txt=\"");
  Serial2.print(waterconlevel);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
}

///**********Counter Timer****************

void print_timePUMPA1()
{
  //  //Serial.println("counter up");
  digitalWrite(Z1PUMPA, LOW);
  ZAApumpSt = "on";
  //Serial.print("PUMP A ON TIME: ");
  //Serial.println(PUMPAON.getCurrentTime());
}
void print_timePUMPA2()
{
  //  PUMPAON.pause();
  digitalWrite(Z1PUMPA, HIGH);
  ZAApumpSt = "off";
  //Serial.print("PUMP A OFF TIME: ");
  //Serial.println(PUMPAOFF.getCurrentTime());
}

void PUMPAONComplete()
{
  //   //Serial.println("pumpaONcomplete");
  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis >= interval) {
  //    previousMillis = currentMillis;
  //PUMPAOFF.restart();
  PUMPAOFF.start();
  //    //Serial.println("call func PUMPAONComplete");
  //  }
}
void PUMPAOFFComplete()
{
  //  //Serial.println("pumpaoffcomplete");
  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis >= interval) {
  //    previousMillis = currentMillis;
  PUMPAOFF.stop();
  //PUMPAON.restart();
  PUMPAON.start();
  //  }
}

void print_timePUMPA1Z2()
{
  digitalWrite(Z2PUMPA, LOW);
  ZABpumpSt = "on";
  //Serial.print("Z2 PUMP A ON TIME: ");
  //Serial.println(Z2PUMPAON.getCurrentTime());
}
void print_timePUMPA2Z2()
{
  //  Z2PUMPAON.pause();
  digitalWrite(Z2PUMPA, HIGH);
  ZABpumpSt = "off";
  //Serial.print("Z2 PUMP A OFF TIME: ");
  //Serial.println(Z2PUMPAOFF.getCurrentTime());
}


void Z2PUMPAONComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //PUMPAOFF.restart();
    Z2PUMPAOFF.start();
  }
}
void Z2PUMPAOFFComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Z2PUMPAOFF.stop();
    //PUMPAON.restart();
    Z2PUMPAON.start();
  }
}

////**********************PUMP B**********
void print_timePUMPB1()
{
  digitalWrite(Z1PUMPB, LOW);
  ZBApumpSt = "on";
  //Serial.print("PUMP B ON TIME: ");
  //Serial.println(PUMPBON.getCurrentTime());
}
void print_timePUMPB2()
{
  digitalWrite(Z1PUMPB, HIGH);
  ZBApumpSt = "off";
  //Serial.print("PUMP B OFF TIME: ");
  //Serial.println(PUMPBOFF.getCurrentTime());
}



void PUMPBONComplete()
{
  //PUMPBON.stop();
  //PUMPBOFF.restart();
  PUMPBOFF.start();
}
void PUMPBOFFComplete()
{
  PUMPBOFF.stop();
  //PUMPBON.restart();
  PUMPBON.start();
}

void print_timePUMPB1Z2()
{
  digitalWrite(Z2PUMPB, LOW);
  ZBBpumpSt = "on";
  //Serial.print("Z2 PUMP B ON TIME: ");
  //Serial.println(Z2PUMPBON.getCurrentTime());
}
void print_timePUMPB2Z2()
{
  digitalWrite(Z2PUMPB, HIGH);
  ZBBpumpSt = "off";
  //Serial.print("Z2 PUMP B OFF TIME: ");
  //Serial.println(Z2PUMPBOFF.getCurrentTime());
}

void Z2PUMPBONComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //PUMPAOFF.restart();
    //    Z2PUMPBOFF.start();
  }
}
void Z2PUMPBOFFComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //    Z2PUMPBOFF.stop();
    //PUMPAON.restart();
    //    Z2PUMPBON.start();
  }
}
//************AIR STONE PUMP***********
//char airon;
void print_timeAIRSTONE1()
{
  digitalWrite(Z1AIRPUMP, LOW);
  ZAAirSPumpSt = "on";
  //Serial.print("AIRSTONE ON TIME: ");
  //Serial.println(AIRSTONEON.getCurrentTime());
  ////airon = AIRSTONEON.getCurrentTime();
}
void print_timeAIRSTONE2()
{
  ZAAirSPumpSt = "off";
  digitalWrite(Z1AIRPUMP, HIGH );
  //Serial.print("AIRSTONE OFF TIME: ");
  //Serial.println(AIRSTONEOFF.getCurrentTime());
}

void AIRSTONEONComplete()
{
  //  AIRSTONEON.stop();
  //AIRSTONEOFF.restart();
  AIRSTONEOFF.start();
  
}
void AIRSTONEOFFComplete()
{
  AIRSTONEOFF.stop();
  //AIRSTONEON.restart();
  AIRSTONEON.start();
}

void print_timeAIRSTONE1Z2()
{
  digitalWrite(Z2AIRPUMP, HIGH);
  ZBAirSPumpSt = "off";
  //Serial.print("Z2 AIRSTONE ON TIME: ");
  //Serial.println(Z2AIRSTONEON.getCurrentTime());
  ////  airon = AIRSTONEON.getCurrentTime();
}
void print_timeAIRSTONE2Z2()
{
  ZBAirSPumpSt = "on";
  digitalWrite(Z2AIRPUMP, LOW);
  //Serial.print("Z2 AIRSTONE OFF TIME: ");
  //Serial.println(Z2AIRSTONEOFF.getCurrentTime());
}

void Z2AIRSTONEONComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //PUMPAOFF.restart();
    Z2AIRSTONEOFF.start();
  }
}

void Z2AIRSTONEOFFComplete()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Z2AIRSTONEOFF.stop();
    //PUMPAON.restart();
    Z2AIRSTONEON.start();
  }
}

//****************************************************************


void uva()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;

  float uvIntensity = mapfloat(outputVoltage, 0.0, 31.0, 0.0, 99.0) * 10; //Convert the voltage to a UV intensity level
  solar_Value = uvIntensity;
  //Serial.print("output: ");
  //Serial.print(refLevel);

  //Serial.print("ML8511 output: ");
  //Serial.print(uvLevel);

  //Serial.print(" / ML8511 voltage: ");
  //Serial.print(outputVoltage);

  //Serial.print(" / UV Intensity (mW/cm^2): ");
  //Serial.print(uvIntensity);

  //Serial.println();

  //  delay(3000);
}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setTimeNex() {
  //  //Serial.println("setTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

void runTimeNex() {
  //  //Serial.println("runTimeNex");
  rtc.begin();
  //  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

//void PumpAZ1_Controller(){
//  PUMPAON.setCounter(Z1PASThr, Z1PASTmin, Z1PASTmin, PUMPAON.COUNT_UP, PUMPAONComplete);//PUMPAOFF start
//  PUMPAON.setInterval(print_timePUMPA1, 1000);
//  PUMPAOFF.setCounter(Z1PASPhr, Z1PASPmin, Z1PASPmin, PUMPAOFF.COUNT_DOWN, PUMPAOFFComplete);//PUMPAON start
//  PUMPAOFF.setInterval(print_timePUMPA2, 1000);
//  // PUMPANone.setInterval(print_nonePUMPA, 2000);
//}
//void PumpBZ1_Controller(){
//  PUMPBON.setCounter(Z1PBSThr, Z1PBSTmin, Z1PBSTmin, PUMPBON.COUNT_UP, PUMPBONComplete);
//  PUMPBON.setInterval(print_timePUMPB1, 1000);
//  PUMPBOFF.setCounter(Z1PBSPhr, Z1PBSPmin, Z1PBSPmin, PUMPBOFF.COUNT_DOWN, PUMPBOFFComplete);
//  PUMPBOFF.setInterval(print_timePUMPB2, 1000);
//}
//void PumpDOZ1_Controller(){
//  AIRSTONEON.setCounter(Z1APSThr, Z1APSTmin, Z1APSTmin, AIRSTONEON.COUNT_UP, AIRSTONEONComplete);
//  AIRSTONEON.setInterval(print_timeAIRSTONE1, 1000);
//  AIRSTONEOFF.setCounter(Z1APSPhr, Z1APSPmin, Z1APSPmin, AIRSTONEOFF.COUNT_DOWN, AIRSTONEOFFComplete);
//  AIRSTONEOFF.setInterval(print_timeAIRSTONE2, 1000);
//} //   AIRSTONEone.setInterval(print_noneAIRSTONE, 2000);
//
//void PumpAZ2_Controller(){
//  Z2PUMPAON.setCounter(Z2PASThr, Z2PASTmin, Z2PASTmin, Z2PUMPAON.COUNT_UP, Z2PUMPAONComplete);//Z2PASTmin//Z2PASThr
//  Z2PUMPAON.setInterval(print_timePUMPA1Z2, 1000);
//  Z2PUMPAOFF.setCounter(Z2PASPhr, Z2PASPmin, Z2PASPmin, Z2PUMPAOFF.COUNT_DOWN, Z2PUMPAOFFComplete);//Z2PASPmin
//  Z2PUMPAOFF.setInterval(print_timePUMPA2Z2, 1000);
//}
//void PumpBZ2_Controller(){
//  Z2PUMPBON.setCounter(Z2PBSThr, Z2PBSTmin, Z2PBSTmin, Z2PUMPBON.COUNT_UP, Z2PUMPBONComplete);//Z2PBSTmin
//  Z2PUMPBON.setInterval(print_timePUMPB1Z2, 1000);
//  Z2PUMPBOFF.setCounter(Z2PBSPhr, Z2PBSPmin, Z2PBSPmin, Z2PUMPBOFF.COUNT_DOWN, Z2PUMPBOFFComplete);//Z2PBSPmin
//  Z2PUMPBOFF.setInterval(print_timePUMPB2Z2, 1000);
//}
//void PumpDOZ2_Controller(){
//  Z2AIRSTONEON.setCounter(Z2APSThr, Z2APSTmin, Z2APSTmin, Z2AIRSTONEON.COUNT_UP, Z2AIRSTONEONComplete);//Z2APSTmin
//  Z2AIRSTONEON.setInterval(print_timeAIRSTONE1Z2, 1000);
//  Z2AIRSTONEOFF.setCounter(Z2APSPhr, Z2APSPmin, Z2APSPmin, Z2AIRSTONEOFF.COUNT_DOWN, Z2AIRSTONEOFFComplete);//Z2APSPmin
//  Z2AIRSTONEOFF.setInterval(print_timeAIRSTONE2Z2, 1000);
//}



void PumpAZ1_Controller() {
  PUMPAON.setCounter(Z1PASThr, Z1PASTmin, Z1PASTmin, PUMPAON.COUNT_UP, PUMPAONComplete);//PUMPAOFF start
  PUMPAON.setInterval(print_timePUMPA1, 1000);
  PUMPAOFF.setCounter(Z1PASPhr, Z1PASPmin, Z1PASPmin, PUMPAOFF.COUNT_DOWN, PUMPAOFFComplete);//PUMPAON start
  PUMPAOFF.setInterval(print_timePUMPA2, 1000);
  // PUMPANone.setInterval(print_nonePUMPA, 2000);
}
void PumpBZ1_Controller() {
  PUMPBON.setCounter(Z1PBSThr, Z1PBSTmin, Z1PBSTmin, PUMPBON.COUNT_UP, PUMPBONComplete);
  PUMPBON.setInterval(print_timePUMPB1, 1000);
  PUMPBOFF.setCounter(Z1PBSPhr, Z1PBSPmin, Z1PBSPmin, PUMPBOFF.COUNT_DOWN, PUMPBOFFComplete);
  PUMPBOFF.setInterval(print_timePUMPB2, 1000);
}
void PumpDOZ1_Controller() {
  AIRSTONEON.setCounter(Z1APSThr, Z1APSTmin, Z1APSTmin, AIRSTONEON.COUNT_UP, AIRSTONEONComplete);
  AIRSTONEON.setInterval(print_timeAIRSTONE1, 1000);
  AIRSTONEOFF.setCounter(Z1APSPhr, Z1APSPmin, Z1APSPmin, AIRSTONEOFF.COUNT_DOWN, AIRSTONEOFFComplete);
  AIRSTONEOFF.setInterval(print_timeAIRSTONE2, 1000);
} //   AIRSTONEone.setInterval(print_noneAIRSTONE, 2000);

void PumpAZ2_Controller() {
  Z2PUMPAON.setCounter(Z2PASThr, Z2PASTmin, Z2PASTmin, Z2PUMPAON.COUNT_UP, Z2PUMPAONComplete);//Z2PASTmin//Z2PASThr
  Z2PUMPAON.setInterval(print_timePUMPA1Z2, 1000);
  Z2PUMPAOFF.setCounter(Z2PASPhr, Z2PASPmin, Z2PASPmin, Z2PUMPAOFF.COUNT_DOWN, Z2PUMPAOFFComplete);//Z2PASPmin
  Z2PUMPAOFF.setInterval(print_timePUMPA2Z2, 1000);
}
void PumpBZ2_Controller() {
  Z2PUMPBON.setCounter(Z2PBSThr, Z2PBSTmin, Z2PBSTmin, Z2PUMPBON.COUNT_UP, Z2PUMPBONComplete);//Z2PBSTmin
  Z2PUMPBON.setInterval(print_timePUMPB1Z2, 1000);
  Z2PUMPBOFF.setCounter(Z2PBSPhr, Z2PBSPmin, Z2PBSPmin, Z2PUMPBOFF.COUNT_DOWN, Z2PUMPBOFFComplete);//Z2PBSPmin
  Z2PUMPBOFF.setInterval(print_timePUMPB2Z2, 1000);
}
void PumpDOZ2_Controller() {
  Z2AIRSTONEON.setCounter(Z2APSThr, Z2APSTmin, Z2APSTmin, Z2AIRSTONEON.COUNT_UP, Z2AIRSTONEONComplete);//Z2APSTmin
  Z2AIRSTONEON.setInterval(print_timeAIRSTONE1Z2, 1000);
  Z2AIRSTONEOFF.setCounter(Z2APSPhr, Z2APSPmin, Z2APSPmin, Z2AIRSTONEOFF.COUNT_DOWN, Z2AIRSTONEOFFComplete);//Z2APSPmin
  Z2AIRSTONEOFF.setInterval(print_timeAIRSTONE2Z2, 1000);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led3, OUTPUT);

  Serial.println("        TAYRIX       ");
  Serial.println("IRRIGATION CONTROLLER");

  pinMode(A3 , OUTPUT);
  //WD reset
  // wdt_enable(WDTO_8S);

  //  sensorjson();



  //  readalldata();
  readalldataZ2();
  //TASK
  runTimeNex();
  r.startNow();
  //  //Serial.println("Initialized scheduler");
  t0.enable();  //dateandtime
  t1.enable();  //nexval
  //  r.startNow();
  //  r.addTask(t4);
  //  r.addTask(t1);
  //  //Serial.println("added t1");

  //     t3.enable();
  //     t7.enable();//waterflow

  //  t4.enable();
  //     t6.enable();
  //  r.addTask(t4);
  //  pinMode(encBtn, INPUT_PULLUP);
  //    timedata();

  pinMode(BUZZER, OUTPUT);

  //***Indicator
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  pinMode(Z1PUMPA, OUTPUT);
  pinMode(Z1PUMPAtest, OUTPUT);
  digitalWrite(Z1PUMPAtest, LOW);
  pinMode(Z1PUMPB, OUTPUT);
  pinMode(Z1AIRPUMP, OUTPUT);
  pinMode(Z1SOLVALVE, OUTPUT);
  digitalWrite(Z1PUMPA, LOW);
  digitalWrite(Z1PUMPB, LOW);
  digitalWrite(Z1AIRPUMP, LOW);
  digitalWrite(Z1SOLVALVE, LOW);

  pinMode(Z2PUMPAtest, OUTPUT);
  pinMode(Z2PUMPB, OUTPUT);
  pinMode(Z2AIRPUMP, OUTPUT);
  pinMode(Z2SOLVALVE, OUTPUT);
  digitalWrite(Z2PUMPAtest, LOW);
  digitalWrite(Z2PUMPB, LOW);
  digitalWrite(Z2AIRPUMP, LOW);
  digitalWrite(Z2SOLVALVE, LOW);

  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

  ///Waterlevel
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(watsensor1, INPUT);
  pinMode(watsensor2, INPUT);
  pinMode(watsensor3, INPUT);
  pinMode(watsensor4, INPUT); //Sets the pin as an input
  attachInterrupt(0, Flow, RISING);
  attachInterrupt(1, Flow2, RISING);
  attachInterrupt(3, Flow3, RISING); // pin 20 pump 3
  attachInterrupt(2, Flow4, RISING);  // pin 21 pump 4
  water_level_sensor == false;
}

void loop() {
  //  if (Serial2.available()) {
  SerialCom();
    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 5000) {
    previousMillis = currentMillis;
    waterflow();
  }
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;
    if (water_level_sensor == true) {
      if (waterlevel_sensor == true) {
        getNutrientTL();
      }
      else if (waterlevel_sensor == false) {
        return 0;
      }
    }
  }
  //ZONE A
  if (PumpAZ1bool == true) {
    PumpAZ1.Update();
     Serial.print("nothinh");
  }
  if (PumpBZ1bool == true) {
    PumpBZ1.Update();
  }

  if (PumpDOZ1bool == true) {
    PumpDOZ1.Update();
  }
  //ZONE B
  if (PumpAZ2bool == true) {
    PumpAZ2.Update();
    //  Serial.print("nothinh");
  }
  if (PumpBZ2bool == true) {
    PumpBZ2.Update();
  }

  if (PumpDOZ2bool == true) {
    PumpDOZ2.Update();
  }

  //r.execute();

}
