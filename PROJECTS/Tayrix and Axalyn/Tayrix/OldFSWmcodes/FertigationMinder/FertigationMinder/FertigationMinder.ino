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
#include <TimerOne.h>
#include <EEPROM.h>
#include <SPI.h>
/*==============================================
  Declrations
  ==============================================*/
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
int Eepromhur1 = 206;
int Eeprommin1 = 209;
int Eepromhur2 = 212;
int Eeprommin2 = 215;
bool Pumpstate;
bool Fertigationstate;
bool zone1;
bool zone2;
bool zone3;
bool zonE11;
bool zonE22;
bool zonE33;

#define MainPumpA  7 
#define FertSolenoid  8
#define ZonE1 9
#define ZonE2 10

bool AutoModeBool = false ;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = ontime;
boolean relayState = false;
/*==============================================
  Declrations:Rotary Encoder pins
  ==============================================*/
#define encA    33
#define encB    31
#define encBtn  35
//********************************** ENCODER

ClickEncoder clickEncoder(encA, encB, encBtn, 2);
ClickEncoderStream encStream(clickEncoder, 1);
MENU_INPUTS(in, &encStream);
void timerIsr() {
  clickEncoder.service();
}
/*==============================================
  Declrations: Display Configuration
  ==============================================*/
#define USE_SWSPI
#define U8_DC 17
#define U8_CS 16
#define U8_RST 23
#define fontName  u8g2_font_5x7_tf
#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64
int x = 0;
int scroll_direction = 1;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 23 /* A4 */ , /* data=*/ 17 /* A2 */, /* CS=*/ 16 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
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


MENU(Manual, "ManualMode", showEvent, anyEvent, noStyle
     , SUBMENU(MainPump)
     , SUBMENU(Fertigation)
     , SUBMENU(zonE1)
     , SUBMENU(zonE2)
     , SUBMENU(zonE3)
     , EXIT("<Back")
    );
//*****************Sensor-Section//*****************
MENU(Sensor, "Sensor-Menu", showEvent, noEvent, noStyle
     , OP("---SoilMinder---", SoilMinder, enterEvent)
     , OP("---WeatherMinder---", WeatherMinder, enterEvent)
     , OP("---FoliageMinder---", FoliageMinder, enterEvent)
     , EXIT("<Back")
    );
//*****************Auto-Section//*****************
MENU(Auto, "AutoMode", showEvent, noEvent, wrapStyle
     , FIELD(Ontimehr,  "OntimeHr   ->   ", "", 0, 23, 01, 0, saveonTIMEHR, enterEvent, wrapStyle)
     , FIELD(Ontimemin, "Ontimemin  ->   ", "", 0, 59, 10, 1, saveonTIMEMIN, enterEvent, wrapStyle)
     , FIELD(offtimehr, "OfftimeHr  ->   ", "", 0, 23, 01, 0, saveoffTIMEHR, enterEvent, wrapStyle)
     , FIELD(offtimemin, "Offtimemin ->   ", "", 0, 59, 10, 1, saveoffTIMEMIN, enterEvent, wrapStyle)
     , SUBMENU(Zone1)
     , SUBMENU(Zone2)
     , SUBMENU(Zone3)
     , OP("           Save", Readd, enterEvent)
     , OP("         TURN ON", AutoMode, enterEvent)
     , OP("         TURN OFF", AutoModeOFF, enterEvent)
     , EXIT("<Back")
    );
/*==============================================
  EEPROM Section
  ==============================================*/
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
/*==============================================
  MAIN-Menu Section
  ==============================================*/
MENU(mainMenu, "MAINMENU", doNothing, noEvent, wrapStyle
     , SUBMENU(Sensor)
     , SUBMENU(Manual)
     , SUBMENU(Auto)
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
result SoilMinder(eventMask e, prompt &item) {
  nav.idleOn(SoilInfo);
  return proceed;
}
result WeatherMinder(eventMask e, prompt &item) {
  nav.idleOn(WeatherInfo);
  return proceed;
}
result FoliageMinder(eventMask e, prompt &item) {
  nav.idleOn(FoliageInfo);
  return proceed;
}
/*==============================================
  Sensor Value display
  ==============================================*/
result FoliageInfo(eventMask e, navNode& nav, prompt &item) {
  {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(5, 5, 118, 54);
    u8g2.drawStr(9, 15, "leafWetness:");
    u8g2.drawStr(80, 15, "98");
    u8g2.drawStr(9, 25, "leaftemp:");
    u8g2.drawStr(80, 25, "25");
    u8g2.drawStr(9, 35, "health:");
    u8g2.drawStr(80, 35, "38");
    u8g2.drawStr(9, 45, "vpd:");
    u8g2.drawStr(80, 45, "43");
    u8g2.setCursor(40, 51);

  }
}
result SoilInfo(eventMask e, navNode& nav, prompt &item) {
  {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(5, 5, 118, 54);
    u8g2.drawStr(9, 15, "SoilMoisture:");
    u8g2.drawStr(80, 15, "98");
    u8g2.drawStr(9, 25, "Nitrogen:");
    u8g2.drawStr(80, 25, "25");
    u8g2.drawStr(9, 35, "Phosphorus:");
    u8g2.drawStr(80, 35, "38");
    u8g2.drawStr(9, 45, "Potassium:");
    u8g2.drawStr(80, 45, "43");
    u8g2.setCursor(40, 51);

  }
}
result WeatherInfo(eventMask e, navNode& nav, prompt &item) {
  {
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.drawFrame(5, 5, 118, 54);
    u8g2.drawStr(9, 15, "AT:");
    u8g2.drawStr(80, 15, "32");
    u8g2.drawStr(9, 25, "VPD:");
    u8g2.drawStr(80, 25, "3.5");
    u8g2.drawStr(9, 35, "Humidity:");
    u8g2.drawStr(80, 35, "25");
    u8g2.drawStr(9, 45, "ET:");
    u8g2.drawStr(80, 45, "6.8");
    u8g2.setCursor(40, 51);

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
  while (!Serial);
  Serial.flush();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  SPI.begin();
  u8g2.begin();
  pinMode(encBtn, INPUT_PULLUP);
  x = x + scroll_direction;
  if (x > 40) scroll_direction = -1;
  if (x < 1) scroll_direction = 1;
  u8g2.setFont(fontName);
  nav.idleTask = idle; //point a function to be used when menu is suspended
  Serial.println("setup done."); Serial.flush();
  pinMode(MainPumpA, OUTPUT);
  digitalWrite( MainPumpA, HIGH);
  pinMode(FertSolenoid, OUTPUT);
  digitalWrite( FertSolenoid, HIGH);
  pinMode(ZonE1, OUTPUT);
  digitalWrite( ZonE1, HIGH);
  pinMode(ZonE2, OUTPUT);
  digitalWrite( ZonE2, HIGH);
}
/*==============================================
  ManualMode Relay Section
  ==============================================*/
void Manualrun()
{
  if (AutoModeBool == false)
  {

    if (Pumpstate == HIGH)
    {
      pinMode(MainPumpA, OUTPUT);
      digitalWrite( MainPumpA, LOW);
    }
    if (Pumpstate == LOW)
    {
      pinMode(MainPumpA, OUTPUT);
      digitalWrite( MainPumpA, HIGH);
    }

    if ( Fertigationstate ==  HIGH)
    {
      pinMode(FertSolenoid, OUTPUT);
      digitalWrite( FertSolenoid, LOW);
    }
    if ( Fertigationstate ==  LOW)
    {
      pinMode(FertSolenoid, OUTPUT);
      digitalWrite( FertSolenoid, HIGH);
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
}
/*==============================================
  AutoMode Section
  ==============================================*/
void AutoMode()
{
  AutoModeBool = true;
  Readd();
  long Z1PASThrconvOn = Ontimehr * ms_per_hour;
  long Z1PASTminconvOn = Ontimemin * ms_per_min;
  long PumpAOnTimetotelOn = Z1PASThrconvOn + Z1PASTminconvOn;
  long Z1PASThrconvoff = offtimehr * ms_per_hour;
  long Z1PASTminconvoff = offtimemin * ms_per_min;
  long PumpAOnTimetoteloff = Z1PASThrconvoff + Z1PASTminconvoff;
  ontime = PumpAOnTimetotelOn;
  offtime = PumpAOnTimetoteloff;
  Serial.println(ontime);
  Serial.println(offtime);
}
void AutoModeOFF()
{
  AutoModeBool = false;
  pinMode(MainPumpA, OUTPUT);
  digitalWrite(FertSolenoid, HIGH);
  pinMode(FertSolenoid, OUTPUT);
  digitalWrite(MainPumpA, HIGH);
}
void Autorun()
{
  if (AutoModeBool == true)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      relayState = !relayState;
      if (relayState) {
        Serial.println("onRELAY");
        pinMode(MainPumpA, OUTPUT);
        digitalWrite(FertSolenoid, LOW);
        pinMode(FertSolenoid, OUTPUT);
        digitalWrite(MainPumpA, LOW);
        interval = ontime;
      } else {
        Serial.println("offRELAY");
        pinMode(MainPumpA, OUTPUT);
        digitalWrite(FertSolenoid, HIGH);
        pinMode(FertSolenoid, OUTPUT);
        digitalWrite(MainPumpA, HIGH);
        interval = offtime;
      }
    }
    if (zone1 == HIGH)
    {
      pinMode(ZonE1, OUTPUT);
      digitalWrite( ZonE1, LOW);
    }
    else if (zone1 == LOW)
    {
      pinMode(ZonE1, OUTPUT);
      digitalWrite( ZonE1, HIGH);

    }
    if (zone2 == HIGH)
    {
      pinMode(ZonE2, OUTPUT);
      digitalWrite( ZonE2, LOW);
    }
    else if (zone2 == LOW)
    {
      pinMode(ZonE2, OUTPUT);
      digitalWrite( ZonE2, HIGH);
    }
    if (zone3 == HIGH)
    {
      Serial.println("Zone3 Solenoid ON");
    }
    else if (zone3 == LOW)
    {
      Serial.println("Zone3 Solenoid OFF");

    }
  }
}
/*==============================================
  LOOP Section
  ==============================================*/
unsigned long Autoandmanual = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - Autoandmanual >= 1000)
  {
    Manualrun();
    Autorun();

    Autoandmanual = currentMillis;
  }

  x = x + scroll_direction;
  if (x > 40) scroll_direction = -1;
  if (x < 1) scroll_direction = 1;
  nav.doInput();
  if (nav.changed(1)) {
    u8g2.firstPage();
    do nav.doOutput(); while (u8g2.nextPage());
  }
  delay(500);//simulate other tasks delay
}
