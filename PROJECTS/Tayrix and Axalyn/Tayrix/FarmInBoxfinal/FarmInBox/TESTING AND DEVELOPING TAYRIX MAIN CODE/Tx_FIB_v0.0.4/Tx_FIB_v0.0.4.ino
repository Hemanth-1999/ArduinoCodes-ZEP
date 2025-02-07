//=============================================
// Changelog
//=============================================
/*

   v0.0.1
        Adding Switch Funtion


   v0.0.2

        flow Sensor
        Irrigation pressur Sensor
        Irr mode QTY


   V0.0.3
         Adding Fertigation


*/


#define VERSION "0.0.2"
//===========================================
// Includes
//===========================================
#include <ArduinoJson.h>
#include <FlowMeter.h>  // https://github.com/sekdiy/FlowMeter
#include "millisDelay.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
//////RTC
RTC_DS3231 rtc;
int minn;
int miny;
int secok;
long int shiftTimeDiv;
bool okk = true;
int RTC_Hr ;
int RTC_Min  ;
 int Slot01_DIModeON_SetTime_Hr;
int Slot01_DIModeON_SetTime_min;

int Slot02_DIModeON_SetTime_Hr;
int Slot02_DIModeON_SetTime_min;

int Slot03_DIModeON_SetTime_Hr;
int Slot03_DIModeON_SetTime_min;

////***************/////
///Doser Input values
int Urea_fertilizer ;
int MOP_fertilizer ;
int DOP_fertilizer ;
int Acid_fertilizer ;
int MN_fertilizer ;

float per_nitrogen ;
float per_P ;
float per_K ;
float per_Acid ;
float per_MN ;

float Req_NutrientKg;
float Req_K_Kg ;
float Req_P_Kg;
float Req_Acid_Kg ;
float Req_MN_Kg ;
float Zone_sizein_acre;
long int  Nutrient_tank ;
long int ttHr ;
////bool variables for dosing
bool nutreintstats = false;
bool pstats = false;
bool kstats = false;
bool mstats = true;
bool astats = true;
//////dosing total time in mills
long int  totalnitromills;
long int  totalphsmills;
long int  totalpotamills;
//////////////// sensor readings
float MainInPressure;
float Mainpressure;
int OutFlowTotalVolume ;
int InFlowTotalVolume;


//== == == == == == == == == == == == == == == =
// Defines
//=================================

millisDelay ledDelay;
millisDelay DoserPh_Delay;
millisDelay DoserA_Delay;
millisDelay DoserB_Delay;
millisDelay DoserC_Delay;
millisDelay DoserAcid_Delay;

millisDelay AUTO_DoserPh_Delay; //AUTO_DoserA_Delay
millisDelay AUTO_DoserA_Delay;
millisDelay AUTO_DoserB_Delay;
millisDelay AUTO_DoserC_Delay;
millisDelay AUTO_DoserAcid_Delay;
#define MainInPressure_Pin    A1//irrigation
#define MainInPressure_Pin2  A2//doser




#define IRR_InLetFlow  2
#define IRR_OutLetFlow 3

//#define IRR_Pump_Zone01 12
//#define IRR_Pump_Zone02 13
//#define IRR_Pump_Zone03 11
//#define IRR_Pump_Zone04 10

//#define IRR_MainLine_SolVal 33
//#define IRR_MainLine_SumpPump 12

#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000

#define safetyvalve 26
#define fertpump 27
#define zone2valve 28
#define fertsole 29
#define irrigation 30
#define zone1valve 31
#define mixpump 32
#define Dose_A_sol_pin 33
#define Dose_B_sol_pin 34
#define Dose_C_sol_pin 35
#define Dose_Acid_sol_pin 36
#define Dose_pHUp_sol_pin 37
#define BooosPump_pin 17



#define Relay_ON LOW
#define Relay_OFF HIGH
int relayState;
int InFlowRateMin ;
int OutFlowRateMin;


int SetVal_IrrValume;
//////


void sendData(String );


// set the measurement update period to 1s (1000 ms)
const unsigned long period = 1000;

float appdepth ;//= 0.11811;// inch -ET mm/day mm/25.4;
float flowrate ;//= 79.155;//gpm
float irrareaacre;//= 1.00;//acres000000000000
float appmin, apphour, watersupplied, irrareasqft;

bool PumpStatus;
long minToMilliSec;
bool ZPumpStatus;

String dfd;
char buff[1000];
String feedBack;
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftMina;
uint8_t oftHr;
String str;
bool DurationIrrbool;

float flowrate1;
float driper;
float dripline;
float   etvalue;
int hours_minutes;
float Irrigation_time;
float DWR_Val_ha;
int looping = 0;
float P_rate;

//===========================================
// Flag Defines
//===========================================

bool IrrMode_Qty_bool;
bool DurationIrr_bool;
bool DepthIrrMode_bool;

bool DepthIrrMode_shift01 = false;
bool DepthIrrMode_shift02;
bool DepthIrrMode_shift03;
bool Doser_bool;
bool Doser_pH_Enable;
bool Doser_A_Enable;
bool Doser_B_Enable;
bool Doser_C_Enable;
bool Doser_Acid_Enable;
bool Doser_pH_bool;
bool Doser_A_bool;

unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false;

unsigned long DoserPh_DelayTime;
unsigned long DoserA_DelayTime=10000;
unsigned long DoserB_DelayTime=10000;
unsigned long DoserC_DelayTime=10000 ;
unsigned long long DoserAcid_DelayTime;
//===========================================
// Sensor Defines
//===========================================
const float  OffSet = 0.483 ;
float V, Pressure;

// connect a flow meter to an interrupt pin (see notes on your Arduino model for pin numbers)
FlowMeter *Meter1;
FlowMeter *Meter2;
//===========================================
// Json Defines
//===========================================
DynamicJsonDocument doc(1024);



bool postionstatus;

//===========================================
// Single OnRelay Switcher
//===========================================
void nexval()
{
StaticJsonDocument<256> doc1;

 deserializeJson(doc1, Serial3); // Ok
  deserializeJson(doc1, Serial3); // InvalidInput
  deserializeJson(doc1, Serial3);
  
DeserializationError error = deserializeJson(doc1, Serial3);  
if (error) {

  return;
}

JsonObject Allrelays = doc1["Allrelays"];
int Allrelays_id = Allrelays["id"]; // 0
int Allrelays_SMV = Allrelays["SMV"]; // 1452
int Allrelays_SMP = Allrelays["SMP"]; // 84
int Allrelays_SMV_2 = Allrelays["SMV_2"]; // 1452
int Allrelays_SMP_2 = Allrelays["SMP_2"]; // 85
int Allrelays_TEMPC = Allrelays["TEMPC"]; // -127
int Allrelays_Temp = Allrelays["Temp"]; // 28
int Allrelays_Humi = Allrelays["Humi"]; // 47
int Allrelays_PRE = Allrelays["PRE"]; // 946
int Allrelays_Lat = Allrelays["Lat"]; // 0
int Allrelays_Lon = Allrelays["Lon"]; // 0
int Allrelays_Alt = Allrelays["Alt"]; // 0
int Allrelays_NI = Allrelays["NI"]; // 0
int Allrelays_PH = Allrelays["PH"]; // 0
int Allrelays_POT = Allrelays["POT"]; // 0
int Allrelays_LAT = Allrelays["LAT"]; // 0
int Allrelays_LON = Allrelays["LON"]; // 0
Serial.println(Allrelays_SMP);
  if (Allrelays_SMV > 0)
  {
    Serial2.print("g0.txt=\"");
    Serial2.print(Allrelays_id);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);


    Serial2.print("n1.txt=\"");
    Serial2.print(Allrelays_NI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("p1.txt=\"");
    Serial2.print(Allrelays_PH);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("k1.txt=\"");
    Allrelays_POT = sqrt(( Allrelays_POT) * (Allrelays_POT));
    Serial2.print(Allrelays_POT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ph1.txt=\"");
    Serial2.print(Allrelays_Lat);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ec1.txt=\"");
    Serial2.print(Allrelays_Lon);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sm11.txt=\"");
    Serial2.print(Allrelays_SMV);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sm21.txt=\"");
    Serial2.print(Allrelays_SMV_2);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("st1.txt=\"");
    Serial2.print(Allrelays_TEMPC);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("at1.txt=\"");
    Serial2.print(Allrelays_Temp);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  }
}
class SingleOnRelaySwitcher
{
    // class member variables
    byte relayPin; // number of pin for relay
    long OnTime;
    long OffTime;
    int relayState; // set relay state (active HIGH)
    unsigned long previousMillis; // set time since last update
  public:
    SingleOnRelaySwitcher(byte pin)
    {
      relayPin = pin;
    }
    void begin( long long int  on, bool StatusOfRelay)
    {
      PumpStatus = StatusOfRelay;
      pinMode(relayPin, OUTPUT);
      OnTime = on;
      relayState = HIGH;
      previousMillis = 0;
    }
    void Update()
    {
      Serial.println("okwe");
      unsigned long currentMillis = millis();
      if ((relayState == HIGH) && (currentMillis - previousMillis < OnTime ))
      {
        relayState = LOW; // Turn it off
        Serial.println("relay ON");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
        PumpStatus = true;
      }
      else if ((relayState == LOW) && (currentMillis - previousMillis > OnTime))
      {
        PumpStatus = false;
        ZPumpStatus = false;
        Doser_pH_bool = false;
        relayState = HIGH ; // turn it on
        previousMillis = currentMillis;
        Serial.println("relay OFF");
        digitalWrite(relayPin, relayState);

      }
    }
};
SingleOnRelaySwitcher Zone01_MainLinePump(irrigation);

bool Shift01() {

}

void Meter1ISR() {
  // let our flow meter count the pulses
  Meter1->count();
}

// define an 'interrupt service handler' (ISR) for every interrupt pin you use
void Meter2ISR() {
  // let our flow meter count the pulses
  Meter2->count();
}
int led = 13;
//===========================================
// setup
//===========================================
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);
  pinMode(irrigation, OUTPUT);
  digitalWrite(irrigation, HIGH);
  
  pinMode(safetyvalve, OUTPUT);
  digitalWrite(safetyvalve, HIGH);
  
  pinMode(fertsole, OUTPUT);
  digitalWrite(fertsole, HIGH);

  pinMode(BooosPump_pin, OUTPUT);
  digitalWrite(BooosPump_pin, HIGH);
  pinMode(Dose_pHUp_sol_pin, OUTPUT);
  digitalWrite(Dose_pHUp_sol_pin, HIGH);
  pinMode(Dose_A_sol_pin, OUTPUT);
  digitalWrite(Dose_A_sol_pin, HIGH);
  pinMode(Dose_B_sol_pin, OUTPUT);
  digitalWrite(Dose_B_sol_pin, HIGH);
  pinMode(Dose_C_sol_pin, OUTPUT);
  digitalWrite(Dose_C_sol_pin, HIGH);
  pinMode(Dose_Acid_sol_pin, OUTPUT);
  digitalWrite(Dose_Acid_sol_pin, HIGH);

  pinMode(IRR_InLetFlow, INPUT_PULLUP);
  pinMode(IRR_OutLetFlow, INPUT_PULLUP);

  pinMode(led, OUTPUT);   // initialize the digital pin as an output.
  digitalWrite(led, HIGH); // turn led on


  // get a new FlowMeter instance for an uncalibrated flow sensor and let them attach their 'interrupt service handler' (ISR) on every rising edge
  Meter1 = new FlowMeter(digitalPinToInterrupt(IRR_InLetFlow), UncalibratedSensor, Meter1ISR, RISING);

  // do this setup step for every  FlowMeter and ISR you have defined, depending on how many you need
  Meter2 = new FlowMeter(digitalPinToInterrupt(IRR_OutLetFlow), UncalibratedSensor, Meter2ISR, RISING);


rtc.begin();
rtc.adjust(DateTime(2023, 12, 31, 15, 31, 00));
}
void ok()
{
  deserializeJson(doc, Serial3);
  deserializeJson(doc, Serial3);
  deserializeJson(doc, Serial3);
  deserializeJson(doc, Serial3);
  DeserializationError error = deserializeJson(doc, Serial3);

  if (error)
  {
    return;
  }

  const char* Dayy = doc["Dayy"]; // "Wednesday"
  int  hourss = doc["hourss"]; // 12
  int minutess = doc["minutess"]; // 58
  int secondss = doc["secondss"]; // 5

  if ((secondss > 0) && (okk == true))
  {
    rtc.begin();
    rtc.adjust(DateTime(2023, 12, Dayy, hourss, minutess, secondss));
    okk == false;
  }


}

void rtcupdte()
{
  DateTime now = rtc.now();
  RTC_Hr = now.hour();
  RTC_Min = now.minute();
  secok = now.second();

 
}
void Qty_Doser_setup()
{

  if ((Doser_bool == true) && (Doser_A_Enable == true))
  {  
    if(AUTO_DoserA_Delay.isRunning())
    {
     pinMode(Dose_A_sol_pin,OUTPUT);
     digitalWrite(Dose_A_sol_pin , LOW);
    Serial.println("we are in on state");
    Serial.println(AUTO_DoserA_Delay.remaining());
    }
     if(AUTO_DoserA_Delay.justFinished())
    {
      Serial.println("we are in off state");
      digitalWrite(Dose_A_sol_pin , HIGH);
    }
  }

  if ((Doser_bool == true) && (Doser_B_Enable == true))
  {  
      if(AUTO_DoserB_Delay.isRunning())
    {
     pinMode(Dose_B_sol_pin,OUTPUT);
     digitalWrite(Dose_B_sol_pin , LOW);
    Serial.println("we are in on state");
    Serial.println(AUTO_DoserB_Delay.remaining());
    }
     if(AUTO_DoserB_Delay.justFinished())
    {
      Serial.println("we are in off state");
      digitalWrite(Dose_B_sol_pin , HIGH);
    }
  }
  if ((Doser_bool == true) && (Doser_C_Enable == true))
  {

      if(AUTO_DoserC_Delay.isRunning())
    {
     pinMode(Dose_C_sol_pin,OUTPUT);
     digitalWrite(Dose_C_sol_pin , LOW);
    Serial.println("we are in on state");
    Serial.println(AUTO_DoserC_Delay.remaining());
    }
     if(AUTO_DoserC_Delay.justFinished())
    {
      Serial.println("we are in off state");
      digitalWrite(Dose_C_sol_pin , HIGH);
    }
  }
}



void irr()
{
  if ((looping <= 5) && (Irrigation_time > 1))
  {
    Serial2.print("t0.txt=\"");
    Serial2.print(Irrigation_time);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("t1.txt=\"");
    Serial2.print(hours_minutes);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    looping++;
  } 
    DepthIrrMode();
}

bool Of = true;
bool If = false;
bool Rs = false;
bool pressurecheck =false;

void sensorfun()
{


  if ((Of == true) && (If == false) && (Rs == false))
  {
//    Serial.println("1");
    OutFlowRate();
    Of = false;
    Rs = false;
    If = true;
  }
  else if ((Of == false) && (If == true) && (Rs == false))
  {
//    Serial.println("2");
    InFlowRate();
    Of = false;
    Rs = true;
    If = false;
    pressurecheck =true;
    

  }
  else if ((Of == false) && (If == false) && (Rs == true))
  {
//    Serial.println("3");
    ReadingSensorData();
    Of = true;
    Rs = false;
    If = false;
    pressurecheck =true;
  }
    Serial2.print("irrp.txt=\"");
  Serial2.print(MainInPressure);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("inlf.txt=\"");
  Serial2.print(InFlowRateMin);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("inlft.txt=\"");
  Serial2.print(InFlowTotalVolume);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("otlf.txt=\"");
  Serial2.print(OutFlowRateMin);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("otlft.txt=\"");
  Serial2.print(OutFlowTotalVolume);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("frrp.txt=\"");
  Serial2.print(Mainpressure);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


}
void s_r_status()
{
int r1 =digitalRead(irrigation);
  Serial2.print("bt0.val=" + String(r1));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r2 =digitalRead(safetyvalve);
   Serial2.print("bt1.val=" + String(r2));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r3 =digitalRead(zone1valve);
  Serial2.print("bt2.val=" + String(r3));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r4 =digitalRead(zone2valve);
   Serial2.print("bt3.val=" + String(r4));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r5 =digitalRead(fertpump);
 Serial2.print("bt4.val=" + String(r5));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r6 =digitalRead(fertsole);
   Serial2.print("bt5.val=" + String(r6));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r7 =digitalRead(mixpump);
  Serial2.print("bt6.val=" + String(r7));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r8 =digitalRead(Dose_A_sol_pin);
   Serial2.print("bt7.val=" + String(r8));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r9 =digitalRead(Dose_B_sol_pin);
   Serial2.print("bt8.val=" + String(r9));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r10 =digitalRead(Dose_C_sol_pin);
   Serial2.print("bt9.val=" + String(r10));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r11 =digitalRead(Dose_Acid_sol_pin);
  Serial2.print("bt10.val=" + String(r11));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
int r12 =digitalRead(Dose_pHUp_sol_pin);
 Serial2.print("bt11.val=" + String(r12));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);             
}
//===========================================
// Loop
//===========================================
unsigned long SCpreviousMillis = 0;

unsigned long NXpreviousMillis = 0;

unsigned long irrpreviousMillis = 0;


unsigned long dospreviousMillis = 0;

unsigned long snspreviousMillis = 0;

void loop()
{
//  unsigned long maxx = millis();
//  if (maxx - minn >= 3000)
//  {
//    minn = maxx;
//    ok();
//
//  }
//
//  unsigned long maxy = millis();
//  if (maxy - miny >= 3000)
//  {
//    miny = maxy;
//    rtcupdte();
//
//
//  }
//  
  ///***********************///  ///********************///  ///********************///
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500)
  {
    SCpreviousMillis = currentMillis;

    SerialCom();
  }
    
///**********************///  ///********************///  ///********************///
//  unsigned long NXcurrentMillis = millis();
//  if (NXcurrentMillis - NXpreviousMillis >= 90000)
//  {
//    NXpreviousMillis = NXcurrentMillis;
//
//    nexval();
//  }
//
//  /********************///  ///********************///  ///********************///
// unsigned long irrcurrentMillis = millis();
//  if (irrcurrentMillis - irrpreviousMillis >= 80000)
//  {
//  
//    irrpreviousMillis = irrcurrentMillis;
//    
//   irr();
//  }

 ///***********************//////***********************//////***********************///
  unsigned long doscurrentMillis = millis();
  if (doscurrentMillis - dospreviousMillis >= 3000)
  {
    dospreviousMillis = doscurrentMillis;

    Qty_Doser_setup();
  }
//  unsigned long snscurrentMillis = millis();
//  if (snscurrentMillis - snspreviousMillis >= 20000)
//  {
//    snspreviousMillis = snscurrentMillis;
//    s_r_status();
//    sensorfun();   
//  }

}
