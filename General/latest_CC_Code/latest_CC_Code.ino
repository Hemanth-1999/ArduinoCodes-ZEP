

/*
   Basic CC setup
*/

#include <Arduino.h>
#include <MemoryFree.h>
//#include <SD.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>

#include <Wire.h>
#define nexSer Serial1 
#define dbgSer Serial
// Sensor Input

String data_from_display = "";
String dfd;
String str;
char buff[2000];
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







float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v, MESP_ID_Time_AvgVPD_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;
float MESPWS_ID_Time_AT_v, MESPWS_ID_Time_RH_v, MESPWS_ID_Time_VPD_v, MESPWS_ID_Time_HI_v, MESPWS_ID_Time_EvTr_v, MESPWS_ID_Time_BaPr_v, MESPWS_ID_Time_SoRa_v, MESPWS_ID_Time_DePt_v;
float MESPWS_ID_Time_WSwsclk_v, MESPWS_ID_Time_WSwsaclk_v, MESPWS_ID_Time_WSwdclk_v, MESPWS_ID_Time_WSwdaclk_v, MESPWS_ID_Time_WSclohei_v;
int MESPWS_ID_Time_WSntips_v, MESPWS_ID_Time_SeL_v, MESPWS_ID_Time_ntl_v;
float ATemp, AHumi;
float vpdlowsp = 0.8;
float vpdhighsp = 1.4;
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

//***********************************************************************************************************************
String ssid;
String password;

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


\
int uva;
int WD;
int WSpeed;

uint16_t hrs = 0;
uint16_t mins = 0;

String nightmode;

// *********************************************************************
// Task Scheduler
// *********************************************************************

// objects
Scheduler r;
unsigned long NodepreviousMillis = 0;
const long interval2 = 3000;
//unsigned long mtime = 0;
//
//// Callback methods prototypes
//void Task_Serial_Blink_Example();
void dateandtime();
//void sensorjson();
void nexVal();
void nodes();
void nodes_HWM();
//void TempControl(float TempAutoSetpoint);
void vpd_control(float vpdst );
void AutoTemp_control(float);
void setTimeNex();
void runTimeNex();
void dummy();
void dummy2();
float vpdtemp_I();
float vpdtemp_O();
float VPDcal(float, float);
float NECC_AT, NECC_RH, NECC_VPD, NECC_HI;


//// Tasks
//Task t1(1000,  TASK_FOREVER, &SerialCom, &r);//12
Task t0(3000,  TASK_FOREVER, &dateandtime, &r);

//Task t1(5000,  TASK_FOREVER, &getNutrientPH, &r);
//Task t2(2000,  TASK_FOREVER, &nexVal, &r);//9
Task t3(3000,  TASK_FOREVER, &dummy, &r);
Task t4(4000,  TASK_FOREVER, &dummy2, &r);
//Task t10(6000,  TASK_FOREVER, &nodes_HWM, &r);
Task t5(3500,  TASK_FOREVER, &nodes, &r);
Task t12(4500,  TASK_FOREVER, &setTimeNex, &r);
Task t13(3500,  TASK_FOREVER, &runTimeNex, &r);

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
void dummy() {
 Serial.print("dummy  ");
  vpd_control(MESP_ID_Time_AvgVPD_v);
  modevpdstatus = true;

}
void dummy2() {
  Serial.print("dummy  2");
  AutoTemp_control(MESP_ID_Time_AvgAT_v);
  modetempstatus = true;
}

void nodes() {
//unsigned long currentMillis = millis(); //read current time
//  if (currentMillis - NodepreviousMillis >= 2000) { //if current time - last time > 5 sec
//   NodepreviousMillis = currentMillis;
  //
  //  DynamicJsonDocument doc(256);





  StaticJsonDocument<256> doc;
  deserializeJson(doc, Serial3); // Ok
  deserializeJson(doc, Serial3); // InvalidInput
    DeserializationError error = deserializeJson(doc, Serial3);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
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
  //  MESP_ID_Time_AvgAT_v = random(3,9);

  Serial.print("ESP Received ");
  Serial.print("AvgAT_v: ");
  Serial.println(MESP_ID_Time_AvgAT_v);
  Serial.println("AvgRH_v: ");
  Serial.println(MESP_ID_Time_AvgRH_v);
  Serial.println("MESP_ID_Time_BaPr_v: ");
  Serial.println(MESP_ID_Time_BaPr_v);
  //  // delay(2000);
  //  EEPROM.update(EaddrCC_AT, MESP_ID_Time_AvgAT_v);
  //  EEPROM.update(EaddrCC_RH, MESP_ID_Time_AvgRH_v);
  //  EEPROM.update(EaddrCC_VPD, MESP_ID_Time_AvgVPD_v);
  //  EEPROM.update(EaddrCC_HI, MESP_ID_Time_AvgHI_v);
  //
  //  NECC_AT = EEPROM.readFloat(EaddrCC_AT);
  //  NECC_RH = EEPROM.readFloat(EaddrCC_RH);
  //  NECC_VPD = EEPROM.readFloat(EaddrCC_VPD);
  //  NECC_HI = EEPROM.readFloat(EaddrCC_HI);
  //  Serial3.print("EPROM");
  //  Serial3.print("ep AvgAT_v: ");
  //  Serial3.println(NECC_AT);
  //  Serial3.println("ep AvgRH_v: ");
  //  Serial3.println(NECC_RH);
  delay(1500);
//  }
}


void nodes_HWM() {
 // / DynamicJsonDocument doc(512);

  StaticJsonDocument<1024> doc;
//  deserializeJson(doc, Serial3); // Ok
//  deserializeJson(doc, Serial3); // InvalidInput
  DeserializationError error = deserializeJson(doc, Serial3);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject MESPWS_ID_Time = doc["MESPWS_ID"]["Time"];
  MESPWS_ID_Time_AT_v  =  MESPWS_ID_Time["WSAT_v"]; // 222
  MESPWS_ID_Time_RH_v  =  MESPWS_ID_Time["WSRH_v"]; // 233
  MESPWS_ID_Time_VPD_v =  MESPWS_ID_Time[" WSVPD_v"]; // 1
  MESPWS_ID_Time_HI_v  =  MESPWS_ID_Time["WSHI_v"]; // 29
  MESPWS_ID_Time_EvTr_v   =  MESPWS_ID_Time["WSEvTr_v"]; // 222
  MESPWS_ID_Time_BaPr_v   =  MESPWS_ID_Time["WSBaPr_v"]; // 233
  MESPWS_ID_Time_SoRa_v   =  MESPWS_ID_Time[" WSSoRa_v"]; // 1
  MESPWS_ID_Time_DePt_v   =  MESPWS_ID_Time["WSDePt_v"]; // 29
  MESPWS_ID_Time_SeL_v    =  MESPWS_ID_Time["WSSeL_v"]; //
  MESPWS_ID_Time_WSwsclk_v    =  MESPWS_ID_Time["WSwsclk_v"]; //
  MESPWS_ID_Time_WSwsaclk_v    =  MESPWS_ID_Time["WSwsaclk_v"]; //
  MESPWS_ID_Time_WSwdclk_v    =  MESPWS_ID_Time["WSwdclk_v"]; //
  MESPWS_ID_Time_WSwdaclk_v    =  MESPWS_ID_Time["WSwdaclk_v"]; //
  MESPWS_ID_Time_WSntips_v    =  MESPWS_ID_Time["WSntips_v"]; //
  MESPWS_ID_Time_WSclohei_v    =  MESPWS_ID_Time["WSclohei_v"]; //
  MESPWS_ID_Time_ntl_v    =  MESPWS_ID_Time["ntl_v"]; // 70

  delay(2000);
}




//*************************************************************************************************
//!******************************************************************************
//! Name: sensorjson()                                  *
//! Description:                      *
//! Param :                                      *
//! Returns: void                                     *
//! Example: TIME              *
//!******************************************************************************


//float AT= random(28, 31);
//float RH= random (75, 98);
//float VPD=random (0.9, 1.3);
//float EV=random (150, 450);
//float BP=random (75, 105);
//float CO2=random (350, 1198);
//float SR=random (750, 1580);
//float DP=random (28, 32);
//float HI=random (25, 50);
//float SL=random (65, 100);
////*****************************SETUP CODE******************************************************
//void sensorjson() {
//  //  Serial.print("Json");
//  DynamicJsonDocument doc(150);
//
//  doc["CID"] = "SPF-GCCZ01";
//  doc["Date/Time:"] = 0;
//  JsonObject Sensor = doc.createNestedObject("Sensor");
//  Sensor["AT"] =  (AT);
//  Sensor["RH"] = (RH);
//  Sensor["VPD"] = ( VPD);
//  Sensor["EV"] = (EV);
//  Sensor["BP"] = ( BP);
//  Sensor["CO2"] = ( CO2);
//  Sensor["SR"] = ( SR);
//  Sensor["DP"] = (DP);
//  Sensor["HI"] = (HI);
//  Sensor["SL"] = (SL);
//
//  serializeJsonPretty(doc, Serial3);
//  serializeJsonPretty(doc, Serial);
//}


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
  StaticJsonDocument<150> doc; //150
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
  DynamicJsonDocument doc(150);  //150
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
    data_from_display += char (Serial2.read());
  }
    //Serial.println(data_from_display);
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
    Serial.println("auto mode On:");
    Vpd_RH = MESP_ID_Time_AvgRH_v;


    //       Serial.print("Humidity= ");
    //SPtemp = hmd;
    //       Serial.println(Vpd_RH);

    Vpd_AT = MESP_ID_Time_AvgAT_v;// vpdtemp();
    //       Serial.print("temperature= ");
    //TempSensor = tmp;
    //       Serial.println(Vpd_AT);


  }

  if ((dfd.substring(0, 4) == "vpdt")) {
    //       vpdhmd();
    //t3.enable();
    Serial.println("manual mode On:");
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
    nightmode = "on";
    //t3.enable();
    //vpd_control();
  }
  if ((dfd.substring(0, 5) == "dayoff")) {
    nightmode = "off";
    // t3.enable();
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

  if ((dfd.substring(0, 4) == "temp")) {
    //       vpdtempA();

    //t4.enable();

    float In_TempAutoSetpoint = vpdtemp_I();
    Serial.print("in-temperature= ");
    //SPtemp = atemp;
    Serial.println(In_TempAutoSetpoint);

    float Out_TempAutoSetpoint = vpdtemp_O();
    Serial.print("out-temperature= ");
    Serial.println(Out_TempAutoSetpoint);
    feedBack = "tempat";

  }
  /**************************    NEXTION - RTC   ********************/
  if ((data_from_display.substring(0, 3) == "clk")) {
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();
          data_from_display = "";
    feedBack = "clk";
    dateandtime();
    readalldata();
    t12.enable();//setTimeNex
    t13.disable();
  }
  if ((data_from_display.substring(0, 6) == "setrtc")) {
    t12.disable();
    t13.enable();//runTimeNex
  }

   sendData(data_from_display);
}

void sendData(String dfd) {
  
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
  if (dfd == "vpdon") {
    t3.enable();
    feedBack = "vpdon";

    Serial.println("VPD  ON");
    //   VPDcal(float, float);
    //   vpdset();
    vpdfun_cal = true;
    vpd_func = true;
  }
  if (dfd == "vpdoff") {
    t3.disable();
    feedBack = "vpdoff";

    Serial.println("VPD  OFF");
    vpd_func = false;
  }
  if (dfd == "tmpon") {
    t4.enable();
    feedBack = "tmpon";

    Serial.println("TEMP  ON");
    temp_func = true;
  }
  if (dfd == "tmpoff") {
    t4.disable();
    feedBack = "tmpoff";

    Serial.println("TEMP  OFF");
    temp_func = false;
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
  Serial2.print("at.txt=\"");
  
  
  //Serial2.print(MESP_ID_Time_AvgAT_v);
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
  Serial2.print(MESP_ID_Time_EvTr_v);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("bp.txt=\"");
  Serial2.print(MESP_ID_Time_BaPr_v);
  Serial2.print("\"");
  Serial2.write(0x03ff);
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
  Serial2.print(NECC_HI);
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
  Serial2.print(MESP_ID_Time_AvgVPD_v);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  /***************** HWM ****************/
}

void setTimeNex() {
  //  Serial.println("setTimeNex");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

void runTimeNex() {
  //  Serial.println("runTimeNex");
  rtc.begin();
  //  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}


void vpd_control(float MESP_ID_Time_AvgVPD_v) {

  // float vpd=MESP_ID_Time_AvgVPD_v;
  float vpd = MESP_ID_Time_AvgVPD_v;//=random(0.5,2.1);
  if (nightmode == "on") {

    //NIGHT MODE
    if ((vpd <= vpdlowsp)) {

      Serial.println("night mode oly fans on");
      Serial.println(vpd);
      Serial.println(vpdlowsp);

      if (vpd < 0.4) { //0.41<=0.4
        relayState = LOW; // turn it on

        Serial.println("all fans on");
        digitalWrite(ExFan1, relayState);
        digitalWrite(ExFan2, relayState);
        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan4, relayState);
        digitalWrite(ExFan5, relayState);
        digitalWrite(ExFan6, relayState);
        digitalWrite(ExFan7, relayState);
        digitalWrite(ExFan8, relayState);
      }
      else if ((vpd >= 0.4) && (vpd < 0.5)) {
        Serial.println("4 fans on");
        relayState = LOW; // turn it on
        digitalWrite(ExFan1, relayState);

        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan4, relayState);
        digitalWrite(ExFan5, relayState);
        digitalWrite(ExFan6, relayState);
        digitalWrite(ExFan7, relayState);

      }
      else if ((vpd >= 0.5) && (vpd < 0.6)) {

        Serial.println("4 fans on");
        relayState = LOW; // turn it on
        digitalWrite(ExFan1, relayState);

        digitalWrite(ExFan3, relayState);

        digitalWrite(ExFan5, relayState);

        digitalWrite(ExFan7, relayState);

      }
      else if (vpd >= 0.6 && (vpd < 0.8))
      {
        Serial.println("2 fans on");
        relayState = LOW; // turn it on
        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan6, relayState);
      }
    }
  }


  else if (nightmode == "off") {

    //DAY MODE
    if ((vpd >= vpdhighsp)) {
      relayState = LOW;
      Serial.println("pad and fans on");
      Serial.println(vpd);
      Serial.println(vpdhighsp);
      digitalWrite(pad1, relayState);
      if ((vpd > 1.4) && (vpd < 1.6)) {
        Serial.println("2 Ex fans on");
        relayState = LOW; // turn it on
        digitalWrite(pad1, relayState);
        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan6, relayState);
      }
      else if ((vpd >= 1.6) && (vpd < 1.8)) {
        Serial.println("4 Ex fans on");
        relayState = LOW; // turn it on
        digitalWrite(pad1, relayState);

        digitalWrite(ExFan1, relayState);

        digitalWrite(ExFan3, relayState);

        digitalWrite(ExFan5, relayState);

        digitalWrite(ExFan7, relayState);
        // fogger.Update();
      }
      else if ((vpd >= 1.8) && (vpd < 2.0)) {
        Serial.println("all EX fans on");
        relayState = LOW; // turn it on
        digitalWrite(pad1, relayState);

        digitalWrite(ExFan1, relayState);

        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan4, relayState);
        digitalWrite(ExFan5, relayState);
        digitalWrite(ExFan6, relayState);
        digitalWrite(ExFan7, relayState);

        // fogger.Update();
      }
      else if (vpd >= 2.0) {

        Serial.println("all EX fans on");
        relayState = LOW; // turn it on
        digitalWrite(pad1, relayState);

        digitalWrite(ExFan1, relayState);
        digitalWrite(ExFan2, relayState);
        digitalWrite(ExFan3, relayState);
        digitalWrite(ExFan4, relayState);
        digitalWrite(ExFan5, relayState);
        digitalWrite(ExFan6, relayState);
        digitalWrite(ExFan7, relayState);
        digitalWrite(ExFan8, relayState);
        //fogger.Update();
      }
    }

  }

  else  {


    Serial.println("nothing to do");
    Serial.println(vpd);
    Serial.println(vpdlowsp);
  }

}









//float TempSetHysteris=9;
float OutsideATemp = 35;
float TempSp = 26;
void AutoTemp_control(float MESP_ID_Time_AvgAT_v) {
  //float TempSetHysteris=9;
  float ATempHysteris = (OutsideATemp - TempSetHysteris);
  // float ATemp=33;//31.80
  float ATemp = MESP_ID_Time_AvgAT_v;
  // AHumi=MESP_ID_Time_AvgRH_v;
  //     Serial.print("Temperature:");
  //     Serial.println(ATemp);
  //     Serial.print("Humidity:");
  //     Serial.println(AHumi);

  if ( ATemp >= TempSp) {  // 33>=26  //31.80>=26


    Serial.println(" oly fans on");
    Serial.println(ATemp);
    //    Serial.println(ATempHysteris);
    float ATempLow =  TempSp + 2; //28
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
    else if ((ATemp >= TempSp) && (ATemp < ATempLow)) { //Temp=27 31.80>=26 31.80<28
      Serial.println("2 fans on");
      // relayState = HIGH; // turn it on
      digitalWrite(ExFan3, LOW);
      digitalWrite(ExFan6, LOW);
    }


    else if (ATemp < TempSp) {                                  //31.80<26
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
  }
}





void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  while (!Serial);
  Serial.println("TAYRIX...");
  //  Serial.flush();
  //  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(),now.year());

  // encButton.begin();
  // encoder.begin();
  // pinMode(LEDPIN,OUTPUT);//cant use pin 13 when using hw spi
  // and on esp12 i2c can be on pin 2, and that is also led pin
  // so check first if this is adequate for your board

  // Enable  Read time Task
  //  t0.enable(); //Time
  //  t2.enable();
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
//  r.startNow();
//  t0.enable();
  //    t3.enable();
  //    t1.enable();//Serial com
  //  t2.enable();
  //  t3.enable();
  //  t4.enable();
  //  t10.enable();
//      t5.enable();
  //    t11.enable();

  //  delay(1000);
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
void loop() {
  if (Serial2.available()) {
    SerialCom();
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > 1000) {
    previousMillis = currentMillis;
    nexVal();
  }
//  //  r.execute();
  //
  //  unsigned long currentMillis = millis();
  //  if (currentMillis - previousMillis > 2000) {
  //    previousMillis = currentMillis;
  //    if (vpdfun_cal == true) {
  //      if (vpd_func == true) {
  //        Serial.println("vpd func on");
  //        VPDControl(VPDAutoSetpoint);
  //      }
  //      else if (vpd_func == false) {
  //        return 0;
  //      }
  //    }
  //  }
  
// t5.enable();
//  nodes();
  //}

  //  nodes_HWM();
  //
  //     AutoTemp_control();
  //
  //     nightmode=true;
  //     vpd_control();
  //delay(10000);
  // nightmode=false;
  //     vpd_control();
  //delay(10000);
  //  nexVal();


}
