#include"config.h"

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);
  rtc.begin();
  rtc.adjust(DateTime(2022, 9, 17, 11, 15, 0));
  Pin_mode();
  Serial.println("WELCOME TO IRRIGATION CONTROLLER");
}

void loop() 
{
  ////////////************** Serialcom loop Mode***********///////////
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) 
  {
    SCpreviousMillis = currentMillis;

    SerialCom();
  }
  ////////////**************Schedule loop Mode***********///////////
  if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {
    unsigned long sch1currentMillis = millis();

    if (sch1currentMillis - sch1previousMillis >= 10000) 
    {
      sch1previousMillis = sch1currentMillis;
      scheduleFun();
    }
  }
  //////////************** Irrevent loop Mode***********///////////
  if (((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true)) || ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true)))
  {
    unsigned long EVNTcurrentMillis = millis();
    if (EVNTcurrentMillis - EVNTpreviousMillis >= 5000) 
    {
      EVNTpreviousMillis = EVNTcurrentMillis;
      eventmode2();
    }
  }
  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  if (manualsendcur - manualsend >= 4000) 
  {
    manualsend = manualsendcur;
    manualread();
    counterun();
  }
  if (PumpAZ1bool == true)
  {
    PumpAZ1.Update();

  }

  if (PumpAZ1boolq == true)
  {
    PumpAZ1q.Update();

  }

  if (PumpAZ1boolqq == true)
  {
    PumpAZ1qq.Update();

  }

  if (PumpBZ1bool == true)
  {
    PumpBZ1.Update();
  }

  if (PumpDOZ1bool == true)
  {
    PumpDOZ1.Update();

  }
  if (PumpLBbool == true)
  {
    PumpLB.Update();

  }
  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - ESPpreviousMillis >= 5000)
  {
    ESPpreviousMillis = NXcurrentMillis;
    String str="";
    while(Serial3.available()>5)
    {
      str=Serial3.readString();
    }
    Appdata(str.substring(str.indexOf('{')));
  }
  
}
