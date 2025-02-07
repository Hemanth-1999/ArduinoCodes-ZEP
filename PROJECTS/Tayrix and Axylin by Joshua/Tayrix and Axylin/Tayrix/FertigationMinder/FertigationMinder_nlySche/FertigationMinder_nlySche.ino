/*==============================================
  Libraries:
  ==============================================*/
#include <Arduino.h>
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <menuIO/encoderIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <menuIO/U8GLibOut.h>
#include <ClickEncoder.h>
#include <menuIO/clickEncoderIn.h>
#include <EEPROM.h>
#include <SPI.h>
#include <LoRa.h>
#include <ETH.h>
#include <WiFiManager.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

/*==============================================
  Declrations
  ==============================================*/
#define TRIGGER_PIN 34
bool wm_nonblocking = false;
WiFiManager wm;
WiFiClient espClient;
static bool eth_connected = false;
WiFiManagerParameter Mqttaddress;
WiFiManagerParameter MqttPort;
WiFiManagerParameter Mqttusername;
WiFiManagerParameter Mqttpassword;
WiFiManagerParameter devicename;
WiFiManagerParameter farm;
WiFiManagerParameter field;
WiFiManagerParameter zone;
char hostnamee[40] ;
char portt[6] ;
char userr[20];
char passwordd[40];
char deviceidd[20];
char farmm[20];
char fieldd[20];
char zonee[20];
char broker[40] ;
int mqttports;
char usernames [40];
char passwords[40] ;
char dev[40] ;
char frm[40] ;
char fld[40] ;
char zne[40] ;
#define ss 17
#define rst 35
#define dio0 39
byte MonolineMaster = 0xFF;
byte SoilMinder = 0xBB;
String Mymessage = "";
String SenderNode = "";
String outgoing;
byte msgCount = 0;
String incoming = "";
int Nvalue, Pvalue , Kvalue;
float SMvalue, STvalue, PHvalue, ECvalue;
#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000
int soillevel = 98;
int Ontimehr ;
int offtimehr;
int Ontimemin ;
int offtimemin;
long ontime;
long offtime;
int Eepromhur1 = 10;
int Eeprommin1 = 20;
int Eepromhur2 = 30;
int Eeprommin2 = 40;

int Eepromhur1sl1 = 50;
int Eeprommin1sl1 = 60;
int Eepromhur2sl2 = 70;
int Eeprommin2sl2 = 80;
int Eepromhur1sl1off = 90;
int Eeprommin1sl1off = 100;
int EepromCSD = 130;
int EepromCSM = 110;
int EepromCSY = 120;
bool Pumpstate;
bool Fertigationstate;
bool zone1;
bool zone2;
bool zone3;
bool zonE11;
bool zonE22;
bool zonE33;
bool format;
bool Schedulesstate;
int Slot1hr ;
int Slot1min;
int Slot1hroff ;
int Slot1minoff;
int Slot2hr ;
int Slot2min;
int CSDate ;
int CSMonth  ;
int CSYear ;

#define MainPumpA  33
#define FertSolenoid  32
#define ZonE1 9
#define ZonE2 10

/*==============================================
  Declrations:Rotary Encoder pins
  ==============================================*/
#define encA    10                                                                                                                                   
#define encB    36
#define encBtn  5
hw_timer_t * timer = NULL;
//********************************** ENCODER

ClickEncoder clickEncoder(encA, encB, encBtn, 2);
ClickEncoderStream encStream(clickEncoder, 1);
MENU_INPUTS(in, &encStream);
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {      //Defining Inerrupt function with IRAM_ATTR for faster access
  portENTER_CRITICAL_ISR(&timerMux);
  clickEncoder.service();
  portEXIT_CRITICAL_ISR(&timerMux);
}

/*==============================================
  Declrations: Display Configuration
  ==============================================*/
#define USE_SWSPI
#define fontName  u8g2_font_5x7_tf
#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64
int x = 0;
int scroll_direction = 1;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 23 /* A4 */ , /* data=*/ 18 /* A2 */, /* CS=*/ 21 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
const colorDef<uint8_t> colors[6] MEMMODE = {
  {{0, 0}, {0, 1, 1}}, //bgColor
  {{1, 1}, {1, 0, 0}}, //fgColor
  {{1, 1}, {1, 0, 0}}, //valColor
  {{1, 1}, {1, 0, 0}}, //unitColor
  {{0, 1}, {0, 0, 1}}, //cursorColor
  {{1, 1}, {1, 0, 0}}, //titleColor
};
result doAlert(eventMask e, prompt &item);
result showEvent(eventMask e, navNode& nav, prompt& item) {
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}



/*==============================================
  SubMenu-Section
  ==============================================*/
//*****************Solenoid Section toggle//*****************
TOGGLE(zone1, Zone1, "Zone1", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );

TOGGLE(zone2, Zone2, "Zone2", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );
TOGGLE(zone3, Zone3, "Zone3", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );
//*****************Manual Mode toggle//*****************

TOGGLE(Pumpstate, MainPump, "Mainpump", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );

TOGGLE(Fertigationstate, Fertigation, "Fertigation", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );
TOGGLE(zonE11, zonE1, "Zone1", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );
TOGGLE(zonE22, zonE2, "Zone2", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );

TOGGLE(zonE33, zonE3, "Zone3", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(":ON", HIGH, doNothing, noEvent)
       , VALUE(":OFF", LOW, doNothing, noEvent)
      );


MENU(Manual,       "->MANUAL", showEvent, anyEvent, noStyle
     , SUBMENU(MainPump)
     , SUBMENU(Fertigation)
     , SUBMENU(zonE1)
     , SUBMENU(zonE2)
     , SUBMENU(zonE3)
     , EXIT("<Back")
    );
//*****************Schedule-Section//*****************

TOGGLE(Schedulesstate, State, "       ", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE(" TURN ON ", HIGH, doNothing, noEvent)
       , VALUE(" TURN OFF ", LOW, doNothing, noEvent)
      );
TOGGLE(format, Format, "", doNothing, noEvent, noStyle //,doExit,enterEvent,noStyle
       , VALUE("                  AM", HIGH, doNothing, noEvent)
       , VALUE("                  PM", LOW, doNothing, noEvent)
      );
MENU(Slot1,         "         Slot1", showEvent, anyEvent, noStyle
     , FIELD(Slot1hr,     " OnHr    ->  ", "", 0, 24, 01, 0, saveonTIMEHRslot1, enterEvent, wrapStyle)
     , FIELD(Slot1min,    " OnMin   ->  ", "", 0, 59, 10, 1, saveonTIMEMINslot1, enterEvent, wrapStyle)
     , FIELD(Slot1hroff,  " OffHr   ->  ", "", 0, 24, 01, 0, saveonTIMEHRslot1off, enterEvent, wrapStyle)
     , FIELD(Slot1minoff, " OffMin  ->  ", "", 0, 59, 10, 1, saveonTIMEMINslot1off, enterEvent, wrapStyle)
     , SUBMENU(Format)
     , SUBMENU(zonE1)
     , SUBMENU(zonE2)
     , SUBMENU(zonE3)
     , OP(         "         <Save>", Readsl1, enterEvent)
     , SUBMENU(State)
     , EXIT("<Back")
    );
MENU(Slot2,         "         Slot2", showEvent, anyEvent, noStyle
     , FIELD(Ontimehr,   " OnHr     ->  ", "", 0, 24, 01, 0, saveonTIMEHR, enterEvent, wrapStyle)
     , FIELD(Ontimemin,  " OnMin    ->  ", "", 0, 59, 10, 1, saveonTIMEMIN, enterEvent, wrapStyle)
     , FIELD(offtimehr,  " OffHr    ->  ", "", 0, 24, 01, 0, saveoffTIMEHR, enterEvent, wrapStyle)
     , FIELD(offtimemin, " OffMin   ->  ", "", 0, 59, 10, 1, saveoffTIMEMIN, enterEvent, wrapStyle)
     , SUBMENU(Format)
     , SUBMENU(zonE1)
     , SUBMENU(zonE2)
     , SUBMENU(zonE3)
     , OP(          "        <Save>", Readd, enterEvent)
     , SUBMENU(State)
     , EXIT("<Back")
    );
MENU(Schedule,      "->SCHEDULE", showEvent, anyEvent, noStyle
     , SUBMENU(Slot1)
     , SUBMENU(Slot2)
     , EXIT("<Back")
    );

/*==============================================
  Settings Section
  ==============================================*/
MENU(SetTime,      "->SetTime", showEvent, anyEvent, noStyle
, OP(          "        <Save>", Readd, enterEvent)
     , EXIT("<Back")
    );

MENU(SETTINGS,      "->SETTINGS", showEvent, anyEvent, noStyle
     , SUBMENU(SetTime)
     , EXIT("<Back")
    );

/*==============================================
  Sensor Section
  ==============================================*/
MENU(SensorMenu, "->MINDERS", doNothing, noEvent, wrapStyle
     , OP(         "       SoilMinder", Soilminder, enterEvent)
     , OP(         "      WeatherMinder", WeatherMinder, enterEvent)
     , EXIT("<Exit")
    );

/*==============================================
  EEPROM Section
  ==============================================*/
result cropsetdate() {
  EEPROM.write(EepromCSD, CSDate);
  EEPROM.write(EepromCSM, CSMonth);
  EEPROM.write(EepromCSY, CSYear);
  return proceed;
}

result cropsetmonth() {
  EEPROM.write(EepromCSM, CSMonth);

  return proceed;
}
result cropsetyear() {
  EEPROM.write(EepromCSY, CSYear);

  return proceed;
}
result saveonTIMEHRslot1()
{
  EEPROM.write(Eepromhur1sl1, Slot1hr);
  return proceed;
}
result saveonTIMEMINslot1()
{
  EEPROM.write(Eeprommin1sl1, Slot1min);
  return proceed;
}
result saveonTIMEHRslot1off()
{
  EEPROM.write(Eepromhur1sl1off, Slot1hroff);
  return proceed;
}
result saveonTIMEMINslot1off()
{
  EEPROM.write(Eeprommin1sl1off, Slot1minoff);
  return proceed;
}

result saveonTIMEHR()
{
  EEPROM.write(Eepromhur1, Ontimehr);
  return proceed;
}
result saveonTIMEMIN()
{
  EEPROM.write(Eeprommin1, Ontimemin);
  return proceed;
}
result saveoffTIMEHR()
{
  EEPROM.write(Eepromhur2, offtimehr);
  return proceed;
}
result saveoffTIMEMIN()
{
  EEPROM.write(Eeprommin2, offtimemin);
  return proceed;
}
result Readd()
{
  Ontimehr = EEPROM.read(Eepromhur1);
  Ontimemin = EEPROM.read(Eeprommin1);
  offtimehr = EEPROM.read(Eepromhur2);
  offtimemin = EEPROM.read(Eeprommin2);
  return proceed;
}
result Readsl1()
{
  Slot1hr = EEPROM.read(Eepromhur1sl1);
  Slot1min = EEPROM.read(Eeprommin1sl1);
  Slot1hroff = EEPROM.read(Eepromhur1sl1off);
  Slot1minoff = EEPROM.read(Eeprommin1sl1off);
  return proceed;
}

/*==============================================
  MAIN-Menu Section
  ==============================================*/

MENU(mainMenu,     "          MENU", doNothing, noEvent, wrapStyle
     , SUBMENU(SensorMenu)
     , SUBMENU(Manual)
     , SUBMENU(Schedule)
     , SUBMENU(SETTINGS)
     , EXIT("<Exit")
    );


/*==============================================
  Declrations: Display Configuration
  ==============================================*/
#define MAX_DEPTH 2
serialIn serial(Serial);
#define fontX 3
#define fontY 10
#define MAX_DEPTH 10
MENU_OUTPUTS(out, MAX_DEPTH

             , U8G2_OUT(u8g2, colors, fontX, fontY, offsetX, offsetY, {0, 0, U8_Width / fontX, U8_Height / fontY})
             , SERIAL_OUT(Serial)
             , NONE
            );

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);
result alert(menuOut& o, idleEvent e) {
  if (e == idling) {
    o.setCursor(0, 0);
    o.print("alert test");
    o.setCursor(0, 1);
    o.print("press [select]");
    o.setCursor(0, 2);
    o.print("to continue...");
  }
  return proceed;
}
result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}
result Soilminder(eventMask e,  prompt& item)
{
  nav.idleOn(SoilInfo);
  return proceed;
}
result WeatherMinder(eventMask e,  prompt& item)
{
  nav.idleOn(SoilInfo1);
  return proceed;
}

/*==============================================
  Sensor Value display
  ==============================================*/

result SoilInfo(menuOut& o, idleEvent e)  {
  if (e == idling) {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(0, 0, 128, 53);
    u8g2.drawFrame(0, 0, 128, 43);
    u8g2.drawFrame(0, 0, 128, 33);
    u8g2.drawFrame(0, 0, 128, 23);
    u8g2.drawFrame(0, 0, 128, 13);
    u8g2.drawFrame(64, 12, 128, 64);
    u8g2.drawStr(20, 10, "FERTIGATION MINDER");
    u8g2.setCursor(3, 21);
    u8g2.print("SM :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 31);
    u8g2.print("N  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 41);
    u8g2.print("P  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 51);
    u8g2.print("K  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 21);
    u8g2.print("ST :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 31);
    u8g2.print("AT :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 41);
    u8g2.print("RH :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 51);
    u8g2.print("ET :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 61);
    u8g2.print("ALERTS:");
    u8g2.print("None");
    u8g2.setCursor(66, 61);
    u8g2.print("EVENTS:");
    u8g2.print("None");
  }
  return proceed;

}


result SoilInfo1(menuOut& o, idleEvent e)  {
  {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(0, 0, 128, 53);
    u8g2.drawFrame(0, 0, 128, 43);
    u8g2.drawFrame(0, 0, 128, 33);
    u8g2.drawFrame(0, 0, 128, 23);
    u8g2.drawFrame(0, 0, 128, 13);
    u8g2.drawFrame(64, 12, 128, 64);
    u8g2.drawStr(20, 10, "FERTIGATION MINDER");
    u8g2.setCursor(3, 21);
    u8g2.print("WD :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 31);
    u8g2.print("WS  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 41);
    u8g2.print("RP  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 51);
    u8g2.print("SR  :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 21);
    u8g2.print("DP :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 31);
    u8g2.print("BP :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 41);
    u8g2.print("HI :");
    u8g2.print(SMvalue);
    u8g2.setCursor(66, 51);
    u8g2.print("VP :");
    u8g2.print(SMvalue);
    u8g2.setCursor(3, 61);
    u8g2.print("ALERTS:");
    u8g2.print("None");
    u8g2.setCursor(66, 61);
    u8g2.print("EVENTS:");
    u8g2.print("None");

  }
}

result idle(menuOut& o, idleEvent e) {
  o.clear();
  switch (e) {
    case idleStart: o.println("suspending menu!"); break;
    case idling: o.println("suspended..."); break;
    case idleEnd: o.println("resuming menu."); break;
  }
  return proceed;
}
void setup() {

  Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    Serial.setDebugOutput(true);
  delay(2000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS file system");
    return;
  }
  startup();
  readsavedvalues();
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  while (!Serial);
  Serial.flush();
  EEPROM.begin(150);
  SPI.begin();
  u8g2.begin();
  pinMode(encBtn, INPUT_PULLUP);
  x = x + scroll_direction;
  if (x > 40) scroll_direction = -1;
  if (x < 1) scroll_direction = 1;
  u8g2.setFont(fontName);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  nav.idleTask = idle;
  Serial.println("setup done.");
  Serial.flush();
  pinMode(MainPumpA, OUTPUT);
  digitalWrite( MainPumpA, LOW);
  pinMode(FertSolenoid, OUTPUT);
  digitalWrite( FertSolenoid, LOW);
  pinMode(ZonE1, OUTPUT);
  digitalWrite( ZonE1, HIGH);
  pinMode(ZonE2, OUTPUT);
  digitalWrite( ZonE2, HIGH);
  Serial.println("FertigationMinder LORA");
  LoRa.setPins(ss, rst, dio0);
  //  if (!LoRa.begin(433E6)) {
  //    Serial.println("Starting LoRa failed!");
  //    while (1);
  //  }
}
/*==============================================
  ManualMode Relay Section
  ==============================================*/
void Manualrun()
{


  if (Pumpstate == HIGH)
  {
    pinMode(MainPumpA, OUTPUT);
    digitalWrite( MainPumpA, HIGH);
  }
  if (Pumpstate == LOW)
  {
    pinMode(MainPumpA, OUTPUT);
    digitalWrite( MainPumpA, LOW);
  }

  if ( Fertigationstate ==  HIGH)
  {
    pinMode(FertSolenoid, OUTPUT);
    digitalWrite( FertSolenoid, HIGH);
  }
  if ( Fertigationstate ==  LOW)
  {
    pinMode(FertSolenoid, OUTPUT);
    digitalWrite( FertSolenoid, LOW);
  }
  if (zonE11 == HIGH)
  {
    pinMode(ZonE1, OUTPUT);
    digitalWrite( ZonE1, LOW);
  }
  if (zonE11 == LOW)
  {
    pinMode(ZonE1, OUTPUT);
    digitalWrite( ZonE1, HIGH);
  }
  if (zonE22 == HIGH)
  {
    pinMode(ZonE2, OUTPUT);
    digitalWrite( ZonE2, LOW);
  }
  if (zonE22 == LOW)
  {
    pinMode(ZonE2, OUTPUT);
    digitalWrite( ZonE2, HIGH);
  }
}
void ScheduleMode()
{

}
void Encoderrun()
{
  x = x + scroll_direction;
  if (x > 40) scroll_direction = -1;
  if (x < 1) scroll_direction = 1;
  nav.doInput();
  if (nav.changed(1)) {
    u8g2.firstPage();
    do nav.doOutput(); while (u8g2.nextPage());
  }
  delay(700);//simulate other tasks delay

}
/*==============================================
  LOOP Section
  ==============================================*/
unsigned long Autoandmanual = 0;
void loop() {

  onReceive(LoRa.parsePacket());
  unsigned long currentMillis = millis();
  if (currentMillis - Autoandmanual >= 1000)
  {
    Manualrun();
    delay(100);
    Encoderrun();
    delay(100);
    //    String message = "10";
    //    sendMessage(message, SoilMinder, MonolineMaster);
    //    Serial.println(message);
    Autoandmanual = currentMillis;
  }
   if (wm_nonblocking) wm.process();
  checkButton();
}
