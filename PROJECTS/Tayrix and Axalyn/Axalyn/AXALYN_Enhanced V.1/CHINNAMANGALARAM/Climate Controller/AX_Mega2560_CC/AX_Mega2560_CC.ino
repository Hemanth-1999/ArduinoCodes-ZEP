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
const float SPtemp = 27;
const float ATempHysteris = SPtemp + 1;
extern volatile unsigned long timer0_millis;
String password;
String pwdname();
String pwd();
char buff[2000];

void port();
void prefix();
void nexval();
void VPDMode();
void TempMode();
void meshpass();
void counterun();
void manualread();
void vpd_control();
void sendData(String );
void AutoTemp_control();
void LowState(int fan1, int fan2, int fan3 = -1, int fan4 = -1);
void MidState(int fan1, int fan2, int fan3, int fan4 = -1, int fan5 = -1, int fan6 = -1);
void HighState(int fan1, int fan2, int fan3, int fan4, int fan5 = -1, int fan6 = -1, int fan7 = -1, int fan8 = -1) ;

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

char RelayStatus[1024];

// Temperature setpoints
const float TEMP_LOW = ATempHysteris;   // 30°C
const float TEMP_MID = TEMP_LOW + 2.0;  // 32°C
const float TEMP_HIGH = TEMP_MID + 2.0; // 34°C

// VPD setpoints
const float VPD_LOW = 0.3;
const float VPD_MID = 0.8;
const float VPD_HIGH = 1.2;

NexPage page0 = NexPage(8, 0, "vpd1");
NexPage page1 = NexPage(9, 0, "temp");

//This page is for testing
//NexPage page1 = NexPage(0, 0, "temp");

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
//NexButton bt12 = NexButton(0, 1, "bt12");
//NexButton bt13 = NexButton(0, 6, "bt13");

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
const unsigned long TOGGLE_TIME = 1200000; // Time in milliseconds for relay toggle
//const unsigned long TOGGLE_TIME = 5000; // Time in milliseconds for relay toggle

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

char EVENTSTATUS[1024];

void eventmode()
{
  DynamicJsonDocument doc1(1024);

  if (TemperatureAutoModeOn == true)
  {

    doc1["TEMP"] = "3";

    Serial.println(EVENTSTATUS);
    Serial.println("Temp_Mode_Status");

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
    serializeJson(doc1, EVENTSTATUS);
  }

  if (VpdautoModeOn == true)
  {
    doc1["VPD"] = "4";

    Serial.println(EVENTSTATUS);
    Serial.println("Vpd_Mode_Status");

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
    serializeJson(doc1, EVENTSTATUS);

  }
}

void callSerialCom() {
  if (Serial2.available()) {
    SerialCom("");
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600); // Nextion
  Serial3.begin(9600);
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

String lastPayload = "";       // Store the last sent payload

void loop()
{

  nexLoop(nex_listen_list);

  if (Serial3.available()) {

    String receivedPayload = Serial3.readStringUntil('\n');  // Read the data sent by ESP8266

    // Debug: Print the received data
    Serial.print("Received data: ");
    Serial.println(receivedPayload);

    if (receivedPayload.length() > 0) {
      lastPayload = receivedPayload;  // Store the last received payload

      // Allocate a JsonDocument to deserialize the JSON data
      StaticJsonDocument<1024> avgDoc;
      deserializeJson(avgDoc, Serial3);
      // Deserialize the JSON data
      DeserializationError error = deserializeJson(avgDoc, Serial3);

      if (!error) {
        // Extract values from the JSON document
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

        // Apply fan control logic based on active mode
        if (statusbt12) { // VPD Mode
          if (relayDelay.justFinished()) {
            relayDelay.repeat(); // Restart the delay
            toggleState = !toggleState;  // Toggle between states
            VPDMode();

            // Debugging: print current temperature and toggle state
            Serial.print("Vpd: ");
            Serial.println(MESP_ID_Time_AvgVPD_v);
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

        // Debugging: Print the extracted values
        Serial.println("New data received and parsed:");
        Serial.print("AT_V_avg: "); Serial.println(MESP_ID_Time_AvgAT_v);
        Serial.print("RH_v_avg: "); Serial.println(MESP_ID_Time_AvgRH_v);
        Serial.print("VPD_v_avg: "); Serial.println(MESP_ID_Time_AvgVPD_v);
        Serial.print("HI_v_avg: "); Serial.println(MESP_ID_Time_AvgHI_v);
        Serial.print("Co2_v_avg: "); Serial.println(MESP_ID_Time_co2_v);
        Serial.print("ET_v_avg: "); Serial.println(MESP_ID_Time_EvTr_v);
        Serial.print("BP_v_avg: "); Serial.println(MESP_ID_Time_BaPr_v);
        Serial.print("SR_v_avg: "); Serial.println(MESP_ID_Time_SoRa_v);
        Serial.print("DP_v_avg: "); Serial.println(MESP_ID_Time_DePt_v);
        Serial.print("SL_v_avg: "); Serial.println(MESP_ID_Time_SeL_v);

        nexval();
        callSerialCom();

      } else {

        return;
      }
    }

    // If no new data has arrived, keep using the last received data
    if (lastPayload.length() > 0) {
      Serial.println("Using last received data:");
      Serial.println(lastPayload);
    } else {
      Serial.println("No data received yet.");
    }
  }

  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  unsigned long manualsend = 0;
  if (manualsendcur - manualsend >= 5000) {
    manualsend = manualsendcur;
    manualread();
    counterun();
  }

}
