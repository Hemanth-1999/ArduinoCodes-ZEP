
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
int CS_pin = 10;
//const int TnumReadings = 20;
//int Treadings[TnumReadings];
//int Tindex = 0;
//int Ttotal = 0;
//int TAverage = 0;
const unsigned int MAX_MESSAGE_LENGTH = 12;
float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_AvgVPD_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;
float MESPWS_ID_Time_AT_v, MESPWS_ID_Time_RH_v, MESPWS_ID_Time_VPD_v, MESPWS_ID_Time_HI_v, MESPWS_ID_Time_EvTr_v, MESPWS_ID_Time_BaPr_v, MESPWS_ID_Time_SoRa_v, MESPWS_ID_Time_DePt_v;
float MESPWS_ID_Time_WSwsclk_v, MESPWS_ID_Time_WSwsaclk_v, MESPWS_ID_Time_WSwdclk_v, MESPWS_ID_Time_WSwdaclk_v, MESPWS_ID_Time_WSclohei_v;
int MESPWS_ID_Time_WSntips_v, MESPWS_ID_Time_SeL_v, MESPWS_ID_Time_ntl_v;
float ATemp, AHumi;
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
StaticJsonDocument <500> doc1;
StaticJsonDocument<500> manualdoc;

StaticJsonDocument<500> doc3;
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

#define Z1PUMPA    36
#define Z1PUMPB    37
#define Z1AIRPUMP  38 ///(DRIP-LINE)///
#define Z1SOLVALVE 39///(IRRIGATION B)///
char tempString[1024];

//******************************************Time Date Setting**************************************************************

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

/************************************* NODES **************************************/


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

bool PumpAZ1bool;

long PumpAOnTime ;
long PumpAOffTime ;


//********************************************************SAVE TIME********************************************************///
bool saveZ1PASThr;

void Z1PAsavetime() {
  Z1PASThr  = othr();
  Z1PASTmin = otmin();
  EEPROM.update(EepromZ1PASThr, Z1PASThr);
  EEPROM.update(EepromZ1PASTmin, Z1PASTmin);
}

void Z1PAsavedue() {
  Z1PASPhr  = ofthr();
  Z1PASPmin = oftmin();
  EEPROM.update(EepromZ1PASPhr, Z1PASPhr);
  EEPROM.update(EepromZ1PASPmin, Z1PASPmin);
}

void EepromReadZ1AP()
{
  Z1PASThr  = EEPROM.read(EepromZ1PASThr);
  Z1PASTmin = EEPROM.read(EepromZ1PASTmin);
  Z1PASPhr  = EEPROM.read(EepromZ1PASPhr);
  Z1PASPmin = EEPROM.read(EepromZ1PASPmin);
}

//*****************************************************************
void Z1ZPAsavetime() {
  
  EEPROM.update(EepromZ1ZPASThr, Z1ZPASThr);
  EEPROM.update(EepromZ1ZPASTmin, Z1ZPASTmin);

}
void Z1ZPAsavedue() {
  
  EEPROM.update(EepromZ1ZPASPhr, Z1ZPASPhr);
  EEPROM.update(EepromZ1ZPASPmin, Z1ZPASPmin);

}
void EepromReadZ1ZAP()
{
  Z1ZPASThr  = EEPROM.read(EepromZ1ZPASThr);
  Z1ZPASTmin = EEPROM.read(EepromZ1ZPASTmin);
  Z1ZPASPhr  = EEPROM.read(EepromZ1ZPASPhr);
  Z1ZPASPmin = EEPROM.read(EepromZ1ZPASPmin);

}
//**********************************************************************************
void Z1PBsavetime() {
  Z1PBSThr   = othr();
  Z1PBSTmin = otmin();
  EEPROM.update(EepromZ1PBSThr, Z1PBSThr);
  EEPROM.update(EepromZ1PBSTmin, Z1PBSTmin);
}
void Z1PBsavedue() {
  Z1PBSPhr  = ofthr();
  Z1PBSPmin = oftmin();
  EEPROM.update(EepromZ1PBSPhr, Z1PBSPhr);
  EEPROM.update(EepromZ1PBSPmin, Z1PBSPmin);
}
void EepromReadZ1PB()
{
  Z1PBSThr  = EEPROM.read(EepromZ1PBSThr);
  Z1PBSTmin = EEPROM.read(EepromZ1PBSTmin);
  Z1PBSPhr  = EEPROM.read(EepromZ1PBSPhr);
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
  Z1ZPBSThr  = EEPROM.read(EepromZ1ZPBSThr);
  Z1ZPBSTmin = EEPROM.read(EepromZ1ZPBSTmin);
  Z1ZPBSPhr  = EEPROM.read(EepromZ1ZPBSPhr);
  Z1ZPBSPmin = EEPROM.read(EepromZ1ZPBSPmin);

}
//*****************************************************************
void Z1APsavetime() {
  Z1APSThr  = othr();
  Z1APSTmin = otmin();
  EEPROM.update(EepromZ1APSThr, Z1APSThr);
  EEPROM.update(EepromZ1APSTmin, Z1APSTmin);

}
void Z1APsavedue() {
  Z1APSPhr  = ofthr();
  Z1APSPmin = oftmin();
  EEPROM.update(EepromZ1APSPhr, Z1APSPhr);
  EEPROM.update(EepromZ1APSPmin, Z1APSPmin);
}
void EepromReadZ1Air()
{
  Z1APSThr  = EEPROM.read(EepromZ1APSThr);
  Z1APSTmin = EEPROM.read(EepromZ1APSTmin);
  Z1APSPhr  = EEPROM.read(EepromZ1APSPhr);
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
  Z1ZAPSThr  = EEPROM.read(EepromZ1ZAPSThr);
  Z1ZAPSTmin = EEPROM.read(EepromZ1ZAPSTmin);
  Z1ZAPSPhr  = EEPROM.read(EepromZ1ZAPSPhr);
  Z1ZAPSPmin = EEPROM.read(EepromZ1ZAPSPmin);

}
/**************************************************************************************************************/



void LBPAsavetime() {
  LBPASThr  = othr();
  LBPASTmin = otmin();
  EEPROM.update(EepromLBPASThr, LBPASThr);
  EEPROM.update(EepromLBPASTmin, LBPASTmin);
}

void LBPAsavedue() {
  LBPASPhr  = ofthr();
  LBPASPmin = oftmin();
  EEPROM.update(EepromLBPASPhr, LBPASPhr);
  EEPROM.update(EepromLBPASPmin, LBPASPmin);
}

void EepromReadLBAP()
{
  LBPASThr  = EEPROM.read(EepromLBPASThr);
  LBPASTmin = EEPROM.read(EepromLBPASTmin);
  LBPASPhr  = EEPROM.read(EepromLBPASPhr);
  LBPASPmin = EEPROM.read(EepromLBPASPmin);
}
//////////////////////////////////////////////
void sch1savetime() {
  sch1PSThr  = othr();
  sch1PSTmin = otmin();
  EEPROM.update(Eepromsch1hr, sch1PSThr);
  EEPROM.update(Eepromsch1min, sch1PSTmin);
}
void sch1savedue() {
  sch1PSThr1  = ofthr();
  sch1PSTmin1 = oftmin1();
  EEPROM.update(Eepromsch1hr1, sch1PSThr1);
  EEPROM.update(Eepromsch1min1, sch1PSTmin1);

}
void EepromReadsch1()
{
  sch1PSThr   = EEPROM.read(Eepromsch1hr);
  sch1PSTmin  = EEPROM.read(Eepromsch1min);
  sch1PSThr1  = EEPROM.read(Eepromsch1hr1);
  sch1PSTmin1 = EEPROM.read(Eepromsch1min1);
}
//////////////////////////////////////////////////

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

  //////////////     ******************     /////////////////////
  //////////////     ******************     /////////////////////
  //////////////     ******************     /////////////////////


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
  
  //  //////////////////////// MANUAL MODE IRR PUMP //////////////////////////
  if (dfd == "pAz1On")
  {

    JsonObject MANUALDOC = manualdoc.createNestedObject("MANUALRELAY");
    MANUALDOC["IRR"] = "1";
    serializeJson(manualdoc, Serial);
    serializeJson(manualdoc, Serial3);
    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, LOW);
    feedBack = "pAz1On";
    ZAApumpSt = "on";
    Serial.println("pump IRR is on");

  }
  if (dfd == "pAz1Off")
  {
    JsonObject MANUALDOC = manualdoc.createNestedObject("MANUALRELAY");
    MANUALDOC["IRR"] = "2";
    serializeJson(manualdoc, Serial);
    serializeJson(manualdoc, Serial3);


    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, HIGH);
    feedBack = "pAz1Off";
    Serial.println("pump  IRR is off");
    ZAApumpSt = "off";
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
    long PumpAOnTimetotelON = Z1PASThrconvOn + Z1PASTminconvOn;


    long Z1PASThrconvoff = Z1PASPhr * ms_per_hour;
    long Z1PASTminconvoff = Z1PASPmin * ms_per_min;
    long PumpAOnTimetotelOFF = Z1PASThrconvoff + Z1PASTminconvoff;

    Serial.println(PumpAOnTimetotelON);
    Serial.println(PumpAOnTimetotelOFF);

    PumpAZ1.begin(PumpAOnTimetotelON , PumpAOnTimetotelOFF );

    Serial.println("PUMPA Z1 On");
    PumpAZ1bool = true;
    EepromReadZ1AP();
    feedBack = "z1aon";
  }
  if (dfd == "z1aoff")
  {
    irroff();
    PumpAZ1bool = false;
    Serial.println("PUMPA Z1  Off");
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
    long PumpBOnTimetotelON = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1PBSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1PBSPmin * ms_per_min;
    long PumpBOnTimetotelOFF = Z1PBSThrconvoff + Z1PBSTminconvoff;

    Serial.println(PumpBOnTimetotelON);
    Serial.println(PumpBOnTimetotelOFF);

    PumpBZ1.begin(PumpBOnTimetotelON , PumpBOnTimetotelOFF);
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
    long PumpDOTimeon = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1APSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1APSPmin * ms_per_min;
    long PumpDOTimeoff = Z1PBSThrconvoff + Z1PBSTminconvoff;

    Serial.println(PumpDOTimeon);
    Serial.println(PumpDOTimeoff);

    PumpDOZ1.begin(PumpDOOnTime , PumpDOOffTime);
    PumpDOZ1bool = true;
    Serial.println("PUMP DO ON");

    EepromReadZ1Air();

    feedBack = "z1don";
  }
  if (dfd == "z1doff") {
    dooff();
    PumpDOZ1bool = false;

    Serial.println("PUMP DO OFF");
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

    long z1LBSThrconvoff = LBPASPhr * ms_per_hour;

    long z1LBSTminconvoff = LBPASPmin * ms_per_min;
    PumpLBOffTime = z1LBSThrconvoff + z1LBSTminconvoff;

    Serial.println(PumpLBOnTime);
    Serial.println(PumpLBOnTime);

    PumpLB.begin(PumpLBOnTime , PumpLBOffTime);
    PumpLBbool = true;
    Serial.println("LIGHT B ON");

    EepromReadLBAP();

    feedBack = "albon";
  }
  if (dfd == "alboff") {
    irrboff();
    PumpLBbool = false;

    Serial.println("LIGHT B Off");
    feedBack = "alboff";
  }


  //  /************************* SETTINS ***************************/
  //  if (dfd == "TkMgOn") {
  //    ////Serial.println("Tank Management On");
  //    feedBack = "TkMgOn";
  //    water_level_sensor = true;
  //    waterlevel_sensor = true;
  //
  //  } if (dfd == "TkMgOff") {
  //    ////Serial.println("Tank Management Off");
  //    feedBack = "TkMgOn";
  //    waterlevel_sensor = false;
  //  }

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

void Flow()
{
  count++; //Every time this function is called, increment "count" by 1
}
void Flow2()
{
  count2++; //Every time this function is called, increment "count" by 1
}




void  Nexval() {
  dateandtime();
  /////////////////////////////// Signal ////////////////////////

  Serial.println("nexval");
  Serial2.print("fdbk.txt=\"");
  Serial2.print(feedBack);
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

  /*************************** NODES VALUES *********************/
  /***************** GCC ****************/
  if (MESP_ID_Time_AvgAT_v > 0) {
    Serial2.print("at.txt=\"");
    Serial2.print(MESP_ID_Time_AvgAT_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);



    Serial2.print("rh.txt=\"");
    Serial2.print(MESP_ID_Time_AvgRH_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("vpd.txt=\"");
    Serial2.print(MESP_ID_Time_AvgVPD_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ev.txt=\"");
    MESP_ID_Time_EvTr_v = sqrt(( MESP_ID_Time_EvTr_v) * (MESP_ID_Time_EvTr_v));
    Serial2.print(MESP_ID_Time_EvTr_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("bp.txt=\"");
    Serial2.print(MESP_ID_Time_BaPr_v);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sl.txt=\"");
    Serial2.print(MESP_ID_Time_SeL_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("co.txt=\"");
    Serial2.print(MESP_ID_Time_co2_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("dpp.txt=\"");
    Serial2.print(MESP_ID_Time_DePt_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sr.txt=\"");
    Serial2.print(MESP_ID_Time_SoRa_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("hi.txt=\"");
    Serial2.print(MESP_ID_Time_AvgHI_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    /***************** HWM ****************/
    Serial2.print("att.txt=\"");
    Serial2.print(MESPWS_ID_Time_AT_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rhh.txt=\"");
    Serial2.print(MESPWS_ID_Time_RH_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("bpp.txt=\"");
    Serial2.print(MESPWS_ID_Time_BaPr_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sll.txt=\"");
    Serial2.print(MESPWS_ID_Time_SeL_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("wd.txt=\"");
    Serial2.print(MESPWS_ID_Time_WSwdclk_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ws.txt=\"");
    Serial2.print(MESPWS_ID_Time_WSwsclk_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("srr.txt=\"");
    Serial2.print(MESPWS_ID_Time_SoRa_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("hii.txt=\"");
    Serial2.print(MESPWS_ID_Time_HI_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rp.txt=\"");
    Serial2.print(MESPWS_ID_Time_WSntips_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ch.txt=\"");
    Serial2.print(MESPWS_ID_Time_WSclohei_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("evv.txt=\"");
    Serial2.print(MESPWS_ID_Time_EvTr_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("dppp.txt=\"");
    Serial2.print(MESPWS_ID_Time_DePt_v);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);


  }
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

  //  Serial.print(minupg);
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

  //Serial.print(Datelive);
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

  // Serial.print(Monthlive);
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

  //Serial.print(Yearlive);
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

  Serial2.print("vpdset.txt=\"");
  Serial2.print(VPD);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("avpd.txt=\"");
  Serial2.print(MESP_ID_Time_AvgVPD_v);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  //IRR
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

  Serial2.print("outlet.txt=\"");
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
}



void setTimeNex() {
  //  Serial.println("setTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  runTimeNex();
}

void runTimeNex() {
  //  Serial.println("runTimeNex");
  rtc.begin();
  //  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

//*****************************EVENT****************************//
void eventmode()
{


  if (TemperatureAutoModeOn == true)
  {



    Serial2.print("modestatus.txt=\"");
    Serial2.print("TEMPMODE is in AUTO");
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["TEMP"] = "3";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);

    Serial2.print("modestatus.pco=32799");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }



  if (VpdautoModeOn == true)
  {

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["VPD"] = "4";
    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);

    Serial2.print("modestatus.txt=\"");
    Serial2.print(" VPD is in AUTO");
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("modestatus.pco=32799");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }

}
void eventmode2()
{

  if ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true))
  {

    Serial2.print("mode1status.txt=\"");
    Serial2.print("IRR is in AUTO mode ");
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "5";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


    Serial2.print("mode1status.pco=32799");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }

  if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {

    Serial2.print("mode1status.txt=\"");
    Serial2.print("IRR is in SCHEDULE mode ");
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "6";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


    Serial2.print("mode1status.pco=32799");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);

  }

}
void funtimer()
{
  unsigned long runtime = millis();

  if (runtime <= 100000)
  {
    eventmode();
  }
}
void funtimer2()
{
  unsigned long runtime2 = millis();
  if (runtime2 <= 100000)
  {
    eventmode2();
  }
}

//***************************************************VPD AUTO MODE***********************************************
void vpd_control(float MESP_ID_Time_AvgVPD_v)
{

  if (count1 == 0)
  {
    StaticJsonDocument<200> clearr;
    doc3 = clearr;
    char tempstring1[500];
    serializeJson(clearr, tempstring1);
    memset(tempstring1, 0, 500);

    count1 = 20;
  }

  float vpd = MESP_ID_Time_AvgVPD_v;

  MESP_ID_Time_AvgVPD_v =  random(0, 3);
  if (MESP_ID_Time_AvgVPD_v >= 0.1)
  {
    count1 --;


    if ((MESP_ID_Time_AvgVPD_v >= 0.0) && (MESP_ID_Time_AvgVPD_v <= 0.5))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("CRITICAL");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";


      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


      Serial2.print("alert.pco=53573");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);





    }
    else if ((MESP_ID_Time_AvgVPD_v >= 0.6) && (MESP_ID_Time_AvgVPD_v <= 0.8))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("MODERATE");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



      Serial2.print("alert.pco=65344");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.5) && (MESP_ID_Time_AvgVPD_v <= 1.7))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("LOW");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);





      Serial2.print("alert.pco=9284");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.8) && (MESP_ID_Time_AvgVPD_v <= 2.0))

    {
      Serial2.print("alert.txt=\"");
      Serial2.print("MODERATE");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



      Serial2.print("alert.pco=65344");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    else if (MESP_ID_Time_AvgVPD_v >= 2.1)
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("Critical");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


      Serial2.print("alert.pco=53573");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }

    else if ((MESP_ID_Time_AvgVPD_v >= 0.9) && (MESP_ID_Time_AvgVPD_v <= 1.4))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("NORMAL");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



      Serial2.print("alert.pco=9284");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

    }
  }
  Serial.println(MESP_ID_Time_AvgVPD_v);
  Serial.println("VPD SETPOINT");


  if (MESP_ID_Time_AvgVPD_v != 0) {



    if (MESP_ID_Time_AvgVPD_v < 0.3) { //0.41<=0.4
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


    else if ((MESP_ID_Time_AvgVPD_v > 0.31)  &&   (MESP_ID_Time_AvgVPD_v <= 0.49)) { //0.41<=0.4
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
    else if ((MESP_ID_Time_AvgVPD_v > 0.21) && (MESP_ID_Time_AvgVPD_v < 0.30)) {

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
    else if ((MESP_ID_Time_AvgVPD_v > 0.01) && (MESP_ID_Time_AvgVPD_v < 0.20)) {


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


    //DAY MODE
    //    if ((vpd >= vpdhighsp)) {
    //      relayState = LOW;
    //      Serial.println("pad and fans on");
    //      Serial.println(vpd);
    //      Serial.println(vpdhighsp);
    //      digitalWrite(pad1, relayState);

    else if ((MESP_ID_Time_AvgVPD_v >= 1.51) && (MESP_ID_Time_AvgVPD_v <= 1.69)) {
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
    else if ((MESP_ID_Time_AvgVPD_v >= 1.70) && (MESP_ID_Time_AvgVPD_v <= 1.89)) {

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
    else if ((MESP_ID_Time_AvgVPD_v >= 1.90)) {
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

    else if (MESP_ID_Time_AvgVPD_v >= 0.5 && (MESP_ID_Time_AvgVPD_v <= 1.5))
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

      // relayState = LOW; // turn it on
      pinMode(ExFan1, OUTPUT);
      digitalWrite(ExFan1, HIGH);
      pinMode(ExFan2, OUTPUT);
      digitalWrite(ExFan2, HIGH);
      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);
      Serial.println(MESP_ID_Time_AvgVPD_v);
      //    Serial.println(vpdlowsp);
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

  float ATemp = random(30, 40);
  if (ATemp > 0)
  {
    count3--;


    if ((ATemp >= 36) && (ATemp <= 38))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("LOW");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



      Serial2.print("alert.pco=9284");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    else if ((ATemp >= 39) && (ATemp <= 41))
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("MODERATE");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);



      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

      Serial2.print("alert.pco=65344");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

    }
    else if (ATemp > 42)
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("CRITICAL");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



      Serial2.print("alert.pco=53573");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

    }
    else if (ATemp <= 35)
    {
      Serial2.print("alert.txt=\"");
      Serial2.print("NORMAL");
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


      Serial2.print("alert.pco=9284");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

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
    SerialCom();
  }
}
void callNexVal() {

  Serial.print("");
  Nexval();
}
void callFromEspData() {

  FromEspData();

}
void callVpdMode() {
  if (VpdautoModeOn == true ) {
    vpd_control( MESP_ID_Time_AvgVPD_v);
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

void sensorjson() {
  DynamicJsonDocument doc(512);
  doc["CONTROL ID"] = "321654";
  doc["ZAPumpA "] = random(0, 1); //ZAApumpSt;
  doc["ZAPumpB"] =  random(0, 1); //ZABpumpSt;
  doc["ZAAirSPump"] =  random(0, 1); //ZAAirSPumpSt;
  doc["ZARO"] =  random(0, 1); //ZAROSt;
  doc["ZAInletFlow"] = flowRate;
  doc["ZAOutletFlow"] = flowRate2;
  doc["ZANTankLevel"] = distanceincm;//waterconlevel
  doc["RADIATION"] = solar_Value;

  serializeJsonPretty(doc, Serial3);
  serializeJsonPretty(doc, Serial);
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
const long period = 1000;
unsigned long NXpreviousMillis = 0;
const long NXperiod = 10000;
unsigned long ESPpreviousMillis = 0;
const long ESPperiod = 10000;
unsigned long ATpreviousMillis = 0;
const long ATperiod = 1000;
unsigned long VPpreviousMillis = 0;
const long VPperiod = 1000;
unsigned long sch1previousMillis = 0;
const long sch1period = 1000;
unsigned long EVNTpreviousMillis = 0;
const long EVNTperiod = 1000;


void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500)
  {
    SCpreviousMillis = currentMillis;

    callSerialCom();

  }

  if (TemperatureAutoModeOn == true ) {

    unsigned long ATcurrentMillis = millis();
    if (ATcurrentMillis - ATpreviousMillis >= 10000) {
      ATpreviousMillis = ATcurrentMillis;

      AutoTemp_control( SPtemp);
      Serial.println("TEMP");
      delay(3000);
      funtimer();
    }
  }
  if (VpdautoModeOn == true ) {

    unsigned long VPcurrentMillis = millis();
    if (VPcurrentMillis - VPpreviousMillis >= 10000) {
      VPpreviousMillis = VPcurrentMillis;

      vpd_control( MESP_ID_Time_AvgVPD_v);
      Serial.println("VPD");
      delay(3000);
      funtimer();
    }
  }

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
