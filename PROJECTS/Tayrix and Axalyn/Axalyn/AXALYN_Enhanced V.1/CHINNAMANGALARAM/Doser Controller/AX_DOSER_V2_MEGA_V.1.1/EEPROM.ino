result RefVRead() {
  phRefV = EEPROM.read(EepromPhRV);
  EcRefV = EEPROM.read(EepromECRV);
  return proceed;
}

result phsetsave() {
  EEPROM.writeFloat(EeprompHSetpoint, pHSetpoint);  // 6.5 pH for tomato growth

  return proceed;
}

result phsetHyssave() {

  EEPROM.writeFloat(EepromSetpHHysteris, phSetHysteris);
  return proceed;
}

result ECsetsave() {
  EEPROM.writeFloat(EepromECSetpoint, ECsetvalue);

  return proceed;
}

result ECsethyssave() {

  EEPROM.writeFloat(EepromSetECHysteris, ECSetHysteris);
  return proceed;
}

result SPFTRead() {
  FDate = EEPROM.read(EepromSPFD);
  TDate = EEPROM.read(EepromSPFM);
  FMonth = EEPROM.read(EepromSPTD);
  TMonth = EEPROM.read(EepromSPTM);
  pHsp = EEPROM.readFloat(EeprompHsp);
  ECsp = EEPROM.readFloat(EepromECsp);
  tempsp = EEPROM.readFloat(Eepromtempsp);
  dosp = EEPROM.readFloat(Eepromdosp);
  return proceed;
}

//**********************SCHEDULER ALARM*******************

result Shift1Clock() {
  S1Hr = sendhour();
  S1Min = sendmin();
  EEPROM.write(Eepromhur1, S1Hr);
  EEPROM.write(Eeprommin1, S1Min);
  return proceed;
}

result Shift2Clock() {
  S2Hr = sendhour();
  S2Min = sendmin();
  EEPROM.write(Eepromhur2, S2Hr);
  EEPROM.write(Eeprommin2, S2Min);
  return proceed;
}

//=================================================
//**********************SCHEDULER ALARM*******************
result Shift3Clock() {
  S3Hr = sendhour();
  S3Min = sendmin();
  EEPROM.write(Eepromhur3, S3Hr);
  EEPROM.write (Eeprommin3, S3Min);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift4Clock() {
  S4Hr = sendhour();
  S4Min = sendmin();
  EEPROM.write(Eepromhur4, S4Hr);
  EEPROM.write (Eeprommin4, S4Min);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift5Clock() {
  S5Hr = sendhour();
  S5Min = sendmin();
  EEPROM.write(Eepromhur5, S5Hr);
  EEPROM.write(Eeprommin5, S5Min);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift6Clock() {
  S6Hr = sendhour();
  S6Min = sendmin();
  EEPROM.write(Eepromhur6, S6Hr);
  EEPROM.write(Eeprommin6, S6Min);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift7Clock() {
  S7Hr = sendhour();
  S7Min = sendmin();
  EEPROM.write(Eepromhur7, S7Hr);
  EEPROM.write(Eeprommin7, S7Min);
  return proceed;
}

//**********************SCHEDULER ALARM*******************
result Shift8Clock() {
  S8Hr = sendhour();
  S8Min = sendmin();
  EEPROM.write(Eepromhur8, S8Hr);
  EEPROM.write(Eeprommin8, S8Min);
  return proceed;
}

//*********************************************************************
void allReadTime() {
  EepromReadTime1();
  EepromReadTime2();
  EepromReadTime3();
  EepromReadTime4();
  EepromReadTime5();
  EepromReadTime6();
  EepromReadTime7();
  EepromReadTime8();
}

void EepromReadEC()
{
  ECsetvalue = EEPROM.readFloat(EepromECSetpoint);
  ECSetHysteris = EEPROM.readFloat(EepromSetECHysteris);
}

void EepromReadPHCal()
{
  pHSetpoint = EEPROM.readFloat(EeprompHSetpoint);
  phSetHysteris = EEPROM.readFloat(EepromSetpHHysteris);
}

int Eepromautostatus = 415;

void EepromReadauto()
{
  autoon = EEPROM.read(Eepromautostatus);
  // Serial.print ("on");
  //  Serial.println (autoon);
  return proceed;
}

byte EepromSchetatus = 417;

void EepromReadSche()
{
  Scheon = EEPROM.read(EepromSchetatus);
  // Serial.print ("Sche--on");
  //  Serial.println (Scheon);
  return proceed;
}

void Schesavefun() {
  if (Scheon == 1) {
    ScheduleAuto = true;
  }
  else {
    ScheduleAuto = false;
  }
}

/********************************************************************************************    ***********************************************************/

int totalVolume1;
int EepromNTHset = 300;
int EepromNTVSet = 310;

///*********EPROM********************///
void NTHset() {
  EEPROM.write(EepromNTHset, tankHeight);
}

void NTVset() {
  EEPROM.write(EepromNTVSet, totalVolume1);
}

int EepromTSLSet = 350;

void TSLset() {
  EEPROM.write(EepromTSLSet, tanksetpoint);
}

void EepromReadNT()
{
  tankHeight = EEPROM.read(EepromNTHset);
  totalVolume1 = EEPROM.read(EepromNTVSet);
  tanksetpoint = EEPROM.read(EepromTSLSet);
}

void EepromReadTime1()
{
  S1Hr = EEPROM.read(Eepromhur1);
  S1Min = EEPROM.read(Eeprommin1);
  return proceed;
}

void EepromReadTime2()
{
  S2Hr = EEPROM.read(Eepromhur2);
  S2Min = EEPROM.read(Eeprommin2);
  return proceed;
}

void EepromReadTime3()
{
  S3Hr = EEPROM.read(Eepromhur3);
  S3Min = EEPROM.read(Eeprommin3);
  return proceed;
}

void EepromReadTime4()
{
  S4Hr = EEPROM.read(Eepromhur4);
  S4Min = EEPROM.read(Eeprommin4);
  return proceed;
}

void EepromReadTime5()
{
  S5Hr = EEPROM.read(Eepromhur5);
  S5Min = EEPROM.read(Eeprommin5);
  return proceed;
}

void EepromReadTime6()
{
  S6Hr = EEPROM.read(Eepromhur6);
  S6Min = EEPROM.read(Eeprommin6);
  return proceed;
}

void EepromReadTime7()
{
  S7Hr = EEPROM.read(Eepromhur7);
  S7Min = EEPROM.read(Eeprommin7);
  return proceed;
}

void EepromReadTime8()
{
  S8Hr = EEPROM.read(Eepromhur8);
  S8Min = EEPROM.read(Eeprommin8);
  return proceed;
}

void Readall()
{
  pHs1 = EEPROM.readFloat(EeprompHs1);
  ECs1 = EEPROM.readFloat(EepromECs1);
  pHs2 = EEPROM.readFloat(EeprompHs2);
  ECs2 = EEPROM.readFloat(EepromECs2);
  pHs3 = EEPROM.readFloat(EeprompHs3);
  ECs3 = EEPROM.readFloat(EepromECs3);
  pHs4 = EEPROM.readFloat(EeprompHs4);
  ECs4 = EEPROM.readFloat(EepromECs4);
  pHs5 = EEPROM.readFloat(EeprompHs5);
  ECs5 = EEPROM.readFloat(EepromECs5);
  pHs6 = EEPROM.readFloat(EeprompHs6);
  ECs6 = EEPROM.readFloat(EepromECs6);
  pHs7 = EEPROM.readFloat(EeprompHs7);
  ECs7 = EEPROM.readFloat(EepromECs7);
  pHs8 = EEPROM.readFloat(EeprompHs8);
  ECs8 = EEPROM.readFloat(EepromECs8);
}

///***************Read all data from memoery******************************
void readalldata() {
  EepromReadEC();
  delay(1000);
  EepromReadauto();
  delay(1000);
  EepromReadSche();
  delay(1000);
  EepromReadPHCal();
  delay(1000);
  Readall();
  delay(1000);
  SPFTRead();
  delay(1000);
  RefVRead();
  delay(1000);
  Schesavefun();
  delay(1000);
  allReadTime();
  delay(1000);
}
