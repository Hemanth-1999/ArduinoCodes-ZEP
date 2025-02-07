//===========================================
// Defines
//===========================================


const int DIModeOFF_SetTime_Hr  = 12;
const int DIModeOFF_SetTime_Min = 25;

bool  DIMode = true;



void DepthIrrMode() {

  
  if (DepthIrrMode_shift01 == true)
  {     
    if (RTC_Hr >= Slot01_DIModeON_SetTime_Hr && RTC_Min >= Slot01_DIModeON_SetTime_min && ZPumpStatus == true) 
    {
      
      Zone01_MainLinePump.Update();
        ZPumpStatus=false;  
    }

  }
  
  if (DepthIrrMode_shift02 = true)
  {
    if (RTC_Hr >= Slot02_DIModeON_SetTime_Hr && RTC_Min >= Slot02_DIModeON_SetTime_min && ZPumpStatus == true) 
    {
      Zone01_MainLinePump.Update();
    }
  }
  
  if (DepthIrrMode_shift03 = true) 
  {
    if (RTC_Hr <= Slot03_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true)
    {

      Zone01_MainLinePump.Update();
    }
  }
}
