//===========================================
// Defines
//===========================================
const int RTC_Hr = 12;
const int RTC_Min = 23;

const int Slot01_DIModeON_SetTime_Hr  = 8;
const int Slot01_DIModeON_SetTime_min = 30;

const int Slot02_DIModeON_SetTime_Hr  = 13;
const int Slot02_DIModeON_SetTime_min = 20;

const int Slot03_DIModeON_SetTime_Hr  = 18;
const int Slot03_DIModeON_SetTime_min = 45;

const int DIModeOFF_SetTime_Hr  = 12;
const int DIModeOFF_SetTime_Min = 25;

bool  DIMode = true;



//===========================================
// Zone Irigator Calculation
//===========================================


//SingleOnRelaySwitcher FOGGER(12, 15000, PumpStatus); // RelayPIN,ONTime,RelayStatus

int ZoneIrigatorCal(float ETval, float FlowRate, float Area) // conversion of area from acre to sqft
{

  float ETval_intch = ETval / 25.4;
  float appdepth = ETval_intch;
  float PipeFlowRateSize = FlowRate;
  irrareaacre = Area;
  irrareasqft = irrareaacre * 43560;
  //float a = flowrate * 96.3 / irrarea ;  //96.3 convert gpm,sqfeet,hour into inch
  apphour = (appdepth * irrareasqft) / (PipeFlowRateSize * 96.3);
  appmin = apphour * 60;

  watersupplied = appmin * PipeFlowRateSize;

  //  Serial.print("Number of hours required to irrigate :  ");
  //  Serial.println(apphour);
  //
  //  Serial.print("Number of minutes required to irrigate :  ");
  //  Serial.println(appmin);
  //
  //  Serial.print("Water supplied for the land is : ");
  //  Serial.print(watersupplied);
  //  Serial.println(" Gallons");

  return appmin;
}
//===========================================
// Send Data To Mega
//===========================================
void SendDataToMega()
{

  //  doc["AREA"] = irrareasqft;
  //  doc["apphour"] = apphour;
  //  doc["appmin"] = appmin;
  //  doc["flowrate"] = PipeFlowRateSize;
  //  doc["watersupplied"] = watersupplied;

  megadoc = doc;
  Serial.println("Publish mega:");
  serializeJson(megadoc, Serial);
  Serial.print("\n");
}

//===========================================
// Irrigation Zone Controller
//===========================================
void IrrZoneController(float ETvalue, int FlowRate, int Area, int RelayPin, bool PumpStatus) {

  float ZETvalue = ETvalue ;
  int ZFlowRate = FlowRate;
  int ZArea = Area;
  byte ZRelayPin = RelayPin;
  bool ZPumpStatus = PumpStatus = true;

  int ZoneAMin = ZoneIrigatorCal(ZETvalue, ZFlowRate, ZArea); //(float ETval, float FloeRate,Areain Acre);
  Serial.print("Zone A Irrigation Time :  ");
  Serial.print (ZoneAMin);
  Serial.println("    Min");
  long minToMills = 5000 ; //ZoneAMin*ms_per_min;
  Serial.print("minToMills :  ");
  Serial.print (minToMills);
  Serial.println("    Mills");
  int shift_div=0;
  if (DepthIrrMode_shift01==true) {
     shift_div++;
  }
  if (DepthIrrMode_shift02==true) {
     shift_div++;
  }
  if (DepthIrrMode_shift03==true) {
    shift_div++;
  }
  Serial.print("shift_div    ");
  Serial.print(shift_div);
  int shiftTimeDiv =ZoneAMin/shift_div;
  Serial.print("shiftTimeDiv    ");
  Serial.print(shiftTimeDiv);
  Serial.println("    ");
//  Serial.println("shiftTimeDiv " + String(shiftTimeDiv) + " Min");
  Zone01_MainLinePump.begin( minToMills, ZPumpStatus); // RelayPIN,ONTime,RelayStatus
}
//void switchcase() {
//
//  int num = 8;
//  switch (num) {
//    case 7:
//      printf("Value is 7");
//      break;
//    case 8:
//      printf("Value is 8");
//      break;
//    case 9:
//      printf("Value is 9");
//      break;
//    default:
//      printf("Out of range");
//      break;
//  }
//  return 0;
//
//
//}

SingleOnRelaySwitcher Due_IRR(12);

//===========================================
// Duration Irrigation Mode
//===========================================
void DepthIrrMode() {
   
  if (DepthIrrMode_shift01 = true) {
    if (RTC_Hr <= Slot01_DIModeON_SetTime_Hr && RTC_Min <= Slot01_DIModeON_SetTime_min && ZPumpStatus == true) {

      Zone01_MainLinePump.Update();
      //    IrrZoneController(float ETvalue, int FlowRate, int Area, int RelayPin, bool PumpStatus);
    }
  }
  if (DepthIrrMode_shift02 = true) {
    if (RTC_Hr <= Slot02_DIModeON_SetTime_Hr && RTC_Min <= Slot02_DIModeON_SetTime_min && ZPumpStatus == true) {
      Zone01_MainLinePump.Update();
    }
  }
  if (DepthIrrMode_shift02 = true) {
    if (RTC_Hr <= Slot03_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true) {

      Zone01_MainLinePump.Update();
    }
  }

}

//===========================================
// Volume Irrigation Mode
//===========================================
void VolumeIrrMode () {
  if (RTC_Hr <= Slot01_DIModeON_SetTime_Hr && RTC_Min <= Slot01_DIModeON_SetTime_min && ZPumpStatus == true) {


    InFlowRate();

    if (InFlowTotalVolume >= SetVal_IrrValume) {
      digitalWrite(IRR_MainLine_SumpPump, HIGH);
    }
    else {
      digitalWrite(IRR_MainLine_SumpPump, LOW);
    }
  }

  if (RTC_Hr <= Slot02_DIModeON_SetTime_Hr && RTC_Min <= Slot02_DIModeON_SetTime_min && ZPumpStatus == true) {
    InFlowRate();

    if (InFlowTotalVolume >= SetVal_IrrValume) {
      digitalWrite(IRR_MainLine_SumpPump, HIGH);
    }
    else {
      digitalWrite(IRR_MainLine_SumpPump, LOW);
    }
  }

  if (RTC_Hr <= Slot03_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true) {
    InFlowRate();
    if (InFlowTotalVolume >= SetVal_IrrValume) {
      digitalWrite(IRR_MainLine_SumpPump, HIGH);
    }
    else {
      digitalWrite(IRR_MainLine_SumpPump, LOW);
    }
  }


}
//===========================================
// Duration Irrigation Mode
//===========================================

//This Funtion For Specific Zone Irrigation Pump OnTime Due Switch On
//Nextion Pump on controller Funtion call

void DurationIrrMode() {

  if (RTC_Hr <= Slot01_DIModeON_SetTime_Hr && RTC_Min <= Slot01_DIModeON_SetTime_min && ZPumpStatus == true) {

    Due_IRR.Update();
  }

  if (RTC_Hr <= Slot02_DIModeON_SetTime_Hr && RTC_Min <= Slot02_DIModeON_SetTime_min && ZPumpStatus == true) {

    Due_IRR.Update();
  }
  if (RTC_Hr <= Slot03_DIModeON_SetTime_Hr && RTC_Min <= Slot03_DIModeON_SetTime_min && ZPumpStatus == true) {

    Due_IRR.Update();
  }
}
