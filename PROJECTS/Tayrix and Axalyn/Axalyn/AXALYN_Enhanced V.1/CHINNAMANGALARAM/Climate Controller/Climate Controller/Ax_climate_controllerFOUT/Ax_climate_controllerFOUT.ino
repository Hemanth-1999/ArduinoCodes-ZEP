#include <Arduino.h>
#include <MemoryFree.h>
#include <ArduinoJson.h>
#include "Nextion.h"
#include "RunningAverage.h"
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <EEPROMex.h>
#include <millisDelay.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int hourupg;
int minupg;
uint8_t secslive;
int Monthlive;
int Datelive;
int Yearlive;
String data_from_display = "";
String dfd;
String str;
String ssid;
const float SPtemp = 25;
const float ATempHysteris = SPtemp + 1;
extern volatile unsigned long timer0_millis;
String password;
String pwdname();
String pwd();
char buff[2000];
void AutoTemp_control();
void vpd_control();
void sendData(String );
void meshpass();
void prefix();
void port();
int val;
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
uint8_t otHr;
uint8_t otMin;
uint8_t oftMin;
uint8_t oftHr;
bool VpdautoModeOn;
bool TemperatureAutoModeOn;
bool vpd_func;
bool vpdfun_cal;
bool temp_func;
int count1 = 5;
int count3 = 10;
int count4 = 20;
int f1, f2, f3, f4, f5, f6, f7, f8, cp, fp, fs, co, ir, doo, dr, pb;
const char* Relaystate;
String set_point_from_app ;
String cndchck;
int counterm;
String checkstate ;
String storeprvstate = "";
const char* Allrelays_FAN1;

unsigned long mills = millis();

#define ExFan1  A3
#define ExFan2  A2
#define ExFan3  30
#define ExFan4  28

//Relay-2
#define ExFan5    8
#define ExFan6    9
#define ExFan7   10
#define ExFan8   11

//Relay-3
#define pad1      A4
#define Fogger    A6
#define FoggerSol A5
#define Co2Gen    A7

///////////////////////////////////////////
float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_AvgVPD_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;
float MESPWS_ID_Time_AT_v, MESPWS_ID_Time_RH_v, MESPWS_ID_Time_VPD_v, MESPWS_ID_Time_HI_v, MESPWS_ID_Time_EvTr_v, MESPWS_ID_Time_BaPr_v, MESPWS_ID_Time_SoRa_v, MESPWS_ID_Time_DePt_v;
float MESPWS_ID_Time_WSwsclk_v, MESPWS_ID_Time_WSwsaclk_v, MESPWS_ID_Time_WSwdclk_v, MESPWS_ID_Time_WSwdaclk_v, MESPWS_ID_Time_WSclohei_v;
int MESPWS_ID_Time_WSntips_v, MESPWS_ID_Time_SeL_v, MESPWS_ID_Time_ntl_v;
float WAT , WRH ,  WVPD  , WHI, WEv , WBaPa, WSolar , WDew, WSeaL , WSclk , WSaclk , WDclk, WDaclk ;
float ATemp, AHumi;
///////////////////////////////////////////

// Temperature setpoints
const float TEMP_LOW = ATempHysteris;   // 26°C
const float TEMP_MID = TEMP_LOW + 2.0;  // 28°C
const float TEMP_HIGH = TEMP_MID + 2.0; // 30°C

// VPD setpoints
const float VPD_LOW = 0.3;
const float VPD_MID = 0.8;
const float VPD_HIGH = 1.2;

NexPage page0 = NexPage(8, 0, "vpd1");
NexPage page1 = NexPage(9, 0, "temp");
NexPage page2 = NexPage(10, 0, "manual1");
NexPage page3 = NexPage(11, 0, "manual2");

////////////////////////////////////////// Manual from Nextion //////////////////////////////////////////
// Nextion Objects, ada 5 tombol
// (page id, component id, component name)
NexButton bt0 = NexButton(10, 1, "bt0");
NexButton bt1 = NexButton(10, 2, "bt1");
NexButton bt2 = NexButton(10, 3, "bt2");
NexButton bt3 = NexButton(10, 4, "bt3");
NexButton bt4 = NexButton(10, 5, "bt4");
NexButton bt5 = NexButton(10, 30, "bt5");
NexButton bt6 = NexButton(11, 31, "bt6");
NexButton bt7 = NexButton(11, 1, "bt7");
NexButton bt8 = NexButton(10, 5, "bt8");
NexButton bt9 = NexButton(10, 2, "bt9");
NexButton bt10 = NexButton(10, 3, "bt10");
NexButton bt11 = NexButton(10, 4, "bt11");
NexButton bt12 = NexButton(8, 5, "bt12");
NexButton bt13 = NexButton(9, 16, "bt13");

// Declare variable global
bool statusbt0 = false;
bool statusbt1 = false;
bool statusbt2 = false;
bool statusbt3 = false;
bool statusbt4 = false;
bool statusbt5 = false;
bool statusbt6 = false;
bool statusbt7 = false;
bool statusbt8 = false;
bool statusbt9 = false;
bool statusbt10 = false;
bool statusbt11 = false;
bool statusbt12 = false;
bool statusbt13 = false;

bool toggleState = false; // Tracks which set of fans are on

String r1;
String r2;
String r3;
String r4;
String r5;
String r6;
String r7;
String r8;
String r9;
String r10;
String r11;
String r12;

String temp_mode;
String vpd_mode;

millisDelay relayDelay;       // MillisDelay object for non-blocking delays
const unsigned long TOGGLE_TIME = 5000; // Time in milliseconds for relay toggle

// Register objects to the touch event list
NexTouch *nex_listen_list[] = {
  &bt0,
  &bt1,
  &bt2,
  &bt3,
  &bt4,
  &bt5,
  &bt6,
  &bt7,
  &bt8,
  &bt9,
  &bt10,
  &bt11,
  &bt12,
  &bt13,
  NULL
};

void switchToPage0() {
  Serial.print("page vpd1");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void switchToPage1() {
  Serial.print("page temp");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void switchToPage2() {
  Serial.print("page manual1");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void switchToPage3() {
  Serial.print("page manual2");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

// Function to turn off all fans
void turnOffAllFans() {
  digitalWrite(ExFan1, HIGH);
  digitalWrite(ExFan2, HIGH);
  digitalWrite(ExFan3, HIGH);
  digitalWrite(ExFan4, HIGH);
  digitalWrite(ExFan5, HIGH);
  digitalWrite(ExFan6, HIGH);
  digitalWrite(ExFan7, HIGH);
  digitalWrite(ExFan8, HIGH);
}

// Function to turn on specific fans
void TempLow(int fan1, int fan2, int fan3 = -1, int fan4 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  if (fan3 != -1) digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
}

void TempMid(int fan1, int fan2, int fan3, int fan4 = -1, int fan5 = -1, int fan6 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
}

void TempHigh(int fan1, int fan2, int fan3, int fan4, int fan5 = -1, int fan6 = -1, int fan7 = -1, int fan8 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
  if (fan7 != -1) digitalWrite(fan7, LOW);
  if (fan8 != -1) digitalWrite(fan8, LOW);
}

// Function to handle fans based on VPD
void VPDMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgVPD_v > 0.3 && MESP_ID_Time_AvgVPD_v <= 0.8) {
      TempLow(ExFan2, ExFan4);
    } else if (MESP_ID_Time_AvgVPD_v > 0.8 && MESP_ID_Time_AvgVPD_v <= 1.2) {
      TempMid(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgVPD_v > 1.2) {
      TempHigh(ExFan2, ExFan4, ExFan6, ExFan8);
    }
  } else {
    // Alternate fans when toggleState is false
    if (MESP_ID_Time_AvgVPD_v > VPD_LOW && MESP_ID_Time_AvgVPD_v <= VPD_MID) {
      TempLow(ExFan1, ExFan3);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_MID && MESP_ID_Time_AvgVPD_v <= VPD_HIGH) {
      TempMid(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_HIGH) {
      TempHigh(ExFan1, ExFan3, ExFan5, ExFan7);
    }
  }
}

// Function to handle fans based on Temperature
void TempMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v <= TEMP_MID) {
      TempLow(ExFan1, ExFan3);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v <= TEMP_HIGH) {
      TempMid(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      TempHigh(ExFan1, ExFan3, ExFan5, ExFan7);
    }
  } else {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v <= TEMP_MID) {
      TempLow(ExFan2, ExFan4);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v <= TEMP_HIGH) {
      TempMid(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      TempHigh(ExFan2, ExFan4, ExFan6, ExFan8);
    }
  }
}

//Saat tombol 1 ditekan,relay 1 menyala
void bt0PushCallback(void *ptr) {
  if (statusbt0 == false) {
    digitalWrite(A3, LOW);
    statusbt0 = true;

    //set button color RED
    //        bt0.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(A3, HIGH);
    statusbt0 = false;
    //reset button color
    //        bt0.Set_background_color_bco(57408);

  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt1PushCallback(void *ptr) {
  if (statusbt1 == false) {
    digitalWrite(A2, LOW);
    statusbt1 = true;
    //set button color RED
    //    bt1.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(A2, HIGH);
    statusbt1 = false;
    //reset button color
    //    bt1.Set_background_color_bco(57408);

  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt2PushCallback(void *ptr) {
  if (statusbt2 == false) {
    digitalWrite(30, LOW);
    statusbt2 = true;
    //set button color RED
    //    bt2.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(30, HIGH);
    statusbt2 = false;
    //reset button color
    //    bt2.Set_background_color_bco(57408);

  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void bt3PushCallback(void *ptr) {
  if (statusbt3 == false) {
    digitalWrite(28, LOW);
    statusbt3 = true;
    //set button color RED
    //    bt3.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(28, HIGH);
    statusbt3 = false;
    //reset button color
    //    bt3.Set_background_color_bco(57408);

  }

}

void bt4PushCallback(void *ptr) {
  if (statusbt4 == false) {
    digitalWrite(8, LOW);
    statusbt4 = true;
    //set button color RED
    //    bt4.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(8, HIGH);
    statusbt4 = false;
    //reset button color
    //    bt4.Set_background_color_bco(57408);

  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt5PushCallback(void *ptr) {
  if (statusbt5 == false) {
    digitalWrite(9, LOW);
    statusbt5 = true;
    //set button color RED
    //    bt5.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(9, HIGH);
    statusbt5 = false;
    //reset button color
    //    bt5.Set_background_color_bco(57408);

  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt6PushCallback(void *ptr) {
  if (statusbt6 == false) {
    digitalWrite(10, LOW);
    statusbt6 = true;
    //set button color RED
    //    bt6.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(10, HIGH);
    statusbt6 = false;
    //reset button color
    //    bt6.Set_background_color_bco(57408);

  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void bt7PushCallback(void *ptr) {
  if (statusbt7 == false) {
    digitalWrite(11, LOW);
    statusbt7 = true;
    //set button color RED
    //    bt7.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(11, HIGH);
    statusbt7 = false;
    //reset button color
    //    bt7.Set_background_color_bco(57408);

  }

}

void bt8PushCallback(void *ptr) {
  if (statusbt8 == false) {
    digitalWrite(A4, LOW);
    statusbt8 = true;
    //set button color RED
    //    bt8.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(A4, HIGH);
    statusbt8 = false;
    //reset button color
    //    bt8.Set_background_color_bco(57408);

  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt9PushCallback(void *ptr) {
  if (statusbt9 == false) {
    digitalWrite(9, LOW);
    statusbt1 = true;
    //set button color RED
    //    bt9.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(10, HIGH);
    statusbt9 = false;
    //reset button color
    //    bt9.Set_background_color_bco(57408);

  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt10PushCallback(void *ptr) {
  if (statusbt10 == false) {
    digitalWrite(A5, LOW);
    statusbt10 = true;
    //set button color RED
    //    bt10.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(A5, HIGH);
    statusbt10 = false;
    //reset button color
    //    bt10.Set_background_color_bco(57408);

  }

}

void bt11PushCallback(void *ptr) {
  if (statusbt11 == false) {
    digitalWrite(A7, LOW);
    statusbt11 = true;
    //set button color RED
    //    bt11.Set_background_color_bco(5411);

  }
  else {
    digitalWrite(A7, HIGH);
    statusbt11 = false;
    //reset button color
    //    bt11.Set_background_color_bco(57408);

  }

}

void bt12PushCallback(void *ptr) {
  if (Serial3.available()) {
    if (!statusbt12) {
      statusbt12 = true;
      statusbt13 = false; // Ensure Temperature mode is off
      toggleState = true; // Ensure toggle state is consistent
      VPDMode();
      relayDelay.start(TOGGLE_TIME);
      vpd_mode = "1";
    } else {
      statusbt12 = false;
      relayDelay.stop();
      turnOffAllFans();
      vpd_mode = "0";
    }
    StaticJsonDocument <500> doc3;

    if (count1 == 0)
    {

      StaticJsonDocument<200> clearr;
      doc3 = clearr;
      char tempstring1[500];
      serializeJson(clearr, tempstring1);
      memset(tempstring1, 0, 500);

      count1 = 20;
    }

    if (MESP_ID_Time_AvgVPD_v >= 0.1)
    {
      count1 --;


      if ((MESP_ID_Time_AvgVPD_v >= 0.0) && (MESP_ID_Time_AvgVPD_v <= 0.5))
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "8";

        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);


      }
      else if ((MESP_ID_Time_AvgVPD_v >= 0.6) && (MESP_ID_Time_AvgVPD_v <= 0.8))
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "9";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
      else if ((MESP_ID_Time_AvgVPD_v >= 1.5) && (MESP_ID_Time_AvgVPD_v <= 1.7))
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "10";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);
      }
      else if ((MESP_ID_Time_AvgVPD_v >= 1.8) && (MESP_ID_Time_AvgVPD_v <= 2.0))

      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "9";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
      else if (MESP_ID_Time_AvgVPD_v >= 2.1)
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "8";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }

      else if ((MESP_ID_Time_AvgVPD_v >= 0.9) && (MESP_ID_Time_AvgVPD_v <= 1.4))
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "7";
        Alerts["level"] = "11";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
    }
    Serial.println(MESP_ID_Time_AvgVPD_v);
    Serial.println("VPD SETPOINT");

  }
}

void bt13PushCallback(void *ptr) {
  if (Serial3.available()) {

    if (!statusbt13) {
      statusbt13 = true;
      statusbt12 = false; // Ensure VPD mode is off
      toggleState = true; // Ensure toggle state is consistent
      TempMode();
      relayDelay.start(TOGGLE_TIME);
      temp_mode = "1";
    } else {
      statusbt13 = false;
      relayDelay.stop();
      turnOffAllFans();
      temp_mode = "0";
    }

    StaticJsonDocument <500> doc3;

    if (count3 == 0)
    {
      StaticJsonDocument<200> clearr1;
      doc3 = clearr1;
      char tempstring2[500];
      serializeJson(clearr1, tempstring2);
      memset(tempstring2, 0, 500);

      count3 = 10;
    }

    if (MESP_ID_Time_AvgAT_v > 0)
    {
      count3--;


      if ((MESP_ID_Time_AvgAT_v >= 36) && (MESP_ID_Time_AvgAT_v <= 38))
      {


        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "12";
        Alerts["level"] = "10";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
      else if ((MESP_ID_Time_AvgAT_v >= 39) && (MESP_ID_Time_AvgAT_v <= 41))
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "12";
        Alerts["level"] = "9";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
      else if (MESP_ID_Time_AvgAT_v > 42)
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "12";
        Alerts["level"] = "8";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
      else if (MESP_ID_Time_AvgAT_v <= 35)
      {

        JsonObject Alerts = doc3.createNestedObject("Alerts");
        Alerts["Mode"] = "12";
        Alerts["level"] = "11";
        serializeJson(doc3, Serial);
        serializeJson(doc3, Serial3);

      }
    }
  }
}

float temp_I() {
  char *ui = strtok(buff, "o");
  str = ui;
  int vale = dfd.indexOf("it") + 3;
  str.remove(0, vale);
  float i_temp = float(str.toInt()) / 10;
  return (i_temp);

}

float temp_O() {//tempit10ot10
  Serial.println(dfd);
  int vale = dfd.indexOf("ot") + 3;
  dfd.remove(0, vale);
  Serial.println(dfd);
  float o_temp = float(dfd.toInt()) / 10;
  return (o_temp);

}

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

void port() {
  int vale = dfd.indexOf(">") + 1;
  dfd.remove(0, vale);//port
  Serial.print("Port: ");
  Serial.println(dfd);
}

void meshpass() {
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
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  runTimeNex();
}

void runTimeNex() {

  rtc.begin();

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
void dateandtime() {
  DateTime now = rtc.now();

  hourupg = now.hour();
  minupg = now.minute();
  secslive = now.second();
  Datelive = now.day();
  Monthlive = now.month();
  Yearlive = now.year();

}
/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  val = dfd.indexOf("s") + 1;
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}

void timeMin() {
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  val = cSmn.indexOf("n") + 1;
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt();
  minrc = minRc;
}

void timeHr() {
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1;
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;

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

void ssidpwd() {
  //  Serial.print("Json");
  DynamicJsonDocument doc(150);
  JsonObject SdPwd = doc.createNestedObject("SdPwd");
  SdPwd["ssid"] = ssid;
  SdPwd["password"] = password;
  serializeJsonPretty(doc, Serial3);

}

void SerialCom(String frommob)
{
  String data_from_display = "";
  //       String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  //    Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());

}

void FromEspData1()
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

void eventmode()
{
  StaticJsonDocument<500> doc1;

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

void funtimer()
{
  eventmode();
}

void callSerialCom() {
  if (Serial2.available()) {
    SerialCom("");
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  Serial.println("Axalyn...");
  rtc.begin();
  rtc.adjust(DateTime(2023, 2, 2, 4, 58, 0));
  //Initialize Nextion Library
  nexInit();
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

  // Register the push/pop event callback function
  bt0.attachPush(bt0PushCallback, &bt0);
  bt1.attachPush(bt1PushCallback, &bt1);
  bt2.attachPush(bt2PushCallback, &bt2);
  bt3.attachPush(bt3PushCallback, &bt3);
  bt4.attachPush(bt4PushCallback, &bt4);
  bt5.attachPush(bt5PushCallback, &bt5);
  bt6.attachPush(bt6PushCallback, &bt6);
  bt7.attachPush(bt7PushCallback, &bt7);
  bt8.attachPush(bt8PushCallback, &bt8);
  bt9.attachPush(bt9PushCallback, &bt9);
  bt10.attachPush(bt10PushCallback, &bt10);
  bt11.attachPush(bt11PushCallback, &bt11);
  bt12.attachPush(bt12PushCallback, &bt12);
  bt13.attachPush(bt13PushCallback, &bt13);

  delay(2000);
  turnOffAllFans();
  delay(500);
}

unsigned long SCpreviousMillis = 0;
unsigned long ESPpreviousMillis = 0;
unsigned long ATpreviousMillis = 0;
unsigned long VPpreviousMillis = 0;
unsigned long nextionmills = 0;
unsigned long manualsend = 0;

void loop()
{
  if (Serial3.available()) {
    ////////////************** Serialcom loop Mode***********///////////
    //When push/pop event occured execute component in touch event list

    String receivedPayload = Serial3.readStringUntil('\n');
    DynamicJsonDocument avgDoc(1024);
    deserializeJson(avgDoc, Serial3);
    DeserializationError error = deserializeJson(avgDoc, Serial3);

    if (error) {

      return;
    }

    MESP_ID_Time_AvgAT_v = avgDoc["AT_V_avg"].as<float>();
    MESP_ID_Time_AvgRH_v = avgDoc["RH_v_avg"].as<float>();
    MESP_ID_Time_AvgVPD_v = avgDoc["VPD_v_avg"].as<float>();
    MESP_ID_Time_AvgHI_v = avgDoc["HI_v_avg"].as<float>();
    MESP_ID_Time_co2_v = avgDoc["Co2_v_avg"].as<int>();
    MESP_ID_Time_EvTr_v = avgDoc["ET_v_avg"].as<float>();
    MESP_ID_Time_BaPr_v = avgDoc["BP_v_avg"].as<float>();
    MESP_ID_Time_SoRa_v = avgDoc["SR_v_avg"].as<float>();
    MESP_ID_Time_DePt_v = avgDoc["DP_v_avg"].as<float>();
    MESP_ID_Time_SeL_v = avgDoc["SL_v_avg"].as<int>();

    nexLoop(nex_listen_list);

    // Apply fan control logic based on active mode
    if (statusbt12) { // VPD Mode
      if (relayDelay.justFinished()) {
        relayDelay.repeat(); // Restart the delay
        toggleState = !toggleState;  // Toggle between states
        VPDMode();
        // Debugging: print current temperature and toggle state
        Serial.print("Vpd: ");
        Serial.println(MESP_ID_Time_AvgAT_v);
        Serial.print("Toggle State: ");
        Serial.println(toggleState);
      }
    } else if (statusbt13) { // Temperature Mode
      if (relayDelay.justFinished()) {
        relayDelay.repeat(); // Restart the delay
        toggleState = !toggleState;  // Toggle between states
        TempMode();
        // Debugging: print current temperature and toggle state
        Serial.print("Temperature: ");
        Serial.println(MESP_ID_Time_AvgAT_v);
        Serial.print("Toggle State: ");
        Serial.println(toggleState);
      }
    }

    callSerialCom();

    Serial.println("Data From Esp: ");
    Serial.print("AT:");
    Serial.println(MESP_ID_Time_AvgAT_v);
    Serial.print("RH:");
    Serial.println(MESP_ID_Time_AvgRH_v);
    Serial.print("VPD:");
    Serial.println(MESP_ID_Time_AvgVPD_v);
    Serial.print("HI:");
    Serial.println(MESP_ID_Time_AvgHI_v);
    Serial.print("Co2:");
    Serial.println(MESP_ID_Time_co2_v);
    Serial.print("ET:");
    Serial.println(MESP_ID_Time_EvTr_v);
    Serial.print("BP:");
    Serial.println(MESP_ID_Time_BaPr_v);
    Serial.print("SR:");
    Serial.println(MESP_ID_Time_SoRa_v);
    Serial.print("DP:");
    Serial.println(MESP_ID_Time_DePt_v);
    Serial.print("SL:");
    Serial.println(MESP_ID_Time_SeL_v);

    if (MESP_ID_Time_AvgAT_v > 0)
    {

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

      if (MESP_ID_Time_EvTr_v > 0)
      {
        Serial2.print("ev.txt=\"");
        Serial2.print(MESP_ID_Time_EvTr_v);
        Serial2.print("\"");
        Serial2.write(0x03ff);
        Serial2.write(0x03ff);
        Serial2.write(0x03ff);
      }

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
    }

    if (WAT > 0)
    {

      Serial2.print("att.txt=\"");
      Serial2.print(WAT);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("rhh.txt=\"");
      Serial2.print(WRH);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("bpp.txt=\"");
      Serial2.print(WBaPa);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("sll.txt=\"");
      Serial2.print(WSeaL);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("srr.txt=\"");
      Serial2.print(WSolar);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("hii.txt=\"");
      Serial2.print(WHI);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("rp.txt=\"");
      Serial2.print(WRH);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("evv.txt=\"");
      Serial2.print(WEv);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("dppp.txt=\"");
      Serial2.print(WDew);
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
    //SerialU.println(yearch);

    Serial2.print("yearr.txt=\"");
    Serial2.print(Yearlive);//Yearlive
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  }

  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  if (manualsendcur - manualsend >= 5000) {
    manualsend = manualsendcur;
    manualread();
    counterun();
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
  serializeJson(Manualdoc1, Serial);
  serializeJson(Manualdoc1, Serial3);
  Serial.println(".....");
}
