#include <Arduino.h>
#include <MemoryFree.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <Wire.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeLib.h>
#include <EEPROMex.h>
#include "millisDelay.h"
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
float SPtemp;
extern volatile unsigned long timer0_millis;
String password;
String pwdname();
String pwd();
char buff[2000];
void AutoTemp_control();
void vpd_control();
void sendData(String );
void asd();
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
int f1,f2,f3,f4,f5,f6,f7,f8,cp,fp,fs,co,ir,doo,dr,pb;
const char* Relaystate;
String set_point_from_app ;
String cndchck;
int counterm;
String checkstate ;
String storeprvstate = "";
const char* Allrelays_FAN1;
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
millisDelay getvalue;
///////////////////////////////////////////
float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_AvgVPD_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;
float MESPWS_ID_Time_AT_v, MESPWS_ID_Time_RH_v, MESPWS_ID_Time_VPD_v, MESPWS_ID_Time_HI_v, MESPWS_ID_Time_EvTr_v, MESPWS_ID_Time_BaPr_v, MESPWS_ID_Time_SoRa_v, MESPWS_ID_Time_DePt_v;
float MESPWS_ID_Time_WSwsclk_v, MESPWS_ID_Time_WSwsaclk_v, MESPWS_ID_Time_WSwdclk_v, MESPWS_ID_Time_WSwdaclk_v, MESPWS_ID_Time_WSclohei_v;
int MESPWS_ID_Time_WSntips_v, MESPWS_ID_Time_SeL_v, MESPWS_ID_Time_ntl_v;
float WAT , WRH ,  WVPD  , WHI, WEv , WBaPa, WSolar , WDew, WSeaL , WSclk , WSaclk , WDclk, WDaclk ;
float ATemp, AHumi;
///////////////////////////////////////////
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

  /**************************    NEXTION - RTC   ********************/
  if ((data_from_display.substring(0, 3) == "clk")) {
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();

    feedBack = "clk";
    dateandtime();
    readalldata();
    setTimeNex();

  }
  if ((data_from_display.substring(0, 6) == "settc")) {
    runTimeNex();
  }



  /**************************    MESH    ********************/
  if ((dfd.substring(0, 4) == "mesh")) {
    meshpass();
    prefix();
    port();
    feedBack = "mesh";
  }
  /************************************** TEMPERATURE **************************/

  if ((dfd.substring(0, 4) == "temp")) {



    float In_TempAutoSetpoint = vpdtemp_I();

    SPtemp = In_TempAutoSetpoint ;

    float Out_TempAutoSetpoint = vpdtemp_O();

    feedBack = "tempat";

  }
  //*********************************NEXVAL FUNCTION*****************************
  if ((data_from_display.substring(0, 5) == "page1"))
  {
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
 if(MESP_ID_Time_EvTr_v>0)
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

    }
  }
  if ((data_from_display.substring(0, 5) == "page2"))
  {
    if (MESP_ID_Time_AvgAT_v > 0)
    {
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
    }
  }
  if ((data_from_display.substring(0, 5) == "page3"))
  {
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
    }
  }
  if ((data_from_display.substring(0, 5) == "page4"))
  {
    if (WAT > 0)
    {
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
  }
  //********************************************************************VPD*******************************
  //VPD Controller
  if (dfd == "vpdoff") {
    feedBack = "vpdon";
    Serial.println("VPD  ON");
    VpdautoModeOn = true;

  }
  if (dfd == "vpdon") {
    asd();
    VpdautoModeOn = false;
    feedBack = "vpdoff";

    Serial.println("VPD Off");
    vpd_func = false;
  }


  //********************************************************************AUTO TEMP MODE*******************************

  if (dfd == "tmpon") {


    feedBack = "tmpon";
    TemperatureAutoModeOn = true;
    Serial.println("TEMP  ON");
    temp_func = true;

  }
  if (dfd == "tmpoff") {


    feedBack = "tmpoff";
    TemperatureAutoModeOn = false;
    asd();
    Serial.println("TEMP  OFF");

    temp_func = false;
  }

  sendData(data_from_display);
}

void sendData(String dfd) {



  if (dfd == "f1on" )
  {

    feedBack = "f1on";
    pinMode(ExFan1, OUTPUT);
    digitalWrite(ExFan1, LOW);
  }
  if (dfd == "f1off")
  {
    feedBack = "f1off";
    digitalWrite(ExFan1, HIGH);
  }

  if (dfd == "f2on")
  {

    feedBack = "f2on";

    pinMode(ExFan2, OUTPUT);
    digitalWrite(ExFan2, LOW);
    Serial.println("f2 On");
  }
  if (dfd  == "f2off")
  {

    feedBack = "f2off";



    digitalWrite(ExFan2, HIGH);
    Serial.println("f2 Off");
  }
  if (dfd == "f3on")
  {
    feedBack = "f3on";


    pinMode(ExFan3, OUTPUT);
    digitalWrite(ExFan3, LOW);
    Serial.println("f3 On");
  }
  if (dfd == "f3off")
  {
    feedBack = "f3off";



    digitalWrite(ExFan3, HIGH);
    Serial.println("f3 Off");
  }
  if (dfd == "f4on") {
    feedBack = "f4on";

    pinMode(ExFan4, OUTPUT);
    digitalWrite(ExFan4, LOW);
    Serial.println("f4 On");
  }
  if (dfd == "f4off") {
    feedBack = "f4off";

    digitalWrite(ExFan4, HIGH);
    Serial.println("f4 Off");
  }
  if (dfd == "f5on") {
    feedBack = "f5on";


    pinMode(ExFan5, OUTPUT);
    digitalWrite(ExFan5, LOW);
    Serial.println("f5 On");
  }
  if (dfd == "f5off") {
    feedBack = "f5off";


    digitalWrite(ExFan5, HIGH);
    Serial.println("f5 Off");
  }
  if (dfd == "f6on") {
    feedBack = "f6on";


    pinMode(ExFan6, OUTPUT);
    digitalWrite(ExFan6, LOW);
    Serial.println("f6 On");
  }
  if (dfd == "f6off") {
    feedBack = "f6off";

    digitalWrite(ExFan6, HIGH);
    Serial.println("f6 Off");
  }
  if (dfd == "f7on") {
    feedBack = "f7on";

    pinMode(ExFan7, OUTPUT);
    digitalWrite(ExFan7, LOW);
    Serial.println("f7 On");
  }
  if (dfd == "f7off") {
    feedBack = "f7off";

    digitalWrite(ExFan7, HIGH);
    Serial.println("f7 Off");
  }
  if (dfd == "f8on") {
    feedBack = "f8on";


    pinMode(ExFan8, OUTPUT);
    digitalWrite(ExFan8, LOW);
    Serial.println("f8 On");
  }
  if (dfd == "f8off") {
    feedBack = "f8off";


    digitalWrite(ExFan8, HIGH);
    Serial.println("f8 Off");
  }
  if (dfd == "cpon") {
    feedBack = "cpon";


    pinMode(pad1, OUTPUT);
    digitalWrite(pad1, LOW);
    Serial.println("cp On");
  }
  if (dfd == "cpoff") {
    feedBack = "cpoff";


    digitalWrite(pad1, HIGH);
    Serial.println("cp Off");
  }
  if (dfd == "fpon") {
    feedBack = "fpon";



    pinMode(Fogger, OUTPUT);
    digitalWrite(Fogger, LOW);
    Serial.println("fp On");
  }
  if (dfd == "fpoff") {
    feedBack = "fpoff";


    digitalWrite(Fogger, HIGH);
    Serial.println("fp Off");
  }
  if (dfd == "fcon") {
    feedBack = "fcon";


    pinMode(FoggerSol, OUTPUT);
    digitalWrite(FoggerSol, LOW);
    Serial.println("fc On");
  }
  if (dfd == "fcoff") {
    feedBack = "fcoff";


    digitalWrite(FoggerSol, HIGH);
    Serial.println("fc Off");
  }
  if (dfd == "co2on") {
    feedBack = "co2on";

    pinMode(Co2Gen, OUTPUT);
    digitalWrite(Co2Gen, LOW);
    Serial.println("co2 On");
  }
  if (dfd == "co2off") {
    feedBack = "co2off";


    digitalWrite(Co2Gen, HIGH);
    Serial.println("co2 Off");
  }
}
//***************************************************VPD AUTO MODE***********************************************
void vpd_control(float MESP_ID_Time_AvgVPD_v)
{
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

  float vpd = MESP_ID_Time_AvgVPD_v;
  MESP_ID_Time_AvgVPD_v = random(0, 2);
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
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);

      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);


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
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
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
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
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
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, LOW);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, HIGH);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
      }


      else if (mills >= 2405000)
      {
        noInterrupts ();
        timer0_millis = 0;
        interrupts ();
      }

    }

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
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
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
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
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
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, LOW);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, HIGH);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, LOW);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, HIGH);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, LOW);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, HIGH);
        pinMode(pad1, OUTPUT);
        digitalWrite(pad1, LOW);
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
        Serial.println( mills );
        pinMode(ExFan1, OUTPUT);
        digitalWrite(ExFan1, HIGH);
        pinMode(ExFan2, OUTPUT);
        digitalWrite(ExFan2, LOW);
        pinMode(ExFan3, OUTPUT);
        digitalWrite(ExFan3, HIGH);
        pinMode(ExFan4, OUTPUT);
        digitalWrite(ExFan4, LOW);
        pinMode(ExFan5, OUTPUT);
        digitalWrite(ExFan5, HIGH);
        pinMode(ExFan6, OUTPUT);
        digitalWrite(ExFan6, LOW);
        pinMode(ExFan7, OUTPUT);
        digitalWrite(ExFan7, HIGH);
        pinMode(ExFan8, OUTPUT);
        digitalWrite(ExFan8, LOW);
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


      Serial.println("nothing to do");
      pinMode(ExFan1, OUTPUT);
      digitalWrite(ExFan1, HIGH);
      pinMode(ExFan2, OUTPUT);
      digitalWrite(ExFan2, HIGH);
      pinMode(ExFan3, OUTPUT);
      digitalWrite(ExFan3, HIGH);
      pinMode(ExFan4, OUTPUT);
      digitalWrite(ExFan4, HIGH);
      pinMode(ExFan5, OUTPUT);
      digitalWrite(ExFan5, HIGH);
      pinMode(ExFan6, OUTPUT);
      digitalWrite(ExFan6, HIGH);
      pinMode(ExFan7, OUTPUT);
      digitalWrite(ExFan7, HIGH);
      pinMode(ExFan8, OUTPUT);
      digitalWrite(ExFan8, HIGH);
      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);



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
void callasd()
{
  if (VpdautoModeOn == false)
  {
    asd();
  }
}
//****************************************************************Auto Temp*************************************************
void AutoTemp_control(float SPtemp) {
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


  float ATempHysteris = SPtemp + 1;

  float ATemp = random(25,36); ;
  if (ATemp > 0)
  {
    count3--;


    if ((ATemp >= 36) && (ATemp <= 38))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if ((ATemp >= 39) && (ATemp <= 41))
    {




      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if (ATemp > 42)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

    }
    else if (ATemp <= 35)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




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
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, LOW);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);

          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, LOW);
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
      else if ((ATemp >= ATempMid) && (ATemp < ATempHigh)) //31>30&& 31<32        //Temp=31  31>30  && 31<32//31.80>=30 && 31.80<32
      {

        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 60000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 61000) && (mills <= 120000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
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
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
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
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
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
      pinMode(ExFan3, OUTPUT);
      digitalWrite(ExFan3, HIGH);
      pinMode(ExFan4, OUTPUT);
      digitalWrite(ExFan4, HIGH);
      pinMode(ExFan5, OUTPUT);
      digitalWrite(ExFan5, HIGH);
      pinMode(ExFan6, OUTPUT);
      digitalWrite(ExFan6, HIGH);
      pinMode(ExFan7, OUTPUT);
      digitalWrite(ExFan7, HIGH);
      pinMode(ExFan8, OUTPUT);
      digitalWrite(ExFan8, HIGH);
      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);
    }
  }
}
void FromEspData()
{
  StaticJsonDocument<384> doc;
  deserializeJson(doc, Serial3);
  deserializeJson(doc, Serial3);
  deserializeJson(doc, Serial3);


  DeserializationError error = deserializeJson(doc, Serial3);

  if (error) {

    return;
  }

  MESP_ID_Time_AvgAT_v = doc["AT"];
  MESP_ID_Time_AvgRH_v = doc["RH"];
  MESP_ID_Time_AvgVPD_v = doc["VPD"];
  MESP_ID_Time_AvgHI_v = doc["HI"];
  MESP_ID_Time_co2_v = doc["Co2"];
  MESP_ID_Time_EvTr_v = doc["Ev"];
  MESP_ID_Time_BaPr_v = doc["BaPa"];
  MESP_ID_Time_SoRa_v = doc["Solar"];
  MESP_ID_Time_DePt_v = doc["Dew"];
  MESP_ID_Time_SeL_v = doc["SeaL"];
 MESP_ID_Time_EvTr_v = MESP_ID_Time_EvTr_v /10000000;


  WAT = doc["WAT"];
  WRH = doc["WRH"];
  WVPD = doc["WVPD"];
  WHI = doc["WHI"];
  WEv = doc["WEv"];
  WBaPa = doc["WBaPa"];
  WSolar = doc["WSolar"];
  WDew = doc["WDew"];
  WSeaL = doc["WSeaL"];
  WSclk = doc["WSclk"];
  WSaclk = doc["WSaclk"];
  WDclk = doc["WDclk"];
  WDaclk = doc["WDaclk"];
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

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);
  Serial.println("Axalyn...");
  rtc.begin();
  rtc.adjust(DateTime(2023, 2, 2, 4, 58, 0));

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
unsigned long SCpreviousMillis = 0;
unsigned long ESPpreviousMillis = 0;
unsigned long ATpreviousMillis = 0;
unsigned long VPpreviousMillis = 0;
unsigned long nextionmills = 0;
unsigned long manualsend = 0;

void loop()
{
  ////////////************** Serialcom loop Mode***********///////////
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = currentMillis;

    callSerialCom();
  }
  //////////************** Node receviing loop Mode***********///////////
  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - ESPpreviousMillis >= 3000)
  {
    ESPpreviousMillis = NXcurrentMillis;

    getvalue.start(100);
  }
  if (getvalue.isRunning())
  {
    FromEspData();
  }
  if (getvalue.justFinished())
  {
  }
  //////////************** Temperature loop Mode***********///////////
  if (TemperatureAutoModeOn == true ) {

    unsigned long ATcurrentMillis = millis();
    if (ATcurrentMillis - ATpreviousMillis >= 10000) {
      ATpreviousMillis = ATcurrentMillis;

      AutoTemp_control( SPtemp);
      Serial.println("TEMP");
      funtimer();
    }
  }
  //////////************** VPD loop Mode***********///////////
  if (VpdautoModeOn == true ) {

    unsigned long VPcurrentMillis = millis();
    if (VPcurrentMillis - VPpreviousMillis >= 10000) {
      VPpreviousMillis = VPcurrentMillis;
      vpd_control( MESP_ID_Time_AvgVPD_v);
      funtimer();
    }
  }
  //////////************** Nexval Mode***********///////////
  unsigned long nextionmillsc = millis();
  if (nextionmillsc - nextionmills >= 3000) {
    nextionmillsc = nextionmillsc;
    nexval();
  }
  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  if (manualsendcur - manualsend >= 4000) {
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
void nexval()
{
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

  if(MESP_ID_Time_EvTr_v>0)
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
  /***************** HWM ****************/

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
  //    SerialU.println(yearch);

  Serial2.print("yearr.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

}
  
