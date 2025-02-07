#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <AsyncTCP.h>
#include<EEPROM.h>
#include<Wire.h>
#include<WiFi.h>
#include"MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* ssid = "TayrixPrecision";
const char* password = "Tayrix@123";
const char* mqttServer = "192.168.100.5"; 
const int mqttPort = 1883;
const char* mqttUserName = "svaasafarm"; 
const char* mqttPwd = "svaasafarmmqtt";
const char* clientID = "username0001"; 
String timestamp; 
WiFiClient espClient;
PubSubClient client(espClient);
//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;
const char* sendtopic_Node = "WHSNode";
float referencePressure = 1018.6;  
float outdoorTemp = 4.7;           
float barometerAltitude = 1650.3; 
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
#define UVOUT 35
#define  REF_3V3 34
int p;
int chipId;
//Scheduler r;
int uvLevel;
int refLevel;
float speed_mph;
int direction;
float voltage, outputvoltage;
float t4, h4, p4, alti, dewpt, HI, uvIntensity;
void UVsensor();
int sl, ET;
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
String getReadings(); // Prototype for sending sensor readings
void dummy();
float VPDcal();
void ETcalcul();
void UVA();
int averageAnalogRead(int);
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
  Serial.print("ES_RH");
Serial.println(ES_RH);
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
void nodesave()
{
  //  EEPROM.write(EaddrNode4, nodenumber);
  EEPROM.write(EaddrNodetemp, ES_AT);
  EEPROM.write(EaddrNodehumi, ES_RH);
  EEPROM.write(EaddrNodepre, ES_BP);
  EEPROM.write(EaddrNodeHI, HI);
  EEPROM.write(EaddrNodedewpt, dewpt);
  EEPROM.write(EaddrNodesl, sl);
 
  EEPROM.write(EaddrNodeVPD, VPD);
  EEPROM.write(EaddrNodeET, ETo);

  //N4 = EEPROM.read(EaddrNode4);
  Ntemp = EEPROM.read(EaddrNodetemp);
  Nhumi = EEPROM.read(EaddrNodehumi);
  Npre = EEPROM.read(EaddrNodepre);
  NHI = EEPROM.read(EaddrNodeHI);
  Ndewpt = EEPROM.read(EaddrNodedewpt);
  Nsl = EEPROM.read(EaddrNodesl);
 
  NVPD = EEPROM.read(EaddrNodeVPD);
  NET = EEPROM.read(EaddrNodeET);
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
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void speedcal() {

  int sensorpin = analogRead(32);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorpin * (5.0 / 4095.0);
  // print out the value you read:
  Serial.println(voltage);

  float wind_speed = mapfloat(voltage, 0.0, 2, 0, 32.4);
  speed_mph = ((wind_speed * 3600) / 1609.344);
  Serial.print("Wind Speed =");
  Serial.print(wind_speed);
  Serial.println("m/s");
  Serial.print(speed_mph);
  Serial.println("mph");
  delay(3000);
}
void directioncal() 
{

  int sensorValue = analogRead(33);
  float volt = sensorValue * 10 / 4095;
  direction = map(sensorValue, 0, 4095, 0, 360);
  // print out the value you read:
  Serial.print("ADC : ");
  Serial.println(sensorValue);
  Serial.print("Voltage : ");
  Serial.println(volt);
  Serial.print("Direction : ");
  Serial.println(direction);
  delay(300);

  if (direction < 11.25)
    Serial.println("N");
  else if (direction < 33.75)
    Serial.println("NNE");
  else if (direction < 56.25)
    Serial.println("NE");
  else if (direction < 78.75)
    Serial.println("ENE");
  else if (direction < 101.25)
    Serial.println("E");
  else if (direction < 123.25)
    Serial.println("ESE");
  else if (direction < 146)
    Serial.println("SE");
  else if (direction < 168.75)
    Serial.println("SSE");
  else if (direction < 191.25)
    Serial.println("S");
  else if (direction < 213.75)
    Serial.println("SSW");
  else if (direction < 236.75)
    Serial.println("SW");
  else if (direction < 258.75)
    Serial.println("WSW");
  else if (direction < 281)
    Serial.println("W");
  else if (direction < 303.75)
    Serial.println("WNW");
  else if (direction < 326)
    Serial.println("NW");
  else if (direction < 348.75)
    Serial.println("NNW");
  else
    Serial.println("N");

}

void Display()
{
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  dummy();
  UVA();
  ETcalcul();
  speedcal();
directioncal();
 
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

  String formattedTime = timeClient.getFormattedTime();

  uint32_t ES_did = chipId;

  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["did"] = ES_did;
  TXCC["ts"] = formattedTime;
  JsonObject TXCC_ES_rtd = TXCC.createNestedObject("ES_rtd");
  int E_AT = (int)(ES_AT * 10);
  TXCC_ES_rtd["AT_v"] = (float)(E_AT) / 10;
  int E_RH = (int)(ES_RH * 10);
  TXCC_ES_rtd["RH_v"] = (float)(E_RH) / 10;
  int E_VPD = (int)(VPDcal() * 10);
  TXCC_ES_rtd["VPD_v"] = (float)(E_VPD) / 10;
  int E_HI = (int)(HI * 10);
  TXCC_ES_rtd["HI_v"] = (float)(E_HI) / 10;
  int E_ETo = (int)(ETo * 10);
  TXCC_ES_rtd["ET_v"] = (float)(E_ETo) / 10;
  int E_BP = (int)(ES_BP * 10);
  TXCC_ES_rtd["BP_v"] = (float)(E_BP) / 10;
  TXCC_ES_rtd["SR_v"] = uvIntensity * 10;
  int E_DP = (int)(dewpt * 10);
  TXCC_ES_rtd["DP_v"] = (float)(E_DP) / 10;
  TXCC_ES_rtd["SL_v"] = sl;
  TXCC_ES_rtd["WS"] = speed_mph;
  TXCC_ES_rtd["WD"] = direction;
  
     char buffer[1024];
  serializeJson(doc, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");
}



void reconnect() {
  while (!client.connected()) {
    
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
    
      a = true;
     

    }
    else {
      a = false;

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      ESP.restart();

    }

  }
}



void setup() {

  Serial.begin(115200);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  EEPROM.begin(512);
  Wire.begin();
  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
   
  }
   WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    yield();
  }
  Serial.println("Connected to the WiFi network");

 
  
    timeClient.begin();
    timeClient.setTimeOffset(19800);
   
    client.setServer(mqttServer, 1883); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
  


}
unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
void loop() {
  
    timeClient.update();

    if (!client.connected()) {
      reconnect();
    }
  
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      if (client.connected()) {
        Display();
        delay(100);
        sendDataMQTT();
      }
    }
    client.loop();
    
    
  delay(2000); // wait 2s for next reading
}
