//===========================================
// Defines
//===========================================

unsigned long SAS = 0;
const int DIModeOFF_SetTime_Hr  = 12;
const int DIModeOFF_SetTime_Min = 25;

bool  DIMode = true;
bool REPEATS =true;


void DepthIrrMode()
{
  if ((RTC_Hr == Slot01_DIModeON_SetTime_Hr && RTC_Min == Slot01_DIModeON_SetTime_min )&&(REPEATS==true))
  { 
  
    ZPumpStatus = true;
  }
 if (RTC_Hr == Slot02_DIModeON_SetTime_Hr && RTC_Min == Slot02_DIModeON_SetTime_min) 
    {
      ZPumpStatus = true;
    }
  
  if (DepthIrrMode_shift01 == true)
  {     
    if ((RTC_Hr == Slot01_DIModeON_SetTime_Hr && RTC_Min == Slot01_DIModeON_SetTime_min && ZPumpStatus == true) &&(REPEATS == true))
    {
      irrslot1.start(90000-50000);
          
    }
     if(irrslot1.isRunning())
    {
      REPEATS =false;
     Zone01_MainLinePump.Update();   
    Serial.println(irrslot1.remaining());
//      unsigned long irrcurrentMillis = millis();
//  if (irrcurrentMillis - SAS >= 100000)
//  {
//  
//    SAS = irrcurrentMillis;
//    if((Mainpressure>=2&&Mainpressure<=6)&&(pressurecheck==true))
//    {
//      Serial.println("ok");
//    }
//    else
//    {
//      Serial.println("NOTOK");
//      pinMode(irrigation,OUTPUT);
//      digitalWrite(irrigation,HIGH);
//    }
//  
//  }
    }
     if(irrslot1.justFinished())
    {
      Serial.println("we are done here");
    }            
  }   
}
