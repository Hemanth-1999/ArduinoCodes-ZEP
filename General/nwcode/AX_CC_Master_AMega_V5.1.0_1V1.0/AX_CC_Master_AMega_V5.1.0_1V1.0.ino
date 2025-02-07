

/*17/05/2022
 * 19/105/22
 * New Mutitask

   VPD Mode working
   Basic CC setup
*/

#include <Arduino.h>
#include <MemoryFree.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <MultiTasking.h>  //Adding the MultiTasking library.
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <EEPROMex.h>
#include <TaskScheduler.h>

String data_from_display = "";
String dfd;
String str;
String pwdname();
String pwd();
String feedBack;
String ssid;
String password;


char buff[2000];
//float tmp;
//float hmd;
//bool vpd_func;
//bool vpdfun_cal;
bool temp_func;

long seconds = 00;
long minutes = 00;
long hours = 00;
uint16_t hourrc;
uint16_t minrc;
uint16_t secsrc;
uint16_t Monthrc;
uint16_t Daterc;
uint8_t secslive;


int Yearrc;
int val;
int CS_pin = 10;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;
int MESPWS_ID_Time_WSntips_v,MESPWS_ID_Time_SeL_v,MESPWS_ID_Time_ntl_v;
int MESP_ID_Time_SeL_v_NZvalue, MESP_ID_Time_co2_v_NZvalue,MESPWS_ID_Time_SeL_v_NZvalue;
int relayState;
int hourupg;
int minupg;
int Monthlive;
int Datelive;
int Yearlive;


float VPD_val;
float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v,MESP_ID_Time_AvgVPD_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v, MESP_ID_Time_AvgAT_v_NZvalue, MESP_ID_Time_AvgVPD_v_NZvalue;
float MESPWS_ID_Time_AT_v,MESPWS_ID_Time_RH_v,MESPWS_ID_Time_VPD_v,MESPWS_ID_Time_HI_v,MESPWS_ID_Time_EvTr_v,MESPWS_ID_Time_BaPr_v,MESPWS_ID_Time_SoRa_v,MESPWS_ID_Time_DePt_v;
float MESPWS_ID_Time_WSwsclk_v,MESPWS_ID_Time_WSwsaclk_v,MESPWS_ID_Time_WSwdclk_v,MESPWS_ID_Time_WSwdaclk_v,MESPWS_ID_Time_WSclohei_v;
float MESP_ID_Time_AvgRH_v_NZvalue,MESP_ID_Time_AvgHI_v_NZvalue,MESP_ID_Time_EvTr_v_NZvalue,MESP_ID_Time_BaPr_v_NZvalue;
float MESP_ID_Time_SoRa_v_NZvalue,MESP_ID_Time_DePt_v_NZvalue;
float  MESPWS_ID_Time_AT_v_NZvalue,MESPWS_ID_Time_RH_v_NZvalue,MESPWS_ID_Time_VPD_v_NZvalue,MESPWS_ID_Time_HI_v_NZvalue,MESPWS_ID_Time_EvTr_v_NZvalue;
float MESPWS_ID_Time_BaPr_v_NZvalue,MESPWS_ID_Time_DePt_v_NZvalue,MESPWS_ID_Time_SoRa_v_NZvalue;
float ATemp, AHumi;
float vpdlowsp = 0.8;
float vpdhighsp = 1.4;


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
//File sd_file;

//RTC

#define DEBUG 0                                // change value to 1 to enable debuging using serial monitor  
#define SERIAL_PORT_SPEED 115200//9600, 19200, 38400, 57600, 115200

RTC_DS3231 rtc; //for date time
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


//***********************************************************************************************************************


int test = 55;

int ledCtrl = HIGH;

//Relay-1
const int ExFan1 = A3; // 8;//11;
const int ExFan2 = A2;//9;//10;
const int ExFan3 = 30;//10;//9;
const int ExFan4 = 28;//11;//8;
//Relay-2
const int ExFan5 = 9;//A3;//28;
const int ExFan6 = 8; // A2; // 30;
const int ExFan7 = 10;//30;//A3;
const int ExFan8 = 11;//28;//A2;
//Relay-3
const int pad1 = A4;//A7;//cooling
int Fogger = A6;//A5;//pump
int FoggerSol = A5;//A6;//controlz
int Co2Gen = A7;//A4;
//Relay-4(change)
//const int pad1 = 4;
//int Fogger = 3;
//int FoggerSol = 23;
//int Co2Gen = 25;

int Topven = LOW;
int GWindow = LOW;
int Curtains = LOW;


int heater = LOW;
int hp = LOW;
int Chiller = LOW;

int Cfan1 = 23;
int Cfan2 = 25;
//int BLledCtrl = LOW;

//******************************************Time Date Setting**************************************************************


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


//// Tasks
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
void VpdControlMode() {
  Serial.print(F("dummy  "));
  vpd_control(MESP_ID_Time_AvgVPD_v_NZvalue);
  modevpdstatus = true;

}
void AutoControlMode() {
  Serial.print("dummy  2");
  AutoTemp_control(MESP_ID_Time_AvgAT_v_NZvalue);
  modetempstatus = true;
}

//!******************************************************************************
//! Name: node();---->FromEspData()                                  *
//! Description:                      *
//! Param :                                      *
//! Returns: void                                     *
//! Example: TIME              *
//!******************************************************************************
void FromEspData() {

  StaticJsonDocument<512> doc;
  deserializeJson(doc, Serial3); // Ok
  deserializeJson(doc, Serial3); // InvalidInput
  //  deserializeJson(doc, Serial3); // InvalidInput
  DeserializationError error = deserializeJson(doc, Serial3);

  if (error) {
    //    Serial.print(F("deserializeJson() failed: "));
    //    Serial.println(error.f_str());
    return;
  }

  long Time = doc["Time"].as<long>(); // 13420231
  long ID = doc["ID"].as<long>(); // 13420231
  
  MESP_ID_Time_AvgAT_v = doc["AT"].as<float>(); // 30.38000107
  MESP_ID_Time_AvgRH_v = doc["RH"].as<float>(); // 37.27999878
  MESP_ID_Time_AvgVPD_v = doc["VPD"].as<float>(); // 2.717657566
  MESP_ID_Time_AvgHI_v = doc["HI"].as<float>(); // 33.03442383
  MESP_ID_Time_co2_v = doc["Co2"].as<long>(); // 283
  MESP_ID_Time_EvTr_v = doc["Ev"].as<long>(); // 38
  MESP_ID_Time_BaPr_v = doc["BaPa"].as<long>(); // 26
  MESP_ID_Time_SoRa_v = doc["Solar"].as<long>(); // 139
  MESP_ID_Time_DePt_v = doc["Dew"].as<long>(); // 20
  MESP_ID_Time_SeL_v = doc["SeaL"].as<long>(); // 1269
  MESP_ID_Time_ntl_v = doc["ex"].as<long>(); // 34


  MESPWS_ID_Time_AT_v = doc["WAT"].as<float>(); // 32
  MESPWS_ID_Time_RH_v = doc["WRH"].as<float>(); // 74
  MESPWS_ID_Time_VPD_v = doc["WVPD"].as<float>(); // 1
  MESPWS_ID_Time_HI_v = doc["WHI"].as<float>(); // 27
  MESPWS_ID_Time_EvTr_v = doc["WEv"].as<float>(); // 10
  MESPWS_ID_Time_BaPr_v = doc["WBaPa"].as<long>(); // 32
  MESPWS_ID_Time_SoRa_v = doc["WSolar"].as<long>(); // 240
  MESPWS_ID_Time_DePt_v = doc["WDew"].as<long>(); // 0
  MESPWS_ID_Time_SeL_v = doc["WSeaL"].as<long>(); // 3990
  MESPWS_ID_Time_WSwsclk_v = doc["WSclk"].as<long>(); // 0
  MESPWS_ID_Time_WSwsaclk_v = doc["WSaclk"].as<long>(); // 0
  MESPWS_ID_Time_WSwdclk_v = doc["WDclk"].as<long>(); // 0
  MESPWS_ID_Time_WSwdaclk_v = doc["WDaclk"].as<long>(); // 0
  //  MESP_ID_Time_AvgAT_v = random(3,9);
  
if((MESP_ID_Time_AvgAT_v>0)&&(MESP_ID_Time_AvgVPD_v>0)){
  

     MESP_ID_Time_AvgAT_v_NZvalue= MESP_ID_Time_AvgAT_v; 
  MESP_ID_Time_AvgVPD_v_NZvalue=MESP_ID_Time_AvgVPD_v;
  MESP_ID_Time_AvgRH_v_NZvalue =MESP_ID_Time_AvgRH_v; 

   MESP_ID_Time_AvgHI_v_NZvalue= MESP_ID_Time_AvgHI_v; 
 MESP_ID_Time_co2_v_NZvalue= MESP_ID_Time_co2_v;
  MESP_ID_Time_EvTr_v_NZvalue= MESP_ID_Time_EvTr_v;
   MESP_ID_Time_BaPr_v_NZvalue=MESP_ID_Time_BaPr_v; 
  MESP_ID_Time_SoRa_v_NZvalue=MESP_ID_Time_SoRa_v; 
   MESP_ID_Time_DePt_v_NZvalue=MESP_ID_Time_DePt_v; 
  MESP_ID_Time_SeL_v_NZvalue=MESP_ID_Time_SeL_v;
  
 
  Serial.print("ESP Received ");
  Serial.print("AvgAT_v: ");
  Serial.println(MESP_ID_Time_AvgAT_v_NZvalue); 
  Serial.println("AvgRH_v: ");
  Serial.println(MESP_ID_Time_AvgRH_v);
  Serial.println("AvgVPD_v: ");
  Serial.println(MESP_ID_Time_AvgVPD_v_NZvalue);
  Serial.println("MESP_ID_Time_BaPr_v: ");
  Serial.println(MESP_ID_Time_BaPr_v);
}
if(MESPWS_ID_Time_AT_v>0){
  
 MESPWS_ID_Time_AT_v_NZvalue=MESPWS_ID_Time_AT_v; 
  MESPWS_ID_Time_RH_v_NZvalue=MESPWS_ID_Time_RH_v; 
   MESPWS_ID_Time_VPD_v_NZvalue=MESPWS_ID_Time_VPD_v; 
   MESPWS_ID_Time_HI_v_NZvalue=MESPWS_ID_Time_HI_v;
  MESPWS_ID_Time_EvTr_v_NZvalue=MESPWS_ID_Time_EvTr_v; 
    MESPWS_ID_Time_BaPr_v_NZvalue=MESPWS_ID_Time_BaPr_v; 
   MESPWS_ID_Time_DePt_v_NZvalue=MESPWS_ID_Time_DePt_v; 
   MESPWS_ID_Time_SeL_v_NZvalue=MESPWS_ID_Time_SeL_v; 
 MESPWS_ID_Time_SoRa_v_NZvalue=MESPWS_ID_Time_SoRa_v;
  Serial.println("wadher: ");
  Serial.println(MESPWS_ID_Time_AT_v);
  Serial.println("wadher: ");
  Serial.println(MESPWS_ID_Time_RH_v);

  //    delay(1500);
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
//!******************************************************************************
//! Name: vpd                               *
//! Description: For Climate Auto                     *
//! Param : Temp AND humi                                *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************
/*Vpd
  https://growdoctorguides.com/vpd-and-cannabis-explained/
  https://growdoctorguides.com/wp-content/uploads/2021/01/GDG-VPD-Table-1.pdf

  VP (deficit)=VP (leaf)−VP (air)
  VP (deficit)= (610.7∗10 (7.5∗LeafTemp)(237.3+LeafTemp) )/1000 - (610.7∗10 (7.5∗AirTemp)(237.3+AirTemp))/1000 ∗ RH/100

*/
float Vpd_ReHumi;
float Vpd_AT;
float Vpd_RH;
float LeafTemp, p1, p2, VPD_cal, AirTemp;

float  VPDcal(float Vpd_AT, float Vpd_RH) {

  LeafTemp = Vpd_AT;
  AirTemp = Vpd_AT;
  Vpd_ReHumi = Vpd_RH;
  p1 = pow ( 10 , ((float)(7.5 * AirTemp) / (float)(237.3 + AirTemp)));
  p2 = pow ( 10 , ((float)(7.5 * LeafTemp) / (float)(237.3 + LeafTemp)));
  float VPD_cal = (((float)610.7 * (float) p1) / 1000) - (((float)610.7 * (float)p2) / 1000) * (((float)Vpd_ReHumi) / 100);
  VPD = VPD_cal;
  Serial.println("************");
  Serial.print("VPD  ");
  Serial.print(VPD_cal);
  Serial.println("  kPa");
  Serial.println("************");
  Serial.println(VPD);
  return VPD;


}
//!******************************************************************************
//! Name: TempControl()                               *
//! Description: For Climate Auto                     *
//! Param : TempAutoSetpoint                                 *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

long TemppreviousMillis = 0; //             |
long TemppinHighTime = 100; //             |
long TemppinLowTime = 7500; //             |
long TemppinTime = 100; //

float TempHysterisMin;
float TempHysterisPlus;
float TempAutoSetpoint;
float TempSetHysteris;

float TempDown = 0;
float   TempUp = 0;
int previousMillis = 0; //
int pinHighTime = 100; //
int pinLowTime = 7500; // OLD 7500
int pinTime = 100; //


void TempControl(float TempAutoSetpoint) {
  Serial.println("vpd func");
  //  digitalWrite(TPUMPR, HIGH);
  //  digitalWrite(MIXERR, HIGH);

  TempHysterisMin = (TempAutoSetpoint - TempSetHysteris);
  TempHysterisPlus = (TempAutoSetpoint + TempSetHysteris);
  int Temp;
  int AT = Temp;
  if (Temp == TempAutoSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - TemppreviousMillis > TemppinTime) {
      TemppreviousMillis = currentMillis;

      digitalWrite(ExFan1, LOW);
      digitalWrite(ExFan2, LOW);
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);
      digitalWrite(ExFan7, LOW);
      digitalWrite(ExFan8, LOW);

      //      digitalWrite(Pad1, LOW);

    }


    TempDown = 0;
    TempUp = 0;
    TemppinTime = TemppinLowTime;
  }



  if (Temp < TempHysterisMin) {
    unsigned long currentMillis = millis();
    if (currentMillis - TemppreviousMillis > TemppinTime) {
      TemppreviousMillis = currentMillis;


      ///OFF


      TempDown = 0;
      TempUp = 0;
      TemppinTime = TemppinLowTime;

    }
  }

  if (Temp >= TempHysterisMin && Temp < TempAutoSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      digitalWrite(ExFan1, HIGH);
      digitalWrite(ExFan2, HIGH);
      digitalWrite(ExFan3, HIGH);
      digitalWrite(ExFan4, HIGH);
      digitalWrite(ExFan5, HIGH);
      digitalWrite(ExFan6, HIGH);
      digitalWrite(ExFan7, HIGH);
      digitalWrite(ExFan8, HIGH);

    }
  }
  //
  //if (Temp > TempHysterisPlus) {
  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis > pinTime) {
  //    previousMillis = currentMillis;
  //    digitalWrite(ExFan1, LOW);
  //      digitalWrite(ExFan2, LOW);
  //      digitalWrite(ExFan3, LOW);
  //      digitalWrite(ExFan4, LOW);
  //      digitalWrite(ExFan5, LOW);
  //      digitalWrite(ExFan6, LOW);
  //      digitalWrite(ExFan7, LOW);
  //      digitalWrite(ExFan7, LOW);
  //
  //    //ALL FAN OFF
  //
  //  }
  //
  //
  //    TempDown = 0;
  //    TempUp = 0;
  //    TemppinTime = TemppinLowTime;
  //}

}


//************************
boolean a = LOW, b = HIGH;
float TempSensor;
float SPtemp;

long interval = 2000; //Read sensor each 2 seconds
void tempcpntroller(float Temp, float SPtemp) {
  //  int Temp =TempSensor;
  unsigned long currentMillis = millis();//time elapsed
  if (currentMillis - previousMillis > interval) //Comparison between the elapsed time and the time in which the action is to be executed
  {
    previousMillis = currentMillis; //"Last time is now"


    if (TempSensor >= SPtemp && a == LOW) //if temperature above of SPtemp degrees
    {
      digitalWrite(pad1, HIGH);
      digitalWrite(ExFan1, HIGH);
      digitalWrite(ExFan2, HIGH);
      digitalWrite(ExFan3, HIGH);
      digitalWrite(ExFan4, HIGH);
      digitalWrite(ExFan5, HIGH);
      digitalWrite(ExFan6, HIGH);
      digitalWrite(ExFan7, HIGH);
      digitalWrite(ExFan8, HIGH);//Active air conditioner

      a = HIGH;
      b = LOW;

    }
    else if (TempSensor <= SPtemp && b == LOW) //if temperature is under SPtemp degrees
    {
      digitalWrite(pad1, LOW);
      digitalWrite(ExFan1, LOW);
      digitalWrite(ExFan2, LOW);
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);
      digitalWrite(ExFan7, LOW);
      digitalWrite(ExFan8, LOW);//Turn off air conditioner
      a = LOW;
      b = HIGH;
    }

  }
}
//!******************************************************************************
//! Name: VPDControl()                               *
//! Description: For Climate Auto                     *
//! Param : VPDSetpoint                                 *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

long VPDpreviousMillis = 0; //             |
long VPDpinHighTime = 100; //             |
long VPDpinLowTime = 7500; //             |
long VPDpinTime = 100; //

float VPDHysterisMin;
float VPDHysterisPlus;
float VPDAutoSetpoint;
//float VPDAutoSetpoint;
float VPDSetHysteris;
int VPDDown;
int VPDUp;
//int VPDpinTime;
void VPDControl(float VPDAutoSetpoint) {
  Serial.println("vpd func");
  //  digitalWrite(TPUMPR, HIGH);
  //  digitalWrite(MIXERR, HIGH);

  VPDHysterisMin = (VPDAutoSetpoint - VPDSetHysteris);
  VPDHysterisPlus = (VPDAutoSetpoint + VPDSetHysteris);

  if (VPD == VPDAutoSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - VPDpreviousMillis > VPDpinTime) {
      VPDpreviousMillis = currentMillis;

      digitalWrite(ExFan1, LOW);
      digitalWrite(ExFan2, LOW);
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);
      digitalWrite(ExFan7, LOW);
      digitalWrite(ExFan8, LOW);

      //      digitalWrite(Pad1, LOW);

    }

    VPDDown = 0;
    VPDUp = 0;
    VPDpinTime = VPDpinLowTime;
  }


  if (VPD < VPDHysterisMin) {
    unsigned long currentMillis = millis();
    if (currentMillis - VPDpreviousMillis > VPDpinTime) {
      VPDpreviousMillis = currentMillis;


      ///OFF


      VPDDown = 0;
      VPDUp = 0;
      VPDpinTime = VPDpinLowTime;

    }
  }

  if (VPD >= VPDHysterisMin && VPD < VPDAutoSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;

      digitalWrite(ExFan1, HIGH);
      digitalWrite(ExFan2, HIGH);
      digitalWrite(ExFan3, HIGH);
      digitalWrite(ExFan4, HIGH);
      digitalWrite(ExFan5, HIGH);
      digitalWrite(ExFan6, HIGH);
      digitalWrite(ExFan7, HIGH);
      digitalWrite(ExFan8, HIGH);

    }
  }

  if (VPD > VPDHysterisPlus) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      digitalWrite(ExFan1, LOW);
      digitalWrite(ExFan2, LOW);
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);
      digitalWrite(ExFan7, LOW);
      digitalWrite(ExFan8, LOW);

      //ALL FAN OFF

    }

    VPDDown = 0;
    VPDUp = 0;
    VPDpinTime = VPDpinLowTime;
  }

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

  if ((dfd.substring(0, 7) == "vpdauto")) {
    //t3.enable();
    Serial.println("VPD ON");
    Vpd_RH = MESP_ID_Time_AvgRH_v;

    Serial.println("VPD OFF");
    VpdautoModeOn = false;
    //       Serial.print("Humidity= ");
    //SPtemp = hmd;
    //       Serial.println(Vpd_RH);

    Vpd_AT = MESP_ID_Time_AvgAT_v_NZvalue;// vpdtemp();
    //       Serial.print("temperature= ");
    //TempSensor = tmp;
    //       Serial.println(Vpd_AT);

    feedBack = "vpdt";
  }

  if ((dfd.substring(0, 4) == "vpdt")) {
    //       vpdhmd();
    //t3.enable();
    Serial.println("VPD ON");
    Vpd_RH = vpdhmd();
    //       Serial.print("Humidity= ");
    //SPtemp = hmd;
    Serial.println(Vpd_RH);
    //
    //
    //       vpdtemp();
    Vpd_AT = vpdtemp();
    //       Serial.print("temperature= ");
    //TempSensor = tmp;
    Serial.println(Vpd_AT);
    VPDcal(Vpd_AT, Vpd_RH);
    feedBack = "vpdt";

  }
  if ((dfd.substring(0, 5) == "dayon")) {
    //nightmode = "on";
    //t3.enable();
    //vpd_control();
    Serial.print("dayon");
  }
  if ((dfd.substring(0, 5) == "dayoff")) {
    //nightmode = "off";
    // t3.enable();
    Serial.print("dayoff");
    //vpd_control();
  }

  //   if ((dfd.substring(0, 6) == "calvpd")) {
  ////////     float calcul=
  ////    Serial.println("vpd cal"[);
  //     VPDcal(Vpd_AT,Vpd_RH);
  //
  // feedBack = "calvpd";
  //    }
  //   }
  //   }
  //
  /************************************** TEMPERATURE **************************/

//  if ((dfd.substring(0, 4) == "temp")) {
//    //       vpdtempA();
//
//    //t4.enable();
//
//    float In_TempAutoSetpoint = vpdtemp_I();
//    Serial.print("in-temperature= ");
//    SPtemp = In_TempAutoSetpoint ;
//    Serial.println(SPtemp);
//
//    float Out_TempAutoSetpoint = vpdtemp_O();
//    Serial.print("out-temperature= ");
//    Serial.println(Out_TempAutoSetpoint);
//    feedBack = "tempat";
//
//  }
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
    //    t12.enable();//setTimeNex
    //    t13.disable();
  }
  if ((data_from_display.substring(0, 6) == "setrtc")) {
    //    t12.disable();
    //    t13.enable();//runTimeNex
  }

  sendData(data_from_display);
}

void sendData(String dfd) {
  if (dfd == "f1on" ) {
    feedBack = "f1on";
    //      relayPA = HIGH;
    digitalWrite(ExFan1, LOW);
    Serial.println("f1 On");
  }
  if (dfd == "f1off") {
    feedBack = "f1off";
    //      relayPA = LOW;
    digitalWrite(ExFan1, HIGH);
    Serial.println("f1 Off");
  }
  if (dfd == "f2on") {
    feedBack = "f2on";
    //      relayPA = HIGH;
    digitalWrite(ExFan2, LOW);
    Serial.println("f2 On");
  }
  if (dfd  == "f2off") {
    feedBack = "f2off";
    //      relayPA = LOW;
    digitalWrite(ExFan2, HIGH);
    Serial.println("f2 Off");
  }
  if (dfd == "f3on") {
    feedBack = "f3on";
    //      relayPA = HIGH;
    digitalWrite(ExFan3, LOW);
    Serial.println("f3 On");
  }
  if (dfd == "f3off") {
    feedBack = "f3off";
    //      relayPA = LOW;
    digitalWrite(ExFan3, HIGH);
    Serial.println("f3 Off");
  }
  if (dfd == "f4on") {
    feedBack = "f4on";
    //      relayPA = HIGH;
    digitalWrite(ExFan4, LOW);
    Serial.println("f4 On");
  }
  if (dfd == "f4off") {
    feedBack = "f4off";
    //      relayPA = LOW;
    digitalWrite(ExFan4, HIGH);
    Serial.println("f4 Off");
  }
  if (dfd == "f5on") {
    feedBack = "f5on";
    //      relayPA = HIGH;
    digitalWrite(ExFan5, LOW);
    Serial.println("f5 On");
  }
  if (dfd == "f5off") {
    feedBack = "f5off";
    //      relayPA = LOW;
    digitalWrite(ExFan5, HIGH);
    Serial.println("f5 Off");
  }
  if (dfd == "f6on") {
    feedBack = "f6on";
    //      relayPA = HIGH;
    digitalWrite(ExFan6, LOW);
    Serial.println("f6 On");
  }
  if (dfd == "f6off") {
    feedBack = "f6off";
    //      relayPA = LOW;
    digitalWrite(ExFan6, HIGH);
    Serial.println("f6 Off");
  }
  if (dfd == "f7on") {
    feedBack = "f7on";
    //      relayPA = HIGH;
    digitalWrite(ExFan7, LOW);
    Serial.println("f7 On");
  }
  if (dfd == "f7off") {
    feedBack = "f7off";
    //      relayPA = LOW;
    digitalWrite(ExFan7, HIGH);
    Serial.println("f7 Off");
  }
  if (dfd == "f8on") {
    feedBack = "f8on";
    //      relayPA = HIGH;
    digitalWrite(ExFan8, LOW);
    Serial.println("f8 On");
  }
  if (dfd == "f8off") {
    feedBack = "f8off";
    //      relayPA = LOW;
    digitalWrite(ExFan8, HIGH);
    Serial.println("f8 Off");
  }
  if (dfd == "cpon") {
    feedBack = "cpon";
    //      relayPA = HIGH;
    digitalWrite(pad1, LOW);
    Serial.println("cp On");
  }
  if (dfd == "cpoff") {
    feedBack = "cpoff";
    //      relayPA = LOW;
    digitalWrite(pad1, HIGH);
    Serial.println("cp Off");
  }
  if (dfd == "fpon") {
    feedBack = "fpon";
    //      relayPA = HIGH;
    digitalWrite(Fogger, LOW);
    Serial.println("fp On");
  }
  if (dfd == "fpoff") {
    feedBack = "fpoff";
    //      relayPA = LOW;
    digitalWrite(Fogger, HIGH);
    Serial.println("fp Off");
  }
  if (dfd == "fcon") {
    feedBack = "fcon";
    //      relayPA = HIGH;
    digitalWrite(FoggerSol, LOW);
    Serial.println("fc On");
  }
  if (dfd == "fcoff") {
    feedBack = "fcoff";
    //      relayPA = LOW;
    digitalWrite(FoggerSol, HIGH);
    Serial.println("fc Off");
  }
  if (dfd == "cgon") {
    feedBack = "cgon";
    //      relayPA = HIGH;
    digitalWrite(Co2Gen, LOW);
    Serial.println("cg On");
  }
  if (dfd == "cgoff") {
    feedBack = "cgoff";
    //      relayPA = LOW;
    digitalWrite(Co2Gen, HIGH);
    Serial.println("cg Off");
  }
  //********************************************************************VPD*******************************
  //VPD Controller
  if (dfd == "vpdon") {
    // t3.enable();
    feedBack = "vpdon";
    //dfd = "vpdstop";
    Serial.println("VPD  ON");
    //   VPDcal(float, float);
    //   vpdset();
   // vpdfun_cal = true;
   // vpd_func = true;
    VpdautoModeOn = false;
  }
  if (dfd == "vpdoff") {
    //                t3.disable();
    feedBack = "vpdoff";

    Serial.println("VPD off");
    VpdautoModeOn = true;
   // vpd_func = false;
  }
  //********************************************************************End VPD*******************************

  //********************************************************************AUTO TEMP MODE*******************************
if ((dfd.substring(0, 4) == "temp")){
  if (dfd == "tempon") {
    // t4.enable();
    feedBack = "tempon";
    TemperatureAutoModeOn = true;
    Serial.println("TEMP  ON");
    temp_func = true;
  }
  if (dfd == "tempoff") {
    // t4.disable();
    feedBack = "tempoff";
    TemperatureAutoModeOn = false;
    Serial.println("TEMP  OFF");
    temp_func = false;
  }
}
}
/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  //  Serial.println(dfd);
  val = dfd.indexOf("s") + 1; //clkd24m2y2022h2n58s30
  //  Serial.println(val);
  dfd.remove(0, val);
  //  Serial2.print("sec:");
  Serial.println(dfd);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
  Serial.print("Seconds: ");
  Serial.println(secRc);
}
void timeMin() {
  //  Serial.println(buff);
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  //  Serial.println(cSmn);
  val = cSmn.indexOf("n") + 1; //    Serial.println(val);
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
  Serial.print("Minutes: ");
  Serial.println(minRc);
}
void timeHr() {
  //  Serial.println(buff);
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1; //    Serial.println(val);
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;
  Serial.print("Hours: ");
  Serial.println(hrRc);

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
  Serial.print("Date: ");
  Serial.println(Daterc);

}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  //  Serial.println(cSmnt);
  val = cSmnt.indexOf("m") + 1; //    Serial.println(val);
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Monthrc = mntRc;
  Serial.print("Month: ");
  Serial.println(Monthrc);

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //    Serial.println(val);
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Yearrc = yrRc;
  Serial.print("Year: ");
  Serial.println(Yearrc);
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

//void vpdset(){
////  VPDcal();   //(Vpd_AT,Vpd_RH);
//  int vale = dfd.indexOf("n") + 1;
//  dfd.remove(0, vale);
//  float setval = dfd.toFloat();
//  Serial.println(setval);
//}

//void vpdOnOff(){
//   if(vpd_func == true){
//    Serial.println("vpd func on");
//    VPDControl(float VPDAutoSetpoint);
//   }
//   else if(vpd_func == false){
//    Serial.println("vpd func off");
//    exit;
//   }
//}



void nexVal() {
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
  if((MESP_ID_Time_AvgAT_v_NZvalue>0)||(MESPWS_ID_Time_AT_v_NZvalue>0)){
  Serial2.print("at.txt=\"");
  Serial2.print((float)MESP_ID_Time_AvgAT_v_NZvalue,1);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("rh.txt=\"");
  Serial2.print(MESP_ID_Time_AvgRH_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("vpd.txt=\"");
  Serial2.print(MESP_ID_Time_AvgVPD_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("ev.txt=\"");
  Serial2.print(MESP_ID_Time_EvTr_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("bp.txt=\"");
  Serial2.print((float)MESP_ID_Time_BaPr_v_NZvalue,1);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("sl.txt=\"");
  Serial2.print(MESP_ID_Time_SeL_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("co.txt=\"");
  Serial2.print(MESP_ID_Time_co2_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("dpp.txt=\"");
  Serial2.print(MESP_ID_Time_DePt_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("sr.txt=\"");
  Serial2.print(MESP_ID_Time_SoRa_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("hi.txt=\"");
  Serial2.print(MESP_ID_Time_AvgHI_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /***************** HWM ****************/
  Serial2.print("att.txt=\"");
  Serial2.print(MESPWS_ID_Time_AT_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("rhh.txt=\"");
  Serial2.print(MESPWS_ID_Time_RH_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("bpp.txt=\"");
  Serial2.print(MESPWS_ID_Time_BaPr_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("sll.txt=\"");
  Serial2.print(MESPWS_ID_Time_SeL_v_NZvalue);
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
  Serial2.print(MESPWS_ID_Time_SoRa_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("hii.txt=\"");
  Serial2.print(MESPWS_ID_Time_HI_v_NZvalue);
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
  Serial2.print(MESPWS_ID_Time_EvTr_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("dppp.txt=\"");
  Serial2.print(MESPWS_ID_Time_DePt_v_NZvalue);
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

  //Serial.print(minupg);
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
  Serial2.print(MESP_ID_Time_AvgVPD_v_NZvalue);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /***************** HWM ******


  **********/
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
//***************************************************Fogging Switching*******************************
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
      Serial.println(pin);

    }

    void begin( long off)
    {
      pinMode(relayPin, OUTPUT);
      //      OnTime = on;
      OffTime = off;
      relayState = HIGH;
      Serial.println("bee");

      previousMillis = 0;
    }
    void Update()
    { Serial.println("Fogging ON");

      // check the time to see if relays need to be turned on
      // or off
      unsigned long currentMillis = millis();

      if ((relayState == HIGH) && (currentMillis - previousMillis >= OffTime))
      {
        relayState = LOW; // Turn it off
        Serial.println("Fogging off");
        previousMillis = currentMillis; // Remember the time
        digitalWrite(relayPin, relayState); //update the relay
        Serial.println("");

        digitalWrite(Fogger, relayState);
        digitalWrite(Cfan1, relayState);
        digitalWrite(Cfan2, relayState);
      }
      //   else if ((relayState == LOW) && (currentMillis - previousMillis >= OnTime))
      //   {
      //   relayState = HIGH; // turn it on
      //   previousMillis = currentMillis;
      //    Serial.println("relay On");
      //   digitalWrite(relayPin, relayState);
      //   }
    }
};
Switcher fogger(14);
//***************************************************VPD AUTO MODE***********************************************
void vpd_control(float MESP_ID_Time_AvgVPD_v_NZvalue) {

  // float vpd=MESP_ID_Time_AvgVPD_v;
  // float vpd = MESP_ID_Time_AvgVPD_v;//=random(0.5,2.1);



  //NIGHT MODE
  //    if ((vpd <= vpdlowsp)) {
  //
  //      Serial.println("night mode oly fans on");
  Serial.println(MESP_ID_Time_AvgVPD_v_NZvalue);
  Serial.println("VPD SETPOINT");

  if (MESP_ID_Time_AvgVPD_v_NZvalue < 0.4) { //0.41<=0.4
    // relayState = LOW; // turn it on

    Serial.println("all 8 fans on");
    digitalWrite(ExFan1, LOW);
    digitalWrite(ExFan2, LOW);
    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan4, LOW);
    digitalWrite(ExFan5, LOW);
    digitalWrite(ExFan6, LOW);
    digitalWrite(ExFan7, LOW);
    digitalWrite(ExFan8, LOW);
  }
  else if ((MESP_ID_Time_AvgVPD_v_NZvalue >= 0.4) && (MESP_ID_Time_AvgVPD_v_NZvalue < 0.5)) {
    Serial.println("6 fans on");
    //relayState = LOW; // turn it on
    digitalWrite(ExFan1, LOW);

    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan4, LOW);
    digitalWrite(ExFan5, LOW);
    digitalWrite(ExFan6, LOW);
    digitalWrite(ExFan7, LOW);

  }
  else if ((MESP_ID_Time_AvgVPD_v_NZvalue >= 0.5) && (MESP_ID_Time_AvgVPD_v_NZvalue < 0.6)) {

    Serial.println("4 fans on");
    //relayState = LOW; // turn it on
    digitalWrite(ExFan1, LOW);

    digitalWrite(ExFan3, LOW);

    digitalWrite(ExFan5, LOW);

    digitalWrite(ExFan7, LOW);

  }
  else if ((MESP_ID_Time_AvgVPD_v_NZvalue >= 0.6) && (MESP_ID_Time_AvgVPD_v_NZvalue < 0.8))
  {
    Serial.println("2 fans on");
    // relayState = LOW; // turn it on
    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan6, LOW);
  }

  //DAY MODE
  //    if ((vpd >= vpdhighsp)) {
  //      relayState = LOW;
  //      Serial.println("pad and fans on");
  //      Serial.println(vpd);
  //      Serial.println(vpdhighsp);
  //      digitalWrite(pad1, relayState);

  else if ((MESP_ID_Time_AvgVPD_v_NZvalue > 1.4) && (MESP_ID_Time_AvgVPD_v_NZvalue < 1.6)) {
    Serial.println("2 Ex fans on");
    //relayState = LOW; // turn it on
    digitalWrite(pad1, LOW);
    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan6, LOW);
  }
  else if ((MESP_ID_Time_AvgVPD_v_NZvalue >= 1.6) && (MESP_ID_Time_AvgVPD_v_NZvalue < 1.8)) {
    Serial.println("4 Ex fans on");
    // relayState = LOW; // turn it on
    digitalWrite(pad1, LOW);

    digitalWrite(ExFan1, LOW);

    digitalWrite(ExFan3, LOW);


    digitalWrite(ExFan5, LOW);

    digitalWrite(ExFan7, LOW);
    fogger.Update();
  }
  else if ((MESP_ID_Time_AvgVPD_v_NZvalue >= 1.8) && (MESP_ID_Time_AvgVPD_v_NZvalue < 2.0)) {
    Serial.println(" 6 EX fans on");
    // relayState = LOW; // turn it on
    digitalWrite(pad1, LOW);

    digitalWrite(ExFan1, LOW);

    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan4, LOW);
    digitalWrite(ExFan5, LOW);
    digitalWrite(ExFan6, LOW);
    digitalWrite(ExFan7, LOW);
    fogger.Update();
    // fogger.Update();
  }
  else if (MESP_ID_Time_AvgVPD_v_NZvalue >= 2.0) {

    Serial.println("all 8 EX fans on");
    // relayState = LOW; // turn it on
    digitalWrite(pad1, LOW);

    digitalWrite(ExFan1, LOW);
    digitalWrite(ExFan2, LOW);
    digitalWrite(ExFan3, LOW);
    digitalWrite(ExFan4, LOW);
    digitalWrite(ExFan5, LOW);
    digitalWrite(ExFan6, LOW);

    digitalWrite(ExFan7, LOW);
    digitalWrite(ExFan8, LOW);
    fogger.Update();
    //fogger.Update();
  }

  else  {

    Serial.println("nothing to do");
    Serial.println(MESP_ID_Time_AvgVPD_v_NZvalue);
    Serial.println(vpdlowsp);
  }

}

//****************************************************************Auto Temp*********************************************************
//float TempSetHysteris=9;
float OutsideATemp = 35;
//float TempSp = 26;

void AutoTemp_control(float SPtemp) {
  //float TempSetHysteris=9;
  //SPtemp 24
  //  float ATempHysteris = (OutsideATemp - TempSetHysteris);
  float ATempHysteris = SPtemp + 1;
  // float ATemp=33;//31.80
 float ATemp = MESP_ID_Time_AvgAT_v_NZvalue;
 //float ATemp = 25.0;
  // AHumi=MESP_ID_Time_AvgRH_v;
  //     Serial.print("Temperature:");
  Serial.println(SPtemp);
  //     Serial.print("Humidity:");
  Serial.println(ATempHysteris);


  //if ( ATemp >= ATempHysteris) {  // 33>=26  //31.80>=26w


  //  Serial.println(" oly fans on");
    Serial.println(ATemp);
    //    Serial.println(ATempHysteris);
    float ATempLow =  ATempHysteris + 2; //28
    float ATempMid = ATempLow + 2; //30
    float ATempHigh = ATempMid + 2; //32



    Serial.println("Modes");
    Serial.println(ATempHigh);
    Serial.println(ATempMid);
    Serial.println(ATempLow);
    if (ATemp >= ATempHigh) //32<31                 // Temp=33    32<33//31.80>=32 fail
    {

      //  relayState = LOW; // turn it on

      Serial.println("all 8 fans on");
      digitalWrite(ExFan1, LOW);
      digitalWrite(ExFan2, LOW);
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);
      digitalWrite(ExFan7, LOW);
      digitalWrite(ExFan8, LOW);
    }
    else if ((ATemp >= ATempMid) && (ATemp < ATempHigh)) //31>30&& 31<32        //Temp=31  31>30  && 31<32//31.80>=30 && 31.80<32
    {

      //  if (ATempMid <=  ATemp && ATemp  <=  ATempHigh ) {
      Serial.println("6 fans on");
      //      relayState = LOW; // turn it on
      digitalWrite(ExFan1, LOW);

      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan4, LOW);
      digitalWrite(ExFan5, LOW);
      digitalWrite(ExFan6, LOW);

      digitalWrite(ExFan7, LOW);

    }
    else  if ((ATemp >= ATempLow) && (ATemp < ATempMid)) //Temp=29 //31.80>=28 && 31.80<30
    {
      Serial.println("4 fans on");
      //relayState = LOW; // turn it on
      digitalWrite(ExFan1, LOW);

      digitalWrite(ExFan3, LOW);

      digitalWrite(ExFan5, LOW);

      digitalWrite(ExFan7, LOW);

    }
    else if ((ATemp > SPtemp) && (ATemp < ATempLow)) { //Temp=27 31.80>=26 31.80<28
      Serial.println("2 fans on");
      // relayState = HIGH; // turn it on
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan6, LOW);
    }

else{
   // else if (ATemp < SPtemp) {
      //31.80<26
      Serial.println("OFF");
      //relayState = HIGH;
      digitalWrite(ExFan1, HIGH);
      digitalWrite(ExFan2, HIGH);
      digitalWrite(ExFan3, HIGH);
      digitalWrite(ExFan4, HIGH);
      digitalWrite(ExFan5, HIGH);
      digitalWrite(ExFan6, HIGH);
      digitalWrite(ExFan7, HIGH);
      digitalWrite(ExFan8, HIGH);

    }
 // }
}



void callSerialCom() {
  if (Serial2.available()) {
    SerialCom();
  }
}
void callNexVal() {

  nexVal();

}
void callFromEspData() {

  FromEspData();

}
void callVpdMode() {
  if (VpdautoModeOn == true ) {
    //vpd_control(1.41);
 vpd_control( MESP_ID_Time_AvgVPD_v_NZvalue);
    Serial.println("VPD");
  }
}

void callAutoTempMode() {
  if (TemperatureAutoModeOn == true ) {
    AutoTemp_control( SPtemp);
    Serial.println("AUTO TEMP");
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  while (!Serial);
  //Fogger ON Timer
  fogger.begin(20000);
  Serial.println("TAYRIX...");
  //  Serial.flush();
  //  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(),now.year());
  
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
  //vpdfun_cal = false;
  superTask.setTimer( callSerialCom, 1100, 0 );
 // superTask.setTimer( callFromEspData, 2000, 0 );
  superTask.setTimer( callFromEspData, 1000, 0 );
 // superTask.setTimer( callVpdMode, 5000, 0 );
 // superTask.setTimer( callAutoTempMode, 6000, 0 );
  superTask.setTimer( callVpdMode, 7000, 0 );
 superTask.setTimer( callAutoTempMode, 8000, 0 );
superTask.setTimer(callNexVal , 5000, 0 );
 

  superTask.startTasks(); //Starting tasks.
}
void loop() {




}
