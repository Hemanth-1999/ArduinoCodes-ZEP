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

//****************************************************RELAYS************************************
#define Z1PUMPA  36
#define Z1PUMPB  38

#define Z1AIRPUMP  23
#define Z1SOLVALVE 25

#define DOOA  32
#define DOOB  27
//******************************RTC MODULE**************************************************
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int Rtc_hour;
int Rtc_min;
int Rtc_sec;
int Rtc_day;
int Rtc_month;
int Rtc_year;
void update_rtc_time();//THIS FUNCTION IS USED TO SET TIME AND DATE TO RTC MODULE MANUALLY.
void read_date_time_rtc();//THIS FUNCTION WILL READ CURRENT DATE AND TIME FROM RTC MODULE.

//*******************************************************MANUAL MODE CREDENTIALS************************************
int irz1a;
int irz1b;
int irz2a;
int irz2b;
int doa;
int dob;
int counterm;
void manualread();
void counterun();
void runmanual();

//********************************************************DISPLAY DATA************************************************

String dfd="";
char buff[200];//THIS IS USED TO STORE INCOMING DATA FROM DISPLAY.
String feedBack;
bool checkflag;
bool checkflag1;
bool checkflag2;
bool checkflag3;
bool checkflag4;
bool checkflag5;
bool checkflag6;
bool checkflag7;
bool checkflag8;
bool checkflag9;
void SerialCom();

  //*****************************************************************AUTO MODE*********************************************
  bool saveZ1PASThr;
  uint8_t Z1PASThr;//STORES ON TIME IN HOURS
  uint8_t Z1PASTmin;//STORES ON TIME IN MINUTES
  uint8_t Z1PASPhr;//STORES OFF TIME IN HOURS
  uint8_t Z1PASPmin;//STORES OFF TIME IN MINUTES

  uint8_t Z1PASThrq;
  uint8_t Z1PASTminq;
  uint8_t Z1PASPhrq;
  uint8_t Z1PASPminq;

  uint8_t Z1PASThrqq;
  uint8_t Z1PASTminqq;
  uint8_t Z1PASPhrqq;
  uint8_t Z1PASPminqq;

  uint8_t Z1PBSThr;
  uint8_t Z1PBSTmin;
  uint8_t Z1PBSPhr;
  uint8_t Z1PBSPmin;

  uint8_t Z1APSThr;
  uint8_t Z1APSTmin;
  uint8_t Z1APSPhr;
  uint8_t Z1APSPmin;

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
  //**************************************************************DISPLAY WIFI CREDENTIALS*************************************
  String ssid;//STORES SSID 
  String password;//STORES PASSWORD
  String  pwdname();//THIS WILL RETURN SSID
  String pwd();//THIS WILL RETURN PASSWORD
  //************************************************************READING DATE AND TIME FROM DISPLAY***********************
  void timeSec();
  void timeMin();
  void timeHr();
  void timeDt();
  void timeMnt();
  void timeYr();
  uint16_t hourrc;
  uint16_t minrc;
  uint16_t secsrc;
  uint16_t Monthrc;
  uint16_t Daterc;
  int Yearrc;
  //****************************************************************DISPLAY MANUAL MODE******************************
  bool ZAApumpSt = true;
  bool ZBApumpSt = true;
  bool ZAAirSPumpSt = false;
  bool ZAROSt = false;
  #define ms_per_hour  3600000
  #define ms_per_min    60000
  #define ms_per_sec    1000
  bool status1;
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
  void irroff();
//***************************************************************EEPROME CREDENTIALS****************************
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
//***********************************************************************MILLIS CREDENTIALS**************************
unsigned long SCpreviousMillis = 0;
unsigned long sch1previousMillis = 0;
unsigned long EVNTpreviousMillis = 0;
unsigned long ESPpreviousMillis = 0;
unsigned long manualsend = 0;

//*******************************************EEPROME**********************************
void Z1PAsavetimeq();
