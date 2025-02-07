//////*************LIBRARY***************//////////////
#include <Arduino.h>
#include <MemoryFree.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <EEPROMex.h>
RTC_DS3231 rtc;
//////*************Declaration***************//////////////
RunningAverage RAT(20);
String data_from_display = "";
String dfd;
String str;
char buff[2000];
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftHr; extern volatile unsigned long timer0_millis;
String feedBack;
long seconds = 00;
long minutes = 00;
long hours = 00;
uint16_t hourrc;
uint16_t minrc;
uint16_t secsrc;
uint16_t Monthrc;
uint16_t Daterc;
int Yearrc;
int relayState;
String pwdname();
String pwd();
void meshpass();
void timeMin();
void prefix();
void port();
void timeHr();
void timeSec();
void timeYr();
void timeMnt();
void timeDt();
void sendData(String );
void scheduleFun();
bool checkflag;
bool checkflag1;
bool checkflag2;
bool checkflag3;
bool checkflag4;
bool checkflag5;
bool checkflag6;
bool checkflag7;
bool checkflag8;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int hourupg;
int minupg;
uint8_t secslive;
int Monthlive;
int Datelive;
int Yearlive;
int val;
bool ZAApumpSt = true;
bool ZBApumpSt = true;
bool ZAAirSPumpSt = false;
bool ZAROSt = false;
int counterm;
String checkstate ;
String storeprvstate = "";
int irz1a;
int irz1b;
int irz2a;
int irz2b;
int doa;
int dob;
/////////****************************** Water Level US Sensor ***********************************
#define RORelay 19
#define trigPin (uint8_t)52
#define echoPin (uint8_t)53
int duration;
int distance;
float waterHeight;
uint8_t tankHeight ;
uint8_t totalVolume ;
float waterQuantity;
float volume;
float pi = 3.14159265359;
float offset = 245;
//Water distance
float Wtime = 0, Wdistance = 0;
uint8_t Wtemp = 0;
bool TankM;
uint8_t tanksetpoint;
int distanceincm;
int distanceinInch;
int totalVolume1;
int watsensor1 = 2;
int watsensor2 = 3;
float flowRate;
float flowRate2;
volatile int count;
volatile int count2;
String ssid;
String password;
#define Z1PUMPA    36 //Z1PUMPA
#define Z1PUMPB    38 //Z1PUMPB
#define Z1AIRPUMP  23 //Z2PUMPA
#define Z1SOLVALVE 25 //Z2PUMPB
#define DOOA  32 // Z1DO
#define DOOB 27  // Z2DO
//////*************EEPROM  Declaration***************//////////////
uint8_t EepromZ1PASThrq = 915;
uint8_t EepromZ1PASTminq = 917;
uint8_t EepromZ1PASPhrq = 919;
uint8_t EepromZ1PASPminq = 921;
uint8_t EepromZ1PASThrqq = 923;
uint8_t EepromZ1PASTminqq = 925;
uint8_t EepromZ1PASPhrqq = 927;
uint8_t EepromZ1PASPminqq = 931;
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
uint8_t EepromLBPASThr = 100;
uint8_t EepromLBPASTmin = 102;
uint8_t EepromLBPASPhr = 104;
uint8_t EepromLBPASPmin = 106;
uint8_t Eepromsch1hr = 506;
uint8_t Eepromsch1min = 516;
uint8_t Eepromsch1hr1 = 526;
uint8_t Eepromsch1min1 = 536;
uint8_t Eepromsch2hr = 606;
uint8_t Eepromsch2min = 616;
uint8_t Eepromsch2hr1 = 626;
uint8_t Eepromsch2min1 = 636;
uint8_t Eepromsch3hr = 646;
uint8_t Eepromsch3min = 656;
uint8_t Eepromsch3hr1 = 666;
uint8_t Eepromsch3min1 = 676;
uint8_t Eepromsch4hr = 686;
uint8_t Eepromsch4min = 696;
uint8_t Eepromsch4hr1 = 706;
uint8_t Eepromsch4min1 = 716;
uint8_t Eepromsch5hr = 726;
uint8_t Eepromsch5min = 736;
uint8_t Eepromsch5hr1 = 746;
uint8_t Eepromsch5min1 = 756;
uint8_t Eepromsch6hr = 766;
uint8_t Eepromsch6min = 776;
uint8_t Eepromsch6hr1 = 786;
uint8_t Eepromsch6min1 = 796;
uint8_t Eepromsch7hr = 800;
uint8_t Eepromsch7min = 810;
uint8_t Eepromsch7hr1 = 820;
uint8_t Eepromsch7min1 = 830;
uint8_t Eepromsch8hr = 840;
uint8_t Eepromsch8min = 850;
uint8_t Eepromsch8hr1 = 860;
uint8_t Eepromsch8min1 = 870;
uint8_t Eepromsch9hr = 880;
uint8_t Eepromsch9min = 890;
uint8_t Eepromsch9hr1 = 900;
uint8_t Eepromsch9min1 = 910;
//////*************RTCSETUP***************//////////////
boolean Reset = false;

void dateandtime() {
  DateTime now = rtc.now();
  hourupg = now.hour();
  minupg = now.minute();
  secslive = now.second();
  Datelive = now.day();
  Monthlive = now.month();
  Yearlive = now.year();
}
void readalldata() {
  EEPROM.write(200, hourrc);
  EEPROM.write(206, minrc);
  EEPROM.write(209, secsrc);
  EEPROM.write(212, Daterc);
  EEPROM.write(215, Monthrc);
  EEPROM.write(218, Yearrc);

  EEPROM.read(200);
  EEPROM.read(206);
  EEPROM.read(209);
  EEPROM.read(212);
  EEPROM.read(215);
  EEPROM.read(218);
}
//////*************TIME SAVING VARIABLES***************//////////////
uint8_t Z1PASThr;
uint8_t Z1PASTmin;
uint8_t Z1PASPhr;
uint8_t Z1PASPmin;
uint8_t Z1PASThrq;
uint8_t Z1PASTminq;
uint8_t Z1PASPhrq;
uint8_t Z1PASPminq;
uint8_t Z1PASThrqq;
uint8_t Z1PASTminqq;
uint8_t Z1PASPhrqq;
uint8_t Z1PASPminqq;
uint8_t Z1ZPASThr;
uint8_t Z1ZPASTmin;
uint8_t Z1ZPASPhr;
uint8_t Z1ZPASPmin;
uint8_t Z1PBSThr;
uint8_t Z1PBSTmin;
uint8_t Z1PBSPhr;
uint8_t Z1PBSPmin;
uint8_t Z1ZPBSThr;
uint8_t Z1ZPBSTmin;
uint8_t Z1ZPBSPhr;
uint8_t Z1ZPBSPmin;
uint8_t Z1APSThr;
uint8_t Z1APSTmin;
uint8_t Z1APSPhr;
uint8_t Z1APSPmin;
uint8_t Z1ZAPSThr;
uint8_t Z1ZAPSTmin;
uint8_t Z1ZAPSPhr;
uint8_t Z1ZAPSPmin;
uint8_t LBPASThr;
uint8_t LBPASTmin;
uint8_t LBPASPhr;
uint8_t LBPASPmin;
uint8_t sch1PSThr;
uint8_t sch1PSTmin;
uint8_t sch1PSThr1;
uint8_t sch1PSTmin1;
uint8_t sch2PSThr;
uint8_t sch2PSTmin;
uint8_t sch2PSThr1;
uint8_t sch2PSTmin1;
uint8_t sch3PSThr;
uint8_t sch3PSTmin;
uint8_t sch3PSThr1;
uint8_t sch3PSTmin1;
uint8_t sch4PSThr;
uint8_t sch4PSTmin;
uint8_t sch4PSThr1;
uint8_t sch4PSTmin1;
uint8_t sch5PSThr;
uint8_t sch5PSTmin;
uint8_t sch5PSThr1;
uint8_t sch5PSTmin1;
uint8_t sch6PSThr;
uint8_t sch6PSTmin;
uint8_t sch6PSThr1;
uint8_t sch6PSTmin1;
uint8_t sch7PSThr;
uint8_t sch7PSTmin;
uint8_t sch7PSThr1;
uint8_t sch7PSTmin1;
uint8_t sch8PSThr;
uint8_t sch8PSTmin;
uint8_t sch8PSThr1;
uint8_t sch8PSTmin1;
uint8_t sch9PSThr;
uint8_t sch9PSTmin;
uint8_t sch9PSThr1;
uint8_t sch9PSTmin1;
uint8_t sch10PSTmin1;
bool status1;
//////*************SCWITCHER FUNCTION***************//////////////
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
      status1 = true;
    }
    void Update()
    {

      unsigned long currentMillis = millis();
      if ((currentMillis < OffTime) && (status1 == true))
      {
        Serial.println("ok");
        currentMillis =  OffTime;
        status1 = false;
      }

      if ((relayState == HIGH) && (currentMillis - previousMillis >= OffTime))
      {
        unsigned long currentMillis = millis();
        relayState = LOW; // Turn it off
        Serial.println("relay ON");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
      }
      else if ((relayState == LOW) && (currentMillis - previousMillis >= OnTime))
      {
        relayState = HIGH ; // turn it on
        previousMillis = currentMillis;
        Serial.println("relay OFF");
        digitalWrite(relayPin, relayState);
      }
    }
};

Switcher PumpAZ1q(DOOA);
Switcher PumpAZ1qq(DOOB);
Switcher PumpAZ1(Z1PUMPA);//PUMP A///
Switcher PumpBZ1(Z1PUMPB);//LIGHT A///
Switcher PumpDOZ1(Z1AIRPUMP);//PUMP DO///
Switcher PumpLB(Z1SOLVALVE);// LIGHT B///

bool PumpAZ1bool;
bool PumpAZ1boolq;
bool PumpAZ1boolqq;
bool PumpBZ1bool;
bool PumpDOZ1bool;
bool PumpLBbool;
long PumpAOnTime ;
long PumpAOffTime ;
long PumpAOnTimeq ;
long PumpAOffTimeq ;
long PumpAOnTimeqq ;
long PumpAOffTimeqq ;
long PumpBOnTime ;
long PumpBOffTime ;
long PumpDOOnTime;
long PumpDOOffTime ;
long PumpLBOnTime;
long PumpLBOffTime;
/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  val = dfd.indexOf("s") + 1;
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}
void timeMin() {

  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  val = cSmn.indexOf("n") + 1;
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
}
void timeHr() {
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1;
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt();
  hourrc = hrRc;


}
void timeDt() {
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;

  val = cSmnt.indexOf("m") + 1;
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt();
  Monthrc = mntRc;

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt();
  Yearrc = yrRc;
}
int othr() {

  int val = dfd.indexOf("g") + 1;//
  dfd.remove(0, val);
  otHr = dfd.toInt();
  return (otHr);
}

int otmin() {
  char *nt = strtok(buff, "g");//off time hr val
  str = nt;
  int val = str.indexOf("f") + 1;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);
}
int otmin1() {
  char *nt = strtok(buff, "g");
  str = nt;
  int val = str.indexOf("f") ;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);

}
int ofthr() {
  char *fr = strtok(buff, "f");
  str = fr;
  int val = str.indexOf("e") + 1;
  str.remove(0, val);
  oftHr = str.toInt();
  return (oftHr);
}
int oftmin() {
  char *ui = strtok(buff, "e");
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}
int oftmin1() {
  char *ui = strtok(buff, "e");
  str = ui;
  str.remove(0, 4);
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
String chipID;
String cloudIp;
String wifiStatus;
String userSsid;
String userPswd;
/////*****************WIFI SETUP*************//////
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

    Serial.print("chipID: ");
    Serial.println(chipID);
    Serial.println("cloudIp: ");
    Serial.print(cloudIp);
    Serial.println("wifiStatus: ");
    Serial.print(wifiStatus);
    Serial.println("userSsid: ");
    Serial.print(userSsid);
    Serial.println("userPswd: ");
    Serial.print(userPswd);
    delay(1000);
  }
}
void ssidpwd() {
  DynamicJsonDocument doc(150);
  JsonObject SdPwd = doc.createNestedObject("SdPwd");
  SdPwd["ssid"] = ssid;
  SdPwd["password"] = password;
  serializeJsonPretty(doc, Serial3);
}
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
///////////////////////////////////////

void Z1PAsavetimeq()
{

  Z1PASThrq = othr();
  Z1PASTminq = otmin();
  EEPROM.update(Eepromsch8hr, Z1PASThrq);
  EEPROM.update(Eepromsch8min, Z1PASTminq);

}

void Z1PAsavedueq()
{
  Z1PASPhrq = ofthr();
  Z1PASPminq = oftmin();
  EEPROM.update(Eepromsch8hr1, Z1PASPhrq);
  EEPROM.update(Eepromsch8min1, Z1PASPminq);
}

void EepromReadZ1APq()
{
  Z1PASThrq = EEPROM.read(Eepromsch8hr);
  Z1PASTminq = EEPROM.read(Eepromsch8min);
  Z1PASPhrq = EEPROM.read(Eepromsch8hr1);
  Z1PASPminq = EEPROM.read(Eepromsch8min1);
}
///////////////////////////////////////////////
void Z1PAsavetimeqq() {

  Z1PASThrqq = othr();
  Z1PASTminqq = otmin();

  Serial.println("hr");
  Serial.println(Z1PASThrqq);

  Serial.println("min");
  Serial.println(Z1PASTminqq);

  EEPROM.update(Eepromsch9hr, Z1PASThrqq);
  EEPROM.update(Eepromsch9min, Z1PASTminqq);
}

void Z1PAsavedueqq() {
  Z1PASPhrqq = ofthr();
  Z1PASPminqq = oftmin();

  EEPROM.update(Eepromsch9hr1, Z1PASPhrqq);
  EEPROM.update(Eepromsch9min1, Z1PASPminqq);
}

void EepromReadZ1APqq()
{
  Z1PASThrqq = EEPROM.read(Eepromsch9hr1);
  Z1PASTminqq = EEPROM.read(Eepromsch9min);
  Z1PASPhrqq = EEPROM.read(Eepromsch9hr1);
  Z1PASPminqq = EEPROM.read(Eepromsch9min1);
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
/**************************************************************************************************************/

void LBPAsavetime() {
  LBPASThr = othr();
  LBPASTmin = otmin1();
  EEPROM.update(EepromLBPASThr, LBPASThr);
  EEPROM.update(EepromLBPASTmin, LBPASTmin);
}

void LBPAsavedue() {
  LBPASPhr = ofthr();
  LBPASPmin = oftmin();
  EEPROM.update(EepromLBPASPhr, LBPASPhr);
  EEPROM.update(EepromLBPASPmin, LBPASPmin);
}

void EepromReadLBAP()
{
  LBPASThr = EEPROM.read(EepromLBPASThr);
  LBPASTmin = EEPROM.read(EepromLBPASTmin);
  LBPASPhr = EEPROM.read(EepromLBPASPhr);
  LBPASPmin = EEPROM.read(EepromLBPASPmin);
}
//////////////////////////////////////////////
void sch1savetime() {
  sch1PSThr = othr();
  sch1PSTmin = otmin();
  EEPROM.update(Eepromsch1hr, sch1PSThr);
  EEPROM.update(Eepromsch1min, sch1PSTmin);
}
void sch1savedue() {
  sch1PSThr1 = ofthr();
  sch1PSTmin1 = oftmin1();
  EEPROM.update(Eepromsch1hr1, sch1PSThr1);
  EEPROM.update(Eepromsch1min1, sch1PSTmin1);

}
void EepromReadsch1()
{
  sch1PSThr = EEPROM.read(Eepromsch1hr);
  sch1PSTmin = EEPROM.read(Eepromsch1min);
  sch1PSThr1 = EEPROM.read(Eepromsch1hr1);
  sch1PSTmin1 = EEPROM.read(Eepromsch1min1);
}
//////////////////////////////////////////////////

void sch2savetime() {
  sch2PSThr = othr();
  sch2PSTmin = otmin();
  EEPROM.update(Eepromsch2hr, sch2PSThr);
  EEPROM.update(Eepromsch2min, sch2PSTmin);
}
void sch2savedue() {
  sch2PSThr1 = ofthr();
  sch2PSTmin1 = oftmin();
  EEPROM.update(Eepromsch2hr1, sch2PSThr1);
  EEPROM.update(Eepromsch2min1, sch2PSTmin1);

}
void EepromReadsch2()
{
  sch2PSThr = EEPROM.read(Eepromsch2hr);
  sch2PSTmin = EEPROM.read(Eepromsch2min);
  sch2PSThr1 = EEPROM.read(Eepromsch2hr1);
  sch2PSTmin1 = EEPROM.read(Eepromsch2min1);
}



/////////////////////////////////////////////////////////

void sch3savetime() {
  sch3PSThr = othr();
  sch3PSTmin = otmin();
  EEPROM.update(Eepromsch3hr, sch3PSThr);
  EEPROM.update(Eepromsch3min, sch3PSTmin);
}
void sch3savedue() {
  sch3PSThr1 = ofthr();
  sch3PSTmin1 = oftmin();
  EEPROM.update(Eepromsch3hr1, sch3PSThr1);
  EEPROM.update(Eepromsch3min1, sch3PSTmin1);

}
void EepromReadsch3()
{
  sch3PSThr = EEPROM.read(Eepromsch3hr);
  sch3PSTmin = EEPROM.read(Eepromsch3min);
  sch3PSThr1 = EEPROM.read(Eepromsch3hr1);
  sch3PSTmin1 = EEPROM.read(Eepromsch3min1);
}
//////////////////////////////////////////////////////////////////
void sch4savetime() {
  sch4PSThr = othr();
  sch4PSTmin = otmin();
  EEPROM.update(Eepromsch4hr, sch4PSThr);
  EEPROM.update(Eepromsch4min, sch4PSTmin);
}
void sch4savedue() {
  sch4PSThr1 = ofthr();
  sch4PSTmin1 = oftmin();
  EEPROM.update(Eepromsch4hr1, sch4PSThr1);
  EEPROM.update(Eepromsch4min1, sch4PSTmin1);

}
void EepromReadsch4()
{
  sch4PSThr = EEPROM.read(Eepromsch4hr);
  sch4PSTmin = EEPROM.read(Eepromsch4min);
  sch4PSThr1 = EEPROM.read(Eepromsch4hr1);
  sch4PSTmin1 = EEPROM.read(Eepromsch4min1);
}
////////////////////////////////////////////////////////////////
void sch5savetime() {
  sch5PSThr = othr();
  sch5PSTmin = otmin();
  EEPROM.update(Eepromsch5hr, sch5PSThr);
  EEPROM.update(Eepromsch5min, sch5PSTmin);
}
void sch5savedue() {
  sch5PSThr1 = ofthr();
  sch5PSTmin1 = oftmin();
  EEPROM.update(Eepromsch5hr1, sch5PSThr1);
  EEPROM.update(Eepromsch5min1, sch5PSTmin1);

}
void EepromReadsch5()
{
  sch5PSThr = EEPROM.read(Eepromsch5hr);
  sch5PSTmin = EEPROM.read(Eepromsch5min);
  sch5PSThr1 = EEPROM.read(Eepromsch5hr1);
  sch5PSTmin1 = EEPROM.read(Eepromsch5min1);
}
////////////////////////////////////////////////////////////////
void sch6savetime() {
  sch6PSThr = othr();
  sch6PSTmin = otmin();
  EEPROM.update(Eepromsch6hr, sch6PSThr);
  EEPROM.update(Eepromsch6min, sch6PSTmin);
}
void sch6savedue() {
  sch6PSThr1 = ofthr();
  sch6PSTmin1 = oftmin();
  EEPROM.update(Eepromsch6hr1, sch6PSThr1);
  EEPROM.update(Eepromsch6min1, sch6PSTmin1);

}
void EepromReadsch6()
{
  sch6PSThr = EEPROM.read(Eepromsch6hr);
  sch6PSTmin = EEPROM.read(Eepromsch6min);
  sch6PSThr1 = EEPROM.read(Eepromsch6hr1);
  sch6PSTmin1 = EEPROM.read(Eepromsch6min1);
}
//////////////////////////////////////////////////////////////
void sch7savetime() {
  sch7PSThr = othr();
  sch7PSTmin = otmin();
  EEPROM.update(Eepromsch7hr, sch7PSThr);
  EEPROM.update(Eepromsch7min, sch7PSTmin);
}
void sch7savedue() {
  sch7PSThr1 = ofthr();
  sch7PSTmin1 = oftmin();
  EEPROM.update(Eepromsch7hr1, sch7PSThr1);
  EEPROM.update(Eepromsch7min1, sch7PSTmin1);

}
void EepromReadsch7()
{
  sch7PSThr = EEPROM.read(Eepromsch7hr);
  sch7PSTmin = EEPROM.read(Eepromsch7min);
  sch7PSThr1 = EEPROM.read(Eepromsch7hr1);
  sch7PSTmin1 = EEPROM.read(Eepromsch7min1);
}
////////////////////////////////////////////////////////////////
void sch8savetime() {
  sch8PSThr = othr();
  sch8PSTmin = otmin();
  EEPROM.update(Eepromsch8hr, sch8PSThr);
  EEPROM.update(Eepromsch8min, sch8PSTmin);
}
void sch8savedue() {
  sch8PSThr1 = ofthr();
  sch8PSTmin1 = oftmin();
  EEPROM.update(Eepromsch8hr1, sch8PSThr1);
  EEPROM.update(Eepromsch8min1, sch8PSTmin1);

}
void EepromReadsch8()
{
  sch8PSThr = EEPROM.read(Eepromsch8hr);
  sch8PSTmin = EEPROM.read(Eepromsch8min);
  sch8PSThr1 = EEPROM.read(Eepromsch8hr1);
  sch8PSTmin1 = EEPROM.read(Eepromsch8min1);
}
void sch9savetime() {
  sch9PSThr = othr();
  sch9PSTmin = otmin();
  EEPROM.update(Eepromsch9hr, sch9PSThr);
  EEPROM.update(Eepromsch9min, sch9PSTmin);
}
void sch9savedue() {
  sch9PSThr1 = ofthr();
  sch9PSTmin1 = oftmin();
  EEPROM.update(Eepromsch9hr1, sch9PSThr1);
  EEPROM.update(Eepromsch9min1, sch9PSTmin1);

}
void EepromReadsch9()
{
  sch9PSThr = EEPROM.read(Eepromsch9hr);
  sch9PSTmin = EEPROM.read(Eepromsch9min);
  sch9PSThr1 = EEPROM.read(Eepromsch9hr1);
  sch9PSTmin1 = EEPROM.read(Eepromsch9min1);
}

/************************************** SERIALCOM **************************/

void SerialCom() {
  String data_from_display = "";
  //       String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  //    Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());
  /**************** AUTO MODE ******************/
  //////////////////////////////////PUMP A //////////////////////////////////////////
  if ((data_from_display.substring(0, 5) == "saoz1")) {

    Z1PAsavetime();
    Z1PAsavedue();

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
  ///////////////////////////////////////////
  if ((data_from_display.substring(0, 6) == "slado")) {

    Z1PAsavetimeq();
    Z1PAsavedueq();

    Serial.print("othr: ");
    Serial.println(Z1PASThrq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminq);
    feedBack = "saved";


  }
  /////////////////////////////////
  if ((data_from_display.substring(0, 5) == "sladl")) {

    Z1PAsavetimeqq();
    Z1PAsavedueqq();

    Serial.print("othr: ");
    Serial.println(Z1PASThrqq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminqq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrqq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminqq);
    feedBack = "saved";


  }
  //////////////////////////////////////LIGHT A///////////////////////////////

  if ((data_from_display.substring(0, 5) == "slaz1")) {

    Z1PBsavetime();
    Z1PBsavedue();
    Serial.println(Z1PBSThr);

    Serial.println(Z1PBSTmin);
    Serial.println(Z1PBSPhr);

    Serial.println(Z1PBSPmin);

    feedBack = "saved";
  }

  /////////////////////////////////PUMPDO////////////////////////////////

  if ((data_from_display.substring(0, 5) == "sdoz1")) {

    Z1APsavetime();
    Z1APsavedue();
    Serial.println(Z1APSThr);

    Serial.println(Z1APSTmin);

    Serial.println(Z1APSPhr);

    Serial.println(Z1APSPmin);
    feedBack = "saved";
  }


  ////////////////////////IRRIGATION B///////////////////////////
  if ((data_from_display.substring(0, 5) == "slbz1")) {
    LBPAsavedue();
    LBPAsavetime();
    Serial.println("SAVE IRR B Time");

    Serial.println(LBPASThr);
    Serial.println(LBPASTmin);

    Serial.println(LBPASPhr);

    Serial.println(LBPASPmin);
    feedBack = "saved";
  }

  if ((data_from_display.substring(0, 4) == "A s1")) {
    sch1savetime();
    sch1savedue();
    checkflag = true;
    Serial.println("slot 1 schedule time");

    Serial.println(sch1PSThr);
    Serial.println(sch1PSTmin);

    Serial.println(sch1PSThr1);

    Serial.println(sch1PSTmin1);
    feedBack = "saved";
  }

  if ((data_from_display.substring(0, 5) == "az1s2"))
  {
    sch2savetime();
    sch2savedue();
    checkflag1 = true;
    Serial.println("slot 2 schedule time");

    Serial.println(sch2PSThr);

    Serial.println(sch2PSTmin);

    Serial.println(sch2PSThr1);

    Serial.println(sch2PSTmin1);
    feedBack = "saved";
  }


  if ((data_from_display.substring(0, 5) == "az1s3"))
  {
    sch3savetime();
    sch3savedue();
    checkflag2 = true;
    Serial.println("slot 3 schedule time");

    Serial.println(sch3PSThr);

    Serial.println(sch3PSTmin);

    Serial.println(sch3PSThr1);

    Serial.println(sch3PSTmin1);
    feedBack = "saved";
  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  if ((data_from_display.substring(0, 5) == "lbss1"))
  {
    sch4savetime();
    sch4savedue();
    checkflag3 = true;
    Serial.println("slot 4 schedule time");

    Serial.println(sch4PSThr);

    Serial.println(sch4PSTmin);

    Serial.println(sch4PSThr1);

    Serial.println(sch4PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss2"))
  {
    sch5savetime();
    sch5savedue();
    checkflag4 = true;
    Serial.println("slot 5 schedule time");

    Serial.println(sch5PSThr);

    Serial.println(sch5PSTmin);

    Serial.println(sch5PSThr1);

    Serial.println(sch5PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss3"))
  {
    sch6savetime();
    sch6savedue();
    checkflag5 = true;
    Serial.println("slot 6 schedule time");

    Serial.println(sch6PSThr);

    Serial.println(sch6PSTmin);

    Serial.println(sch6PSThr1);

    Serial.println(sch6PSTmin1);
    feedBack = "saved";
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if ((data_from_display.substring(0, 5) == "lass1"))
  {
    sch7savetime();
    sch7savedue();
    checkflag6 = true;
    Serial.println("slot 7 schedule time");

    Serial.println(sch7PSThr);

    Serial.println(sch7PSTmin);

    Serial.println(sch7PSThr1);

    Serial.println(sch7PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass2"))
  {
    sch8savetime();
    sch8savedue();
    checkflag7 = true;
    Serial.println("slot 8 schedule time");

    Serial.println(sch8PSThr);

    Serial.println(sch8PSTmin);

    Serial.println(sch8PSThr1);

    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass3"))
  {
    sch8savetime();
    sch8savedue();
    checkflag8 = true;
    Serial.println("slot 9 schedule time");

    Serial.println(sch8PSThr);

    Serial.println(sch8PSTmin);

    Serial.println(sch8PSThr1);

    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  ///************************************** WIFI - CREDENTIALS
  if ((dfd.substring(0, 4) == "sdpd")) {
    ssid = pwdname();
    Serial.print("User Name: ");
    Serial.println(str);
    str = ssid;
    password = pwd();
    Serial.print("Password: ");
    Serial.println(dfd);
    dfd = password;
    feedBack = "sdpd";
  }


  ///**************************    MESH
  if ((dfd.substring(0, 4) == "mesh")) {
    meshpass();
    prefix();
    port();
    feedBack = "mesh";
  }
  /**************************    NEXTION - RTC   ********************/
  if ((data_from_display.substring(0, 3) == "clk")) {
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();
    feedBack = "clk";
    dateandtime();
    readalldata();
    setTimeNex();
  }
  if ((data_from_display.substring(0, 6) == "settc")) {
    runTimeNex();
  }
  sendData(data_from_display);
}

void runTimeNex() {

  rtc.begin();

}
void setTimeNex() {
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  runTimeNex();
}
//////*************IRRIGATION MANUAL*****************//////////
void sendData(String dfd) {
  if (dfd == "pAz1On")
  {
    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, LOW);
    feedBack = "pAz1On";
    ZAApumpSt = "on";
    Serial.println("pump IRR is on");
  }
  if (dfd == "pAz1Off")
  {
    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, HIGH);
    feedBack = "pAz1Off";
    Serial.println("pump  IRR is off");
    ZAApumpSt = "off";
  }

  if (dfd == "pdoaon")
  {
    pinMode(DOOA, OUTPUT);
    digitalWrite(DOOA, LOW);
    Serial.println("pump DOA is on");
  }
  if (dfd == "pdoaoff")
  {
    pinMode(DOOA, OUTPUT);
    digitalWrite(DOOA, HIGH);
    Serial.println("pump  doa is off");
  }
  if (dfd == "pdbon")
  {
    pinMode(DOOB, OUTPUT);
    digitalWrite(DOOB, LOW);
    Serial.println("pump DOB is on");
  }
  if (dfd == "pdboff")
  {
    pinMode(DOOB, OUTPUT);
    digitalWrite(DOOB, HIGH);
    Serial.println("pump DOB is off");
  }
  if (dfd == "pBz1On")
  {
    pinMode(Z1AIRPUMP, OUTPUT);
    digitalWrite(Z1AIRPUMP, LOW);
    Serial.println("Pump DO is on");
    feedBack = "pBz1On";
    ZBApumpSt = "on";
  }
  if (dfd == "pBz1Off")
  {
    pinMode(Z1AIRPUMP, OUTPUT);
    digitalWrite(Z1AIRPUMP, HIGH);

    Serial.println("Pump DO off");
    feedBack = "pBz1Off";
    ZBApumpSt = "off";
  }

  if (dfd == "laon")
  {
    pinMode(Z1PUMPB, OUTPUT);
    digitalWrite(Z1PUMPB, LOW);
    Serial.println("LIGHT A IS ON");
    feedBack = "laon";
    ZAAirSPumpSt = "on";
  }
  if (dfd == "laoff")
  {
    pinMode(Z1PUMPB, OUTPUT);
    digitalWrite(Z1PUMPB , HIGH);
    Serial.println("LIGHT A IS Off");
    feedBack = "laoff";
    ZAAirSPumpSt = "off";
  }

  if (dfd == "lbon")
  {
    pinMode(Z1SOLVALVE, OUTPUT);
    digitalWrite(Z1SOLVALVE, LOW);
    Serial.println("LIGHT B IS ON");
    feedBack = "lbon";
    ZAROSt = "on";
  }
  if (dfd == "lboff")
  {
    pinMode(Z1SOLVALVE, OUTPUT);
    digitalWrite(Z1SOLVALVE, HIGH);
    Serial.println("LIGHT B IS OFF");
    feedBack = "lboff";
    ZAROSt = "off";
  }
#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000

  if (dfd == "z1aon")
  {

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

    long Z1PASThrconvoff = Z1PASPhr * ms_per_hour;
    long Z1PASTminconvoff = Z1PASPmin * ms_per_min;
    long PumpAOnTimetoteloff = Z1PASThrconvoff + Z1PASTminconvoff;

    PumpAZ1.begin(PumpAOnTimetotelOn , PumpAOnTimetoteloff );

    Serial.println("PUMPA Z1 On");
    PumpAZ1bool = true;
    EepromReadZ1AP();
    feedBack = "z1aon";
  }
  if (dfd == "z1aoff")
  {
    irroff();
    PumpAZ1bool = false;
    Serial.println("PUMP Z1  Off");
    feedBack = "z1aoff";
  }

  if (dfd == "z1aonq")
  {


    Serial.print("othr: ");
    Serial.println(Z1PASThrq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminq);

    long Z1PASThrconvOnq = Z1PASThrq * ms_per_hour;
    long Z1PASTminconvOnq = Z1PASTminq * ms_per_min;
    long PumpAOnTimetotelOnq = Z1PASThrconvOnq + Z1PASTminconvOnq;

    long Z1PASThrconvoffq = Z1PASPhrq * ms_per_hour;
    long Z1PASTminconvoffq = Z1PASPminq * ms_per_min;
    long PumpAOnTimetoteloffq = Z1PASThrconvoffq + Z1PASTminconvoffq;

    PumpAZ1q.begin(PumpAOnTimetotelOnq , PumpAOnTimetoteloffq );

    Serial.println("PUMP do On");
    PumpAZ1boolq = true;
    EepromReadZ1APq();
    feedBack = "z1aon";
  }
  if (dfd == "z1aoffq")
  {

    PumpAZ1boolq = false;
    Serial.println("PUMP do Off");
    feedBack = "z1aoff";
  }


  if (dfd == "z1aonqq")
  {

    Serial.print("othr: ");
    Serial.println(Z1PASThrqq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminqq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrqq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminqq);
    long Z1PASThrconvOnqq = Z1PASThrqq * ms_per_hour;
    long Z1PASTminconvOnqq = Z1PASTminqq * ms_per_min;
    long PumpAOnTimetotelOnqq = Z1PASThrconvOnqq + Z1PASTminconvOnqq;


    long Z1PASThrconvoffqq = Z1PASPhrqq * ms_per_hour;
    long Z1PASTminconvoffqq = Z1PASPminqq * ms_per_min;
    long PumpAOnTimetoteloffqq = Z1PASThrconvoffqq + Z1PASTminconvoffqq;


    PumpAZ1qq.begin(PumpAOnTimetotelOnqq , PumpAOnTimetoteloffqq );

    Serial.println("PUMPA do2 On");
    PumpAZ1boolqq = true;
    EepromReadZ1APqq();

  }
  if (dfd == "z1aoff")
  {

    PumpAZ1boolqq = false;
    Serial.println("PUMP do2  Off");
    feedBack = "z1aoff";
  }


  //*************************************************LIGHT A **********************************
  if (dfd == "alaon") {
    Serial.println("LIGHT A");

    Serial.println(Z1PBSThr);

    Serial.println(Z1PBSTmin);

    Serial.println(Z1PBSPhr);

    Serial.println(Z1PBSPmin);
    long Z1PBSThrconvOn = Z1PBSThr * ms_per_min;
    long Z1PBSTminconvOn = Z1PBSTmin * ms_per_sec;
    long PumpBOnTimetotelOn = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1PBSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1PBSPmin * ms_per_min;
    long PumpBOnTimetoteloff = Z1PBSThrconvoff + Z1PBSTminconvoff;

    PumpBZ1.begin(PumpBOnTimetotelOn , PumpBOnTimetoteloff);
    PumpBZ1bool = true;
    Serial.println("LIGHT A ON");
    EepromReadZ1PB();

    feedBack = "alaon";
  }
  if (dfd == "alaoff") {
    dripoff();
    PumpBZ1bool = false;
    Serial.println("LIGHT A OFF ");
    feedBack = "alaoff";
  }
  //////////////////////////////////////// PUMP DO////////////////////////////////////////////
  if (dfd == "z1don") {

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

    EepromReadZ1Air();

    feedBack = "z1don";
  }
  if (dfd == "z1doff") {
    dooff();
    PumpDOZ1bool = false;

    Serial.println("PUMP DO Off");
    feedBack = "z1doff";
  }

  ///////////////////////////////////////LIGHT B////////////////////////////////////
  if (dfd == "albon") {

    Serial.println(LBPASThr);                  //Shift1Clockhr();

    Serial.println(LBPASTmin);                //Shift1Clockmin();

    Serial.println(LBPASPhr);                  //Shift1Clockhr();

    Serial.println(LBPASPmin);
    long z1LBSThrconvOn = LBPASThr * ms_per_hour;
    long z1LBSTminconvOn = LBPASTmin * ms_per_min;
    PumpLBOnTime = z1LBSThrconvOn + z1LBSTminconvOn;
    Serial.println("LBPASPmin");
    Serial.println(LBPASPmin);
    long z1LBSThrconvoff = LBPASPhr * ms_per_hour;

    long z1LBSTminconvoff = LBPASPmin * ms_per_min;
    PumpLBOffTime = z1LBSThrconvoff + z1LBSTminconvoff;
    Serial.println("PumpLBOnTime");                  //Shift1Clockhr();

    Serial.println(PumpLBOnTime);                //Shift1Clockmin();

    Serial.println("PumpLBOffTime");                  //Shift1Clockhr();

    Serial.println(PumpLBOffTime);

    PumpLB.begin(PumpLBOnTime , PumpLBOffTime);
    PumpLBbool = true;
    Serial.println("LIGHT B ON");


    feedBack = "albon";
  }

  if (dfd == "alboff")
  {
    irrboff();
    PumpLBbool = false;

    Serial.println("LIGHT B Off");
    feedBack = "alboff";
  }
}
/**************************   WIFI  ***********************/
String pwd() {
  int vale = dfd.indexOf("<") + 1;
  dfd.remove(0, vale);
  return (dfd);
}
String  pwdname() {
  char *pd = strtok(buff, "<");
  str = pd;
  str.remove(0, 4);
  str = str;
  return (str);
}
/**************************    MESH     ***********************/
void port() {
  int vale = dfd.indexOf(">") + 1;
  dfd.remove(0, vale);//port
  Serial.print("Port: ");
  Serial.println(dfd);
}
void meshpass() { //meshkji<hhyf>5555
  char *pd = strtok(buff, ">");
  str = pd;
  int vale = str.indexOf("<") + 1;
  str.remove(0, vale);//meshpass
  String mstr = str;
  Serial.print("Mesh Pswd: ");
  Serial.println(mstr);
}

void prefix() {
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
  Serial.print("Prefix: ");
  Serial.println(str1);
}


void irroff()
{
  Serial.println("nothing to do");

  pinMode(Z1PUMPA, OUTPUT);
  digitalWrite(Z1PUMPA, HIGH);
}
void dooff()
{
  pinMode(Z1PUMPB, OUTPUT);
  digitalWrite(Z1PUMPB, HIGH);
}
void dripoff()
{
  pinMode(Z1AIRPUMP, OUTPUT);
  digitalWrite(Z1AIRPUMP, HIGH);

}
void irrboff()
{
  pinMode(Z1SOLVALVE, OUTPUT);
  digitalWrite(Z1SOLVALVE, HIGH);
}

void callSerialCom() {
  if (Serial2.available()) {
    SerialCom();
  }
}
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);
  Serial.println("Axalyn...");
  rtc.begin();
  rtc.adjust(DateTime(2022, 9, 17, 11, 15, 0));
  pinMode(Z1PUMPA, OUTPUT);
  digitalWrite(Z1PUMPA, HIGH);
  pinMode(Z1PUMPB, OUTPUT);
  digitalWrite(Z1PUMPB, HIGH);
  pinMode(Z1AIRPUMP, OUTPUT);
  digitalWrite(Z1AIRPUMP, HIGH);
  pinMode(Z1SOLVALVE, OUTPUT);
  digitalWrite(Z1SOLVALVE, HIGH);
  pinMode(DOOA, OUTPUT);
  digitalWrite(DOOA, HIGH);
  pinMode(DOOB, OUTPUT);
  digitalWrite(DOOB, HIGH);

}
unsigned long SCpreviousMillis = 0;
unsigned long sch1previousMillis = 0;
unsigned long EVNTpreviousMillis = 0;
unsigned long manualsend = 0;
void loop()
{ ////////////************** Serialcom loop Mode***********///////////
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = currentMillis;

    callSerialCom();
  }
  ////////////**************Schedule loop Mode***********///////////
  if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {
    unsigned long sch1currentMillis = millis();

    if (sch1currentMillis - sch1previousMillis >= 10000) {
      sch1previousMillis = sch1currentMillis;

      scheduleFun();
    }
  }
  //////////************** Irrevent loop Mode***********///////////
  if (((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true)) || ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true)))
  {

    unsigned long EVNTcurrentMillis = millis();

    if (EVNTcurrentMillis - EVNTpreviousMillis >= 5000) {
      EVNTpreviousMillis = EVNTcurrentMillis;
      funtimer2();
    }
  }
  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  if (manualsendcur - manualsend >= 4000) {
    manualsend = manualsendcur;
    manualread();
    counterun();
  }
  //////////////////////////
  if (PumpAZ1bool == true)
  {
    PumpAZ1.Update();

  }

  if (PumpAZ1boolq == true)
  {
    PumpAZ1q.Update();

  }

  if (PumpAZ1boolqq == true)
  {
    PumpAZ1qq.Update();

  }

  if (PumpBZ1bool == true)
  {
    PumpBZ1.Update();
  }

  if (PumpDOZ1bool == true)
  {
    PumpDOZ1.Update();

  }
  if (PumpLBbool == true)
  {
    PumpLB.Update();

  }
}
void scheduleFun() {


  if (checkflag == true)
  {
    Serial.println(sch1PSThr);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin);                //Shift1Clockmin();

    Serial.println(sch1PSThr1);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin1);

    Serial.println(hourupg);
    Serial.println(minupg);


    int S1Hr = sch1PSThr;
    int S1Min = sch1PSTmin;
    int S2Hr = sch1PSThr1;
    int S2Min = sch1PSTmin1;



    dateandtime();


    if ((hourupg == S1Hr)  &&  (minupg  ==  S1Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((hourupg == S2Hr)  &&  (minupg  ==  S2Min))
    {

      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);

    }
  }
  if (checkflag1 == true)
  {




    int S3Hr = sch2PSThr;
    int S3Min = sch2PSTmin;
    int S4Hr = sch2PSThr1;
    int S4Min = sch2PSTmin1;


    dateandtime();

    if ((hourupg == S3Hr)  &&  (minupg  ==  S3Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((hourupg == S4Hr)  &&  (minupg  ==  S4Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);
    }


  }


  if (checkflag2 == true)
  {



    int S5Hr = sch3PSThr;
    int S5Min = sch3PSTmin;
    int S6Hr = sch3PSThr1;
    int S6Min = sch3PSTmin1;


    dateandtime();

    if ((hourupg == S5Hr)  &&  (minupg  ==  S5Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((hourupg == S6Hr)  &&  (minupg  ==  S6Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);
    }


  }


  if (checkflag3 == true)
  {



    int S7Hr = sch4PSThr;
    int S7Min = sch4PSTmin;
    int S8Hr = sch4PSThr1;
    int S8Min = sch4PSTmin1;


    dateandtime();

    if ((hourupg == S7Hr)  &&  (minupg  ==  S7Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((hourupg == S8Hr)  &&  (minupg  ==  S8Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }

  if (checkflag4 == true)
  {



    int S9Hr = sch5PSThr;
    int S9Min = sch5PSTmin;
    int S10Hr = sch5PSThr1;
    int S10Min = sch5PSTmin1;


    dateandtime();

    if ((hourupg == S9Hr)  &&  (minupg  ==  S9Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((hourupg == S10Hr)  &&  (minupg  ==  S10Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }

  if (checkflag5 == true)
  {



    int S11Hr = sch6PSThr;
    int S11Min = sch6PSTmin;
    int S12Hr = sch6PSThr1;
    int S12Min = sch6PSTmin1;


    dateandtime();

    if ((hourupg == S11Hr)  &&  (minupg  ==  S11Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((hourupg == S12Hr)  &&  (minupg  ==  S12Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }
  if (checkflag6 == true)
  {



    int S13Hr = sch7PSThr;
    int S13Min = sch7PSTmin;
    int S14Hr = sch7PSThr1;
    int S14Min = sch7PSTmin1;


    dateandtime();

    if ((hourupg == S13Hr)  &&  (minupg  ==  S13Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((hourupg == S14Hr)  &&  (minupg  ==  S14Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }


  }
  if (checkflag7 == true)
  {



    int S15Hr = sch8PSThr;
    int S15Min = sch8PSTmin;
    int S16Hr = sch8PSThr1;
    int S16Min = sch8PSTmin1;


    dateandtime();

    if ((hourupg == S15Hr)  &&  (minupg  ==  S15Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((hourupg == S16Hr)  &&  (minupg  ==  S16Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }


  }
  if (checkflag8 == true)
  {



    int S17Hr = sch9PSThr;
    int S17Min = sch9PSTmin;
    int S18Hr = sch9PSThr1;
    int S18Min = sch9PSTmin1;


    dateandtime();

    if ((hourupg == S17Hr)  &&  (minupg  ==  S17Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((hourupg == S18Hr)  &&  (minupg  ==  S18Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }
  }
}
void manualread()
{
  //Z1PUMPA    36 //Z1PUMPA
  //Z1PUMPB    38 //Z1PUMPB
  //Z1AIRPUMP  23 //Z2PUMPA
  //Z1SOLVALVE 25 //Z2PUMPB
  //DOOA       32 // Z1DO
  //DOOB       27 // Z2DO

  irz1a = digitalRead(Z1PUMPA);
  irz1b = digitalRead(Z1PUMPB);
  irz2a = digitalRead(Z1AIRPUMP);
  irz2b = digitalRead(Z1SOLVALVE);
  doa = digitalRead(DOOA);
  dob = digitalRead(DOOB);

  storeprvstate = "";
  storeprvstate =   irz1a +  irz1b +  irz2a +  irz2b + doa + dob;

  if (storeprvstate != checkstate)
  {

    checkstate = storeprvstate;
    counterm = 5;
    Serial.println(checkstate);

  }
}

void counterun()
{

  if (counterm > 0)
  {
    runmanual();
    counterm --;
  }

}
void runmanual()
{
  StaticJsonDocument <500> Manualdoc1;
  JsonObject Manualstatus = Manualdoc1.createNestedObject("Manualstatus");

  Manualstatus["irz1a"] = irz1a;
  Manualstatus["irz1b"] = irz1b;
  Manualstatus["irz2a"] = irz2a;
  Manualstatus["irz2b"] = irz2b;
  Manualstatus["doa"] = doa;
  Manualstatus["dob"] = dob;
  serializeJson(Manualdoc1, Serial);
  serializeJson(Manualdoc1, Serial3);
  Serial.println(".....");

}
void eventmode2()
{
  StaticJsonDocument <500> doc1;
  if (((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true)) && ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true)))
  {
    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "21";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
  }
  else if ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "5";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }

  else if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "6";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }
}

void funtimer2()
{


  eventmode2();

}
