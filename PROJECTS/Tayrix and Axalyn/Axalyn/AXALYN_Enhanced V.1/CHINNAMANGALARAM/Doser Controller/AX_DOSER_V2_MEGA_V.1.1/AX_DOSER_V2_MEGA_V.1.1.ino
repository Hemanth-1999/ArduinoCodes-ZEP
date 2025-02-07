#include "DFRobot_PH.h"
#include "DFRobot_EC.h"
#include <EEPROMex.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Time.h>
#include <MemoryFree.h>
#include <SD.h>
#include <Wire.h>
#include <TimeLib.h>
#include <menu.h>
#include <TimerOne.h>
#include <TaskScheduler.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <GravityTDS.h>
using namespace Menu;

#define LEDPIN LED_BUILTIN

#include <EEPROMex.h>

//Define
#define BUZZER 13 //37
#define LEDPOWER 10
#define LEDRED 11
#define LEDGREEN 9
#define LEDBLUE 12

//RELA
#define TPUMPR A15   // IN 1
#define HEATERR A12  // IN 4
#define MIXERR A13   // 14

//****************************************** Water Level US Sensor **************************************************************
#define RORELAYR A14 // RO WATER ON

#define trigPin (uint8_t)46
#define echoPin (uint8_t)48

#define _TEST_
#define DEBUG 0                         // Change value to 1 to enable debuging using serial monitor  

#define PH_PIN A0
#define EC_PIN A1

#define ONE_WIRE_BUS 1 // Pin 4 is used for SD card in ethernet shield

#define PHLOW_STEP_PIN        3
#define PHLOW_DIR_PIN         4

#define PHHIGH_STEP_PIN       23
#define PHHIGH_DIR_PIN        22

#define EC_A_STEP_PIN         5
#define EC_A_DIR_PIN          6

#define EC_B_STEP_PIN         7
#define EC_B_DIR_PIN          8

#define EC_C_STEP_PIN         9
#define EC_C_DIR_PIN          10

//************SD Card **********************************************///
#define SD_DETECT_PIN   53

#define SDSS            53

# define Offsetec 0.00 //deviation compensate
# define Offset 0.00 //deviation compensate
# define samplingInterval 20
# define printInterval 800
# define ArrayLenth 5 //times of collection

DFRobot_PH ph;
DFRobot_EC ec;

GravityTDS gravityTds;

OneWire oneWireBus(ONE_WIRE_BUS);
DallasTemperature sensors (&oneWireBus);

RTC_DS3231 rtc; //for date time

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int duration;
int distance;
int distanceincm;
int distanceinInch;

int FDate;
int FMonth;
int FYear;
int TDate;
int TMonth;
int TYear;

float waterHeight;

uint8_t tankHeight ; //enter your water tank height here in [cm]
uint8_t totalVolume ; // volume here is in [cm^3], change depending on tank volume

float waterQuantity;
float volume;
float pi = 3.14159265359;

//Water distance
float Wtime = 0, Wdistance = 0;

uint8_t Wtemp = 0;

bool TankM;

uint8_t tanksetpoint;

File sdcard_file;

struct Config {
  char hostname[64];
  int port;
};
const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object

//*PH*//
uint16_t EeprompHSetpoint = 31;      //location of pHSetpoint in Eeprom
uint16_t EepromSetpHHysteris = 41;   //location of SetpHHysteris in Eeprom

//*EC*//
uint16_t EepromECSetpoint = 51;      //location of ECSetpoint in Eeprom
uint16_t EepromSetECHysteris = 61;   //location of SetECHysteris in Eeprom

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

String ssid;
String password;

String RoSt;
String mixerSt;
String HeaterSt;
String sampumpSt;

//****************TIME AND DATE**************************************************************************************************

short S1Hr;
short S1Min;
short S2Hr;
short S2Min;
short S3Hr;
short S3Min;
short S4Hr;
short S4Min;
short S5Hr;
short S5Min;
short S6Hr;
short S6Min;
short S7Hr;
short S7Min;
short S8Hr;
short S8Min;

int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex = 0;

int ECArray[ArrayLenth]; //Store the average value of the sensor feedback
int ECArrayIndex = 0;

//**** Hysteris ****//
float dummyPHHYS;
float pHSetpoint;
float HysterisMin;
float HysterisPlus;
float SetpHHysteris;

float phSetHysteris = 0.2;
float ECSetHysteris = 0.1;

float phsetvalue;
float ECsetvalue;

byte ECSetpoint;
byte ECHysterisMin;
byte ECHysterisPlus;

int previousMillis = 0; //
int pinHighTime = 100; //
int pinLowTime = 7500; // OLD 7500
int pinTime = 100; //

int ECpreviousMillis = 0; //            |
int ECpinHighTime = 100; //             |
int ECpinLowTime = 7500; //             |
int ECpinTime = 100; //
int Scheon;
int autoon;

bool pHAvg;
bool ECAvg;
bool Resetfun;
bool StopPHHys;
bool EcCalStart;
bool autostart = autoon;
bool ScheduleAuto = Scheon;

float pH; //generates the value of pH

float phtest;
float phUp = 2;
float phDown = 2; //

float EC; //generates the value of EC

int ECUp = 2;
int ECDown = 2;
int var = 0;

float unitConduct;
float Fahrenheit;
float offset = 245;

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

#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point

#define READ_TEMP (nutrientTemperature) //Current water temperature ℃, Or temperature sensor function

int analogBuffer[SCOUNT]; // store the analog value in the array, readfrom ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;

float voltagePH, voltageEC, phValue, ecValue, temperature = 25;
float averageVoltage = 0, tdsValue = 0;

float pH_Value;
float Ec_Value;

float nutrientTemperature;

float nutrientPH;//  = 0;
float nutrientEC;//  = 0;
float nutrientCF;//  = 0;
float nutrientTDS;// = 0;

float unitTemp ;

float phValueavrg;
float ECValueavrg;

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

int Speed = 500;

int phplus;
int phmin;
int ECA;
int ECB ;
int ECC ;

//***************Eprom Save SP****************//

int EepromSPFD = 260;
int EepromSPFM = 265;
int EepromSPTD = 270;
int EepromSPTM = 275;
int EeprompHsp = 280;
int EepromECsp = 285;
int Eepromtempsp = 290;
int Eepromdosp = 295;

//************************************************************************************************************

int PHUPR = 1;

int PHDOWNR = 1;

int ECRatioA = 1;

int ECRatioB = 1;

int ECRatioC = 1;

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

/////////////////////////////// SHEDULE MODE ///////////////////////
String data_from_display = "";

char buff[2000];
char *ph1;
char *ec1;
char *nt1;
char *hr;

int val;

String str;
String dfd;
String chipID;
String cloudIp;
String wifiStatus;
String userSsid;
String userPswd;

short shr;
short smn;

//////////////setpoints/////////////
char *hph;
char *dph;
char *dec;
char *dnt;

uint8_t otHr;
uint8_t otMin;
uint8_t oftHr;
uint8_t oftMin;

float pHAutoSP;
float pHHys = 0.1;
float ECAutoSP;
float ECHys;

float pHsp;
float ECsp;
float tempsp;
float dosp;
float nullval;
float phRefV;      //reference voltage
float EcRefV;

int EepromPhRV = 0x00;
int EepromECRV = 0x00;

bool calph;
bool extph;
bool calEc;
bool extEc;

bool wificon;

bool caliber;
bool startph;
bool startEc;

bool Tankfull;
bool waterlevelnt;
bool waterlevel_sensor;

long waterconlevel;

bool autoboolcheck;
bool scheboolcheck;

bool probeEc;
bool probeTemp;
bool probeDo;

float savpH;
float savEc;

String feedBack;
//char cmd[10];
//char startcalib[10];
uint8_t resetPin = 11;

// Callback methods prototypes
void GetSensorReading();
void Calibration();
void scheduleFun();
void setTimeNex();
void runTimeNex();
void sensorjson();
void AutoFun();
void nexVal();

void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial2.begin(9600); // Nextion
  sensors.begin();     // DALLAS
  
  ph.begin();
  ec.begin();
  
  pinMode(PHLOW_STEP_PIN  , OUTPUT);
  pinMode(PHLOW_DIR_PIN    , OUTPUT);

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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT_PULLUP);

  //Sensor Reading
  readalldata();

  if (probeDo == false) {
    getNutrientDO();
  }
  if (probeTemp == false) {

  }

  pinMode(resetPin, OUTPUT);
}

void sensorjson() {

  StaticJsonDocument<500> doc;

  doc["pH"] = pH;
  doc["EC"] = ECValueavrg;
  doc["NT"] = nutrientTemperature;
  doc["DO"] = DOvalue;
  doc["NTankLevel"] = waterconlevel;

  doc["Sampler"] = sampumpSt;
  doc["Heater"] = HeaterSt;
  doc["Mixer"] = mixerSt;
  doc["RO"] = RoSt;

  doc["pHSetPA"] = pHAutoSP;
  doc["ECSetPA"] = ECAutoSP;

  serializeJson(doc, Serial3);
  serializeJsonPretty(doc, Serial);
  Serial.println();

}

void GetSensorReading() {
  sensorjson();
  delay(1000);
  getNutrientPHavg();
  delay(1000);
  getNutrientECavg();
  delay(1000);
  getNutrientTemperature();
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

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
}

void toUpperCase(char* str)
{
  if (str == NULL) return;
  for (int i = 0; str[i] != '\0'; i++)
  {
    str[i] = toupper(str[i]);
  }
}

int previousMillis1 = 0;
int previousMillis2 = 0;
int previousMillis3 = 0;
int previousMillis4 = 0;

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis1 >= 10000) {
    previousMillis1 = currentMillis;

    GetSensorReading();

    setTimeNex();
    runTimeNex();

    nexVal();
    SerialCom();
  }

  if (currentMillis - previousMillis2 >= 3000) {
    previousMillis2 = currentMillis;
    manualread();
  }

  if (currentMillis - previousMillis3 >= 4000) {
    previousMillis3 = currentMillis;
    eventmode();
  }

  if (currentMillis - previousMillis4 >= 45000) {
    previousMillis4 = currentMillis;
    Calibration();
  }
}
