

/*updated topic name
  "version": "2.0",
  Updated


*/

#include <WiFiManager.h>

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CO2Sensor.h"
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include<EEPROM.h>
#include<Wire.h>
#include<WiFi.h>
#include"MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>



uint32_t chipId = 0;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
uint32_t chipid;
const char* ssid = "Zeptogreens";//Kumar_Xstream
const char* password =  "Zeptogreens@2023";//703664703664

const char* mqttServer = "192.168.0.183";//"192.168.1.183";//"192.168.1.102";
const int mqttPort = 1883;
const char* mqttUserName = "Zeptogreens";//"Zeptogreens";//"zepto";
const char* mqttPwd = "123456";//"123456";
const char* clientID = "username0001"; // client id username+0001

const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata"; //publish topic
//const char* sendtopic_Mesh ="account-name/PD1/W1/A1/Climate";
const char* sendtopic_CT = "Chinnamangalaram/GH1/WS/WeatherData";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
const char* Rectopic = "esp/test"; //publish topic

String timestamp;


WiFiClient espClient;
PubSubClient client(espClient);

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout

bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button
bool WMISBLOCKING    = false; // use blocking or non blocking mode

#define batteryvolt 34
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_16,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);

#define UVOUT 32
#define  REF_3V3 33

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int pagecount = 3;
int p;

//Scheduler r;
int uvLevel;
int refLevel;
int sensorvalue;
float voltage, outputvoltage;
int battry;
float t4, h4, p4, alti, dewpt, HI, uvIntensity;
void UVsensor();
int sl, co2, ET;
bool a;
float Tme, dn1, dn, num1, enum1, enum2, expon, pdenom, powercal, top1, top2, num2, num3, denom2, powercalc2, expnum1, expnum2, expcal;
float expnum3, expnum4, expcal2, ETcal, Es, RHcal, RHcal2, num5, num6, numcalc, fdenom1, fdenom2, fdenom3, fdenom4, fnum1, fnum2, fnum3, fnum4, fnum5, fnum6, fnum7, fnum8, fnum9, fnum10;

float ETo, ES_BP, ES_AT, ES_RH;
float Tmean;
float U2, P, Ea, delta, gama, ETmax, ETmin;
float Tmax, Tmin;
float h;//h=8.2;//height of the measurement above the ground surface
float T;//t=7.7;//air temp
float z = 840; //3.6;//elevation above sealevel//
//The elevation of the land surface varies from the low point of −418 m at the Dead Sea, to a maximum altitude of 8,848 m at the top of Mount Everest.
//The mean Height of land above sea level is 840 m.”

float Un = 2.4; ////measured wind speed z m above the ground surface
float RHmax, RHmin;
float Rn;//UV//=5.9;//net radiation flux
float G;//=HI//4.5;//sensible heat flux into the soil
// User stub
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void Display();
void dummy();
float VPDcal();
void page1();
void page2();
void page3();
void page4();
void ETcalcul();
void UVA();
void batterypercent();
int averageAnalogRead(int);

void (*pages[])() = { page1, page2, page3};
int duration [pagecount] = { 1000, 1000, 1000};

byte EaddrNode = 10;
byte EaddrNodetemp = 12;
byte EaddrNodehumi = 14;
byte EaddrNodepre = 16;
byte EaddrNodeHI = 18;
byte EaddrNodedewpt = 20;
byte EaddrNodesl = 22;
byte EaddrNodeco2 = 24;
byte EaddrNodeVPD = 26;
byte EaddrNodeET = 28;
int  Nco2, Nsl;
float Ntemp, Nhumi, Npre, Ndewpt, NVPD, NET, NHI;
float tem;
float RH, p1, p2;
float AirTemp;
float LeafTemp;
float VPD, outputVoltage;

void saveWifiCallback() {
  Serial.println("[CALLBACK] saveCallback fired");
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");
  // myWiFiManager->setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  // Serial.println(myWiFiManager->getConfigPortalSSID());
  //
  // esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_BW_HT20);
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  // wm.stopConfigPortal();
}

void bindServerCallback() {

  wm.server->on("/custom", handleRoute);
  // wm.server->on("/info",handleRoute); // you can override wm!
}

void handleRoute() {
  Serial.println("[HTTP] handle route");
  wm.server->send(200, "text/plain", "hello from user code");
}


void nodesave()
{
  //  EEPROM.write(EaddrNode4, nodenumber);
  EEPROM.write(EaddrNodetemp, ES_AT);
  EEPROM.write(EaddrNodehumi, ES_RH);
  EEPROM.write(EaddrNodepre, ES_BP);
  EEPROM.write(EaddrNodeHI, HI);
  EEPROM.write(EaddrNodedewpt, dewpt);
  EEPROM.write(EaddrNodesl, sl);
  EEPROM.write(EaddrNodeco2, co2);
  EEPROM.write(EaddrNodeVPD, VPD);
  EEPROM.write(EaddrNodeET, ETo);

  //N4 = EEPROM.read(EaddrNode4);
  Ntemp = EEPROM.read(EaddrNodetemp);
  Nhumi = EEPROM.read(EaddrNodehumi);
  Npre = EEPROM.read(EaddrNodepre);
  NHI = EEPROM.read(EaddrNodeHI);
  Ndewpt = EEPROM.read(EaddrNodedewpt);
  Nsl = EEPROM.read(EaddrNodesl);
  Nco2 = EEPROM.read(EaddrNodeco2);
  NVPD = EEPROM.read(EaddrNodeVPD);
  NET = EEPROM.read(EaddrNodeET);
}
void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
}

void ETcalcul()
{
  Tmax = ES_AT;
  Tmin = ES_AT - 0.3;
  Tme = Tmax + Tmin;

  Tmean = Tme / 2;
  h = ES_RH;
  dn1 = 67.8 * h;
  dn = dn1 - 5.42;
  num1 = 4.87 / dn;
  U2 = Un * num1;
  //U2=(Un*(4.87/(67.8*h-5.42)));

  enum1 = 17.27 * Tmean;
  enum2 = Tmean + 237.3;
  expon = exp(enum1 / enum2);
  pdenom = Tmean + 237.3;
  powercal = pow(pdenom, 2);
  top1 = 0.6108 * expon;
  top2 = top1 * 4098;
  delta = top2 / powercal;


  //delta=4098*0.6108*exp(17.27*Tmean/Tmean+237.3)/pow(Tmean+237.3,2);

  P = 101.3 * pow(((293 - 0.0065 * z) / 293), 5.26);
  num2 = 0.0065 * z;
  num3 = 293 - num2;
  denom2 = num3 / 293;
  powercalc2 = pow(denom2, 5.26);
  P = 101.3 * powercalc2;
  gama = (0.000665 * P);
  expnum1 = 17.27 * Tmax;
  expnum2 = Tmax + 237.3;
  expcal = exp(expnum1 / expnum2);
  ETmax = 0.6108 * expcal;
  //ETmax=0.6108*exp(17.27*Tmax/Tmax+237.3);

  expnum3 = 17.27 * Tmin;
  expnum4 = Tmin + 237.3;
  expcal2 = exp(expnum3 / expnum4);
  ETmin = 0.6108 * expcal2;
  //ETmin=0.6108*exp(17.27*Tmin/Tmin+237.3);

  ETcal = ETmax + ETmin;
  Es = ETcal / 2;
  //Es=(ETmax+ETmin)/2;


  RHmax = ES_RH;
  RHmin = ES_RH - 0.3;
  RHcal = RHmax / 100;
  RHcal2 = RHmin / 100;
  num5 = ETmin * RHcal;
  num6 = ETmax * RHcal2;
  numcalc = num5 + num6;
  Ea = numcalc / 2;
  //Ea=ETmin*(RHmax/100)+ETmax*(RHmin/100)/2;

  T = ES_AT;
  Rn = uvIntensity;
  G = HI;
  fdenom1 = 0.34 * U2;
  fdenom2 = 1 + fdenom1;
  fdenom3 = gama * fdenom2;
  fdenom4 = delta + fdenom3;
  fnum1 = 0.408 * delta;
  fnum2 = Rn - G;
  fnum3 = fnum1 * fnum2;
  fnum4 = Es - Ea;
  fnum5 = U2 * fnum4;
  fnum6 = T + 273;
  fnum7 = 900 / fnum6;
  fnum8 = fnum7 * fnum5;
  fnum9 = gama * fnum8;
  fnum10 = fnum3 + fnum9;
  ETo = fnum10 / fdenom4;
  //ETo=0.408*delta*(Rn-G)+gama*(900/T+273)*U2*(Es-Ea)/delta+gama*(1+0.34*U2);
}
void dummy()
{
  printBME280Data(&Serial);
  //delay(500);
}

float tempdec;
void printBME280Data( Stream* client)
{
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);


  ES_AT = temp;
  tempdec = (temp, 2);

  ES_RH = hum;

  ES_BP = pres;


  EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
  EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

  /// To get correct local altitude/height (QNE) the reference Pressure
  ///    should be taken from meteorologic messages (QNH or QFF)
  float altitude = EnvironmentCalculations::Altitude(pres, envAltUnit, referencePressure, outdoorTemp, envTempUnit);

  float dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);

  /// To get correct seaLevel pressure (QNH, QFF)
  ///    the altitude value should be independent on measured pressure.
  /// It is necessary to use fixed altitude point e.g. the altitude of barometer read in a map
  float seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(barometerAltitude, temp, pres, envAltUnit, envTempUnit);

  float absHum = EnvironmentCalculations::AbsoluteHumidity(temp, hum, envTempUnit);


  alti = altitude;

  dewpt = dewPoint;

  sl = (int)seaLevel;

  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);

  HI = heatIndex;

}

float VPDcal()
{

  AirTemp = ES_AT;
  LeafTemp = ES_AT;
  RH = ES_RH;
  p1 = pow ( 10 , ((float)(7.5 * AirTemp) / (float)(237.3 + AirTemp)));
  p2 = pow ( 10 , ((float)(7.5 * LeafTemp) / (float)(237.3 + LeafTemp)));
  VPD = (((float)610.7 * (float) p1) / 1000) - (((float)610.7 * (float)p2) / 1000) * (((float)RH) / 100);
  return VPD;
}

void page3()
{


  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0, 14);
  u8g2.print(chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");


  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);
    //   u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    //  u8g2.drawVLine(108,0,12);
    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    //  u8g2.drawVLine(108,0,12);
    //  u8g2.drawVLine(113,0,12);
    u8g2.sendBuffer();
  }


  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 35);
  u8g2.print("VPD");
  u8g2.setCursor(45, 35);
  u8g2.print(":");
  u8g2.setCursor(55, 35);
  u8g2.print(VPDcal(), 1);
  u8g2.print("");
  u8g2.print("kPa");
  u8g2.setCursor(0, 49);
  u8g2.print("ET");
  u8g2.setCursor(45, 49);
  u8g2.print(":");
  u8g2.setCursor(55, 49);
  u8g2.print(ETo, 1);
  u8g2.print("mm");
  u8g2.setCursor(0, 63);
  u8g2.print("SR");

  u8g2.setCursor(45, 63);
  u8g2.print(":");
  u8g2.setCursor(55, 63);
  u8g2.print(uvIntensity * 10, 1);


  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.print("nm");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
}





void page1()
{

  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0, 14);
  u8g2.print(chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }



  u8g2.setFont(u8g2_font_ncenB12_tr);

  u8g2.setCursor(0, 35);
  u8g2.print("AT");
  u8g2.setCursor(40, 35);
  u8g2.print(":");
  u8g2.setCursor(50, 35);
  u8g2.print(ES_AT, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 49);
  u8g2.print("RH");
  u8g2.setCursor(40, 49);
  u8g2.print(":");
  u8g2.setCursor(50, 49);
  u8g2.print(ES_RH, 1);
  u8g2.print("%");
  u8g2.setCursor(0, 63);
  u8g2.print("P");
  u8g2.setCursor(40, 63);
  u8g2.print(":");
  u8g2.setCursor(50, 63);
  u8g2.print(ES_BP, 1);
  u8g2.print("hPa");
  // u8g2.print("°C");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
}

void page2()
{

  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0, 14);
  u8g2.print(chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);

  u8g2.setCursor(0, 35);
  u8g2.print("HI");
  u8g2.setCursor(45, 35);
  u8g2.print(":");
  u8g2.setCursor(55, 35);
  u8g2.print(HI, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 49);
  u8g2.print("Dep");
  u8g2.setCursor(45, 49);
  u8g2.print(":");
  u8g2.setCursor(55, 49);
  u8g2.print(dewpt, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 63);
  u8g2.print("Sl");
  u8g2.setCursor(45, 63);
  u8g2.print(":");
  u8g2.setCursor(55, 63);
  u8g2.print(sl, 1);
  u8g2.print("hPa");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
}




int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  float refanalog = analogRead(pinToRead);
  float DCvalue = mapFloat( refanalog, 0, 1023, 0, 5);

  runningValue /= numberOfReadings;
  return (runningValue);
}


void Display()
{

  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  batterypercent();
  dummy();
  UVA();
  ETcalcul();

  page1();

  page2();

  page3();


}

void UVA() {

  uvLevel = averageAnalogRead(UVOUT);
  refLevel = averageAnalogRead(REF_3V3);
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  outputvoltage = 3.3 / refLevel * uvLevel;
  //float dcvoltage=mapFloat(refLevel,0,1023,0,5);
  uvIntensity = mapFloat( outputvoltage, 0.0, 31.0, 0.0, 99.0) * 10; //Convert the voltage to a UV intensity level
  //float uvIntensity = mapFloat(outputVoltage,0.99, 2.8, 0.0, 15.0)* 10;
  //uvIntensitynm =uvIntensity*10;


}




void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();
  //  Serial.print("Epoch Time: ");
  //  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  //  Serial.print("Formatted Time: ");
  //  Serial.println(formattedTime);
  uint32_t ES_did = chipId;

  unsigned long currentMillis = millis(); //read current time



  DynamicJsonDocument doc(512);
  DynamicJsonDocument Kura_doc(512);
  JsonObject Ws = doc.createNestedObject("Ws");
  // doc["ts"] = formattedTime;

  int W_AT = (int)(ES_AT * 10); // AT_value;
  Kura_doc["AT"] = (float)(W_AT) / 10;
  int W_RH = (int)(ES_RH * 10);
  Kura_doc["RH"] = (float)(W_RH) / 10; // RH_value;
  int W_VPD = (int)(VPDcal() * 10);
  Kura_doc["VPD"]  = (float)(W_VPD) / 10; // VPD_value;
  int W_ET = (int)(ETo * 10);
  Kura_doc["ET"] = (float)(W_ET) / 10; //ET_value;
  int W_BP = (int)(ES_BP * 10);
  Kura_doc["BP"] = (float)(W_BP) / 10; //BP_value;
  Kura_doc["SR"] = uvIntensity * 10; //SR_value;
  int W_dewpt = (int)(dewpt * 10);
  Kura_doc["DP"] = (float)(W_dewpt) / 10;
  int W_HI = (int)(HI * 10);
  Kura_doc["HI"]  = (float)(W_HI) / 10; //HI_value;
  Kura_doc["SL"]  = sl;//SL_value;
  Kura_doc["WSclk"] = 0;
  Kura_doc["WSaclk"] = 0;
  Kura_doc["WDclk"] = 0;
  Kura_doc["WDaclk"] = 0;
  Kura_doc["ntips"] = 0;

  Ws["did"] = chipId;
  Ws["ts"] = formattedTime;
  Ws["AT_v"] = (float)(W_AT) / 10;
  Ws["RH_v"] = (float)(W_RH) / 10;
  Ws["VPD_v"] = (float)(W_VPD) / 10;
  Ws["HI_v"] = (float)(W_HI) / 10;
  Ws["ET_v"] = (float)(W_ET) / 10;
  Ws["BP_v"] = (float)(W_BP) / 10;
  Ws["SR_v"] = uvIntensity * 10;
  Ws["DP_v"] = (float)(W_dewpt) / 10;
  Ws["SL_v"] = sl;
  Ws["WSclk"] = 0;
  Ws["WSaclk"] = 0;
  Ws["WDclk"] = 0;
  Ws["WDaclk"] = 0;
  Ws["ntips"] = 0;

  char buffer[1024];
  char tempString[1024];
  serializeJson(doc, buffer);
  DynamicJsonDocument doc2(1024);
  doc2["metrics"] = Kura_doc;
  serializeJson(doc2, tempString);
  client.publish(sendtopic_CT, tempString); //
  client.publish(sendtopic_Mesh, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(buffer, Serial);
  Serial.println();
  Serial.println("-----------------------");
  memset(tempString, 0, 1024);
  serializeJson(Kura_doc, Serial);
  Serial.println();
  Serial.println("-----------------------");


}
void wifiInfo() {
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}

void reconnect() {
  while (!client.connected()) {
    // ESP.restart();
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe("api/command");
      //      Serial.println("Topic Subscribed");
      a = true;
      //      sendDataMQTT();

    }
    else {
      a = false;
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      ESP.restart();

    }

  }
  // delay(2000);
}

void setup() {

  Serial.begin(115200);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  EEPROM.begin(512);
  u8g2.begin();

  Wire.begin();
  pinMode(34, INPUT);
//  while (!bme.begin())
//  {
//    Serial.println("Could not find BME280 sensor!");
//    // delay(1000);
//  }
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //  wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("Axalyn Sensor Wi-Fi")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  wifiInfo();
//  if (WiFi.status() == WL_CONNECTED) {
//    // put your setup code here, to run once:
//    switch (bme.chipModel())
//    {
//      case BME280::ChipModel_BME280:
//        //      Serial.println("Found BME280 sensor! Success.");
//        break;
//      case BME280::ChipModel_BMP280:
//        //      Serial.println("Found BMP280 sensor! No Humidity available.");
//        break;
//      default:
//        Serial.println("Found UNKNOWN sensor! Error!");
//    }
//    // Initialize a NTPClient to get time
    timeClient.begin();
    timeClient.setTimeOffset(19800);
    //    getTime();
    client.setServer(mqttServer, 1883); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
//  }
//  else {
//    Serial.println("No Wifi");
//
//  }

}
unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
void loop() {

  timeClient.update();
  if (!WMISBLOCKING) {
    wm.process();
  }

  unsigned long now1 = millis();
  if (now1 - lastMsg > 3000) {
    lastMsg = now1;
    if (!client.connected()) {
      reconnect();
    }
  }
  unsigned long nowon = millis();
  if (nowon - lastMsg01 > 1000) {
    lastMsg01 = nowon;
    Display();
  }


  // the interval at which you want .
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    Serial.println("Sending Data");
    if (client.connected()) {
      sendDataMQTT();
    }

  }
  client.loop();
  // put your main code here, to run repeatedly:
  delay(100);
}
