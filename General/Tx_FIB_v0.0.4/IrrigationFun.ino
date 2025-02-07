//===========================================
// Defines
//===========================================

const int Slot01_DIModeON_SetTime_Hr  = 4;
const int Slot01_DIModeON_SetTime_min = 18;

const int Slot02_DIModeON_SetTime_Hr  = 13;
const int Slot02_DIModeON_SetTime_min = 20;

const int Slot03_DIModeON_SetTime_Hr  = 18;
const int Slot03_DIModeON_SetTime_min = 45;

const int DIModeOFF_SetTime_Hr  = 12;
const int DIModeOFF_SetTime_Min = 25;

bool  DIMode = true;

void DepthIrrMode() {
  
  if (DepthIrrMode_shift01 == true) {
    if (RTC_Hr <= Slot01_DIModeON_SetTime_Hr && RTC_Min <= Slot01_DIModeON_SetTime_min && ZPumpStatus == true) {
     
      Zone01_MainLinePump.Update();
     
    }
  }
  if (DepthIrrMode_shift02 = true) {
    if (RTC_Hr <= Slot02_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true) {
      Zone01_MainLinePump.Update();
    }
  }
  if (DepthIrrMode_shift02 = true) {
    if (RTC_Hr <= Slot03_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true) {

      Zone01_MainLinePump.Update();
    }
  }

}
