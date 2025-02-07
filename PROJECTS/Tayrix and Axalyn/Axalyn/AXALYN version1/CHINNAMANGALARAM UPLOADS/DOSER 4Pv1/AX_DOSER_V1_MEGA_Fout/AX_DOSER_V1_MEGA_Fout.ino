
/*/
   07/06/22 Added GetSensorReading

   3.1.1v
   New Auto Doser Funtion Upgarade

  DOSER Arduino MEGA with ESP8266 Board Code...
  Serial communication to esp8266
  ArduinoJson
  Auto Save
  Mode Save
  Audo Sheduler delay
  TaskScheduler enable disable

  //2/02/2022  - tank lelvel - nextino code and uva sensor - ard + nex
  //Ax Doser 02/06/22
  //new update 23/06/22

  28/06/22-Fine Tuning

*/

#include "DFRobot_PH.h"
#include "DFRobot_EC.h"
#include "math.h"
//#include <EEPROM.h>
#include <Arduino.h>
#include <menu.h>

#include <TimerOne.h>
#include <MemoryFree.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>
#include "DFRobot_PH.h"
#include "DFRobot_EC.h"
#include <Wire.h>
//int divisor = 1;
//String data_from_display="";
//int decimalPlaces = 0;
/////////////////////////////// SHEDULE MODE ///////////////////////
String data_from_display = "";
char buff[2000];
char *ph1;
char *ec1;
char *nt1;
char *hr;
int val;
//int valu;
//float sub;
String str;
String dfd;
short shr;
short smn;
//String smTaskOn;
//String smTaskOff;
//String amTaskOn;
//String amTaskOff;
//////////////setpoints/////////////
char *hph;
//char *hec;
//char *Ido;
char *dph;
char *dec;
char *dnt;
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
float pHAutoSP;
float pHHys = 0.1;
float ECAutoSP;
float ECHys;
//float uvIntensity;
//int UVOUT = A2; //Output from the sensor
//int REF_3V3 = A3; //3.3V power on the Arduino board
//union {
//  char charByte[4];
//  long valLong;
//} value;
bool wificon;
bool waterlevelnt;
bool waterlevel_sensor;
bool caliber;
bool startph;
bool calph;
bool extph;
bool startEc;
bool calEc;
bool extEc;
float savpH;
float savEc;
String feedBack;
//char cmd[10];
//char startcalib[10];
uint8_t resetPin = 11;
/////////////////////////////////////////////
//RTC
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
char t[32];
#include <DallasTemperature.h>
#include <OneWire.h>
#include <GravityTDS.h>
using namespace Menu;

#define LEDPIN LED_BUILTIN

#include <EEPROMex.h>
//WatchDogTimer
//#include <avr/wdt.h>

//Define
#define BUZZER 13 //37
#define LEDPOWER 10
#define LEDRED 11
#define LEDGREEN 9
#define LEDBLUE 12

//RELA
#define TPUMPR A15   // IN 1
#define HEATERR A12   // IN 4
#define MIXERR A13 //14


//****************************************** Water Level US Sensor **************************************************************
#define RORELAYR A14 // RO WATER ON

#define trigPin (uint8_t)46
#define echoPin (uint8_t)48
int duration;
int distance;
int distanceincm;
int distanceinInch;
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

float unitConduct;
//***********************************************************************************************************************


// Debug and Test options
//#define _DEBUG_
#define _TEST_
#define DEBUG 0                                // change value to 1 to enable debuging using serial monitor  
#define SERIAL_PORT_SPEED 115200//9600, 19200, 38400, 57600, 115200
#define PH_PIN A1                        //pH meter Analog output to Arduino Analog Input 0

#define ONE_WIRE_BUS 4 // Pin 4 is used for SD card in ethernet shield
//#define STATUS_LED 13

//#define STEP_MODE 4 // (1: Full Step, 2: Half Step, 4: Quarter Step, ...)

#define PHLOW_STEP_PIN        25
#define PHLOW_DIR_PIN          24
//#define PHLOW_ENABLE_PIN      0

#define PHHIGH_STEP_PIN       23
#define PHHIGH_DIR_PIN         22


#define EC_A_STEP_PIN         5

#define EC_A_DIR_PIN          6
//#define EC_A_ENABLE_PIN         0

#define EC_B_STEP_PIN          9  //26 7
#define EC_B_DIR_PIN          10 //28   8
//#define EC_B_ENABLE_PIN         00  //24

#define EC_C_STEP_PIN          3 //3  //36
#define EC_C_DIR_PIN            4 //4   //34
//#define EC_C_ENABLE_PIN         00  //30

//************SD Card **********************************************///
#define SD_DETECT_PIN   53

#define SDSS            53 //53
File sdcard_file;

struct Config {
  char hostname[64];
  int port;
};
const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object

///********************************************************************
GravityTDS gravityTds;

OneWire oneWireBus(ONE_WIRE_BUS);
DallasTemperature sensors (&oneWireBus);

RTC_DS3231 rtc; //for date time
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

uint16_t hourrc;
uint16_t minrc;
uint16_t secsrc;
uint16_t Monthrc;
uint16_t Daterc;
int Yearrc;

uint16_t hourupg;
uint16_t minupg;
uint16_t secslive;
uint16_t Monthlive;
uint16_t Datelive;
uint16_t Yearlive;

float phsetvalue ;
float phSetHysteris = 0.2 ;
int phplus;
int phmin;
int ECA;
int ECB ;
int ECC ;
float temp1 = 0;
String ssid;
String password;

//**** hysteris****//
float pH ; //generates the value of pH

float phtest;
float phUp = 2;
float phDown = 2; //

float EC; //generates the value of EC
int ECUp = 2;
int ECDown = 2;
int var = 0;

byte ECSetpoint;
byte ECHysterisMin;
byte ECHysterisPlus;


bool StopPHHys;
int autoon;
//bool autostart=autoon;
bool autostart = autoon;
bool EcCalStart;
int Scheon;
bool ScheduleAuto = Scheon;
bool Resetfun;

int Speed = 500;

# define Offsetec 0.00 //deviation compensate
# define Offset 0.00 //deviation compensate
# define LED 13
# define samplingInterval 20
# define printInterval 800
# define ArrayLenth 5 //times of collection

int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex = 0;

int ECArray[ArrayLenth]; //Store the average value of the sensor feedback
int ECArrayIndex = 0;

//float Setpoint;
float HysterisMin;
float HysterisPlus;
//float SetHysteris;


int previousMillis = 0; //
int pinHighTime = 100; //
int pinLowTime = 7500; // OLD 7500
int pinTime = 100; //

int ECpreviousMillis = 0; //             |
int ECpinHighTime = 100; //             |
int ECpinLowTime = 7500; //             |
int ECpinTime = 100; //

//*******Sensor Int***********///
//*********DO SENSOR************//

#define DO_PIN A13

String inString;
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0

//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (1600) //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (1300) //mv
#define CAL2_T (15)   //℃
float DOvalue;

const uint16_t DO_Table[41] = {
  14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
  11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
  9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
  7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410
};

uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;

int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
#if TWO_POINT_CALIBRATION == 0
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#endif
}

//*******EC SENSOR************///

#define EC_PIN A0
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, readfrom ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature ;
float  voltagePH, voltageEC, phValue, ecValue;
float phValueavrg;
float ECValueavrg;
DFRobot_PH ph;
DFRobot_EC ec;
float nutrientTDS;// = 0;
float nutrientEC;// = 0;
float nutrientCF;// = 0;
float nutrientTemperature;
float unitTemp ;
#define READ_TEMP (nutrientTemperature) //Current water temperature ℃, Or temperature sensor function
float nutrientPH;
float Fahrenheit;
float Status = 0;
unsigned long currentTime;
unsigned long int avgValue;  //Store the average value of the sensor feedback
int buf[10], temp;

//***EPROM***//
//*PH*//
uint16_t EeprompHSetpoint = 31;      //location of pHSetpoint in Eeprom
uint16_t EepromSetpHHysteris = 41;   //location of SetpHHysteris in Eeprom

//*EC*//
uint16_t EepromECSetpoint = 51;      //location of ECSetpoint in Eeprom
uint16_t EepromSetECHysteris = 61;   //location of SetECHysteris in Eeprom

//***Time***save eprom adress///

int Eepromhur = 200;
int Eeprommin = 203;

int Eepromhur1 = 206;
int Eeprommin1 = 209;

int Eepromhur2 = 212;
int Eeprommin2 = 215;

int Eepromhur3 = 218;
int Eeprommin3 = 221;

int Eepromhur4 = 224;
int Eeprommin4 = 227;

int Eepromhur5 = 230;
int Eeprommin5 = 233;

int Eepromhur6 = 380;
int Eeprommin6 = 383;

int Eepromhur7 = 386;
int Eeprommin7 = 389;

int Eepromhur8 = 393;
int Eeprommin8 = 396;

int EepromDate = 240;
int EepromMonth = 245;
int EepromYear = 250;

byte EeprompHs1 = 300;
byte EeprompHs2 = 305;
byte EeprompHs3 = 310;

byte EeprompHs4 = 315;
byte EeprompHs5 = 320;
byte EeprompHs6 = 399;

byte EeprompHs7 = 402;
byte EeprompHs8 = 405;


int EepromECs1 = 325;
int EepromECs2 = 330;
int EepromECs3 = 335;

int EepromECs4 = 340;
int EepromECs5 = 344;
int EepromECs6 = 408;

int EepromECs7 = 411;
int EepromECs8 = 413;

int smon;
int amon;
int smoff;
int amoff;

String sampumpSt;
String mixerSt;
String RoSt;
String HeaterSt;
//********************** NL ***********
//int EepromNTHset = 90;
//int EepromNTVset = 91;
//


// *********************************************************************
// Task Scheduler
// *********************************************************************

// objects
Scheduler r;

// Callback methods prototypes
void GetSensorReading();
void sensorjson();
void calibartion();
void setTimeNex();
void runTimeNex();
void AutoFun();
void scheduleFun();
void nexVal();
// Tasks

Task t1(5000,  TASK_FOREVER, &GetSensorReading, &r);//getNutrientPHavg

Task t5(10000,  TASK_FOREVER, &AutoFun, &r, false);//AutoFun
Task t6(10000,  TASK_FOREVER, &scheduleFun, &r, false);//scheduleFun
Task t12(2000,  TASK_FOREVER, &nexVal, &r);
//Task t11(9000,  TASK_FOREVER, &readalldata, &r);
Task t16(4500,  TASK_FOREVER, &calibartion, &r);
Task t20(4000,  TASK_FOREVER, &setTimeNex, &r);
Task t21(3000,  TASK_FOREVER, &runTimeNex, &r);



/*********************************** WIFI - JSON   *********************************/

//String wifiStatusRec;
String chipID;
String cloudIp;
String wifiStatus;
String userSsid;
String userPswd;

void wifireceived() {
  Serial.print(F("Chip Id: "));
  //      Serial.println(localip);
  Serial.print(F("wifiStatus: "));
  //      Serial.print(wifiStatusRec);
  //  StaticJsonDocument<150> doc;
  //  DeserializationError err1 = deserializeJson(doc, Serial);
  //  if (err1 == DeserializationError::Ok)
  //    chipID = doc["Wifi"]["Cid"].as<String>();
  //    cloudIp = doc["Wifi"]["IpC"].as<String>();
  //    wifiStatus = doc["Wifi"]["WiSt"].as<String>();
  //    userSsid = doc["Wifi"]["Ssid"].as<String>();
  //    userPswd = doc["Wifi"]["Pswd"].as<String>();
  //
  //      Serial.print("chipID: ");
  //      Serial.print(chipID);
  //      Serial.println("cloudIp: ");
  //      Serial.print(cloudIp);
  //      Serial.println("wifiStatus: ");
  //      Serial.print(wifiStatus);
  //      Serial.println("userSsid: ");
  //      Serial.print(userSsid);
  //      Serial.println("userPswd: ");
  //      Serial.print(userPswd);
  //      delay(1000);
  //  }
}

void ssidpwd() {
  //  Serial.print("Json");
  DynamicJsonDocument doc(150);
  JsonObject SdPwd = doc.createNestedObject("SdPwd");
  SdPwd["ssid"] = ssid;
  SdPwd["password"] = password;
  serializeJsonPretty(doc, Serial3);

}

int Eepromautostatus = 415;
///autosavefun
void autosavefun() {
  if (autoon == 1) {
    autostart = true;
  }
  else {
    autostart = false;
  }
}
result autosaveon() {
  EEPROM.write(Eepromautostatus, autostart);
}
//
void EepromReadauto()
{
  autoon = EEPROM.read(Eepromautostatus);
  // Serial.print ("on");
  //  Serial.println (autoon);
  return proceed;
}
//*****************************************************************


byte EepromSchetatus = 417;

void Schesavefun() {
  if (Scheon == 1) {
    ScheduleAuto = true;
  }
  else {
    ScheduleAuto = false;
  }
}

result Schesaveon() {
  EEPROM.write(EepromSchetatus, ScheduleAuto);
}

void EepromReadSche()
{
  Scheon = EEPROM.read(EepromSchetatus);
  // Serial.print ("Sche--on");
  //  Serial.println (Scheon);
  return proceed;
}

//****************************************************************


result schedautoOn() {
  ScheduleAuto = true;
  return proceed;
}
result schedautoOff() {
  ScheduleAuto = false;
  return proceed;
}



//********************************PH CAL*************************************************************************
float SetpHHysteris;
float pHSetpoint ;


///*********EPROM*******************************************************************************************///

result phsetsave() {
  EEPROM.writeFloat(EeprompHSetpoint, pHSetpoint);  // 6.5 pH for tomato growth

  return proceed;
}

result phsetHyssave() {

  EEPROM.writeFloat(EepromSetpHHysteris, phSetHysteris);
  return proceed;
}

void EepromReadPHCal()
{
  pHSetpoint = EEPROM.readFloat(EeprompHSetpoint);
  phSetHysteris = EEPROM.readFloat(EepromSetpHHysteris);
}
//**************************EC cal**************************************************************************************

float ECSetHysteris = 0.1;
float ECsetvalue;
///*********EPROM********************///
result ECsetsave() {
  EEPROM.writeFloat(EepromECSetpoint, ECsetvalue);

  return proceed;
}
result ECsethyssave() {

  EEPROM.writeFloat(EepromSetECHysteris, ECSetHysteris);
  return proceed;
}
void EepromReadEC()
{
  ECsetvalue = EEPROM.readFloat(EepromECSetpoint);
  ECSetHysteris = EEPROM.readFloat(EepromSetECHysteris);
}
//************************************************************************************************************
int PHUPR = 1;
int PHDOWNR = 1;
int ECRatioA = 1;
int ECRatioB = 1;
int ECRatioC = 1;


////////*********EPROM********************///

int Eepromphplus = 350;
int Eepromphmin = 360;
int EepromECA = 365;
int EepromECB = 370;
int EepromECC = 375;

result manualdose() {
  EEPROM.writeFloat(Eepromphplus, phplus);
  EEPROM.writeFloat(Eepromphmin, phmin);
  //  EEPROM.writeFloat(EepromECA, ECA);
  //  EEPROM.writeFloat(EepromECB, ECB);
  //  EEPROM.writeFloat(EepromECC, ECC);

  EepromReadMD();
  return proceed;
}

void EepromReadMD()
{
  phplus = EEPROM.readFloat(Eepromphplus);
  phmin = EEPROM.readFloat(Eepromphmin);
  //  ECA = EEPROM.readFloat(EepromECA);
  //  ECB = EEPROM.readFloat(EepromECB);
  //  ECC = EEPROM.readFloat(EepromECC);
}


result MDreset() {

  {
    float phplus, phmin, ECA, ECB, ECC = 0;
  }
  return ;

}

//int oneml = 125;
//int onemlphdown = 320; //320
//int onemlphup = 340;  //370
//int onemlA = 320; //315
//int onemlB = 320; // 380
//int onemlC = 320; //315

result manualdosemin() {
  digitalWrite(LEDBLUE, HIGH);
  int RPHUP =  PHUPR * 230;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pinTime) {
    previousMillis = currentMillis;
    digitalWrite(PHHIGH_DIR_PIN, LOW);
    // Motor rotate with ml input
    for ( int y = 0; y < phplus; y++) {
      for ( int x = 0; x < RPHUP; x++) //(onemlphdown * phmin) //420
      {
        digitalWrite(PHHIGH_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PHHIGH_STEP_PIN, LOW);
        delayMicroseconds(Speed);

      }
    }
    pinTime = pinLowTime;
  }
}


result manualdoseplus() {
  digitalWrite(LEDBLUE, HIGH);
  int RPHDOWN =  PHDOWNR * 230;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pinTime) {
    previousMillis = currentMillis;
    digitalWrite(PHLOW_DIR_PIN, LOW);
    for ( int y = 0; y < phmin ; y++) {
      for (int x = 0; x < RPHDOWN ; x++) //(onemlphup * phplus)390
      {
        digitalWrite(PHLOW_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PHLOW_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }

    }
    pinTime = pinLowTime;
  }
}
int ECRA;
result manualdoseEcA() {
  digitalWrite(LEDBLUE, HIGH);
  int ECRA =  ECRatioA * 230;
  digitalWrite(EC_A_DIR_PIN, LOW);

  // Motor rotate with ml input
  for ( int z = 0; z <  ECA; z++) {
    for (int x = 0; x < ECRA; x++)//375
    {
      digitalWrite(EC_A_STEP_PIN, HIGH);
      delayMicroseconds(Speed);

      digitalWrite(EC_A_STEP_PIN, LOW);
      delayMicroseconds(Speed);

    }
  }
}
int ECRB;
result manualdoseEcB() {
  digitalWrite(LEDBLUE, HIGH);
  int ECRB =  ECRatioB * 240;
  digitalWrite(EC_B_DIR_PIN, LOW);
  for ( int b = 0; b <  ECB; b++) {
    // Spin motor slowly
    for (int x = 0; x < ECRB; x++) //375
    {
      digitalWrite(EC_B_STEP_PIN, HIGH);
      delayMicroseconds(Speed);
      digitalWrite(EC_B_STEP_PIN, LOW);
      delayMicroseconds(Speed);
    }
    //delay(1000);
    //  return proceed;
  }
}
int ECRC;
result manualdoseEcC() {
  digitalWrite(LEDBLUE, HIGH);
  int ECRC =  ECRatioC * 230;
  digitalWrite(EC_C_DIR_PIN, LOW);
  for ( int y = 0; y <  ECC; y++) {
    // Spin motor slowly
    for (int x = 0; x < ECRC ; x++) //385
    {
      digitalWrite(EC_C_STEP_PIN, HIGH);
      delayMicroseconds(Speed);
      digitalWrite(EC_C_STEP_PIN, LOW);
      delayMicroseconds(Speed);
    }
    //delay(1000);
    //  return proceed;
  }
}

//*********************************************************************************************************

int ledRED = LOW;
int ledGREEN = LOW;
int ledBLUE = LOW;

bool probeEc;
bool probeTemp;
bool probeDo;

//*********************************************************************************************************

bool Autosave = HIGH;

short S1Hr;
short S1Min;


//
//uint16_t S2Hr;
//uint16_t S2Min;
////unsigned long secs0;
short S2Hr;
short S2Min;
//uint16_t S3Hr;
//uint16_t S3Min;
////unsigned long secs1;
short S3Hr;
short S3Min;
//uint16_t S4Hr;
//uint16_t S4Min;
////unsigned long secs1;
short S4Hr;
short S4Min;
//uint16_t S5Hr;
//uint16_t S5Min;
////unsigned long secs1;
short S5Hr;
short S5Min;
//uint16_t S6Hr;
//uint16_t S6Min;
short S6Hr;
short S6Min;
//uint16_t S7Hr;
//uint16_t S7Min;
short S7Hr;
short S7Min;
//uint16_t S8Hr;
//uint16_t S8Min;
short S8Hr;
short S8Min;

////******************************************Shift Time Setting Save ********************************************************

result Shift1Clockhr() {
  S1Hr = sendhour();
  EEPROM.write(Eepromhur1, S1Hr);
  Serial.print("Hr = ");
  Serial.print(S1Hr);

  return proceed;
}
result Shift1Clockmin() {
  S1Min = sendmin();
  EEPROM.write(Eeprommin1, S1Min);
  Serial.print("Min = ");
  Serial.print(S1Min);
  return proceed;
}
void EepromReadTime1()
{
  S1Hr = EEPROM.read(Eepromhur1);
  S1Min = EEPROM.read(Eeprommin1);
  return proceed;
}
//=================================================
//**********************SCHEDULER ALARM*******************
result Shift2Clock() {
  S2Hr = sendhour();
  S2Min = sendmin();
  EEPROM.write(Eepromhur2, S2Hr);
  EEPROM.write(Eeprommin2, S2Min);
  return proceed;
}

void EepromReadTime2()
{
  S2Hr = EEPROM.read(Eepromhur2);
  S2Min = EEPROM.read(Eeprommin2);
  return proceed;
}
//=================================================
//**********************SCHEDULER ALARM*******************
result Shift3Clock() {
  S3Hr = sendhour();
  S3Min = sendmin();
  EEPROM.write(Eepromhur3, S3Hr);
  EEPROM.write (Eeprommin3, S3Min);
  return proceed;
}
void EepromReadTime3()
{
  S3Hr = EEPROM.read(Eepromhur3);
  S3Min = EEPROM.read(Eeprommin3);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift4Clock() {
  S4Hr = sendhour();
  S4Min = sendmin();
  EEPROM.write(Eepromhur4, S4Hr);
  EEPROM.write (Eeprommin4, S4Min);
  return proceed;
}
void EepromReadTime4()
{
  S4Hr = EEPROM.read(Eepromhur4);
  S4Min = EEPROM.read(Eeprommin4);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift5Clock() {
  S5Hr = sendhour();
  S5Min = sendmin();
  EEPROM.write(Eepromhur5, S5Hr);
  EEPROM.write(Eeprommin5, S5Min);
  return proceed;
}
void EepromReadTime5()
{
  S5Hr = EEPROM.read(Eepromhur5);
  S5Min = EEPROM.read(Eeprommin5);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift6Clock() {
  S6Hr = sendhour();
  S6Min = sendmin();
  EEPROM.write(Eepromhur6, S6Hr);
  EEPROM.write(Eeprommin6, S6Min);
  return proceed;
}
void EepromReadTime6()
{
  S6Hr = EEPROM.read(Eepromhur6);
  S6Min = EEPROM.read(Eeprommin6);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift7Clock() {
  S7Hr = sendhour();
  S7Min = sendmin();
  EEPROM.write(Eepromhur7, S7Hr);
  EEPROM.write(Eeprommin7, S7Min);
  return proceed;
}
void EepromReadTime7()
{
  S7Hr = EEPROM.read(Eepromhur7);
  S7Min = EEPROM.read(Eeprommin7);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift8Clock() {
  S8Hr = sendhour();
  S8Min = sendmin();
  EEPROM.write(Eepromhur8, S8Hr);
  EEPROM.write(Eeprommin8, S8Min);
  return proceed;
}
void EepromReadTime8()
{
  S8Hr = EEPROM.read(Eepromhur8);
  S8Min = EEPROM.read(Eeprommin8);
  return proceed;
}
//*********************************************************************
void allReadTime() {
  EepromReadTime1();
  EepromReadTime2();
  EepromReadTime3();
  EepromReadTime4();
  EepromReadTime5();
  EepromReadTime6();
  EepromReadTime7();
  EepromReadTime8();
}

////
float pHs1;
float ECs1;
float temps;
float dos1;

float pHs2;
float ECs2;
float temps2;
float dos2;

float pHs3;
float ECs3;
float temps3;
float dos3;

float pHs4;
float ECs4;
float dos4;

float pHs5;
float ECs5;
float dos5;

float pHs6;
float ECs6;
float dos6;

float pHs7;
float ECs7;
float dos7;

float pHs8;
float ECs8;
float dos8;
//

//
////****Save Slot1*****************************************************
result saveslot1() {

  return proceed;
}

void S1Read()
{
  pHs1 = EEPROM.readFloat(EeprompHs1);
  ECs1 = EEPROM.readFloat(EepromECs1);

}
////****Save Slot2*****************************************************
//result saveslot2() {
//  EEPROM.updateFloat(EeprompHs2, pHs2);
//  EEPROM.updateFloat(EepromECs2, ECs2);
//  return proceed;
//}
//


//
////****Save Slot3*****************************************************
//result saveslot3() {
//  EEPROM.updateFloat(EeprompHs3, pHs3);
//  EEPROM.writeFloat(EepromECs3, ECs3);
//  return proceed;
//}
//
//void S3Read()
//{
//  pHs3 = EEPROM.readFloat(EeprompHs3);
//  ECs3 = EEPROM.readFloat(EepromECs3);
//}
//
////****Save Slot4*****************************************************
//result saveslot4() {
//  EEPROM.writeFloat(EeprompHs4, pHs4);
//  EEPROM.writeFloat(EepromECs4, ECs4);
//  return proceed;
//}
//
//void S4Read()
//{
//  pHs4 = EEPROM.readFloat(EeprompHs4);
//  ECs4 = EEPROM.readFloat(EepromECs4);
//}
//
////****Save Slot5************************************
//result saveslot5() {
//  EEPROM.writeFloat(EeprompHs5, pHs5);
//  EEPROM.writeFloat(EepromECs5, ECs5);
//  return proceed;
//}
//
//
//void S5Read()
//{
//  pHs5 = EEPROM.readFloat(EeprompHs5);
//  ECs5 = EEPROM.readFloat(EepromECs5);
//}
////****Save Slot6********************************************************
//result savepHs6() {
//  EEPROM.writeFloat(EeprompHs6, pHs6);
//
//  return proceed;
//}
//
//result saveECs6() {
//
//  EEPROM.writeFloat(EepromECs6, ECs6);
//  return proceed;
//}
//void S6Read()
//{
//  pHs6 = EEPROM.readFloat(EeprompHs6);
//  ECs6 = EEPROM.readFloat(EepromECs6);
//}


//*****************************************************************************
void Readall()
{
  pHs1 = EEPROM.readFloat(EeprompHs1);
  ECs1 = EEPROM.readFloat(EepromECs1);
  pHs2 = EEPROM.readFloat(EeprompHs2);
  ECs2 = EEPROM.readFloat(EepromECs2);
  pHs3 = EEPROM.readFloat(EeprompHs3);
  ECs3 = EEPROM.readFloat(EepromECs3);
  pHs4 = EEPROM.readFloat(EeprompHs4);
  ECs4 = EEPROM.readFloat(EepromECs4);
  pHs5 = EEPROM.readFloat(EeprompHs5);
  ECs5 = EEPROM.readFloat(EepromECs5);
  pHs6 = EEPROM.readFloat(EeprompHs6);
  ECs6 = EEPROM.readFloat(EepromECs6);
  pHs7 = EEPROM.readFloat(EeprompHs7);
  ECs7 = EEPROM.readFloat(EepromECs7);
  pHs8 = EEPROM.readFloat(EeprompHs8);
  ECs8 = EEPROM.readFloat(EepromECs8);
}



int FDate;
int FMonth;
int FYear;
int TDate;
int TMonth;
int TYear;


//***************Eprom Save SP****************//

int EepromSPFD = 260;
int EepromSPFM = 265;
int EepromSPTD = 270;
int EepromSPTM = 275;
//
//result SPFD() {
//  EEPROM.write(EepromSPFD, FDate);
//
//  return proceed;
//}
//result SPFM() {
//  EEPROM.write(EepromSPFM, FMonth);
//
//  return proceed;
//}
//result SPTD() {
//  EEPROM.write(EepromSPTD, TDate);
//
//  return proceed;
//}
//result SPTM() {
//  EEPROM.write(EepromSPTM, TMonth);
//
//  return proceed;
//}


float pHsp;
float ECsp;
float tempsp;

float dosp;
float nullval;
float phRefV;      //reference voltage
float EcRefV;
int EepromPhRV = 0x00;
int EepromECRV = 0x00;
result RefVRead() {
  phRefV = EEPROM.read(EepromPhRV);
  EcRefV = EEPROM.read(EepromECRV);
  return proceed;

}

//***************Eprom Save SP****************//

int EeprompHsp = 280;
int EepromECsp = 285;
int Eepromtempsp = 290;
int Eepromdosp = 295;

result SPph() {

  EEPROM.writeFloat(EeprompHsp, pHsp);

  return proceed;
}
result SPec() {
  EEPROM.writeFloat(EepromECsp, ECsp);

  return proceed;
}



result SPFTRead() {
  FDate = EEPROM.read(EepromSPFD);
  TDate = EEPROM.read(EepromSPFM);
  FMonth = EEPROM.read(EepromSPTD);
  TMonth = EEPROM.read(EepromSPTM);
  pHsp = EEPROM.readFloat(EeprompHsp);
  ECsp = EEPROM.readFloat(EepromECsp);
  tempsp = EEPROM.readFloat(Eepromtempsp);
  dosp = EEPROM.readFloat(Eepromdosp);
  return proceed;
}


///***************Read all data from memoery******************************
void readalldata() {

  //  Serial.println("SAVE DATA TO EEPROM");
  //  EepromRead();
  EepromReadEC();
  EepromReadPHCal();
  SPFTRead();


  autosavefun();//auto on Power
  //  Serial.println(autoon);
  EepromReadauto();
  RefVRead();
  Schesavefun();
  EepromReadSche();
  allReadTime();

  Readall();

}


//!******************************************************************************
//! Name: getNutrientTL()                               *
//! Description: For Doser Shedule                     *
//! Param : waterconlevel                                  *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************
bool Tankfull;
long waterconlevel;
void getNutrientTL()
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds (5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  //  Serial.print("this is duration:");
  //  Serial.println(duration);
  distanceincm = duration * 0.034 / 2;
  distanceinInch = distanceincm / 2.54;
  volume = pi * 16 * distanceincm; //change based on the volume for the tank
  waterHeight = tankHeight - distanceinInch;

  //  waterQuantity = totalVolume - volume - offset;
  waterconlevel = tankHeight - waterHeight;

  //  Serial.print("Tank level: ");
  //  Serial.println(waterconlevel);
  //  Serial.print("Tank is: ");
  //  Serial.println(distanceincm);
  //  Serial.println(" Cm");
  //  Serial.println(" Intch");
  //  Serial.println(distanceinInch);
  //  Serial.println(tankHeight);
  //  Serial.println(waterHeight);
  //  Serial.println(waterconlevel);
  //  Serial.println(" Intch");
  //Serial.print("Water level [cm]: ");
  //Serial.println(waterHeight);
  //Serial.print("Water Quantity [ml]: ");
  //Serial.println(waterQuantity);
  //    Serial.println(tanksetpoint);
  int Wtemp;
  int tankSetHysteris = 10; //10Inches
  int TankHysteris = (tanksetpoint - tankSetHysteris);
  if (distanceinInch < tanksetpoint && Wtemp == 0)
  {
    digitalWrite(RORELAYR, HIGH);
    Serial.print("Water PUMP OFF");

    Tankfull == false;

    Wtemp = 1;
  }

  else if (distanceinInch < tanksetpoint && Wtemp == 1)
  {
    digitalWrite(RORELAYR, HIGH);
    Serial.print("Water PUMP OFF");
    Tankfull == false;
  }
  else if (distanceinInch < TankHysteris)
  {
    digitalWrite(RORELAYR, LOW);
    Serial.print("Water PUMP ON");
    Tankfull == false;
    Wtemp = 0;
  }
  else if (distanceinInch < tankHeight)
  {

    Serial.print("Water PUMP ON");
    Tankfull == true;
    Wtemp = 0;
  }
  return distance;

}
//!******************************************************************************
//! Name: getNutrientTemperature()                               *
//! Description: For Doser Shedule                     *
//! Param : Temperature                                  *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

// Read water temperature
float getNutrientTemperature() {
  // Read DALLAS
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  float ntValue = (sensors.getTempCByIndex(0)); // Why "byIndex"?
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire
  ntValue = roundDecimalPoint(ntValue, 1);
  Fahrenheit = sensors.toFahrenheit(ntValue);

  nutrientTemperature = ntValue;
  //  Serial.print(nutrientTemperature, 1);
  //  Serial.println("C");
  //  Serial.print(Fahrenheit, 1);
  //  Serial.println("F");
  return ntValue;
}
//!******************************************************************************
//! Name: getNutrientPHavg()                               *
//! Description: For Doser Shedule                     *
//! Param : pH                                  *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

bool pHAvg;
float getNutrientPHavg() {

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static unsigned long timepoint = millis();
  static float voltagePHavrg, phtest;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(PH_PIN);
    if (pHArrayIndex == ArrayLenth)
      pHArrayIndex = 0;
    voltagePH = analogRead(PH_PIN) / 1024.0 * 5000;      // read the ph voltage
    phValue    = (ph.readPH(voltagePH, nutrientTemperature)) + Offset;

    voltagePHavrg = avergearray(pHArray, ArrayLenth) / 1024.0 * 5000;
    phValueavrg    = (ph.readPH(voltagePHavrg, nutrientTemperature)) + Offset;
    //    Serial.print("phValueavrg: ");
    //    Serial.println(phValueavrg);

    //        Serial.print("pH :");
    //        Serial.println(phValue, 2);
    phtest = roundDecimalPoint(phValue, 2);
    int phRv = phtest;
    //    Serial.print(phRv);
    //    Serial.println("                ");
    //        Serial.print("pH Arg:");
    //        Serial.println(phValueavrg, 2);
    int phValueavrgO = roundDecimalPoint(phValueavrg, 2);
    samplingTime = millis();

    if (phRv == phValueavrgO) {
      //            Serial.println("PH Stable");
      pHAvg = true;
    }
    else {
      //            Serial.println("PH Not Stable");
      pHAvg = false;
    }
  }
  pH = roundDecimalPoint(phValueavrg, 2);
  //Serial.print("PH raed SENSOR : ");
  //Serial.println(pH);
  return phValueavrg;

}
//Read ph value from water


//!******************************************************************************
//! Name: getNutrientECavg()                               *
//! Description: For Doser Shedule                     *
//! Param : EC AVRAGE                                 *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

bool ECAvg;
float getNutrientECavg() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static unsigned long timepoint = millis();
  static float voltageECavrg, ECtest;

  if (millis() - samplingTime > samplingInterval) {
    ECArray[ECArrayIndex++] = analogRead(EC_PIN);
    if (ECArrayIndex == ArrayLenth)
      ECArrayIndex = 0;
    voltageEC = analogRead(EC_PIN) / 1024.0 * 5000;      // read the ph voltage
    ecValue        = ec.readEC(voltageEC, nutrientTemperature) + Offsetec;

    voltageECavrg = avergearrayec(ECArray, ArrayLenth) / 1024.0 * 5000;
    float EC_Value_avrg = ((float)(ec.readEC(voltageECavrg, nutrientTemperature) + Offsetec));
    //    float EC_Value_avrg = ((float)(ec.readEC(voltageECavrg, nutrientTemperature) + Offsetec));
    //    Serial.println(EC_Value_avrg);
    String EC_Value_str = String(EC_Value_avrg, 2);
    //    Serial.println(EC_Value_str);
    ECValueavrg = EC_Value_str.toFloat();
    //    float fjskldf = EC_Value_avrg * 1000;
    //    float printec =roundf(EC_Value_avrg * 1000);
    //    Serial.println(printec,2);
    //    fjskldf = (printec*1000)/1000;
    //    Serial.println(fjskldf);
    //      ECValueavrg = fjskldf;

    //    String neweval = String(EC_Value_avrg, 2);
    //    String neweval = " ";
    //    newval.concat(EC_Value_avrg);
    //    Serial.println(neweval);
    //    Serial.println(EC_Value_avrg);
    ////    String neweval = " ";
    ////    neweval.concat(ec.readEC(voltageECavrg, nutrientTemperature) + Offsetec);
    //    char buffr[100];
    //    neweval.toCharArray(buffr, newval.length());
    //    char *ecavg = strtok(buffr, "e");
    //    String testvalecavg = ecavg;
    //    ECValueavrg = testvalecavg.toFloat();
    //    Serial.println(ECValueavrg);
    //    String EC_Value_str = String(EC_Value_avrg, 2);

    //    neweval = String(EC_Value_avrg, 2);
    //    ECValueavrg = EC_Value_str.toFloat();


    //    float ECval   = (ECValueavrg, 2);

    //    Serial.print("_________________");
    //        Serial.print("EC :");
    //        Serial.println(ECval);

    ECtest = roundDecimalPoint(ecValue, 2);
    int ECRv = ECtest;
    //    Serial.print(ECRv);
    //    Serial.println(voltageEC);
    //    Serial.println(ecValue);
    //    Serial.println(voltageECavrg);
    getNutrientTemperature();
    //    Serial.println(nutrientTemperature);//nutrientTemperature
    //    //    Serial.println("                ");
    //        Serial.print("EC Arg:");
    //        Serial.println(ECValueavrg, 2);
    //        Serial.println(ECValueavrg);

    int ECValueavrgO = roundDecimalPoint(ECValueavrg, 2);
    samplingTime = millis();

    //    Serial.println(ecValue);
    //TDS
    float tdsValue = ecValue * 500;
    tdsValue = roundDecimalPoint(tdsValue, 2);
    nutrientTDS = tdsValue ;
    //    Serial.println(nutrientTDS);

    //CF

    float cfValue = ecValue * 10;
    nutrientCF = cfValue;
    //    Serial.println(nutrientCF);

    if (ECRv == ECValueavrgO) {
      //            Serial.println("EC Stable");
      ECAvg = true;
    }
    else {
      //            Serial.println("EC Not Stable");
      ECAvg = false;
    }
  }
  //  Serial.print("_________________");
  EC = roundDecimalPoint(ECValueavrg, 2);
  //  Serial.println(EC);
  //Serial.print("EC raed SENSOR : ");
  //  Serial.println(EC);
  return ECValueavrg;
  //  Serial.println(ECValueavrg);
}
//!******************************************************************************
//! Name: getNutrientEC()                               *
//! Description: For Doser Shedule                     *
//! Param : EC                                     *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

float getNutrientEC() {

  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {
    timepoint = millis();
    voltageEC = analogRead(EC_PIN) / 1024.0 * 5000;
    ecValue    = (ec.readEC(voltageEC, nutrientTemperature)) + 0.25;    // convert voltage to EC with temperature compensation
    unitConduct = ecValue;
    //    Serial.println(unitConduct);
    //    Serial.print(", EC:");
    //    Serial.print(ecValue, 2);
    //    Serial.print("ms/cm");
    return ecValue;
  }
}

//!******************************************************************************
//! Name: getNutrientTDS()                               *
//! Description: For Doser Shedule                     *
//! Param : EC- TDS                                  *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

float getNutrientTDS() {
  float tdsValue = ecValue * 500;
  tdsValue = roundDecimalPoint(tdsValue, 2);

  nutrientTDS = tdsValue ;
  unitConduct = nutrientTDS;
  //  Serial.println(unitConduct);
  //  Serial.print("EC Value:");
  //  Serial.print(nutrientEC,2);
  return tdsValue;
}
//!******************************************************************************
//! Name: getNutrientCF()                               *
//! Description: For Doser Shedule                     *
//! Param : EC- CF                                     *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

float getNutrientCF() {
  float cfValue = ecValue * 10;
  nutrientCF = cfValue;
  //  Serial.println(cfValue);
  //  unitConduct = nutrientCF;
  //  Serial.print("CF Value:");
  //  Serial.println(nutrientCF,0);
  return cfValue;
}

//!******************************************************************************
//! Name: getNutrientDO()                               *
//! Description: For Doser Shedule                     *
//! Param : DO Sensor                                    *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

float getNutrientDO() {
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;

  // Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
  // Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
  //Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
  //Serial.println("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");
  float DO = (readDO(ADC_Voltage, Temperaturet));
  float DOvalue = DO;
  //Serial.print(DOvalue);

  return DOvalue;
}
bool phAutoon;
//***************************************AUTO DOSAER *******************************************
//!******************************************************************************
//! Name: DoserAuto()                               *
//! Description: For Doser Shedule                     *
//! Param :                                   *
//! Returns: void                                     *
//! Example:DoserAuto(float pHAutoSP, float pHcalibration, float ECAutoSP, float ECHys )             *
//!******************************************************************************

void DoserAuto(float pHAutoSP, float pHHys, float ECAutoSP, float ECHys ) {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  Serial.print(F("DoserAuto() is running."));
  EepromReadPHCal();
  EepromReadEC();
  digitalWrite(LEDGREEN, HIGH);


//  Serial.print(F(pHHys));
//  Serial.print(F("pH automatic start"));
//  Serial.print(F("pH SETPOINT : "));
//  Serial.print(F(pHAutoSP));
//  Serial.print(F("EC SETPOINT : "));
//  Serial.print(F(ECAutoSP));
//  Serial.print(F(pHHys));
  //Sampler pump and mixer relay ON...
  digitalWrite(TPUMPR, LOW);//h
  digitalWrite(MIXERR, LOW);

  pHSetpoint = pHAutoSP ;
  phSetHysteris = pHHys;
  pH = roundDecimalPoint(phValueavrg, 2);
//  Serial.print(F("PH SENSOR : "));
//  Serial.print(F(pH));
//  Serial.print(F("EC HYS : "));
//  Serial.print(F(ECHys));

  float  ECSetpoint = ECAutoSP;
  //  ECAutoSP =ECSetpoint;
  ECSetHysteris = ECHys;
  Serial.print(F("EC setpoint : "));
  Serial.print(ECSetpoint);
  EC = roundDecimalPoint(ecValue, 2);

  Serial.print(F("EC SENSOR : "));
  Serial.print(EC);

  float HysterisMin = (pHSetpoint - phSetHysteris);
  float HysterisPlus = (pHSetpoint + phSetHysteris);
  Serial.print(F("PH HYS MIN : "));
  Serial.println(HysterisMin);
  Serial.print(F("PH HYS MAX : "));
  Serial.print(HysterisPlus);

  float ECHysterisMin = (ECSetpoint - ECSetHysteris);
  float ECHysterisPlus = (ECSetpoint + ECSetHysteris);
  Serial.print(F("EC HYS MIN : "));
  Serial.print(ECHysterisMin);
  Serial.print(F("EC HYS MAX : "));
  Serial.print(ECHysterisPlus);

  //  if (StopPHHys == false) {
  if (pH == pHSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      digitalWrite(PHHIGH_DIR_PIN, LOW);
      //digitalWrite(PHLOW_DIR_PIN, LOW);
      for (int X = 0; X < 1000; X++) {
        //digitalWrite(PHLOW_STEP_PIN, HIGH);
        digitalWrite(PHHIGH_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(PHHIGH_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        Serial.print(F("PH == pHSetpoint"));
      }

      Serial.print("ph Max pump and Min pump are OFF");
      phDown = 0;
      phUp = 0;
      pinTime = pinLowTime;
    }
  }

  if (pH < HysterisMin) {
    digitalWrite(TPUMPR, LOW);//h
    digitalWrite(MIXERR, LOW);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      digitalWrite(PHHIGH_DIR_PIN, LOW);
      //digitalWrite(PHLOW_DIR_PIN, LOW);
      for (int X = 0; X < 1000; X++) {
        //digitalWrite(PHLOW_STEP_PIN, HIGH);
        digitalWrite(PHHIGH_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PHHIGH_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      Serial.print(F("ph UP pump is HIGH"));
    }
  }
  if (pH >= HysterisMin && pH < pHSetpoint) {


    Serial.print(F("ph EQL2"));

  }


  if (pH > HysterisPlus) {

    digitalWrite(TPUMPR, LOW);//h
    digitalWrite(MIXERR, LOW);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      digitalWrite(PHLOW_DIR_PIN, LOW);
      //digitalWrite(PHLOW_DIR_PIN, LOW);
      for (int X = 0; X < 1000; X++) {
        digitalWrite(PHLOW_STEP_PIN, HIGH);
        //digitalWrite(PHLOW_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(PHLOW_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        phDown = 1;
        phUp = 0;
        pinTime = pinLowTime;
      }
      Serial.print(F("ph DOWN pump is HIGH HysterisPlus"));

    }
  }
  //  if (pH <= HysterisPlus && pH > pHSetpoint)
  if (pH <= HysterisPlus && pH > pHSetpoint) {

    Serial.print(F("PH EQL"));
    //    unsigned long currentMillis = millis();
    //    if (currentMillis - previousMillis > pinTime) {
    //      previousMillis = currentMillis;
    //
    //      digitalWrite(PHLOW_DIR_PIN, LOW);
    //      //digitalWrite(PHLOW_DIR_PIN, LOW);
    //      for (int X = 0; X < 1000; X++) {
    //        digitalWrite(PHLOW_STEP_PIN, HIGH);
    //        //digitalWrite(PHLOW_STEP_PIN, LOW);
    //        delayMicroseconds(Speed);
    //        digitalWrite(PHLOW_STEP_PIN, LOW);
    //        delayMicroseconds(Speed);
    //
    //        Serial.print("ph DOWN pump is HIGH");
    //        phUp = 0;
    //        phDown = 1;
    //        pinTime = pinLowTime;
    //      }
    //    }
  }

  //********************EC DOSER ******************************
  if ((pH >= HysterisMin && pH <= HysterisPlus)) {
    //           digitalWrite(TPUMPR, HIGH);
    //           digitalWrite(MIXERR, HIGH);
    //      StopPHHys == true;
    Serial.print(F("PH EQL & EC Correction START"));
    // if(StopPHHys == true){


  }
  //EC FUNtion
  if (EC == ECSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      ///ec ABC low

      Serial.print(F("EC SETPoit EQL"));
      phDown = 0;
      phUp = 0;
      pinTime = pinLowTime;
    }
  }
  if (EC >= ECHysterisMin && EC <= ECHysterisPlus) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      //LOW

      int ECRA =  ECRatioA * 230;
      digitalWrite(EC_A_DIR_PIN, LOW);
      for (int X = 0; X < ECRA; X++) {
        digitalWrite(EC_A_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(EC_A_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      int ECRB =  ECRatioB * 230;
      digitalWrite(EC_B_DIR_PIN, LOW);

      for (int X = 0; X < ECRB; X++) {
        digitalWrite(EC_B_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(EC_B_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      //          int ECRC =  ECRatioC * 230;
      //          digitalWrite(EC_C_DIR_PIN, LOW);
      //          for (int X = 0; X < ECRC; X++) {
      //            digitalWrite(EC_C_STEP_PIN, LOW);
      //            delayMicroseconds(Speed);
      //            digitalWrite(EC_C_STEP_PIN, LOW);
      //            delayMicroseconds(Speed);
      //
      //          }


      Serial.print(F("PH & EC EQL"));
      phUp = 0;
      phDown = 0;
      pinTime = pinLowTime;
    }
  }
  if (EC >= ECHysterisMin && EC < ECSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      int ECRA =  ECRatioA * 230;
      digitalWrite(EC_A_DIR_PIN, LOW);
      for (int X = 0; X < ECRA; X++) {
        digitalWrite(EC_A_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(EC_A_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      int ECRB =  ECRatioB * 230;
      digitalWrite(EC_B_DIR_PIN, LOW);

      for (int X = 0; X < ECRB; X++) {
        digitalWrite(EC_B_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        digitalWrite(EC_B_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      //          int ECRC =  ECRatioC * 230;
      //          digitalWrite(EC_C_DIR_PIN, LOW);
      //          for (int X = 0; X < ECRC; X++) {
      //            digitalWrite(EC_C_STEP_PIN, LOW);
      //            delayMicroseconds(Speed);
      //            digitalWrite(EC_C_STEP_PIN, LOW);
      //            delayMicroseconds(Speed);
      //
      //          }
      //need to check
      Serial.print(F("EC EQL"));
      phUp = 1;
      phDown = 0;
      pinTime = pinHighTime;
    }
  }

  if (EC < ECHysterisMin) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      int ECRA =  ECRatioA * 230;
      digitalWrite(EC_A_DIR_PIN, LOW);
      for (int X = 0; X < ECRA; X++) {
        digitalWrite(EC_A_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(EC_A_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      int ECRB =  ECRatioB * 230;
      digitalWrite(EC_B_DIR_PIN, LOW);

      for (int X = 0; X < ECRB; X++) {
        digitalWrite(EC_B_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(EC_B_STEP_PIN, LOW);
        delayMicroseconds(Speed);
      }
      //          int ECRC =  ECRatioC * 230;
      //          digitalWrite(EC_C_DIR_PIN, LOW);
      //          for (int X = 0; X < ECRC; X++) {
      //            digitalWrite(EC_C_STEP_PIN, HIGH);
      //            delayMicroseconds(Speed);
      //            digitalWrite(EC_C_STEP_PIN, LOW);
      //            delayMicroseconds(Speed);
      //
      //          }
    }
    Serial.print(F("EC A and B  HIGH"));

  }

  //0.19>0.30
  if (EC > ECHysterisPlus) {
    // if (ECHysterisPlus  > EC) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      digitalWrite(LEDRED, HIGH);
      //RO WATER ON
      if (Tankfull = true) {
        digitalWrite( RORELAYR, HIGH);
      }
      else {
        Serial.print(F("TankFull"));

      }

      Serial.print(F("EC RO Relay ON"));
      phDown = 1;
      phUp = 0;
      pinTime = pinLowTime;

    }
  }

  if (EC <= ECHysterisPlus && EC > ECSetpoint) {


    Serial.print(F("EC EQL"));

  }

}

//}

///*****************************************SHEDULER PH AND EC *****************************************************
//!******************************************************************************
//! Name: SheduleDoser()                               *
//! Description: For Doser Shedule                     *
//! Param : DATE /TIME                                      *
//! Returns: void                                     *
//! Example:SheduleDoser(float pHAutoSetValue, float pHHys, float ECAutoSetValue, float ECHys )               *
//!******************************************************************************

void SheduleDoser(float pHAutoSetValue, float pHHys, float ECAutoSetValue, float ECHys ) {
  digitalWrite(LEDGREEN, HIGH);
  //  Serial.print("pH Sheduler start");
  //  Serial.print("pH SETPOINT : ");
  //  Serial.println(pHAutoSetValue);
  //  Serial.print("EC SETPOINT : ");
  //  Serial.println(ECAutoSetValue);

  digitalWrite(TPUMPR, LOW);//h
  digitalWrite(MIXERR, LOW);

  pHSetpoint = pHAutoSetValue ;

  //  Serial.print("shceduledoser.....phaurosetvalue:");
  //  Serial.println(pHAutoSetValue);
  phSetHysteris = pHHys;
  pH = roundDecimalPoint(phValue, 2);
  //  Serial.print("PH SENSOR : ");
  //  Serial.println(pH);

  ECSetpoint = ECAutoSetValue;
  ECHys = ECSetHysteris;
  EC = roundDecimalPoint(ecValue, 2);
  //  Serial.print("EC SENSOR : ");
  //  Serial.println(EC);

  float HysterisMin = (pHSetpoint - phSetHysteris);
  float HysterisPlus = (pHSetpoint + phSetHysteris);
  //  Serial.print("PH HYS MIN : ");
  //  Serial.println(HysterisMin);
  //  Serial.print("PH HYS MAX : ");
  //  Serial.println(HysterisPlus);

  ECHysterisMin = (ECSetpoint - ECSetHysteris);
  ECHysterisPlus = (ECSetpoint + ECSetHysteris);
  //  Serial.print("EC HYS MIN : ");
  //  Serial.println(ECHysterisMin);
  //  Serial.print("EC HYS MAX : ");
  //  Serial.println(ECHysterisPlus);

  if (StopPHHys == false) {
    if (pH == pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }

        //          Serial.println("ph Max pump and Min pump are OFF");
        phDown = 0;
        phUp = 0;
        pinTime = pinLowTime;
      }
    }

    if (pH < HysterisMin) {
      digitalWrite(TPUMPR, LOW);//h
      digitalWrite(MIXERR, LOW);
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        //      Serial.println("ph Max pump is HIGH");
      }
    }
    if (pH >= HysterisMin && pH < pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          //            Serial.println("ph Max pump is HIGH");
          phUp = 1;
          phDown = 0;
          pinTime = pinHighTime;
        }
      }
    }
    if (pH > HysterisPlus) {
      digitalWrite(TPUMPR, LOW);//h
      digitalWrite(MIXERR, LOW);
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHLOW_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          digitalWrite(PHLOW_STEP_PIN, HIGH);
          //digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);

          phDown = 1;
          phUp = 0;
          pinTime = pinLowTime;
        }
        //      Serial.println("ph Min pump is HIGH");

      }
    }

    if (pH <= HysterisPlus && pH > pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHLOW_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          digitalWrite(PHLOW_STEP_PIN, HIGH);
          //digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);

          //            Serial.println("ph Min pump is HIGH");
          phUp = 0;
          phDown = 1;
          pinTime = pinLowTime;
        }
      }
    }

    //********************EC DOSER ******************************
    if ((pH >= HysterisMin && pH <= HysterisPlus)) {
      //           digitalWrite(TPUMPR, HIGH);
      //           digitalWrite(MIXERR, HIGH);
      //      StopPHHys == true;
      //      Serial.println("PH EQL & EC Correction START");
      // if(StopPHHys == true){

      //EC FUNtion
      if (EC == ECSetpoint) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis > pinTime) {
          previousMillis = currentMillis;

          ///ec ABC low

          //          Serial.println("EC + and - are OFF");
          phDown = 0;
          phUp = 0;
          pinTime = pinLowTime;
        }
      }
    }
    if (EC >= ECHysterisMin && EC <= ECHysterisPlus) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        //LOW

        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        //          int ECRC =  ECRatioC * 230;
        //          digitalWrite(EC_C_DIR_PIN, LOW);
        //          for (int X = 0; X < ECRC; X++) {
        //            digitalWrite(EC_C_STEP_PIN, LOW);
        //            delayMicroseconds(Speed);
        //            digitalWrite(EC_C_STEP_PIN, LOW);
        //            delayMicroseconds(Speed);
        //
        //          }
        Serial.print(F("EC EQL "));


        //        Serial.print("EC MIN and MAX are LOW (hysteria correction)");
        phUp = 0;
        phDown = 0;
        pinTime = pinLowTime;
      }
    }
    if (EC >= ECHysterisMin && EC < ECSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        //          int ECRC =  ECRatioC * 230;
        //          digitalWrite(EC_C_DIR_PIN, LOW);
        //          for (int X = 0; X < ECRC; X++) {
        //            digitalWrite(EC_C_STEP_PIN, LOW);
        //            delayMicroseconds(Speed);
        //            digitalWrite(EC_C_STEP_PIN, LOW);
        //            delayMicroseconds(Speed);
        //
        //          }
        Serial.print(F("EC hys min EQL "));
        //        Serial.print("EC MAX pin is HIGH");
        phUp = 1;
        phDown = 0;
        pinTime = pinHighTime;
      }
    }

    if (EC < ECHysterisMin) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        Serial.print(F("A B ROTATE"));
        //          int ECRC =  ECRatioC * 230;
        //          digitalWrite(EC_C_DIR_PIN, LOW);
        //          for (int X = 0; X < ECRC; X++) {
        //            digitalWrite(EC_C_STEP_PIN, HIGH);
        //            delayMicroseconds(Speed);
        //            digitalWrite(EC_C_STEP_PIN, LOW);
        //            delayMicroseconds(Speed);
        //
        //          }
      }
      //          Serial.print("EC doser  HIGH");

    }


    if (EC > ECHysterisPlus) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        digitalWrite(LEDRED, HIGH);
        Serial.print(F("RO ON"));
        //RO WATER ON
        digitalWrite( RORELAYR, HIGH);

        //        Serial.print("ph - pin is HIGH");
        phDown = 1;
        phUp = 0;
        pinTime = pinLowTime;

      }
    }

    if (EC <= ECHysterisPlus && EC > ECSetpoint) {
      //      unsigned long currentMillis = millis();
      //      if (currentMillis - previousMillis > pinTime) {
      //        previousMillis = currentMillis;
      //        digitalWrite(LEDRED, HIGH);
      //
      //        //RO WATER ON
      //        digitalWrite( RORELAYR, HIGH);
      //
      //        //        Serial.print("ph - pin is HIGH");
      //        phUp = 0;
      //        phDown = 1;
      //        pinTime = pinLowTime;
      //      }
      Serial.print(F("EC hys plus EQL "));
    }
  }


}

//*************************************  TIME SHEDULER   ***********************************************
//!******************************************************************************
//! Name: scheduleFun()                               *
//! Description: For Doser Shedule                     *
//! Param : DATE /TIME                                      *
//! Returns: void                                     *
//! Example:               *
//!******************************************************************************


void scheduleFun() {
  Serial.print(F("scheduleFun() is running "));
  digitalWrite(LEDGREEN, HIGH);
  digitalWrite(TPUMPR, HIGH);
  digitalWrite(MIXERR, HIGH);
  if (pHAvg == true && ECAvg == true) {
    static byte trigger = 1;
    dateandtime();
    Serial.print(F("RTC time"));
    Serial.print(hourupg);
    Serial.print(minupg);
    Serial.print(F("Slot Set Time"));
    //
//    Serial.println(S1Hr);
//    Serial.println(S1Min);
    if (hourupg <= S1Hr && minupg <= S1Min && Resetfun == false) {
      saveslot1();
      S1Read();
//      Serial.print(F("RTC time"));
//      Serial.println(hourupg);
//      Serial.println(minupg);
//      Serial.print(F("Slot Set Time"));

//      Serial.println(S1Hr);
//      Serial.println(S1Min);

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs1;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs1;
      float ECHys =  ECSetHysteris;
      Serial.print(F("schedule func() phs1: "));
//      Serial.println(pHs1);
//      Serial.print("schedule func() phautosetvale: ");
//      Serial.println(pHAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
//      Serial.println("PH SETPOINTS");
//      Serial.println(pHAutoSetValue);
//      Serial.println("PH HYS  SETPOINTS");
//      Serial.println(pHHys);
//      Serial.println("EC SETPOINTS");
//      Serial.println(ECAutoSetValue);
//      Serial.println("EC HYS  SETPOINTS");
//      Serial.println(ECHys);
//
//      Serial.println("Shift 1 Progressing");

    }
    //
    if (hourupg <= S2Hr && minupg <= S2Min && Resetfun == false) {
//      Serial.print("RTC time");
//      Serial.println(hourupg);
//      Serial.println(minupg);

//      Serial.print("Slot Set Time");
//      Serial.println(S2Hr);
//      Serial.println(S2Min);

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs2;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs2;
      float ECHys =  ECSetHysteris;
      //      Serial.print("schedule func() phs2: ");
      //      Serial.println(pHs2);
      //      Serial.print("schedule func() phautosetvale: ");
      //      Serial.println(pHAutoSetValue);
      //      Serial.print("schedule func() ec2: ");
      //      Serial.println(ECs2);
      //      Serial.print("schedule func() ECAutoSetValue: ");
      //      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      //      Serial.println("PH SHIFT 2 SETPOINTS");
      //      Serial.println(pHAutoSetValue);
      //      Serial.println("PH  HYS  SETPOINTS");
      //      Serial.println(pHHys);
      //      Serial.println("EC SHIFT 2 SETPOINTS");
      //      Serial.println(ECAutoSetValue);
      //      Serial.println("EC HYS  SETPOINTS");
      //      Serial.println(ECHys);
      //
      //      Serial.println("Shift 2 Progressing");

    }
    if (hourupg <= S3Hr && minupg <= S3Min && Resetfun == false) {
      //      Serial.print("RTC time");
      //      Serial.println(hourupg);
      //      Serial.println(minupg);
      //
      //      Serial.print("Slot Set Time");
      //      Serial.println(S3Hr);
//      Serial.println(S3Min);
      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs3;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs3;
      float ECHys =  ECSetHysteris;
      //      Serial.print("schedule func() phs3: ");
      //      Serial.println(pHs3);
      //      Serial.print("schedule func() phautosetvale: ");
      //      Serial.println(pHAutoSetValue);
      //      Serial.print("schedule func() ec3: ");
      //      Serial.println(ECs3);
      //      Serial.print("schedule func() ECAutoSetValue: ");
      //      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      //      Serial.println("PH SHIFT 2 SETPOINTS");
      //      Serial.println(pHAutoSetValue);
      //      Serial.println("PH  HYS  SETPOINTS");
      //      Serial.println(pHHys);
      //      Serial.println("EC SHIFT 2 SETPOINTS");
      //      Serial.println(ECAutoSetValue);
      //      Serial.println("EC HYS  SETPOINTS");
      //      Serial.println(ECHys);
      //
      //      Serial.println("Shift 3 Progressing");
    }

    if (hourupg <= S4Hr && minupg <= S4Min && Resetfun == false) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      //      Serial.print("Slot Set Time");
      //      Serial.println(S4Hr);
      //      Serial.println(S4Min);
      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs4;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs4;
      float ECHys =  ECSetHysteris;
      //      Serial.print("schedule func() phs4: ");
      //      Serial.println(pHs4);
      //      Serial.print("schedule func() phautosetvale: ");
      //      Serial.println(pHAutoSetValue);
      //      Serial.print("schedule func() ec4: ");
      //      Serial.println(ECs4);
      //      Serial.print("schedule func() ECAutoSetValue: ");
      //      Serial.println(ECAutoSetValue);
      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      //      Serial.println("PH SHIFT 2 SETPOINTS");
      //      Serial.println(pHAutoSetValue);
      //      Serial.println("PH  HYS  SETPOINTS");
      //      Serial.println(pHHys);
      //      Serial.println("EC SHIFT 2 SETPOINTS");
      //      Serial.println(ECAutoSetValue);
      //      Serial.println("EC HYS  SETPOINTS");
      //      Serial.println(ECHys);
      //
      //      Serial.println("Shift 4 Progressing");
    }
    if (hourupg == S5Hr && minupg == S5Min && Resetfun == false) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
//      Serial.print("Slot Set Time");
      //      Serial.println(S5Hr);
      //      Serial.println(S5Min);
      float pHAutoSetValue =  pHs5;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs5;
      float ECHys =  ECSetHysteris;
      //      Serial.print("schedule func() phs5: ");
      //      Serial.println(pHs5);
      //      Serial.print("schedule func() phautosetvale: ");
      //      Serial.println(pHAutoSetValue);
      //      Serial.print("schedule func() ec5: ");
      //      Serial.println(ECs5);
      //      Serial.print("schedule func() ECAutoSetValue: ");
      //      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      //      Serial.println("PH SHIFT 5 SETPOINTS");
      //      Serial.println(pHAutoSetValue);
      //      Serial.println("PH  HYS  SETPOINTS");
      //      Serial.println(pHHys);
      //      Serial.println("EC SHIFT 5 SETPOINTS");
      //      Serial.println(ECAutoSetValue);
      //      Serial.println("EC HYS  SETPOINTS");
      //      Serial.println(ECHys);
      //
      //      Serial.println("Shift 5 Progressing");
    }

    if (hourupg == S6Hr && minupg == S6Min && Resetfun == false) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
//      Serial.println(S6Hr);
//      Serial.println(S6Min);
      float pHAutoSetValue =  pHs6;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs6;
      float ECHys =  ECSetHysteris;
//      Serial.print("schedule func() phs6: ");
//      Serial.println(pHs6);
//      Serial.print("schedule func() phautosetvale: ");
//      Serial.println(pHAutoSetValue);
//      Serial.print("schedule func() ec6: ");
//      Serial.println(ECs6);
//      Serial.print("schedule func() ECAutoSetValue: ");
//      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
//      Serial.println("PH SHIFT 6 SETPOINTS");
//      Serial.println(pHAutoSetValue);
//      Serial.println("PH  HYS  SETPOINTS");
//      Serial.println(pHHys);
//      Serial.println("EC SHIFT 6 SETPOINTS");
//      Serial.println(ECAutoSetValue);
//      Serial.println("EC HYS  SETPOINTS");
//      Serial.println(ECHys);

//      Serial.println("Shift 6 Progressing");
    }

  }
}
bool Wifiset;



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
  hourupg = now.hour();//hourupg
  minupg = now.minute();//minupg
  secslive = now.second();//secslive
  Datelive = now.day();//Datelive
  Monthlive = now.month();//Monthlive
  Yearlive = now.year();//Yearlive
}


float roundDecimalPoint( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}

double avergearrayec(int * arr, int number) {
  int i;
  int max,
      min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println(F("Error number for the array to avraging!/n"));
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    } else {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min; //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max; //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      } //if
    } //for
    avg = (double)amount / (number - 2);
  } //if
  return avg;
}
double avergearray(int * arr, int number) {
  int i;
  int max,
      min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println(F("Error number for the array to avraging!/n"));
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    } else {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min; //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max; //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      } //if
    } //for
    avg = (double)amount / (number - 2);
  } //if
  return avg;
}

//!******************************************************************************
//! Name: regulateNutrient()                                  *
//! Description:                      *
//! Param : void                                      *
//! Returns: void                                     *
//! Example: regulateNutrient(float temperature);              *
//!******************************************************************************

void regulateNutrient(float temperature)
{
  //Regulator Control - Regulator in socket 5
  if (temperature < 25 && temperature > 0) { //>0 to avoid -999.00*C error
    //    digitalWrite(HEATERR, HIGH);

    //    Serial.println("Heaating...");
  }

  if (temperature > 26 || temperature < 0) { //<0 to avoid -999.00*C error
    //    digitalWrite(HEATERR, LOW);

    //    Serial.println("Heater standby");
  }

  // //Cooling Fan Control - Fan in socket 4
  if (temperature > 27) {
    //    digitalWrite(COOLER_OPIN, HIGH);

    //    Serial.println("Cooling...");
  }

  if (temperature < 26) {
    //    digitalWrite(COOLER_OPIN, LOW);

    //    Serial.println("Cooler standby");
  }

  // Alarm if temperature is too high
  if (temperature > 30) {
    digitalWrite (BUZZER, HIGH);
    delay(100);
    digitalWrite (BUZZER, LOW);
    delay(100);
    digitalWrite (BUZZER, HIGH);
    delay(100);
    digitalWrite (BUZZER  , LOW);
    delay(100);
  }

}


void sensorjson() {

  StaticJsonDocument<500> doc;

  //  doc["DeviceID"] = "SPF-GDCZ01";
  //  doc["RTCNow"] = "t";

  //  JsonObject NutrientData = doc.createNestedObject("NutrientData");
  //  DynamicJsonDocument doc(1024);
  doc["pH"] = (pH);
  doc["EC"] = (ECValueavrg);
  doc["NT"] = (nutrientTemperature);
  doc["DO"] = (DOvalue);
  doc["NL"] = (waterconlevel);

  //  JsonObject RelayStatus = doc.createNestedObject("RelayStatus");
  doc["Sampler"] = sampumpSt;
  doc["Heater"] = HeaterSt;
  doc["Mixer"] = mixerSt;
  doc["RO"] = RoSt;
  //
  //  //  JsonObject SetPoints = doc.createNestedObject("SetPoints");
  doc["pHSetPA"] = ( pHAutoSP);
  doc["ECSetPA"] = ( ECAutoSP);
  //    doc["pHHyster"] = ( pHHys);
  //    doc["ECHyster"] = (ECHys);

  serializeJson(doc, Serial3);
  serializeJsonPretty(doc, Serial);

}



//void shedtask() {
//  if (ScheduleAuto == true) {
//    t6.enable();
//    Serial.println("Sheduler dose ON");
//  }
//  else if (ScheduleAuto == false)
//  {
//    t6.disable();
//  }
//
//}

/********************************************************************************************    ***********************************************************/

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


//**************************_________NEXTION - RTC___________***********************/
void timeSec() {
  //  Serial.println(dfd);
  val = dfd.indexOf("s") + 1; //clkd24m2y2022h2n58s30
  //  Serial.println(val);
  dfd.remove(0, val);
  //  Serial2.print("sec:");
  //  Serial2.println(dfd);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
  //  Serial.print("Seconds: ");
  //  Serial.println(secRc);
}
int timeMin() {
  //  Serial.println(buff);
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  //  Serial.println(cSmn);
  val = cSmn.indexOf("n") + 1; //    Serial.println(val);
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
  //  Serial.print("Minutes: ");
  //  Serial.println(minRc);
  return (minRc);
}
void timeHr() {
  //  Serial.println(buff);
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1; //    Serial.println(val);
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;
  //  Serial.print("Hours: ");
  //  Serial.println(hrRc);

}
void timeDt() {
  //  Serial.println(buff);
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  //  Serial.println(str);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
  //  Serial.print("Date: ");
  //  Serial.println(Daterc);

}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  //  Serial.println(cSmnt);
  val = cSmnt.indexOf("m") + 1; //    Serial.println(val);
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Monthrc = mntRc;
  //  Serial.print("Month: ");
  //  Serial.println(Monthrc);

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //    Serial.println(val);
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Yearrc = yrRc;
  //  Serial.print("Year: ");
  //  Serial.println(Yearrc);
}

/**************************    WIFI - CLOUD     ***********************/
String pwd(void) {
  int val = dfd.indexOf("<") + 1;
  dfd.remove(0, val);
  return (dfd);
}
String pwdname(void) {
  char *pd = strtok(buff, "<");
  str = pd;
  str.remove(0, 4);
  str = str;
  return (str);
}

/**************************    MESH     ***********************/
void port(void) {
  int val = dfd.indexOf(">") + 1;
  dfd.remove(0, val);//port
  //  Serial.print("Port: ");
  //  Serial.println(dfd);
}
void meshpass() {
  char *pd = strtok(buff, ">");
  str = pd;
  int val = str.indexOf("<") + 1;
  str.remove(0, val);//meshpass
  str = str;

  //  Serial.print("Mesh Pswd: ");
  //  Serial.println(str);
}

void prefix() {
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
  //  Serial.print("Prefix: ");
  //  Serial.println(str1);
}
/**************************    SCHEDULE MODE TIME     ***********************/
int sendhour(void) {
  hr = strtok(buff, "m");
  str = hr;
  val = str.indexOf("h") + 3; //    Serial.println(val);
  str.remove(0, val);
  shr = str.toInt();
  //    Serial.print("Hr = ");
  //    Serial.println(shr);
  return (shr);
}
int sendmin(void) {
  val = dfd.indexOf("m") + 3; //
  dfd.remove(0, val);
  //    Serial.println(dfd);
  ////    Serial.println(dfd);
  smn = dfd.toInt();
  ////    Serial.print("Min = ");
  ////    Serial.println(smn);
  return (smn);
}

int ratio_A() {
  hr = strtok(buff, "m");
  str = hr;
  val = str.indexOf("r") + 3; //    Serial.println(val);
  str.remove(0, val);
  int rhr = str.toInt();
  Serial.print(F("ratio_A = "));
  Serial.println(rhr);
  return (rhr);
}

int ratio_B() {
  val = dfd.indexOf("m") + 3; //
  dfd.remove(0, val);
  //    Serial.println(dfd);
  ////    Serial.println(dfd);
  int rmn = dfd.toInt();
  Serial.print("ratio_B = ");
  Serial.println(rmn);
  return (rmn);
}
/**************************    AUTO MODE     ***********************/
void PhData() {
  ph1 = strtok(buff, "e");  // break string into part using 'e' as delimiter,   // buff is string here
  //  Serial.println(buff);
  //  delay(500);
  //  Serial.println("after buff");
  //  Serial.println(buff);
  str = ph1;
  str.remove(0, 6);

  //  Serial.println(str);
}
void EcData() {
  //  ec1 = strtok(buff, "d");
  //  str = ec1;
  //  Serial.println(dfd);
  val = dfd.indexOf("ec") + 3;
  //  delay(500);
  //  Serial.println(val);
  dfd.remove(0, val);
  //  Serial.println("ec removed");
  //    Serial.println(dfd);
}

/**************************    CALIBRATION     ***********************/
void saveph() {
  val = dfd.indexOf("ph") + 2;
  //  delay(500);
  dfd.remove(0, val);
  //  smn = dfd.toInt();
  //  Serial.print("this is ph: ");
  savpH = dfd.toFloat();
  //  Serial.println(savpH);
}
//
void savec() {
  val = dfd.indexOf("c") + 1;
  //  delay(500);
  dfd.remove(0, val);
  //  smn = dfd.toInt();
  //  Serial.print("this is Ec: ");
  savEc = dfd.toFloat();
  //  Serial.println(savEc);
}
/******************************** SLOT1-pH *************************/
void sendph1() {
  PhData();
  pHs1 = float(str.toInt()) / 100;
  Serial.print("Slot1 - pH = ");
  Serial.println(pHs1);
  EEPROM.updateFloat(EeprompHs1, pHs1);
  //    S2Read();

  Serial.print(" eeprom save phs2= ");
  Serial.println(pHs2);

}
/********************************* SLOT1-Ec ************************/
void sendec1 () {
  EcData();
  ECs1 = float(dfd.toInt()) / 100;
  Serial.print("Slot1 - ec = ");
  Serial.println(ECs1);
}

/********************************** SLOT2-pH ***********************/
void sendph2() {
  PhData();
  pHs2 = float(str.toInt()) / 100;
  Serial.print("Slot2 - pH = ");
  Serial.println(pHs2);
  EEPROM.updateFloat(EeprompHs2, pHs2);
  //    S2Read();

  Serial.print(" eeprom save phs2= ");
  Serial.println(pHs2);
}
/******************************** SLOT2-Ec ************************/
void sendec2 () {
  EcData();
  ECs2 = float(dfd.toInt()) / 100;
  Serial.print("Slot2 - Ec = ");
  Serial.println(ECs2);
  EEPROM.updateFloat(EepromECs2, ECs2);
  //  S2Read();
  Serial.print(" eeprom save phs2= ");
  Serial.println(ECs2);
}

/***************************** SLOT3-pH *************************/
void sendph3() {
  PhData();
  pHs3 = float(str.toInt()) / 100;
  Serial.print("Slot3 - pH = ");
  Serial.println(pHs3);
  EEPROM.updateFloat(EeprompHs3, pHs3);
  //S3Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(pHs3);
}
/**************************** SLOT3-Ec *************************/
void sendec3 () {
  EcData();
  ECs3 = float(dfd.toInt()) / 100;
  Serial.print("Slot3 - Ec = ");
  Serial.println(ECs3);
  EEPROM.updateFloat(EepromECs3, ECs3);
  //S3Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(ECs3);
}

/*************************** SLOT4-pH *************************/
void sendph4() {
  PhData();
  pHs4 = float(str.toInt()) / 100;
  Serial.print("Slot4 - pH = ");
  Serial.println(pHs4);
  EEPROM.updateFloat(EeprompHs4, pHs4);
  //S4Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(pHs3);
}
/************************** SLOT4-Ec *************************/
void sendec4 () {
  EcData();
  ECs4 = float(dfd.toInt()) / 100;
  Serial.print("Slot4 - Ec = ");
  Serial.println(ECs4);
  EEPROM.updateFloat(EepromECs4, ECs4);
  //S4Read();
  Serial.print(" eeprom save phs4= ");
  Serial.println(ECs4);
}

/************************** SLOT5-pH ***********************/
void sendph5() {
  PhData();
  pHs5 = float(str.toInt()) / 100;
  Serial.print("Slot5 - pH = ");
  Serial.println(pHs5);
  EEPROM.updateFloat(EeprompHs5, pHs5);
  //S4Read();
  Serial.print(" eeprom save phs5= ");
  Serial.println(pHs5);
}
/************************* SLOT5-Ec **********************/
void sendec5 () {
  EcData();
  ECs5 = float(dfd.toInt()) / 100;
  Serial.print("Slot5 - Ec = ");
  Serial.println(ECs5);
  EEPROM.updateFloat(EepromECs5, ECs5);
  //S4Read();
  Serial.print(" eeprom save phs5= ");
  Serial.println(ECs5);
}

/*********************** SLOT6-pH *********************/
void sendph6() {
  PhData();
  pHs6 = float(str.toInt()) / 100;
  Serial.print("Slot6 - pH = ");
  Serial.println(pHs6);
  EEPROM.updateFloat(EeprompHs6, pHs6);
  //S4Read();
  Serial.print(" eeprom save phs6= ");
  Serial.println(pHs6);
}
/********************** SLOT6-Ec *********************/
void sendec6 () {
  EcData();
  ECs6 = float(dfd.toInt()) / 100;
  Serial.print("Slot6 - Ec = ");
  Serial.println(ECs6);
  EEPROM.updateFloat(EepromECs6, ECs6);
  //S4Read();
  Serial.print(" eeprom save phs6= ");
  Serial.println(ECs6);
}

/********************** SLOT7-pH ********************/
void sendph7() {
  PhData();
  pHs7 = float(str.toInt()) / 100;
  //  Serial.print("Slot7 - pH = ");
  //  Serial.println(pHs7);
  EEPROM.updateFloat(EeprompHs7, pHs7);
  //S4Read();
  //  Serial.print(" eeprom save phs7= ");
  //  Serial.println(pHs7);
}
/********************** SLOT7-Ec ********************/
void sendec7 () {
  EcData();
  ECs7 = float(dfd.toInt()) / 100;
  //  Serial.print("Slot7 - Ec = ");
  //  Serial.println(ECs7);
  EEPROM.updateFloat(EepromECs7, ECs7);
  //S4Read();
  //  Serial.print(" eeprom save phs7= ");
  //  Serial.println(ECs7);
}

/******************** SLOT8-pH ********************/
void sendph8() {
  PhData();
  pHs8 = float(str.toInt()) / 100;
  //  Serial.print("Slot8 - pH = ");
  //  Serial.println(pHs8);
  EEPROM.updateFloat(EeprompHs8, pHs8);
  //S4Read();
  //  Serial.print(" eeprom save phs8= ");
  //  Serial.println(pHs8);
}
/******************** SLOT8-Ec ********************/
void sendec8 () {
  EcData();
  ECs8 = float(dfd.toInt()) / 100;
  //  Serial.print("Slot8 - Ec = ");
  //  Serial.println(ECs8);
  EEPROM.updateFloat(EepromECs8, ECs8);
  //S4Read();
  //  Serial.print(" eeprom save phs8= ");
  //  Serial.println(ECs8);
}

/****************** pH ********************/
void sendph() {
  //    Serial.print(dfd);
  PhData();
  pHAutoSP = float(str.toInt()) / 100;
  Serial.print("Auto - pH = ");
  Serial.println(pHAutoSP);
  pHsp =  pHAutoSP;//auto mode value
  // call function
}

/****************** EC ********************/

void sendec () {
  //  ec1 = strtok(buff, "n");
  //  Serial.println(ec1);
  //  String str = ec1;
  //    Serial.println(dfd);
  val = dfd.indexOf("ec") + 3;
  //    Serial.println(val);
  dfd.remove(0, val);
  //  Serial.println(dfd);
  //  float ecval=(val, 3);
  //  Serial.println(ecval);
  ECAutoSP = float(dfd.toInt()) / 100;
  //  ECAutoSP = val;
  Serial.print(F("Auto - Ec = "));
  Serial.println(ECAutoSP);
  ECsp = ECAutoSP;
  // call function
}

float dummyPHHYS;
/************************************* SetPoints ****************************/
/************************************** HyspH **************************/
void sendHph() {
  hph = strtok(buff, "b");
  String str = hph;
  //    Serial.println(str);
  val = str.indexOf("a") + 2;
  str.remove(0, val);
  pHHys = float(str.toInt()) / 10;
  Serial.print("HyspH = ");
  Serial.println(pHHys);
  dummyPHHYS = pHHys;
  if (pHHys == 0) {

    pHHys = 0.1;
    Serial.print("auto HyspH = ");
    Serial.println(pHHys);
  }
}

/************************************** HysEC **************************/
void sendHec() {
  val = dfd.indexOf("b") + 2;
  //Serial.println(val);
  dfd.remove(0, val);
  ECHys = float(dfd.toInt()) / 10;
  Serial.print("HysEc = ");
  Serial.println(ECHys);
}
//////////////////    //// TIME ///         ////////////////


bool TPRelay;
bool HRelay;
bool MRelay;
bool RRelay;

/************************************** AUTO FUNCTION **************************/

void AutoFun() {
  //  Serial.println("AutoFun() is running.");
  //  Serial.print("AUTO DOSER PROGRESSING...");
  digitalWrite(TPUMPR, LOW);//h
  if (pHAvg == true && ECAvg == true) {
    //float

    pHsp =  pHAutoSP;//auto mode value
    //float
    dummyPHHYS =  phSetHysteris;
    phSetHysteris = pHHys;
    Serial.println("PH Auto Setpoint");
    Serial.println(pHsp);
    Serial.println("PHyes Auto Setpoint");
    Serial.println(phSetHysteris);
    Serial.println(pHHys); //dummyPHHYS
    Serial.println(dummyPHHYS);
    //    float ECAutoSP =  ECsp;
    ECsp = ECAutoSP;
    //    float
    //ECHys =  ECSetHysteris;
    ECSetHysteris = ECHys;
    Serial.println("EC Auto Setpoint");
    Serial.println(ECsp);
    Serial.println(ECSetHysteris);
    DoserAuto(pHAutoSP, pHHys, ECAutoSP, ECHys);
  }
}

float pH_Value;
float Ec_Value;
/************************************  CALIBARTION    **********************************/
void calibartion() {
  //  Serial.println("calibartion");
  char startpH[10] = "enterph";
  char calpH[10] = "calph";
  char exitpH[10] = "exitph";
  char starteC[10] = "enterec";
  char caleC[10] = "calec";
  char exiteC[10] = "exitec";
  char cmd[10];
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {                      //time interval: 1s
    timepoint = millis();
    //temperature = readTemperature();                   // read your temperature sensor to execute temperature compensation
    voltagePH = analogRead(PH_PIN) / 1024.0 * 5000;      // read the ph voltage
    phValue    = ph.readPH(voltagePH, temperature);      // convert voltage to pH with temperature compensation
    //        Serial.print("pH:");
    //        Serial.print(phValue,2);
    float readpHvalue = phValue;
    pH_Value = readpHvalue;
    voltageEC = analogRead(EC_PIN) / 1024.0 * 5000;
    ecValue    = ec.readEC(voltageEC, temperature);      // convert voltage to EC with temperature compensation
    //        Serial.print(", EC:");
    //        Serial.print(ecValue,2);
    float readEcvalue = ecValue;
    Ec_Value = readEcvalue;
    //        Serial.println("ms/cm");
  }
  //    if(readSerial(cmd)){
  if (startph == true) {
    extph = false;
    strcpy(cmd, startpH);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD
    }
    //          }
    delay(1000);
  }
  if (calph == true) {
    startph = false;
    strcpy(cmd, calpH);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD
    }
    //          }
    delay(1000);
  }
  if (extph == true) {
    calph = false;
    strcpy(cmd, exitpH);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "PH")) {
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD
    }
    //          }
    delay(1000);
  }

  if (startEc == true) {
    extEc = false;
    strcpy(cmd, starteC);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "EC")) {
      ec.calibration(voltageEC, temperature, cmd);     //EC calibration process by Serail CMD
    }
    //          }
    delay(1000);
  }

  if (calEc == true) {
    startEc = false;
    strcpy(cmd, caleC);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "EC")) {
      ec.calibration(voltageEC, temperature, cmd);     //EC calibration process by Serail CMD
    }
    //          }
    delay(1000);
  }
  if (extEc == true) {
    calEc = false;
    strcpy(cmd, exiteC);
    strupr(cmd);
    //          if (millis() - previousMillis >= 3000) {
    //              previousMillis = millis();
    if (strstr(cmd, "EC")) {
      ec.calibration(voltageEC, temperature, cmd);     //EC calibration process by Serail CMD
    }
    //          }
  }
  delay(1000);

}

int i = 0;
bool readSerial(char result[]) {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == '\n') {
      result[i] = '\0';
      Serial.flush();
      i = 0;
      return true;
    }
    if (inChar != '\r') {
      result[i] = inChar;
      i++;
    }
    delay(1);
  }
  return false;
}
//bool rtcTime;

//void modeStatus(){
//  EEPROM.update(421, amon);
//  EEPROM.update(423, amoff);
//  EEPROM.update(425, smon);
//  EEPROM.update(427, smoff);

//  EEPROM.update(429, Hourch);
//  EEPROM.update(431, Minch);
//  EEPROM.update(433, secsch);
//  EEPROM.update(435, Datech);
//  EEPROM.update(437, monthch);
//  EEPROM.update(441, yearch);
//}

//void othr() {
//  int val = dfd.indexOf("g") + 1;
//  dfd.remove(0, val);
//  otHr = dfd.toInt();
//}
int tank() {
  //  char *nt = strtok(buff, "g");
  //  str = nt;
  int val = dfd.indexOf("z") + 1;
  dfd.remove(0, val);
  otMin = dfd.toInt();
  return (otMin);
}
int oftmin() {
  char *ui = strtok(buff, "z");
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}

void setTimeNex() {
  //  Serial.println("setTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

void runTimeNex() {
  //  Serial.println("runTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}


/******************* SERIALCOM *******************************/
void SerialCom() {
  String data_from_display = "";
  delay(60);
  //   String dfd;           //use for setpoints but will effect schedule mode
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  dfd = data_from_display;
  //    Serial.println(data_from_display);
  dfd.toCharArray(buff, dfd.length());
  //    data_from_display = "";

  /**************************    SCHEDULE MODE     ***********************/
  if (dfd.substring(0, 3) == "sl1") {
    //      senddo1();
    sendec1();
    sendph1();
    feedBack = "sl1";
  }
  if ((dfd.substring(0, 3) == "hr1")) {
    //      sendhour();
    //      S1Hr = sendhour();
    //      Serial.print("Hr = ");
    //      Serial.println(S1Hr);
    Shift1Clockhr();
    //      sendmin();

    //      S1Min = sendmin();
    //      Serial.print("Min = ");
    //      Serial.println(S1Min);
    Shift1Clockmin();
    feedBack = "hr1";
  }

  if (dfd.substring(0, 3) == "sl2") {
    // senddo2();
    sendec2();
    sendph2();
    feedBack = "sl2";
  }
  if ((dfd.substring(0, 3) == "hr2")) {
    //      sendhour();
    //      S2Hr = sendhour();
    //      S2Min = sendmin();
    //      Serial.print("Hr = ");
    //      Serial.println(S2Hr);
    //      sendmin();
    //      S2Min = sendmin();
    //      Serial.print("Min = ");
    //      Serial.println(S2Min);
    Shift2Clock();
    feedBack = "hr2";
  }

  if (dfd.substring(0, 3) == "sl3") {
    // senddo3();
    sendec3();
    sendph3();
    feedBack = "sl3";
  }
  if ((dfd.substring(0, 3) == "hr3")) {
    //      sendhour();
    //      S3Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S3Hr);

    //      sendmin();
    //      S3Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S3Min);
    Shift3Clock();
    feedBack = "hr3";
  }


  if (dfd.substring(0, 3) == "sl4") {
    //      senddo4();
    sendec4();
    sendph4();
    feedBack = "sl4";
  }
  if ((dfd.substring(0, 3) == "hr4")) {
    //      sendhour();
    //      S4Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S4Hr);
    //      sendmin();
    //      S4Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S4Min);
    Shift4Clock();
    feedBack = "hr4";
  }



  if (dfd.substring(0, 3) == "sl5") {
    //senddo5();
    sendec5();
    sendph5();
    feedBack = "sl5";
  }
  if ((dfd.substring(0, 3) == "hr5")) {
    //      sendhour();
    //      S5Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S5Hr);
    //      sendmin();
    //      S5Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S5Min);
    Shift5Clock();
    feedBack = "hr5";
  }


  if (dfd.substring(0, 3) == "sl6") {
    //      senddo6();
    sendec6();
    sendph6();
    feedBack = "sl6";
  }
  if ((dfd.substring(0, 3) == "hr6")) {
    //      sendhour();
    //      S6Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S6Hr);
    //      sendmin();
    //      S6Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S6Min);
    Shift6Clock();
    feedBack = "hr6";
  }


  if (dfd.substring(0, 3) == "sl7") {
    //senddo7();
    sendec7();
    sendph7();
    feedBack = "sl7";
  }
  if ((dfd.substring(0, 3) == "hr7")) {
    //      sendhour();
    //      sendhour();
    //      S7Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S7Hr);
    //      sendmin();
    //      S7Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S7Min);
    Shift7Clock();
    feedBack = "hr7";
  }


  if (dfd.substring(0, 3) == "sl8") {
    //senddo8();
    sendec8();
    sendph8();
    feedBack = "sl8";
  }
  if ((dfd.substring(0, 3) == "hr8")) {
    //      sendhour();
    //      sendhour();
    //      S8Hr = shr;
    //      Serial.print("Hr = ");
    //      Serial.println(S8Hr);
    //      sendmin();
    //      S8Min = smn;
    //      Serial.print("Min = ");
    //      Serial.println(S8Min);
    Shift8Clock();
    feedBack = "hr8";
  }

  /**************************    AUTO MODE     ***********************/
  if (dfd.substring(0, 3) == "atm") {
    sendph();
    sendec();

    feedBack = "atm";
  }


  /**************************    MANUAL MODE     **************************/

  if ((data_from_display.substring(0, 3) == "phl")) {
    val = data_from_display.indexOf("l") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    phplus = data_from_display.toInt();
    ////      Serial.print("pH+ = ");
    //      Serial.println(phmin);
    //      data_from_display = "";
    manualdosemin();
    feedBack = "phl";
  }

  if ((data_from_display.substring(0, 3) == "phm")) {
    val = data_from_display.indexOf("m") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    phmin = data_from_display.toInt();
    //      Serial.print("pH- = ");
    //      Serial.println(phplus);
    //      Serial.println(String(value.valLong));
    //      data_from_display = "";
    manualdoseplus();
    feedBack = "phm";
  }

  if ((data_from_display.substring(0, 3) == "nta")) {
    val = data_from_display.indexOf("a") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    ECA = data_from_display.toInt();
    Serial.print("Nutrient A = ");
    Serial.println(ECA);
    //Serial.println(String(value.valLong));
    //      data_from_display = "";
    manualdoseEcA();
    feedBack = "nta";
  }
  if ((data_from_display.substring(0, 3) == "ntb")) {
    val = data_from_display.indexOf("b") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    ECB = data_from_display.toInt();
    Serial.print("Nutrient B = ");
    Serial.println(ECB);
    //      data_from_display = "";
    manualdoseEcB();
    feedBack = "ntb";
  }

  /**************************    SET POINTS   ********************/

  if (data_from_display.substring(0, 3) == "spt") {
    sendHec();
    sendHph();
    feedBack = "spt";
  }

  /**************************    TANK MANAGEMENT   ********************/
  if ((data_from_display.substring(0, 5) == "tkmst")) {
    //       othr();
    //       Serial.print("Tank Level= ");
    //       totalVolume1 = otHr;
    //       Serial.println(totalVolume1);

    //       tank();
    Serial.print("Tank Height= ");
    tankHeight = tank();
    Serial.println(tankHeight);

    oftmin();
    Serial.print("Set Level= ");
    tanksetpoint = oftmin();
    Serial.println(tanksetpoint);
    feedBack = "tkmst";
  }
  /**************************    WIFI   ********************/
  if (dfd.substring(0, 4) == "sdpd") {
    //    if (dfd == "sdpd"){
    //      Wifiset = true;
    //      wifion =true;
    //       pwdname();
    //       str = ssid;

    ssid = pwdname();
    //      Serial.print("User Name: ");
    //      Serial.println(ssid);



    //      pwd();
    //      dfd = password;
    password = pwd();
    //      Serial.print("Password: ");
    //      Serial.println(password);
    feedBack = "sdpd";
    //    uint16_t time_elapsed = 0;
    //    bool wificon = true;
    //
    //     if(wificon){
    //     while(time_elapsed < 15000)
    //    {
    //      wifireceived();
    //      ssidpwd();
    //      time_elapsed = millis();
    //      delay(10);
    //    }
    //    wificon = false;
    //      wificon = 1;

  }
  //      wifireceived();
  //      ssidpwd();
  //        if ((dfd.substring(0,7) == "wificon")){
  //      wificon = 0;
  //    }
  /**************************    MESH    ********************/
  if ((dfd.substring(0, 4) == "mesh")) {
    prefix();
    meshpass();
    port();
    feedBack = "mesh";
  }
  /**************************    CLOUD    ********************/
  if ((dfd.substring(0, 4) == "clod")) {

    ssid = pwdname();
    Serial.print("Cloud SSID: ");
    Serial.println(ssid);

    password = pwd();
    Serial.print("Cloud Password: ");
    Serial.println(password);
    //      dfd = password;
    feedBack = "clod";
  }
  /**************************   CALIBRATION - pH    ********************/
  if ((data_from_display.substring(0, 7) == "enterph")) {
    //Serial.println("from nexiton enterph:");
    //         startph = Serial.readString();
    //         Serial.println(Serial.readString());
    digitalWrite(resetPin, HIGH);
    caliber = true;
    startph = true;
    feedBack = "enterph";
  }
  if ((data_from_display.substring(0, 5) == "calph")) {
    //       startcalib[10] = "calph";
    //       strcpy(cmd,startcalib);
    calph = true;
    feedBack = "calph";
  }
  if ((data_from_display.substring(0, 6) == "saveph")) {
    //       startcalib[10] = "exitph";
    //       strcpy(cmd,startcalib);
    saveph();
    extph = true;
    feedBack = "saveph";
  }
  if ((data_from_display.substring(0, 8) == "extcalib")) {
    caliber = false;
    feedBack = "extcalib";
  }

  /**************************   CALIBRATION - EC    ********************/

  if ((data_from_display.substring(0, 7) == "enterec")) {
    startEc = true;
    caliber = true;
    //           startcalib[10] = "enterec";
    //           strcpy(cmd,startcalib);
    feedBack = "enterec";
  }
  if ((data_from_display.substring(0, 5) == "calec")) {
    calEc = true;
    //       startcalib[10] = "calec";
    //       strcpy(cmd,startcalib);
    feedBack = "calec";
  }
  if ((data_from_display.substring(0, 5) == "savec")) {
    extEc = true;
    //       startcalib[10] = "exitec";
    //       strcpy(cmd,startcalib);
    savec();
    feedBack = "savec";
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
    t20.enable();//setTimeNex
    t21.disable();
  }
  if ((data_from_display.substring(0, 6) == "setrtc")) {
    t20.disable();
    t21.enable();//runTimeNex
  }
  if ((data_from_display.substring(0, 3) == "rat")) {

  }
  sendData(data_from_display);  /////  UNITS, DEBUG & SPEED RATIO ////
}

void sendData(String dfd) {

  /********************************* SETTINGS - UNITS **************************/

  if (dfd == "TdsOn") {
    Serial.println("TDS is On");
    var = 1;
    feedBack = "TdsOn";
  }
  if (dfd == "EccOn") {
    Serial.println("Ec is On");
    var = 0;
    feedBack = "EccOn";
  }
  if (dfd == "CfOn") {
    Serial.println("CF is On");
    var = 2;
    feedBack = "CfOn";
  }
  if (dfd == "CelOn") {
    Serial.println("Celsius is On");
    unitTemp = nutrientTemperature;
    feedBack = "CelOn";
  }
  if (dfd == "FarOn") {
    Serial.println("Fahrenheit is On");
    unitTemp = Fahrenheit;
    feedBack = "FarOn";
  }

  /********************************* SETTINGS - MIX RATIO  **************************/
  if (dfd == "v1On") {
    Serial.println("Version 1 is On");
    feedBack = "v1On";
  }
  if (dfd == "v2On") {
    Serial.println("Version 2 is On");
    feedBack = "v2On";
  }
  if (dfd == "v3On") {
    Serial.println("Version 3 is On");
    feedBack = "v3On";
  }
  if (dfd == "Low") {
    Speed = 1000;
    Serial.println("Low Flow Rate");
    feedBack = "Low";
  }
  if (dfd == "Medium") {
    Speed = 750;
    Serial.println("Medium Flow Rate");
    feedBack = "Medium";
  }
  if (dfd == "High") {
    Speed = 500;
    Serial.println("High Flow Rate");
    feedBack = "High";
  }
  /********************************* DEBUG **************************/
  if (dfd == "PhOn") {
    Serial.println("pH is On");
  } if (dfd == "PhOff") {
    Serial.println("pH is Off");
  }
  if (dfd == "EcOn") {
    Serial.println("Ec is On");
  } if (dfd == "EcOff") {
    Serial.println("Ec is Off");
  }
  if (dfd == "DoOn") {
    Serial.println("Do is On");
  } if (dfd == "DoOff") {
    Serial.println("Do is Off");
  }
  if (dfd == "NtOn") {
    Serial.println("Nutrient Temp is On");
  } if (dfd == "NtOff") {
    Serial.println("Nutrient Temp is Off");
  }
  if (dfd == "SpOn") {
    //    TPRelay = HIGH;
    digitalWrite(TPUMPR, LOW);
    Serial.println("Sub Pump is On");
    sampumpSt = "on";
    feedBack = "SpOn";
  }
  if (dfd == "SpOff") {
    //    TPRelay = LOW;
    digitalWrite(TPUMPR, HIGH);
    Serial.println("Sub Pump is Off");
    sampumpSt = "off";
    feedBack = "SpOff";
  }
  if (dfd == "MixOn") {
    //    MRelay = HIGH;
    digitalWrite(MIXERR, LOW);
    Serial.println("Mixer is On");
    mixerSt = "on";
    feedBack = "MixOn";
  }
  if (dfd == "MixOff") {
    //    MRelay = LOW;
    digitalWrite(MIXERR, HIGH);
    Serial.println("Mixer is Off");
    mixerSt = "off";
    feedBack = "MixOff";
  }
  if (dfd == "HtOn") {
    //    HRelay = HIGH;
    digitalWrite(HEATERR, LOW);
    Serial.println("Heater is On");
    feedBack = "HtOn";
    HeaterSt = "on";
  }
  if (dfd == "HtOff") {
    //    HRelay = LOW;
    digitalWrite(HEATERR, HIGH);
    Serial.println("Heater is Off");
    feedBack = "HtOff";
    HeaterSt = "off";
  }
  if (dfd == "ClOn") {
    //    RRelay = HIGH;
    digitalWrite(RORELAYR, LOW);
    Serial.println("Cooler is On");
    feedBack = "ClOn";
    RoSt = "on";
  }
  if (dfd == "ClOff") {
    //    RRelay = LOW;
    digitalWrite(RORELAYR, HIGH);
    Serial.println("Cooler is Off");
    feedBack = "ClOff";
    RoSt = "off";
  }
  /************************************** AUTO / SCHEDULE - MODE[ [ON/OFF] **************************/

  if (dfd == "amOn") {  // && == true
    //    String amTaskOn = dfd;
    //    amon = amTaskOn.toInt();
    //    Serial.println(amon);
    t5.enable();
    Serial.println("Auto Mode is Enabled");
    feedBack = "amOn";
  }
  if (dfd == "amOff") {
    //    String amTaskOff = dfd;
    //    amoff = amTaskOff.toInt();
    //    Serial.println(amoff);
    t5.disable();
    Serial.println("Auto Mode is Disabled");
    feedBack = "amOff";
  }

  if (dfd == "smOn") {
    //    String smTaskOn = dfd;
    //    smon = smTaskOn.toInt();
    t6.enable();
    Serial.println("Schedule Mode is Enabled");
    feedBack = "smOn";
  }
  if (dfd == "smOff") {
    //    String smTaskOff = dfd;
    //    smoff = smTaskOff.toInt();
    t6.disable();
    Serial.println("Schedule Mode is Disabled");
    feedBack = "smOff";
  }
  /************************************** WATER LEVEL SENSOR **************************/

  if (dfd == "TkMgOn") {
    waterlevelnt = true;
    waterlevel_sensor = true;
    Serial.println("Tank Management On");
    feedBack = "TkMgOn";


  } if (dfd == "TkMgOff") {
    waterlevelnt = false;
    Serial.println("Tank Management Off");
    feedBack = "TkMgOff";

  }

}

void nexVal() {
  dateandtime();
  switch (var) {
    case 0:
      unitConduct = ECValueavrg;
      //        Serial.println(ECValueavrg);
      break;
    case 1:
      unitConduct = nutrientTDS;
      //        Serial.println(nutrientTDS);
      break;
    case 2:
      unitConduct = nutrientCF;
      break;
    default:
      unitConduct = ECValueavrg;
      break;
  }
  /**************** WIFI ***********************/
  Serial2.print("hstatus.txt=\"");
  Serial2.print(wifiStatus);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /**************** WATER LEVEL SENSOR ***********************/
  Serial2.print("readtank.txt=\"");
  Serial2.print(waterconlevel);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /****************  TEMPERATURE  ***********************/
  Serial2.print("WaterTemp.txt=\"");
  Serial2.print(unitTemp); // unitTemp = nutrientTemperature(or) Fahrenheit
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //    Serial2.print("degF.txt=\"");
  //    Serial2.print(Fahrenheit); // unitTemp = nutrientTemperature(or) Fahrenheit
  //    Serial2.print("\"");
  //    Serial2.write(0x03ff);
  //    Serial2.write(0x03ff);
  //    Serial2.write(0x03ff);
  /**************** EC / TDS / CF ***********************/
  //int ecTEST= random(850, 999);//ecTEST
  //    Serial.print(unitConduct);

  //    float readnewec=0.9;
  //    readnewec=1.2;
  Serial2.print("ECvalue.txt=\"");
  Serial2.print(unitConduct); // unitConduct = nutrientCF (or)nutrientTDS nutrientEC /ecValue
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  /**************** CALIBRATION ***********************/
  Serial2.print("phreading.txt=\"");
  Serial2.print(savpH);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("ecreading.txt=\"");
  Serial2.print(savEc);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /***************************************/
  //    Serial2.print("solar.txt=\"");
  //    Serial2.print(uvIntensity);
  //    Serial2.print("\"");
  //    Serial2.write(0x03ff);
  //    Serial2.write(0x03ff);
  //    Serial2.write(0x03ff);

  Serial2.print("pHvalue.txt=\"");
  Serial2.print(pH);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("DOvalue.txt=\"");
  Serial2.print(DO);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  /**************** DATE & TIME ***********************/
  //Serial.print(hourupg);
  Serial2.print("hour.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //    Serial.println("time: ");
  //    Serial.println(hourupg);
  Serial2.print("hourr.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //  Serial.println(minupg);
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

  /**************** PH AND EC VALUES *****************/
  Serial2.print("readph.txt=\"");
  Serial2.print(pH_Value);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("readec.txt=\"");
  Serial2.print(Ec_Value);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /**************** FEEDBACK ***********/
  Serial2.print("fdbk.txt=\"");
  Serial2.print(feedBack);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

}



/************************************  SETUP **********************************/
void setup() {

  ////#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial2.begin(9600); //Nextion
  //  delay(2000);
  ////#endif

  while (!Serial);
  //  Serial.println("ZEEPTOGREENS...........");
  //  Serial.flush();
  runTimeNex();
  sensors.begin(); // DALLAS
  ph.begin();
  ec.begin();
  //  r.init();
  r.startNow();
  //  wdt_disable();
  //  wdt_enable(WDTO_8S);

  //  pinMode(encBtn, INPUT_PULLUP);;
  // Stepper configer
  pinMode(PHLOW_STEP_PIN  , OUTPUT);
  pinMode(PHLOW_DIR_PIN    , OUTPUT);
  //  pinMode(PHLOW_ENABLE_PIN    , OUTPUT);

  pinMode(PHHIGH_STEP_PIN  , OUTPUT);
  pinMode(PHHIGH_DIR_PIN    , OUTPUT);

  pinMode(EC_A_STEP_PIN, OUTPUT);
  pinMode(EC_B_STEP_PIN, OUTPUT);
  pinMode(EC_C_STEP_PIN, OUTPUT);

  pinMode(EC_A_DIR_PIN, OUTPUT);
  pinMode(EC_B_DIR_PIN, OUTPUT);
  pinMode(EC_C_DIR_PIN, OUTPUT);



  digitalWrite(EC_A_DIR_PIN, LOW);
  digitalWrite(EC_B_DIR_PIN, LOW);

  digitalWrite(EC_C_DIR_PIN, LOW);



  pinMode(BUZZER, OUTPUT);

  //***Indicator
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);
  pinMode(LEDPOWER, OUTPUT);
  digitalWrite(LEDPOWER, HIGH);
  pinMode(TPUMPR, OUTPUT);
  pinMode(HEATERR, OUTPUT);
  pinMode(MIXERR, OUTPUT);
  pinMode(RORELAYR, OUTPUT);
  
  digitalWrite(TPUMPR, HIGH);
  digitalWrite(HEATERR, HIGH);
  digitalWrite(MIXERR, HIGH);
  digitalWrite(RORELAYR, HIGH);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT_PULLUP);

  //Sensor Reading
  readalldata();
  //  if (probePh == false) {
  //        t1.enable(); //PH Reading
  //  }
  //
  //  if (probeEc == false) {
  //t2.enable();  // Ec Reading
  //  }
  if (probeDo == false) {
    getNutrientDO();
  }
  if (probeTemp == false) {

    //    t3.enable(); //Temperature
  }
  //  t11.enable();
  //  if (Autosave == HIGH) {
  //    t11.enable();//t4.disable();
  //  }
  //  else {
  //    t11.disable();
  //  }

  //  if (Wifiset == true) {
  //    t5.enable();//t4.disable();
  //  }
  //  else {
  //    t5.disable();
  //  }



  pinMode(resetPin, OUTPUT);
  // Enable  Read time Task
  //  t0.enable();   // Time
  t1.enable();   // Getsensorreading

  t12.enable();  // Nextion Reading
  //  t4.enable();   //jsondata send to Esp8266


}

void GetSensorReading() {
  //Serial.println(F("GetSensorReading"));
  getNutrientPHavg();
  getNutrientECavg();
  getNutrientTemperature();
  getNutrientTL();
  //sensorjson();
}
/*************************************  LOOP **********************************/
//uint32_t time_elapsed=0;
void loop() {
  if (Serial2.available()) {
    SerialCom();
  }
  r.execute();

  /*************************** CALLIBRATION ********************/
  if (caliber == true) {
    t16.enable();
    //   Serial.println("caliber");
  }
  else if (caliber == false) {
    //     Serial.println("caliber off");
    t16.disable();
  }

}
