

//void (*pages[])() = { page1, page2, page3, page4};
//int duration [pagecount] = { 1000, 1000, 1000, 1000};

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

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");

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
  u8g2.print("Co2");
  u8g2.setCursor(45, 35);
  u8g2.print(":");
  u8g2.setCursor(55, 35);
  u8g2.print(co2);
  u8g2.print("ppm");
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

void page4()
{
  // delay(1000);
  //u8g2.clearBuffer();
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

    u8g2.sendBuffer();
  }



  u8g2.setFont(u8g2_font_ncenB12_tr);

  u8g2.setCursor(0, 35);
  u8g2.print("VPD");
  u8g2.setCursor(40, 35);
  u8g2.print(":");
  u8g2.setCursor(50, 35);
  u8g2.print(VPDcal(), 1);
  u8g2.print("");
  u8g2.print("kPa");
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
  co2 = 00;
  page1();

  page2();

  page3();

  page4();

 
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
  char buffer[1024];
  serializeJson(doc, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");
}
