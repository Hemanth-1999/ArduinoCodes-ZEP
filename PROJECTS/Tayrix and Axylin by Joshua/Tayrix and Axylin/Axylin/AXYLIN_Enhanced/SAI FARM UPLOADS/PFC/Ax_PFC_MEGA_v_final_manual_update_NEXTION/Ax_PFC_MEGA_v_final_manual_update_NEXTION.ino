

//=============================================
// Changelog
//=============================================
/*

   v2.0.4.1
   LAST UPDATED
        Date-02/11/2021
        1.Adding VPD Mode Steblity
        2.Code Compact with- Ax_PFC_ESPMaster_v2.0
        3.Nextion Code - ???
        4. ADDING ALTERNATE FAN FUNCTION IN AUTO TEMP AND AUTO VPD(15 SEP 2022)
        5. ADDED SCHEDULE MODE FOR IRRA PUMP IRR B PUMP  AND DRIP IRR PUMP(17 SEP 2022)
        6. SAVING VALUES TO EEPROM TO RESTORE THE VALUES AFTER A POWER CUT
        DateStarted -14/10/2022
        7. ADDING ALERTS AND EVENTS
         7.1 SENDING TO NEXTION
         7.2 CHANGING COLOUR RANGE WISE
        8. SENDING EVENTS AND ALERTS STATUS TO ESP MASTER
        9. SENDING EVNETS AND ALERTS TO CLOUD
        10. RECEVING from Cloud to Esp
        11. Sending to Mega from Esp
        12. Changing the Relay Status(on/off)
        13. To Control ON and OFF button through Mega code

   Pending
        1.Code Opt.



*/
#define VERSION "2.0.3"

//===========================================
// Includes
//===========================================
#include <Arduino.h>
#include <MemoryFree.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include "millisDelay.h"
#include <Wire.h>
// Sensor Input


//===========================================
// Defines
//===========================================
RunningAverage RAT(20);
String data_from_display = "";
String dfd;
String str;
char buff[2000];
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftHr;
String aba;
float SPtemp;
extern volatile unsigned long timer0_millis;
//float tmp;
//float hmd;
bool vpd_func;
bool vpdfun_cal;
bool temp_func;
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
float VPD_val;
int val;
int counterm;
String checkstate ;
String storeprvstate = "";
int CS_pin = 10;
//const int TnumReadings = 20;
//int Treadings[TnumReadings];
const unsigned int MAX_MESSAGE_LENGTH = 12;
const char* Relaystate;
String set_point_from_app ;
String cndchck;
double WSNODE_ES_AT ;
double WSNODE_ES_RH;
double WSNODE_ES_VPD;
double WSNODE_ES_HI ;
double WSNODE_ES_ET;
double WSNODE_ES_SR ;
double WSNODE_ES_BP ;
double WSNODE_ES_DP;
int WSNODE_ES_SL ;
double ESNODE_ES_AT ;
int ESNODE_ES_RH ;
double ESNODE_ES_VPD ;
double ESNODE_ES_HI ;
int ESNODE_ES_CO2 ;
int ESNODE_ES_ET ;
double ESNODE_ES_SR ;
double ESNODE_ES_BP ;
long ESNODE_ES_DP ;
int ESNODE_ES_SL ;
float ATemp, AHumi;
const char* Allrelays_FAN1;
float vpdlowsp = 0.8;
float vpdhighsp = 1.4;
int ok ;
int relayState;
String pwdname();
String pwd();
void meshpass();
void timeMin();
void AutoTemp_control();
void vpd_control();
void prefix();
void port();
void timeHr();
void timeSec();
void timeYr();
void timeMnt();
void timeDt();
void sendData(String );
void asd();
void loop();
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
bool auto123;
int count1 = 5;
int count3 = 10;
int count4 = 20;
millisDelay getvalue;
millisDelay rlvalue;
millisDelay manualrun;

StaticJsonDocument<500> manualdoc;

//File sd_file;

//RTC
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <EEPROMex.h>
#define DEBUG 0                                // change value to 1 to enable debuging using serial monitor  
#define SERIAL_PORT_SPEED 115200//9600, 19200, 38400, 57600, 115200

RTC_DS3231 rtc; //for date time
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int hourupg;
int minupg;
uint8_t secslive;
int Monthlive;
int Datelive;
int Yearlive;
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
int distanceincm;
int distanceinInch;
int totalVolume1;
///*******************UVA*********previousMillis**************************************
float solar_Value;
//****************************************WATER FLOW *******************************************************************************
int watsensor1 = 2;    // intterupt 0  pump 1
int watsensor2 = 3;   // interrupt 1  pump 2

float flowRate;
float flowRate2;

volatile int count;
volatile int count2;

//******************************************************************************************
String ssid;
String password;

int test = 55;

int ledCtrl = HIGH;

//Relay-1

#define ExFan1  28
#define ExFan2  29
#define ExFan3  30
#define ExFan4  31

//Relay-2
#define ExFan5 24
#define ExFan6 25
#define ExFan7 26
#define ExFan8 27
//Relay-3
#define pad1      32
#define Fogger    33
#define FoggerSol 34
#define Co2Gen    35

#define Topven  LOW;
#define  GWindow  LOW;
#define Curtains  LOW;


#define heater  LOW;
#define hp  LOW;
#define Chiller  LOW;

#define Cfan1  23;
#define Cfan2  25;
//int BLledCtrl = LOW;

#define Z1PUMPA    36
#define Z1PUMPB    38
#define Z1AIRPUMP  37 ///(DRIP-LINE)///
#define Z1SOLVALVE 39///(IRRIGATION B)///
char tempString[1024];

//******************************************Time Date Setting**************************************************************

int f1;
int f2;
int f3;
int f4;
int f5;
int f6;
int f7;
int f8;
int cp;
int fp;
int fs;
int co;
int ir;
int doo;
int dr;
int pb;

int Co21;
int Co22;
int uva;
int WD;
int WSpeed;

uint16_t hrs = 0;
uint16_t mins = 0;

String nightmode = "off";
bool VpdautoModeOn;
bool TemperatureAutoModeOn;

float NECC_AT, NECC_RH, NECC_VPD, NECC_HI;

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

uint8_t EepromLBPASThr = 100;
uint8_t EepromLBPASTmin = 102;
uint8_t EepromLBPASPhr = 104;
uint8_t EepromLBPASPmin = 106;

////////////////////////////////////// SAVING TIME FOR IRR A PUMP//////////////
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
//////////////////////////////////////// SAVING VALUE FOR IRR B/////////////////////////////////////////

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

///////////////////////////////////////////// SAVING VALUE FOR DRIP IRR////////////////////////////////////////////////
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

uint8_t Eepromsch10min1 = 920;
////Task t1(1000,  TASK_FOREVER, &SerialCom, &r);//12
//Task t0(3000,  TASK_FOREVER, &dateandtime, &r);
//
////Task t1(5000,  TASK_FOREVER, &getNutrientPH, &r);
////Task t2(2000,  TASK_FOREVER, &nexVal, &r);//9
////Task t3(3000,  TASK_FOREVER, &VpdControlMode, &r);
////Task t4(4000,  TASK_FOREVER, &AutoControlMode, &r);
////Task t10(6000,  TASK_FOREVER, &nodes_HWM, &r);
//Task t5(3500,  TASK_FOREVER, &FromEspData, &r);
//Task t12(4500,  TASK_FOREVER, &setTimeNex, &r);
//Task t13(3500,  TASK_FOREVER, &runTimeNex, &r);

//*************************************************************************************************
//!******************************************************************************
//! Name: dateandtime()                                  *
//! Description:                      *
//! Param : DATE /TIME                                      *
//! Returns: void                                     *
//! Example: TIME              *
//!******************************************************************************

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
//*****************************ZONE ONE AUTO ***********************************
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

uint8_t LBPASThr;
uint8_t LBPASTmin;
uint8_t LBPASPhr;
uint8_t LBPASPmin;
///////////////////////////////SAVING TIME FOR SCHEDULE IRR A/////////////////
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
////////////////////////////////////////SAVING TIME FOR IRR B///////////////////////////////////////////////

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
///////////////////////////////////////// SAVING TIME FOR DRIP////////////
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
/************************************* NODES **************************************/
int AT;
int RH;
float VPD;
int EV;
int BP;
int CO2;
int SR;
int DP;
int HI;
int SL;
byte EaddrCC_AT = 10;
byte EaddrCC_RH = 15;
byte EaddrCC_VPD = 20;
byte EaddrCC_HI = 25;

bool modevpdstatus;
bool modetempstatus;

//Relay Status
bool ZAApumpSt = true;
bool ZBApumpSt = true;
bool ZAAirSPumpSt = false;
bool ZAROSt = false;
bool waterlevel_sensor;
bool water_level_sensor;

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
        Serial.println("");
        currentMillis = 5000 - OffTime;
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
Switcher PumpAZ1(Z1PUMPA);///////PUMP A////////////
Switcher PumpBZ1(Z1PUMPB);///////LIGHT A///////////
Switcher PumpDOZ1(Z1AIRPUMP);///////PUMP DO////////////
Switcher PumpLB(Z1SOLVALVE);/////// LIGHT B///////////
bool PumpAZ1bool;
bool PumpBZ1bool;
bool PumpDOZ1bool;
bool PumpLBbool;

long PumpAOnTime ;
long PumpAOffTime ;

long PumpBOnTime ;////////// LIGHT A
long PumpBOffTime ;//////// LIGHT A

long PumpDOOnTime;
long PumpDOOffTime ;


long PumpLBOnTime;//////////LIGHT B
long PumpLBOffTime;/////////LIGHT B
class foggSwitcher
{
    // class member variables
    byte relayPin; // number of pin for relay
    long OnTime;
    long OffTime;
    //int relayState; // set relay state (active HIGH)
    unsigned long previousMillis; // set time since last update
  public:
    foggSwitcher(byte pin)
    {
      relayPin = pin;
      Serial.println(pin);

    }

    void begin( long off)
    {
      pinMode(relayPin, OUTPUT);
      //      OnTime = on;
      OffTime = off;
      // relayState = HIGH;
      Serial.println("bee");

      previousMillis = 0;
    }
    void Update()
    { Serial.println("Fogging ON");

      // check the time to see if relays need to be turned on
      // or off
      unsigned long currentMillis = millis();

      if ((relayState == HIGH) && (currentMillis - previousMillis >= 20000))
      {
        relayState = LOW; // Turn it off
        Serial.println("Fogging off");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
        Serial.println("");

        digitalWrite(Fogger, relayState);
      }
      else
      {
        relayState = HIGH; // turn it on

        Serial.println("Fogging off");
        digitalWrite(Fogger, relayState);
      }


      //         else if ((relayState == LOW) && (currentMillis - previousMillis >= OnTime))
      //         {
      //         relayState = HIGH; // turn it on
      //         previousMillis = currentMillis;
      //          Serial.println("relay On");
      //         digitalWrite(relayPin, relayState);
      //         }

    }
};
foggSwitcher fogger(33);

void Vpd() {
  Serial.print("dummy  ");
  vpd_control(ESNODE_ES_VPD);
  modevpdstatus = true;

}
void VpdControlMode() {
  Serial.print("dummy  ");
  vpd_control(ESNODE_ES_VPD);
  modevpdstatus = true;

}
void AutoControlMode() {
  Serial.print("dummy  2");
  AutoTemp_control(ESNODE_ES_AT);
  modetempstatus = true;
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
int oftmin1() {
  char *ui = strtok(buff, "e");//on time hr val
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

//!******************************************************************************
//! Name: node();---->FromEspData()                                  *
//! Description:                      *
//! Param :  Multi Params                                    *
//! Returns: void                                     *
//! Example: TIME              *
//!******************************************************************************
void FromEspData()
{

  StaticJsonDocument<64> doc;
  deserializeJson(doc, Serial3); // Ok
  deserializeJson(doc, Serial3); // InvalidInput
  deserializeJson(doc, Serial3);

  DeserializationError error = deserializeJson(doc, Serial3);

  if (error) {

    return;
  }

  Allrelays_FAN1 = doc["Allrelays"]["FAN1"];
  if (Allrelays_FAN1 != NULL )
  {
    set_point_from_app = Allrelays_FAN1;
    cndchck;
    Serial.print("set_point_from_app:::");
    Serial.println(set_point_from_app);
    Serial.print("break::");
    Serial.println(cndchck);

    if (set_point_from_app != cndchck)
    {
      Serial.println("once");
      SerialCom(set_point_from_app);
      cndchck = set_point_from_app;

    }
  }
}

void FromEspData1()
{
  StaticJsonDocument<512> doc;
  deserializeJson(doc, Serial3); // Ok
  deserializeJson(doc, Serial3); // InvalidInput
  deserializeJson(doc, Serial3);

  DeserializationError error = deserializeJson(doc, Serial3);


  if (error) {

    return;
  }



  if (JsonObject ESNODE = doc["ESNODE"])
  {
    double   ESNODE_ES_AT1 = ESNODE["ES_AT"];
    int   ESNODE_ES_RH1 = ESNODE["ES_RH"];
    double   ESNODE_ES_VPD1 = ESNODE["ES_VPD"];
    double   ESNODE_ES_HI1 = ESNODE["ES_HI"];
    int   ESNODE_ES_CO21 = ESNODE["ES_CO2"];
    int   ESNODE_ES_ET1 = ESNODE["ES_ET"];
    double  ESNODE_ES_SR1 = ESNODE["ES_SR"];
    double   ESNODE_ES_BP1 = ESNODE["ES_BP"];
    long   ESNODE_ES_DP1 = ESNODE["ES_DP"];
    int  ESNODE_ES_SL1 = ESNODE["ES_SL"];
    ESNODE_ES_HI1 = ESNODE_ES_HI1 / 10000000;
     if (ESNODE_ES_AT1 > 0)
  {
    ESNODE_ES_AT = ESNODE_ES_AT1 ;
    ESNODE_ES_RH = ESNODE_ES_RH1 ;
    ESNODE_ES_VPD = ESNODE_ES_VPD1;
    ESNODE_ES_HI = ESNODE_ES_HI1 ;
    ESNODE_ES_CO2 = ESNODE_ES_CO21;
    ESNODE_ES_ET = ESNODE_ES_ET1;
    ESNODE_ES_SR = ESNODE_ES_SR1;
    ESNODE_ES_BP = ESNODE_ES_BP1 ;
    ESNODE_ES_DP = ESNODE_ES_DP1 ;
    ESNODE_ES_SL = ESNODE_ES_SL1 ;
  }
  }
  if (JsonObject WSNODE = doc["WSNODE"])
  {
    WSNODE_ES_AT = WSNODE["WS_AT"];
    WSNODE_ES_RH = WSNODE["WS_RH"];
    WSNODE_ES_VPD = WSNODE["WS_VPD"];
    WSNODE_ES_HI = WSNODE["WS_HI"];
    WSNODE_ES_ET = WSNODE["WS_ET"];
    WSNODE_ES_SR = WSNODE["WS_SR"];
    WSNODE_ES_BP = WSNODE["WS_BP"];
    WSNODE_ES_DP = WSNODE["WS_DP"];
    WSNODE_ES_SL = WSNODE["WS_SL"];

  }
 


}
//*************************************************************************************************
//!******************************************************************************
//! Name: wifireceived()                                  *
//! Description:                      *
//! Param :                                      *
//! Returns: void                                     *
//! Example: TIME              *
//!*****************************************************************************
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
//*************************************************************************************************
//!******************************************************************************
//! Name: ssidpwd()  - SSID AND PASSWORD                               *
//! Description:                      *
//! Param :                                      *
//! Returns: void                                     *
//! Example: TIME              *
//!*****************************************************************************
void ssidpwd() {
  //  Serial.print("Json");
  DynamicJsonDocument doc(150);
  JsonObject SdPwd = doc.createNestedObject("SdPwd");
  SdPwd["ssid"] = ssid;
  SdPwd["password"] = password;
  serializeJsonPretty(doc, Serial3);

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
/**************************************************************************************************************/



void LBPAsavetime() {
  LBPASThr = othr();
  LBPASTmin = otmin();
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
//////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////


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

void sch10savetime()
{
  sch10PSTmin1 = vpdtemp_I(); ;

  EEPROM.update(Eepromsch10min1, sch10PSTmin1);

}

void EepromReadsch10()
{
  sch10PSTmin1 = EEPROM.read(Eepromsch10min1);

}



/////////////////////////////////////////////////////////////////
/************************************** SERIALCOM **************************/

void SerialCom(String frommob)
{

  String data_from_display = "";
  data_from_display = frommob;
  //       String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  //    Serial.println(data_from_display);
  dfd = data_from_display;
  sendData(data_from_display);
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

  //////////////////////////////////////LIGHT A///////////////////////////////

  if ((data_from_display.substring(0, 5) == "slaz1")) {

    Z1PBsavetime();
    Z1PBsavedue();
    Serial.println(Z1PBSThr);                  //Shift1Clockhr();

    Serial.println(Z1PBSTmin);                //Shift1Clockmin();

    Serial.println(Z1PBSPhr);                  //Shift1Clockhr();

    Serial.println(Z1PBSPmin);

    feedBack = "saved";
  }

  /////////////////////////////////PUMPDO////////////////////////////////

  if ((data_from_display.substring(0, 5) == "sdoz1")) {

    Z1APsavetime();
    Z1APsavedue();
    Serial.println(Z1APSThr);                  //Shift1Clockhr();

    Serial.println(Z1APSTmin);                //Shift1Clockmin();

    Serial.println(Z1APSPhr);                  //Shift1Clockhr();

    Serial.println(Z1APSPmin);
    feedBack = "saved";
  }


  ////////////////////////IRRIGATION B///////////////////////////
  if ((data_from_display.substring(0, 5) == "slbz1")) {
    LBPAsavedue();
    LBPAsavetime();
    Serial.println("SAVE IRR B Time");

    Serial.println(LBPASThr);                  //Shift1Clockhr();

    Serial.println(LBPASTmin);                //Shift1Clockmin();

    Serial.println(LBPASPhr);                  //Shift1Clockhr();

    Serial.println(LBPASPmin);
    feedBack = "saved";
  }

  if ((data_from_display.substring(0, 4) == "A s1")) {
    sch1savetime();
    sch1savedue();
    checkflag = true;
    Serial.println("slot 1 schedule time");

    Serial.println(sch1PSThr);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin);                //Shift1Clockmin();

    Serial.println(sch1PSThr1);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin1);
    feedBack = "saved";
  }

  if ((data_from_display.substring(0, 5) == "az1s2"))
  {
    sch2savetime();
    sch2savedue();
    checkflag1 = true;
    Serial.println("slot 2 schedule time");

    Serial.println(sch2PSThr);                  //Shift1Clockhr();

    Serial.println(sch2PSTmin);                //Shift1Clockmin();

    Serial.println(sch2PSThr1);                  //Shift1Clockhr();

    Serial.println(sch2PSTmin1);
    feedBack = "saved";
  }


  if ((data_from_display.substring(0, 5) == "az1s3"))
  {
    sch3savetime();
    sch3savedue();
    checkflag2 = true;
    Serial.println("slot 3 schedule time");

    Serial.println(sch3PSThr);                  //Shift1Clockhr();

    Serial.println(sch3PSTmin);                //Shift1Clockmin();

    Serial.println(sch3PSThr1);                  //Shift1Clockhr();

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

    Serial.println(sch4PSThr);                  //Shift1Clockhr();

    Serial.println(sch4PSTmin);                //Shift1Clockmin();

    Serial.println(sch4PSThr1);                  //Shift1Clockhr();

    Serial.println(sch4PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss2"))
  {
    sch5savetime();
    sch5savedue();
    checkflag4 = true;
    Serial.println("slot 5 schedule time");

    Serial.println(sch5PSThr);                  //Shift1Clockhr();

    Serial.println(sch5PSTmin);                //Shift1Clockmin();

    Serial.println(sch5PSThr1);                  //Shift1Clockhr();

    Serial.println(sch5PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss3"))
  {
    sch6savetime();
    sch6savedue();
    checkflag5 = true;
    Serial.println("slot 6 schedule time");

    Serial.println(sch6PSThr);                  //Shift1Clockhr();

    Serial.println(sch6PSTmin);                //Shift1Clockmin();

    Serial.println(sch6PSThr1);                  //Shift1Clockhr();

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

    Serial.println(sch7PSThr);                  //Shift1Clockhr();

    Serial.println(sch7PSTmin);                //Shift1Clockmin();

    Serial.println(sch7PSThr1);                  //Shift1Clockhr();

    Serial.println(sch7PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass2"))
  {
    sch8savetime();
    sch8savedue();
    checkflag7 = true;
    Serial.println("slot 8 schedule time");

    Serial.println(sch8PSThr);                  //Shift1Clockhr();

    Serial.println(sch8PSTmin);                //Shift1Clockmin();

    Serial.println(sch8PSThr1);                  //Shift1Clockhr();

    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass3"))
  {
    sch8savetime();
    sch8savedue();
    checkflag8 = true;
    Serial.println("slot 9 schedule time");

    Serial.println(sch8PSThr);                  //Shift1Clockhr();

    Serial.println(sch8PSTmin);                //Shift1Clockmin();

    Serial.println(sch8PSThr1);                  //Shift1Clockhr();

    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  ///////////Nextion display code///
  if ((data_from_display.substring(0, 5) == "page1"))
  {
    if(ESNODE_ES_AT>0)
    {
    Serial2.print("at.txt=\"");
    Serial2.print(ESNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("atv.txt=\"");
    Serial2.print(ESNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("rh.txt=\"");
    Serial2.print(ESNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("rhv.txt=\"");
    Serial2.print(ESNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("vpd.txt=\"");
    Serial2.print(ESNODE_ES_VPD);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("vpdv.txt=\"");
    Serial2.print(ESNODE_ES_VPD);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    if (ESNODE_ES_ET > 0)
    {
      Serial2.print("ev.txt=\"");
      ESNODE_ES_ET = sqrt(( ESNODE_ES_ET) * (ESNODE_ES_ET));
      Serial2.print(ESNODE_ES_ET);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

    }

    Serial2.print("bp.txt=\"");
    Serial2.print(ESNODE_ES_BP);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  }
  }
  if ((data_from_display.substring(0, 5) == "page2"))
  {
    if(ESNODE_ES_SL>0)
    {
    Serial2.print("sl.txt=\"");
    Serial2.print(ESNODE_ES_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("co.txt=\"");
    Serial2.print(ESNODE_ES_CO2);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("dpp.txt=\"");
    Serial2.print(ESNODE_ES_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("sr.txt=\"");
    Serial2.print(ESNODE_ES_SR);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("hi.txt=\"");
    Serial2.print(ESNODE_ES_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
  }
  }
  if ((data_from_display.substring(0, 5) == "page3"))
  {
    Serial2.print("att.txt=\"");
    Serial2.print(WSNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rhh.txt=\"");
    Serial2.print(WSNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("bpp.txt=\"");
    Serial2.print(WSNODE_ES_BP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sll.txt=\"");
    Serial2.print(WSNODE_ES_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  }
  if ((data_from_display.substring(0, 5) == "page4"))
  {
    Serial2.print("srr.txt=\"");
    Serial2.print(WSNODE_ES_SR);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("hii.txt=\"");
    Serial2.print(WSNODE_ES_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("evv.txt=\"");
    Serial2.print(WSNODE_ES_ET);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("dppp.txt=\"");
    Serial2.print(WSNODE_ES_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
  }
  if ((data_from_display.substring(0, 5) == "page5"))
  {

  }
  ////////////////////////////////////////////////////////////////
  /************************************** WIFI - CREDENTIALS **************************/

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


  /**************************    MESH    ********************/
  if ((dfd.substring(0, 4) == "mesh")) {
    meshpass();
    prefix();
    port();
    feedBack = "mesh";
  }
  /************************************** VPD SETTINGS **************************/

  /************************************** TEMPERATURE **************************/

  if ((dfd.substring(0, 4) == "temp")) {

    Serial.println("ok");

    sch10savetime();


    float In_TempAutoSetpoint = vpdtemp_I();

    SPtemp = In_TempAutoSetpoint ;

    float Out_TempAutoSetpoint = vpdtemp_O();

    feedBack = "tempat";

  }

  //********************************************************************VPD*******************************
  //VPD Controller
  if (dfd == "vpdoff") {
    feedBack = "vpdon";
    Serial.println("VPD  ON");
    VpdautoModeOn = true;
    vpdfun_cal = true;
    vpd_func = true;
  }
  if (dfd == "vpdon") {
    asd();
    VpdautoModeOn = false;
    feedBack = "vpdoff";

    Serial.println("VPD Off");
    vpd_func = false;
  }
  //********************************************************************End VPD*******************************

  //********************************************************************AUTO TEMP MODE*******************************

  if (dfd == "tmpon") {


    feedBack = "tmpon";
    TemperatureAutoModeOn = true;
    Serial.println("TEMP  ON");
    temp_func = true;

  }
  if (dfd == "tmpoff") {


    feedBack = "tmpoff";
    TemperatureAutoModeOn = false;
    asd();
    Serial.println("TEMP  OFF");

    temp_func = false;
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
    feedBack = "clk";
    dateandtime();
    readalldata();
    setTimeNex();
    //    t12.enable();//setTimeNex
    //    t13.disable();
  }
  if ((data_from_display.substring(0, 6) == "settc")) {
    runTimeNex();
  }
  ///////////////////// climate manual //////////////////////////

}


int c = 20;
void sendData(String dfd) {

  c--;
  if (c == 0)
  {
    DynamicJsonDocument clearr(500);
    manualdoc = clearr;
    char tempstring[1024];
    serializeJson(clearr, tempstring);
    memset(tempstring, 0, 500);
    c = 20;
  }

  if (dfd == "f1on" )
  {

    feedBack = "f1on";
    pinMode(ExFan1, OUTPUT);
    digitalWrite(ExFan1, LOW);

  }
  if (dfd == "f1off")
  {
    feedBack = "f1off";


    digitalWrite(ExFan1, HIGH);
    Serial.println("");
  }

  if (dfd == "f2on")
  {

    feedBack = "f2on";

    pinMode(ExFan2, OUTPUT);
    digitalWrite(ExFan2, LOW);
    Serial.println("f2 On");
  }
  if (dfd  == "f2off")
  {

    feedBack = "f2off";


    //      relayPA = LOW;
    digitalWrite(ExFan2, HIGH);
    Serial.println("f2 Off");
  }
  if (dfd == "f3on")
  {
    feedBack = "f3on";


    pinMode(ExFan3, OUTPUT);
    digitalWrite(ExFan3, LOW);
    Serial.println("f3 On");
  }
  if (dfd == "f3off")
  {
    feedBack = "f3off";



    digitalWrite(ExFan3, HIGH);
    Serial.println("f3 Off");
  }
  if (dfd == "f4on") {
    feedBack = "f4on";

    pinMode(ExFan4, OUTPUT);
    digitalWrite(ExFan4, LOW);
    Serial.println("f4 On");
  }
  if (dfd == "f4off") {
    feedBack = "f4off";

    digitalWrite(ExFan4, HIGH);
    Serial.println("f4 Off");
  }
  if (dfd == "f5on") {
    feedBack = "f5on";

    pinMode(ExFan5, OUTPUT);
    digitalWrite(ExFan5, LOW);
    Serial.println("f5 On");
  }
  if (dfd == "f5off") {
    feedBack = "f5off";


    digitalWrite(ExFan5, HIGH);
    Serial.println("f5 Off");
  }
  if (dfd == "f6on") {
    feedBack = "f6on";


    pinMode(ExFan6, OUTPUT);
    digitalWrite(ExFan6, LOW);
    Serial.println("f6 On");
  }
  if (dfd == "f6off") {
    feedBack = "f6off";


    digitalWrite(ExFan6, HIGH);
    Serial.println("f6 Off");
  }
  if (dfd == "f7on") {
    feedBack = "f7on";


    pinMode(ExFan7, OUTPUT);
    digitalWrite(ExFan7, LOW);
    Serial.println("f7 On");
  }
  if (dfd == "f7off") {
    feedBack = "f7off";


    digitalWrite(ExFan7, HIGH);
    Serial.println("f7 Off");
  }
  if (dfd == "f8on") {
    feedBack = "f8on";


    pinMode(ExFan8, OUTPUT);
    digitalWrite(ExFan8, LOW);
    Serial.println("f8 On");
  }
  if (dfd == "f8off") {
    feedBack = "f8off";

    digitalWrite(ExFan8, HIGH);
    Serial.println("f8 Off");
  }
  if (dfd == "cpon") {
    feedBack = "cpon";

    pinMode(pad1, OUTPUT);
    digitalWrite(pad1, LOW);
    Serial.println("cp On");
  }
  if (dfd == "cpoff") {
    feedBack = "cpoff";


    digitalWrite(pad1, HIGH);
    Serial.println("cp Off");
  }
  if (dfd == "fpon") {
    feedBack = "fpon";



    pinMode(Fogger, OUTPUT);
    digitalWrite(Fogger, LOW);
    Serial.println("fp On");
  }
  if (dfd == "fpoff") {
    feedBack = "fpoff";


    digitalWrite(Fogger, HIGH);
    Serial.println("fp Off");
  }
  if (dfd == "fcon") {
    feedBack = "fcon";


    pinMode(FoggerSol, OUTPUT);
    digitalWrite(FoggerSol, LOW);
    Serial.println("fc On");
  }
  if (dfd == "fcoff") {
    feedBack = "fcoff";


    digitalWrite(FoggerSol, HIGH);
    Serial.println("fc Off");
  }
  if (dfd == "co2on") {
    feedBack = "co2on";


    pinMode(Co2Gen, OUTPUT);
    digitalWrite(Co2Gen, LOW);
    Serial.println("co2 On");
  }
  if (dfd == "co2off") {
    feedBack = "co2off";


    digitalWrite(Co2Gen, HIGH);
    Serial.println("co2 Off");
  }

  //  //////////////////////// MANUAL MODE IRR PUMP //////////////////////////
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

  //*********************************************** PUMP A **********************************************************************

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


  //*************************************************LIGHT A **********************************
  if (dfd == "alaon") {
    Serial.println("LIGHT A");

    Serial.println(Z1PBSThr);                  //Shift1Clockhr();

    Serial.println(Z1PBSTmin);                //Shift1Clockmin();

    Serial.println(Z1PBSPhr);                  //Shift1Clockhr();

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
  if (dfd == "alboff") {
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

float vpdtemp() {
  Serial.println(buff);
  char *ui = strtok(buff, "h");//vpdt112h185
  str = ui;
  int vale = str.indexOf("t") + 2;
  str.remove(0, vale);
  Serial.println(str);
  float tmp = float(str.toInt()) / 10;
  return (tmp);
}

float vpdhmd() {
  Serial.println(dfd);
  int vale = dfd.indexOf("h") + 2;
  dfd.remove(0, vale);
  Serial.println(dfd);
  float hmd = float(dfd.toInt()) / 10;
  //  Serial.println(hmd);
  return (hmd);
}

float vpdtemp_I() {
  char *ui = strtok(buff, "o");
  str = ui;
  int vale = str.indexOf("i") + 3;
  str.remove(0, vale);
  float i_temp = float(str.toInt()) / 10;
  return (i_temp);

}
float vpdtemp_O() {//tempit10ot10
  Serial.println(dfd);
  int vale = dfd.indexOf("o") + 3;
  dfd.remove(0, vale);
  Serial.println(dfd);
  float o_temp = float(dfd.toInt()) / 10;
  return (o_temp);

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





void setTimeNex() {
  //  Serial.println("setTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  runTimeNex();
}

void runTimeNex() {

  rtc.begin();

}

//*****************************EVENT****************************//
void eventmode()
{
  StaticJsonDocument <500> doc1;
  if (TemperatureAutoModeOn == true)
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["TEMP"] = "3";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }



  if (VpdautoModeOn == true)
  {

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["VPD"] = "4";
    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);



  }

}
void eventmode2()
{
  StaticJsonDocument <500> doc1;
  if ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "5";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }

  if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "6";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);



  }

}
void funtimer()
{

  eventmode();

}
void funtimer2()
{


  eventmode2();

}


//***************************************************VPD AUTO MODE***********************************************
void vpd_control(float ESNODE_ES_VPD)
{
  StaticJsonDocument<500> doc3;
  if (count1 == 0)
  {

    StaticJsonDocument<200> clearr;
    doc3 = clearr;
    char tempstring1[500];
    serializeJson(clearr, tempstring1);
    memset(tempstring1, 0, 500);

    count1 = 20;
  }

  float vpd = ESNODE_ES_VPD;
  ESNODE_ES_VPD = random(0, 2);
  if (ESNODE_ES_VPD >= 0.1)
  {
    count1 --;


    if ((ESNODE_ES_VPD >= 0.0) && (ESNODE_ES_VPD <= 0.5))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";


      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);








    }
    else if ((ESNODE_ES_VPD >= 0.6) && (ESNODE_ES_VPD <= 0.8))
    {

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




    }
    else if ((ESNODE_ES_VPD >= 1.5) && (ESNODE_ES_VPD <= 1.7))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);





    }
    else if ((ESNODE_ES_VPD >= 1.8) && (ESNODE_ES_VPD <= 2.0))

    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




    }
    else if (ESNODE_ES_VPD >= 2.1)
    {

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


    }

    else if ((ESNODE_ES_VPD >= 0.9) && (ESNODE_ES_VPD <= 1.4))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);





    }
  }
  Serial.println(ESNODE_ES_VPD);
  Serial.println("VPD SETPOINT");


  if (ESNODE_ES_VPD != 0) {



    if (ESNODE_ES_VPD < 0.3) { //0.41<=0.4
      // relayState = LOW; // turn it on

      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }


    }


    else if ((ESNODE_ES_VPD > 0.31)  &&   (ESNODE_ES_VPD <= 0.49)) { //0.41<=0.4
      // relayState = LOW; // turn it on


      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }



    }
    else if ((ESNODE_ES_VPD > 0.21) && (ESNODE_ES_VPD < 0.30)) {

      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }
    else if ((ESNODE_ES_VPD > 0.01) && (ESNODE_ES_VPD < 0.20)) {


      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }




    else if ((ESNODE_ES_VPD >= 1.51) && (ESNODE_ES_VPD <= 1.69)) {
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }
    else if ((ESNODE_ES_VPD >= 1.70) && (ESNODE_ES_VPD <= 1.89)) {

      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }
    else if ((ESNODE_ES_VPD >= 1.90)) {
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }

    else if (ESNODE_ES_VPD >= 0.5 && (ESNODE_ES_VPD <= 1.5))
    {
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }
    }

    else  {

      Serial.println("nothing to do");


      pinMode(ExFan1, OUTPUT);
      digitalWrite(ExFan1, HIGH);
      pinMode(ExFan2, OUTPUT);
      digitalWrite(ExFan2, HIGH);
      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);
      Serial.println(ESNODE_ES_VPD);

    }
  }
}



//
void asd()
{

  Serial.println("we wre here");

  pinMode(ExFan1, OUTPUT);
  pinMode(ExFan2, OUTPUT);
  pinMode(ExFan3, OUTPUT);
  pinMode(ExFan4, OUTPUT);
  pinMode(ExFan5, OUTPUT);
  pinMode(ExFan6, OUTPUT);
  pinMode(ExFan7, OUTPUT);
  pinMode(ExFan8, OUTPUT);
  pinMode(pad1, OUTPUT);
  pinMode(Fogger, OUTPUT);
  pinMode(FoggerSol, OUTPUT);
  pinMode(Co2Gen, OUTPUT);

  digitalWrite(ExFan1, HIGH);
  digitalWrite(ExFan2, HIGH);
  digitalWrite(ExFan3, HIGH);
  digitalWrite(ExFan4, HIGH);
  digitalWrite(ExFan5, HIGH);
  digitalWrite(ExFan6, HIGH);
  digitalWrite(ExFan7, HIGH);
  digitalWrite(ExFan8, HIGH);
  digitalWrite(pad1, HIGH);
  digitalWrite(Fogger, HIGH);
  digitalWrite(FoggerSol, HIGH);
  digitalWrite(Co2Gen, HIGH);
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


void callasd()
{
  if (VpdautoModeOn == false)
  {
    asd();
  }
}

//****************************************************************Auto Temp*************************************************
void AutoTemp_control(float SPtemp)

{

  StaticJsonDocument<500> doc3;
  if (count3 == 0)
  {
    StaticJsonDocument<200> clearr1;
    doc3 = clearr1;
    char tempstring2[500];
    serializeJson(clearr1, tempstring2);
    memset(tempstring2, 0, 500);

    count3 = 10;
  }


  float ATempHysteris = SPtemp + 1;

  float ATemp = random(25, 35);
  if (ATemp > 0)
  {
    count3--;


    if ((ATemp >= 36) && (ATemp <= 38))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if ((ATemp >= 39) && (ATemp <= 41))
    {




      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if (ATemp > 42)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

    }
    else if (ATemp <= 35)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




    }
  }
  Serial.println(ATemp);

  Serial.println(SPtemp);

  Serial.println(ATempHysteris);
  if (ATemp != 0) {

    if ( ATemp >= ATempHysteris) {  // 33>=26  //31.80>=26w


      Serial.println(" we need  fans to turn on");
      Serial.println(ATemp);
      //    Serial.println(ATempHysteris);
      float ATempLow =  ATempHysteris + 2; //26
      float ATempMid = ATempLow + 2; //28
      float ATempHigh = ATempMid + 2; //30



      Serial.println("Modes");
      Serial.println(ATempHigh);
      Serial.println(ATempMid);
      Serial.println(ATempLow);
      if (ATemp >= ATempHigh) //32<31                 // Temp=33    32<33//31.80>=32 fail
      {
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }


        else if (mills >= 2405000)
        {
          noInterrupts ();
          timer0_millis = 0;
          interrupts ();
        }
      }
      else if ((ATemp >= ATempMid) && (ATemp < ATempHigh)) //31>30&& 31<32        //Temp=31  31>30  && 31<32//31.80>=30 && 31.80<32
      {

        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 60000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 61000) && (mills <= 120000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }


        else if (mills >= 125000)
        {
          noInterrupts ();
          timer0_millis = 0;
          interrupts ();
        }
      }
      else  if ((ATemp >= ATempLow) && (ATemp < ATempMid)) //Temp=29 //31.80>=28 && 31.80<30
      {
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }


        else if (mills >= 2405000)
        {
          noInterrupts ();
          timer0_millis = 0;
          interrupts ();
        }
      }
      else if ((ATemp >= SPtemp) && (ATemp < ATempLow)) { //Temp=27 31.80>=26 31.80<28
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }


        else if (mills >= 2405000)
        {
          noInterrupts ();
          timer0_millis = 0;
          interrupts ();
        }
      }
    }

    if (ATemp < SPtemp) {
      //31.80<26
      Serial.println(ATemp);
      Serial.println("All OFF");
      //relayState = HIGH;
      pinMode(ExFan1, OUTPUT);
      digitalWrite(ExFan1, HIGH);
      pinMode(ExFan2, OUTPUT);
      digitalWrite(ExFan2, HIGH);

      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);
    }
  }
}




void callSerialCom() {
  if (Serial2.available()) {
    SerialCom("");
  }
}
void callNexVal() {

  Serial.print("");

}
void callFromEspData() {

  FromEspData();

}
void callVpdMode() {
  if (VpdautoModeOn == true ) {
    vpd_control( ESNODE_ES_VPD);
    Serial.println("VPD");
  }
}

void callAutoTempMode() {
  if (TemperatureAutoModeOn == true )
  {
    AutoTemp_control( SPtemp);
    Serial.println("TEMP");
  }
}


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  Serial.println("Axalyn...");
  rtc.begin();
  rtc.adjust(DateTime(2022, 9, 17, 11, 15, 0));

  pinMode(ExFan1, OUTPUT);
  pinMode(ExFan2, OUTPUT);
  pinMode(ExFan3, OUTPUT);
  pinMode(ExFan4, OUTPUT);
  pinMode(ExFan5, OUTPUT);
  pinMode(ExFan6, OUTPUT);
  pinMode(ExFan7, OUTPUT);
  pinMode(ExFan8, OUTPUT);
  pinMode(pad1, OUTPUT);
  pinMode(Fogger, OUTPUT);
  pinMode(FoggerSol, OUTPUT);
  pinMode(Co2Gen, OUTPUT);

  digitalWrite(ExFan1, HIGH);
  digitalWrite(ExFan2, HIGH);
  digitalWrite(ExFan3, HIGH);
  digitalWrite(ExFan4, HIGH);
  digitalWrite(ExFan5, HIGH);
  digitalWrite(ExFan6, HIGH);
  digitalWrite(ExFan7, HIGH);
  digitalWrite(ExFan8, HIGH);
  digitalWrite(pad1, HIGH);
  digitalWrite(Fogger, HIGH);
  digitalWrite(FoggerSol, HIGH);
  digitalWrite(Co2Gen, HIGH);

  runTimeNex();
  readalldata();
  vpdfun_cal = false;
}
unsigned long SCpreviousMillis = 0;

unsigned long NXpreviousMillis = 0;

unsigned long ESPpreviousMillis = 0;

unsigned long ATpreviousMillis = 0;

unsigned long VPpreviousMillis = 0;

unsigned long sch1previousMillis = 0;

unsigned long EVNTpreviousMillis = 0;

unsigned long manualsend = 0;

unsigned long nxpreviousMillis = 0;

unsigned long nextionmills = 0;

void loop()
{
  ////////////************** Serialcom loop Mode***********///////////
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500)
  {
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
  //////////************** Node receviing loop Mode***********///////////
  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - ESPpreviousMillis >= 3000)
  {
    ESPpreviousMillis = NXcurrentMillis;

    getvalue.start(100);

  }
  if (getvalue.isRunning())
  {

    FromEspData1();
  }
  if (getvalue.justFinished())
  {

  }
  //////////************** Reverseflow receviing loop Mode***********///////////
  //  unsigned long nxcurrentMillis = millis();
  //  if (nxcurrentMillis - nxpreviousMillis >= 1000)
  //  {
  //
  //    nxpreviousMillis = nxcurrentMillis;
  //    getvalue.start(50);
  //
  //  }
  //  if (getvalue.isRunning())
  //  {
  //
  //    FromEspData();
  //
  //  }
  //  if (getvalue.justFinished())
  //  {
  //
  //  }
  //////////************** Temperature loop Mode***********///////////
  if (TemperatureAutoModeOn == true ) {

    unsigned long ATcurrentMillis = millis();
    if (ATcurrentMillis - ATpreviousMillis >= 10000) {
      ATpreviousMillis = ATcurrentMillis;

      AutoTemp_control( SPtemp);
      Serial.println("TEMP");

      funtimer();
    }
  }
  //////////************** VPD loop Mode***********///////////
  if (VpdautoModeOn == true ) {

    unsigned long VPcurrentMillis = millis();
    if (VPcurrentMillis - VPpreviousMillis >= 10000) {
      VPpreviousMillis = VPcurrentMillis;

      vpd_control( ESNODE_ES_VPD);
      Serial.println("VPD");

      funtimer();
    }
  }
  //////////************** Irrevent loop Mode***********///////////
  if (((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true)) || ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true)))
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
  //////////************** Nexval Mode***********///////////
  unsigned long nextionmillsc = millis();
  if (nextionmillsc - nextionmills >= 600000)
  {
    nextionmillsc = nextionmillsc;
    nexval();
  }


  //////////************** AUTOIRR UPDATE Mode***********///////////
  if (PumpAZ1bool == true)
  {
    PumpAZ1.Update();

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
  f1 = digitalRead(ExFan1);
  f2 = digitalRead(ExFan2);
  f3 = digitalRead(ExFan3);
  f4 = digitalRead(ExFan4);
  f5 = digitalRead(ExFan5);
  f6 = digitalRead(ExFan6);
  f7 = digitalRead(ExFan7);
  f8 = digitalRead(ExFan8);
  cp = digitalRead(pad1);
  co = digitalRead(Co2Gen);
  fp = digitalRead(Fogger);
  fs = digitalRead(FoggerSol);
  ir = digitalRead(Z1PUMPA);
  doo = digitalRead(Z1PUMPB);
  dr = digitalRead(Z1AIRPUMP);
  pb = digitalRead(Z1SOLVALVE);

  storeprvstate = "";
  storeprvstate = f1 + f2 +  f3 +  f4 + f5 + f6 +  f7 +  f8 +  cp +  fp + fs +  co +  ir +  doo +  dr +  pb;

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
  Manualstatus["f1"] = f1;
  Manualstatus["f2"] = f2;
  Manualstatus["f3"] = f3;
  Manualstatus["f4"] = f4;
  Manualstatus["f5"] = f5;
  Manualstatus["f6"] = f6;
  Manualstatus["f7"] = f7;
  Manualstatus["f8"] = f8;
  Manualstatus["cp"] = cp;
  Manualstatus["co"] = co;
  Manualstatus["fp"] = fp;
  Manualstatus["fs"] = fs;
  Manualstatus["ir"] = ir;
  Manualstatus["do"] = doo;
  Manualstatus["dr"] = dr;
  Manualstatus["pb"] = pb;
  serializeJson(Manualdoc1, Serial);
  serializeJson(Manualdoc1, Serial3);
  Serial.println(".....");
}

void nexval()
{
  if (ESNODE_ES_AT > 0)
  {
    Serial2.print("at.txt=\"");
    Serial2.print(ESNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("atv.txt=\"");
    Serial2.print(ESNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("rh.txt=\"");
    Serial2.print(ESNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("rhv.txt=\"");
    Serial2.print(ESNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("vpd.txt=\"");
    Serial2.print(ESNODE_ES_VPD);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("vpdv.txt=\"");
    Serial2.print(ESNODE_ES_VPD);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    if (ESNODE_ES_ET > 0)
    {
      Serial2.print("ev.txt=\"");
      ESNODE_ES_ET = sqrt(( ESNODE_ES_ET) * (ESNODE_ES_ET));
      Serial2.print(ESNODE_ES_ET);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
    }
    Serial2.print("bp.txt=\"");
    Serial2.print(ESNODE_ES_BP);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);



    Serial2.print("sl.txt=\"");
    Serial2.print(ESNODE_ES_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("co.txt=\"");
    Serial2.print(ESNODE_ES_CO2);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("dpp.txt=\"");
    Serial2.print(ESNODE_ES_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("sr.txt=\"");
    Serial2.print(ESNODE_ES_SR);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("hi.txt=\"");
    Serial2.print(ESNODE_ES_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
  }
  if (WSNODE_ES_AT > 0)
  {
    Serial2.print("att.txt=\"");
    Serial2.print(WSNODE_ES_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rhh.txt=\"");
    Serial2.print(WSNODE_ES_RH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("bpp.txt=\"");
    Serial2.print(WSNODE_ES_BP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sll.txt=\"");
    Serial2.print(WSNODE_ES_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);



    Serial2.print("srr.txt=\"");
    Serial2.print(WSNODE_ES_SR);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("hii.txt=\"");
    Serial2.print(WSNODE_ES_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("evv.txt=\"");
    Serial2.print(WSNODE_ES_ET);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("dppp.txt=\"");
    Serial2.print(WSNODE_ES_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
  }
  /**************** DATE & TIME ***********************/

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


  Serial2.print("min.txt=\"");
  Serial2.print(minupg);//minupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //Serial.print(secslive);
  Serial2.print("sec.txt=\"");
  Serial2.print(secslive);//secslive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  ////////////  YEAR ////////


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


  Serial2.print("year.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("yearr.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

}
