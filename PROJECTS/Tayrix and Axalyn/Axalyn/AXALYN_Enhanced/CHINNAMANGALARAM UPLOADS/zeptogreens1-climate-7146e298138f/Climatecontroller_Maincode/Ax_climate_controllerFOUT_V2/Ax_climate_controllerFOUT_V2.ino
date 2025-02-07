/*
THIS CLIMATE CONTROLLER MASTER ATMEGA CODE.
                    
*/
#include"config.h"

void setup() 
{
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  Serial3.begin(115200);//ESP8266
  Serial.println("Axalyn...");
  //RTC RELATED
  rtc.begin();
  rtc.adjust(DateTime(2023, 2, 2, 4, 58, 0));
  Pinmodes();
  alloff();//this will off all relays
}

void loop() 
{
   
  ////////////************** Serialcom loop Mode***********///////////
  
  #ifdef TEST DISPLAY
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) 
  {
    SCpreviousMillis = currentMillis;
    callSerialCom();//THIS FUNCTION IS USED TO READ incoming data from display.
  }
  #endif
  
  //////////************** Node receviing loop Mode***********///////////
  
  #ifdef TEST_NODE_DATA
  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - ESPpreviousMillis >= 5000)
  {
    ESPpreviousMillis = NXcurrentMillis;
    FromEspData();
  }
  #endif

  //////////************** Temperature loop Mode***********///////////
  if (TemperatureAutoModeOn == true )
  {

    unsigned long ATcurrentMillis = millis();
    if (ATcurrentMillis - ATpreviousMillis >= 10000) //FOR EVERY 10 SECONDITIONS THIS WILL BE TRUE
    {
      ATpreviousMillis = ATcurrentMillis;
      AutoTemp_control( SPtemp);//this will automaticially on fans and cooling pad.
      Serial.println("TEMP");
      funtimer();
    }
  }
 //////////************** VPD loop Mode***********///////////
  if (VpdautoModeOn == true ) 
  {
    unsigned long VPcurrentMillis = millis();
    if (VPcurrentMillis - VPpreviousMillis >= 10000) //EVERY 10 SECONDS CONTROLL WILL ENTER INTO THIS. IF CONDITION ALSO SHOULD BE TRUE.
    {
      VPpreviousMillis = VPcurrentMillis;
      vpd_control(_VPD);
      funtimer();
    }
  }
  //////////************** Nexval Mode***********///////////
  unsigned long nextionmillsc = millis();
  if (nextionmillsc - nextionmills >= 30000) 
  {
    nextionmillsc = nextionmillsc;
    nexval();
  }
  //////////************** manualstate loop Mode***********///////////
  unsigned long manualsendcur = millis();
  if (manualsendcur - manualsend >= 5000) //FOR EVERY 5 SECONDS IT WILL BE TRUE
  {
    manualsend = manualsendcur;
    manualread();
    counterun();
  }
}
