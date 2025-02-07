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
//===========================================
// Defines
//===========================================

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
#define MainInPressure_Pin    A1
#define IRR_InLetFlow 2
#define IRR_OutLetFlow 3

#define IRR_Pump_Zone01 12
#define IRR_Pump_Zone02 13
#define IRR_Pump_Zone03 11
#define IRR_Pump_Zone04 10

#define IRR_MainLine_SolVal 33
#define IRR_MainLine_SumpPump 12

#define ms_per_hour  3600000
#define ms_per_min    60000//
#define ms_per_sec    1000

#define Dose_pHUp_sol_pin 12
#define Dose_A_sol_pin 13
#define Dose_B_sol_pin 14
#define Dose_C_sol_pin 15
#define Dose_Acid_sol_pin 16
#define BooosPump_pin 17

#define Relay_ON LOW
#define Relay_OFF HIGH
int relayState;
int InFlowRateMin ;
int InFlowTotalVolume;
int SetVal_IrrValume;
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
uint8_t oftHr;
String str;
bool DurationIrrbool;

//===========================================
// Flag Defines
//===========================================

bool IrrMode_Qty_bool;
bool DurationIrr_bool;
bool DepthIrrMode_bool;
bool DepthIrrMode_shift01;
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
unsigned long DoserA_DelayTime;
unsigned long DoserB_DelayTime;
unsigned long DoserC_DelayTime ;
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
DynamicJsonDocument megadoc(1024);

bool postionstatus;

//===========================================
// Single OnRelay Switcher
//===========================================

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
    void begin( long on, bool StatusOfRelay)
    {

      PumpStatus = StatusOfRelay;
      pinMode(relayPin, OUTPUT);
      OnTime = on;

      relayState = HIGH;
      previousMillis = 0;
    }
    void Update()
    {
      // check the time to see if relays need to be turned on
      // or off
      unsigned long currentMillis = millis();

      if ((relayState == HIGH) && (currentMillis - previousMillis >= OnTime ))
      {
        relayState = LOW; // Turn it off
        Serial.println("relay ON");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
        PumpStatus = true;
      }
      else if ((relayState == LOW) )
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
SingleOnRelaySwitcher Zone01_MainLinePump(12);

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

  pinMode(IRR_Pump_Zone01, OUTPUT);
  digitalWrite(IRR_Pump_Zone01, LOW);
  pinMode(IRR_MainLine_SolVal, OUTPUT);
  digitalWrite(IRR_MainLine_SolVal, LOW);
  digitalWrite(IRR_MainLine_SumpPump, LOW);

  pinMode(BooosPump_pin, OUTPUT);
  digitalWrite(BooosPump_pin, LOW);
  pinMode(Dose_pHUp_sol_pin, OUTPUT);
  digitalWrite(Dose_pHUp_sol_pin, LOW);
  pinMode(Dose_A_sol_pin, OUTPUT);
  digitalWrite(Dose_A_sol_pin, LOW);
  pinMode(Dose_B_sol_pin, OUTPUT);
  digitalWrite(Dose_B_sol_pin, LOW);
  pinMode(Dose_C_sol_pin, OUTPUT);
  digitalWrite(Dose_C_sol_pin, LOW);
  pinMode(Dose_Acid_sol_pin, OUTPUT);
  digitalWrite(Dose_Acid_sol_pin, LOW);

  pinMode(IRR_InLetFlow, INPUT_PULLUP);
  pinMode(IRR_OutLetFlow, INPUT_PULLUP);

  pinMode(led, OUTPUT);   // initialize the digital pin as an output.
  digitalWrite(led, HIGH); // turn led on


  // get a new FlowMeter instance for an uncalibrated flow sensor and let them attach their 'interrupt service handler' (ISR) on every rising edge
  Meter1 = new FlowMeter(digitalPinToInterrupt(IRR_InLetFlow), UncalibratedSensor, Meter1ISR, RISING);

  // do this setup step for every  FlowMeter and ISR you have defined, depending on how many you need
  Meter2 = new FlowMeter(digitalPinToInterrupt(IRR_OutLetFlow), UncalibratedSensor, Meter2ISR, RISING);

  //  float ZETvalue = 5.5 ;
  //  int ZFlowRate = 70 ;
  //  int ZArea = 1.5 ;
  //  byte ZRelayPin = 12 ;
  //  bool ZPumpStatus = true ;
  //
  //  int ZoneAMin = ZoneIrigatorCal(ZETvalue, ZFlowRate, ZArea); //(float ETval, float FloeRate,Areain Acre);
  //  Serial.print("Zone A Irrigation Time :  ");
  //  Serial.println (ZoneAMin);
  //
  //  Zone01_MainLinePump.begin( minToMilliSec, ZPumpStatus); // RelayPIN,ONTime,RelayStatus
  //
  //
  //  int ZoneA_ET = ZoneIrigatorCal(5.4, 70, 1.0);
  //  Serial.print("ZoneA_ET: ");
  //  Serial.println(ZoneA_ET);

  //  //Test Fun
  //  int Z1_IRR_hr = 00;
  //  int Z1_IRR_min = 01;
  //
  //  Serial.print("Z1_IRR_hr: ");
  //  Serial.println(Z1_IRR_hr);
  //  Serial.print("Z1_IRR_min: ");
  //  Serial.println(Z1_IRR_min);
  //
  //  Serial.print("Due switch Start");
  //  long Due_IRR_minToMilliSec = 1;
  //  long Z1_IRR_OnTimeHr = Z1_IRR_hr * ms_per_hour;
  //  long Z1_IRR_OnTimeMIn = Z1_IRR_min * ms_per_min;
  //  long ZoneA_TotelOnTime = Z1_IRR_OnTimeHr + Z1_IRR_OnTimeMIn;
  //
  //  Serial.print("   ZoneA_TotelOnTime:  ");
  //  Serial.println(ZoneA_TotelOnTime);
  //
  //  DurationIrrbool = true;
  //  Due_IRR.begin( ZoneA_TotelOnTime, ZPumpStatus); // RelayPIN,ONTime,RelayStatus
  //
  //  Serial.println("Irr Z1 On");


  /*
    //************************************************************
    //this funtion onetime call from nextion for Depth irrigation
    DepthIrrMode_shift01 = true;
    DepthIrrMode_shift02 = true;
    DepthIrrMode_shift03 = true;

    PumpStatus = true;
    IrrZoneController(5.2, 80, 1, IRR_Pump_Zone01, PumpStatus);
     //************************************************************
  */

 irrigation_area_based_watersupply();
// NPKcal();

}


void Qty_Doser_setup() {
  //Dosing testing
  Doser_pH_Enable = true;
  Doser_A_Enable = true;
  Doser_B_Enable = true;
  Doser_C_Enable = true;
  Doser_Acid_Enable = true;

  Doser_bool = true;
  relayState = HIGH;

  //  delayStart = millis();   // start delay
  //  delayRunning = true; // not finished yet

  unsigned long DoserPh_DelayTime = ms_per_sec * 10;
  unsigned long DoserA_DelayTime = ms_per_sec * 20;
  unsigned long DoserB_DelayTime = ms_per_sec * 5;
  unsigned long DoserC_DelayTime = ms_per_sec * 30;
  unsigned long long DoserAcid_DelayTime = ms_per_sec * 60;

  DoserPh_Delay.start(DoserPh_DelayTime);  // start a 10sec delay
  DoserA_Delay.start(DoserA_DelayTime);  // start a 10sec delay
  DoserB_Delay.start(DoserB_DelayTime);  // start a 10sec delay
  DoserC_Delay.start(120000);  // start a 10sec delay
  DoserAcid_Delay.start(60 * 1000); // start a 10sec delay
  Serial.println("Qty_Doser_setup Start");
}
//Di-ammoium
int dap;
int dap_ratio;

int DAP(int dap_ratio) {

  int ratioOfdap = dap_ratio;
  dap = 100 / 46 * dap_ratio;

  return dap;

}

int mop;
int mop_ratio;

int MOP(int mop_ratio) {


  int ratioOfmpo = mop_ratio;
  mop = 100 / 46 * mop_ratio;

  return mop;

}


void Auto_Doser_setup() {
  AUTO_DoserPh_Delay.start(DoserPh_DelayTime);  // start a 10sec delay
  AUTO_DoserA_Delay.start(DoserA_DelayTime);  // start a 10sec delay
  AUTO_DoserB_Delay.start(DoserB_DelayTime);  // start a 10sec delay
  AUTO_DoserC_Delay.start(120000);  // start a 10sec delay
  AUTO_DoserAcid_Delay.start(60 * 1000); // start a 10sec delay
}
//===========================================
// Loop
//===========================================
void loop() {
  // put your main code here, to run repeatedly:
  //  if (PumpStatus == true) {
  //
  //    Zone01_MainLinePump.Update();
  //
  //  }
  //
  //  if (DepthIrrMode_bool == true) {
  //
  //    Zone01_MainLinePump.Update();
  //  }
  //
  //  if (DurationIrr_bool == true) {
  //
  //    DurationIrrMode();
  //  }
  //  if (IrrMode_Qty_bool) {
  //    VolumeIrrMode();
  //  }
  if (Doser_bool == true) {
    DoserFun();
  }
  //  if (Doser_A_Enable == true) {
  //    DoserFun();
  //  }

  //  ReadingSensorData();
  //  delay(2000);
  //
    Nitrogen();

}
