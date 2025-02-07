
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

millisDelay irrslot1;
millisDelay getvalue;

#define IRR_InLetFlow  2
#define IRR_OutLetFlow 3



#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000

#define safetyvalve 26//solenooid
#define fertpump 27//pump 240 volts
#define zone2valve 28//solenooid
#define fertsole 29//solenooid
#define irrigation 30//pump 240 volts
#define zone1valve 31//solenooid
#define mixpump 33//pump 240 volts
#define Dose_A_sol_pin 32//pump 240 volts
#define Dose_B_sol_pin 34//pump 240 volts
#define Dose_C_sol_pin 35//pump 240 volts
#define Dose_Acid_sol_pin 36//pump 240 volts
#define Dose_pHUp_sol_pin 37//pump 240 volts
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
unsigned long  DoserA_DelayTime;
unsigned long DoserB_DelayTime;
unsigned long DoserC_DelayTime;
unsigned long long DoserAcid_DelayTime;
//===========================================
// Sensor values
int Allsensordata_id ;
int Allsensordata_SMV;
int Allsensordata_SMP ;
int Allsensordata_SMV_2;
int Allsensordata_SMP_2;
int Allsensordata_TEMPC ;
int Allsensordata_Temp ;
int Allsensordata_Humi ;
int Allsensordata_PRE ;
float Allsensordata_Lat ;
float Allsensordata_Lon ;
int Allsensordata_Alt ;
int Allsensordata_NI;
int Allsensordata_PH ;
int Allsensordata_POT ;
int Allsensordata_LAT ;
int Allsensordata_LON ;
int Allsensordata_WS;
int Allsensordata_WD ;
double Allsensordata_UV;
double Allsensordata_TEMP;
double Allsensordata_HUM ;
double Allsensordata_PRS;
int Allsensordata_temp;
double Allsensordata_hum ;
double  Allsensordata_pre ;
double Allsensordata_vpd;
double Allsensordata_et ;
double Allsensordata_bp ;
double Allsensordata_dw ;
int Allsensordata_sl ;
double Allsensordata_sr ;
int Allsensordata_hi ;
int Allsensordata_ch ;
//===========================================

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
int yearr ;
   int monthhh;
   int dayyy;
//===========================================
// Single OnRelay Switcher
//===========================================
void nexval()
{
  StaticJsonDocument<1500> doc1;

  deserializeJson(doc1, Serial3); // Ok
  deserializeJson(doc1, Serial3); // InvalidInput
  deserializeJson(doc1, Serial3);

  DeserializationError error = deserializeJson(doc1, Serial3);
  if (error) {

    return;
  }

JsonObject metrics = doc1["metrics"];
double metrics_longitude = metrics["longitude"]; // 78.32450104
double metrics_latitude = metrics["latitude"]; // 17.38069916
double metrics_WStemp = metrics["WStemp"]; // 37.70000076
double metrics_WShum = metrics["WShum"]; // 47.40000153
double metrics_WSvpd = metrics["WSvpd"]; // 3.400000095
double metrics_WSet = metrics["WSet"]; // -6.800000191
double metrics_WSsr = metrics["WSsr"]; // 267.907135
int metrics_WShi = metrics["WShi"]; // 46
double metrics_WSbp = metrics["WSbp"]; // 945.2000122
int metrics_WSsl = metrics["WSsl"]; // 1129
double metrics_WSdp = metrics["WSdp"]; // 24.60000038
double metrics_WSpr = metrics["WSpr"]; // 47.40000153
int metrics_WSch = metrics["WSch"]; // 1024
int metrics_MSTemp = metrics["MSTemp"]; // 37
int metrics_MSHumi = metrics["MSHumi"]; // 51
int metrics_MSPRE = metrics["MSPRE"]; // 943
int metrics_MSSMV = metrics["MSSMV"]; // 1520
int metrics_MSSMP = metrics["MSSMP"]; // 80
int metrics_MSSMV_2 = metrics["MSSMV_2"]; // 1520
int metrics_SMP_2 = metrics["SMP_2"]; // 80
int metrics_MSTEMPC = metrics["MSTEMPC"]; // -127
int metrics_lw = metrics["lw"]; // 14
int metrics_fat = metrics["fat"]; // 11
int metrics_co2 = metrics["co2"]; // 88
int metrics_fet = metrics["fet"]; // 195
int metrics_lt = metrics["lt"]; // 50
int metrics_frh = metrics["frh"]; // 11
int metrics_ar = metrics["ar"]; // 99
int metrics_fdp = metrics["fdp"]; // 57
int metrics_NI = metrics["NI"]; // 19
int metrics_PH = metrics["PH"]; // 6
int metrics_POT = metrics["POT"]; // 9
int metrics_WS = metrics["WS"]; // 0
int metrics_WD = metrics["WD"]; // 0
Serial.println(metrics_POT);
Serial.println(metrics_PH);
Serial.println(metrics_NI);

  if (metrics_NI > 0)
  {
    Allsensordata_SMV = metrics_MSSMV ;
    Allsensordata_SMP  = metrics_MSSMP ;
    Allsensordata_SMV_2 = metrics_MSSMV_2;
    Allsensordata_SMP_2 = metrics_SMP_2 ;
    Allsensordata_TEMPC = metrics_MSTEMPC;
    Allsensordata_Temp = metrics_MSTemp ;
    Allsensordata_Humi  = metrics_MSHumi;
    Allsensordata_PRE  = metrics_MSPRE;
    Allsensordata_NI = metrics_NI ;
    Allsensordata_PH  = metrics_PH ;
    Allsensordata_POT  = metrics_POT ;
    Allsensordata_LAT  =metrics_latitude;
    Allsensordata_LON  = metrics_longitude;
    Allsensordata_WS = metrics_WS ;
    Allsensordata_WD = metrics_WD ;
    Allsensordata_temp = metrics_WStemp;
    Allsensordata_hum = metrics_WShum;
    Allsensordata_pre = metrics_WSpr ;
    Allsensordata_vpd = metrics_WSvpd ;
    Allsensordata_et = metrics_WSet ;
    Allsensordata_bp = metrics_WSbp ;
    Allsensordata_dw = metrics_WSdp ;
    Allsensordata_sl = metrics_WSsl ;
    Allsensordata_sr = metrics_WSsr ;
    Allsensordata_hi = metrics_WShi ;
    Allsensordata_ch = metrics_WSch ;
Allsensordata_et = abs(Allsensordata_et);
     Serial.println(Allsensordata_et);
  }
}
void tonextion()
{
 

   Serial2.print("hour.txt=\"");
  Serial2.print(RTC_Hr);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("min.txt=\"");
  Serial2.print(RTC_Min);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("sec.txt=\"");
  Serial2.print(secok);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("year.txt=\"");
  Serial2.print(yearr);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("month.txt=\"");
  Serial2.print(monthhh);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

    Serial2.print("day.txt=\"");
  Serial2.print(dayyy);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);




//*************************//   
 if(Allsensordata_NI>0)
  {
     Serial2.print("hour.txt=\"");
  Serial2.print(RTC_Hr);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("min.txt=\"");
  Serial2.print(RTC_Min);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("sec.txt=\"");
  Serial2.print(secok);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("year.txt=\"");
  Serial2.print(yearr);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("month.txt=\"");
  Serial2.print(monthhh);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

    Serial2.print("day.txt=\"");
  Serial2.print(dayyy);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.print("n1.txt=\"");
  Serial2.print(Allsensordata_NI);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("n2.txt=\"");
  Serial2.print(Allsensordata_NI-18);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("p1.txt=\"");
  Serial2.print(Allsensordata_PH);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("p2.txt=\"");
  Serial2.print(Allsensordata_PH+20);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("k1.txt=\"");
  Serial2.print(Allsensordata_POT);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

    Serial2.print("k2.txt=\"");
  Serial2.print(Allsensordata_POT+30);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("ph1.txt=\"");
  Serial2.print(random(6, 9));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
   Serial2.print("ph2.txt=\"");
  Serial2.print(random(6, 9));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("ec1.txt=\"");
  Serial2.print(random(1, 2));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("ec2.txt=\"");
  Serial2.print(random(1, 2));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
 
  Serial2.print("sm11.txt=\"");
  Serial2.print(Allsensordata_SMP);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
 
  
  Serial2.print("sm12.txt=\"");
  Serial2.print(Allsensordata_SMP);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  
 
  Serial2.print("sm21.txt=\"");
  Serial2.print(Allsensordata_SMP);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  
  Serial2.print("sm22.txt=\"");
  Serial2.print(Allsensordata_SMP);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
 
  
  Serial2.print("st1.txt=\"");
  Serial2.print(random(20, 30));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.print("st2.txt=\"");
  Serial2.print(random(20, 30));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("at1.txt=\"");
  Serial2.print(Allsensordata_Temp);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
    Serial2.print("at2.txt=\"");
  Serial2.print(Allsensordata_Temp);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("et1.txt=\"");
  Serial2.print(Allsensordata_et );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  
  Serial2.print("et2.txt=\"");
  Serial2.print(Allsensordata_et );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wat.txt=\"");
  Serial2.print(Allsensordata_temp );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wvpd.txt=\"");
  Serial2.print(Allsensordata_vpd);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.print("wrh.txt=\"");
  Serial2.print(Allsensordata_hum );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wet.txt=\"");
  Serial2.print(Allsensordata_et);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wsr.txt=\"");
  Serial2.print(Allsensordata_sr );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("ws2.txt=\"");
  Serial2.print(Allsensordata_WS);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wrp.txt=\"");
  Serial2.print(random(0, 00));
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wch.txt=\"");
  Serial2.print(Allsensordata_ch );
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wd.txt=\"");
  Serial2.print(Allsensordata_WD);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("wbp.txt=\"");
  Serial2.print(Allsensordata_bp );
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
void setup()
{
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
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));



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
   yearr = now.year();
   monthhh = now.month();
   dayyy = now.day();
   
  //Serial.print(RTC_Hr);
  //Serial.print(":");
  //Serial.print(RTC_Min);
  //Serial.print(":");
  //Serial.print(secok);
  //Serial.println(".");
}
void Qty_Doser_setup()
{

  if ((Doser_bool == true) && (Doser_A_Enable == true))
  {
    if (AUTO_DoserA_Delay.isRunning())
    {
      pinMode(Dose_A_sol_pin, OUTPUT);
      digitalWrite(Dose_A_sol_pin , LOW);

      Serial.println("we are in on state");
      Serial.println(AUTO_DoserA_Delay.remaining());
    }
    if (AUTO_DoserA_Delay.justFinished())
    {
      Serial.println("we are in off state");
      digitalWrite(Dose_A_sol_pin , HIGH);
    }
  }

  if ((Doser_bool == true) && (Doser_B_Enable == true))
  {
    if (AUTO_DoserB_Delay.isRunning())
    {
      pinMode(Dose_B_sol_pin, OUTPUT);
      digitalWrite(Dose_B_sol_pin , LOW);
      Serial.println("we are in on state");
      Serial.println(AUTO_DoserB_Delay.remaining());
    }
    if (AUTO_DoserB_Delay.justFinished())
    {
      Serial.println("we are in off state");
      digitalWrite(Dose_B_sol_pin , HIGH);
    }
  }
  if ((Doser_bool == true) && (Doser_C_Enable == true))
  {

    if (AUTO_DoserC_Delay.isRunning())
    {
      pinMode(Dose_C_sol_pin, OUTPUT);
      digitalWrite(Dose_C_sol_pin , LOW);
      Serial.println("we are in on state");
      Serial.println(AUTO_DoserC_Delay.remaining());

    }
    if (AUTO_DoserC_Delay.justFinished())
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
bool pressurecheck = false;

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
    pressurecheck = true;


  }
  else if ((Of == false) && (If == false) && (Rs == true))
  {
    //    Serial.println("3");
    ReadingSensorData();
    Of = true;
    Rs = false;
    If = false;
    pressurecheck = true;
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
  int r1 = digitalRead(irrigation);
  Serial2.print("bt0.val=" + String(r1));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r2 = digitalRead(safetyvalve);
  Serial2.print("bt1.val=" + String(r2));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r3 = digitalRead(zone1valve);
  Serial2.print("bt2.val=" + String(r3));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r4 = digitalRead(zone2valve);
  Serial2.print("bt3.val=" + String(r4));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r5 = digitalRead(fertpump);
  Serial2.print("bt4.val=" + String(r5));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r6 = digitalRead(fertsole);
  Serial2.print("bt5.val=" + String(r6));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r7 = digitalRead(mixpump);
  Serial2.print("bt6.val=" + String(r7));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r8 = digitalRead(Dose_A_sol_pin);
  Serial2.print("bt7.val=" + String(r8));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r9 = digitalRead(Dose_B_sol_pin);
  Serial2.print("bt8.val=" + String(r9));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r10 = digitalRead(Dose_C_sol_pin);
  Serial2.print("bt9.val=" + String(r10));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r11 = digitalRead(Dose_Acid_sol_pin);
  Serial2.print("bt10.val=" + String(r11));
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  int r12 = digitalRead(Dose_pHUp_sol_pin);
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

  ///***********************///  ///********************///  ///********************///
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500)
  {
    SCpreviousMillis = currentMillis;

    SerialCom();
  }

  ///**********************///  ///********************///  ///********************///

  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - NXpreviousMillis >= 100000)
  {
    NXpreviousMillis = NXcurrentMillis;

    getvalue.start(10000);
  }
  if (getvalue.isRunning())
  {
    nexval();
    tonextion();

  }
  if (getvalue.justFinished())
  {
    Serial.println("we are done here");
  }




  //  /********************///  ///********************///  ///********************///
  unsigned long irrcurrentMillis = millis();
  if (irrcurrentMillis - irrpreviousMillis >= 10000)
  {
    rtcupdte();

    irrpreviousMillis = irrcurrentMillis;

    irr();
  }

  ///***********************//////***********************//////***********************///
  unsigned long doscurrentMillis = millis();
  if (doscurrentMillis - dospreviousMillis >= 10000)
  {


    dospreviousMillis = doscurrentMillis;

    Qty_Doser_setup();
  }


  unsigned long snscurrentMillis = millis();
  if (snscurrentMillis - snspreviousMillis >= 30000)
  {
    snspreviousMillis = snscurrentMillis;
    s_r_status();
    sensorfun();
  }

}
