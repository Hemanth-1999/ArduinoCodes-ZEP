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
 if ((RTC_Hr == Slot02_DIModeON_SetTime_Hr && RTC_Min == Slot02_DIModeON_SetTime_min) &&(REPEATS==true))
    {
      ZPumpStatus = true;
    }
 ///////////////////////////////////////////////slot1///////////////////////////////////// 
  if (DepthIrrMode_shift01 == true)
  {     
    if ((RTC_Hr == Slot01_DIModeON_SetTime_Hr && RTC_Min == Slot01_DIModeON_SetTime_min && ZPumpStatus == true) &&(REPEATS == true))
    {
      irrslot1.start(shiftTimeDiv+100);
          
    }
     if(irrslot1.isRunning())
    {
//      REPEATS =false;
     Zone01_MainLinePump.Update();   
//    Serial.println(irrslot1.remaining());
//      unsigned long irrcurrentMillis = millis();
//  if (irrcurrentMillis - SAS >= 80000)
//  {
//  
//    SAS = irrcurrentMillis;
//    if((Mainpressure>=2&&Mainpressure<=6)&&(pressurecheck==true))
//    {
//      Serial.println("ok");
//    }
//    else if(pressurecheck==true)
//    {
//      Serial.println("NOTOK");
//      pinMode(irrigation,OUTPUT);
//      digitalWrite(irrigation,HIGH);
//    }
//  
//  }
    }
//     if(irrslot1.justFinished())
//    {
//      REPEATS =true;
//      Serial.println("we are done here");
//    }            
  }
  //////////////////////////////////////////////slot2///////////////////////////   
   if (DepthIrrMode_shift02 == true)
  {     
    if ((RTC_Hr == Slot02_DIModeON_SetTime_Hr && RTC_Min == Slot02_DIModeON_SetTime_min && ZPumpStatus == true) &&(REPEATS == true))
    {
      irrslot1.start(shiftTimeDiv+100);
          
    }
     if(irrslot1.isRunning())
    {
//      REPEATS =false;
     Zone01_MainLinePump.Update();   
//    Serial.println(irrslot1.remaining());
//      unsigned long irrcurrentMillis = millis();
//  if (irrcurrentMillis - SAS >= 80000)
//  {
//  
//    SAS = irrcurrentMillis;
//    if((Mainpressure>=2&&Mainpressure<=6)&&(pressurecheck==true))
//    {
//      Serial.println("ok");
//    }
//    else if(pressurecheck==true)
//    {
//      Serial.println("NOTOK");
//      pinMode(irrigation,OUTPUT);
//      digitalWrite(irrigation,HIGH);
//    }
//  
//  }
    }
//     if(irrslot1.justFinished())
//    {
//      REPEATS =true;
//      Serial.println("we are done here");
//    }            
  }   
}
