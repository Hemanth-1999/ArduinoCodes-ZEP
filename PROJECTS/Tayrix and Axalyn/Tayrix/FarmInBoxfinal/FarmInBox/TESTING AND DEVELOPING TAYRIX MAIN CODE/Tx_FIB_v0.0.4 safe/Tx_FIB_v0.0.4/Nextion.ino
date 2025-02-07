
///DECLARATION SECTION///
#include <EEPROMex.h>
uint8_t Eepromprecp = 100;
uint8_t Eepromdstnce = 110;
uint8_t Eepromet = 120;
uint8_t Eepromdkc = 130;
//**************************//

///NEXTION STRING BREAK SECTION///
int othr2()
{
  int val = dfd.indexOf("fr") + 3;//
  dfd.remove(0, val);

  int btHr = dfd.toInt();
  return (btHr);
}
int othr() {
  int val = dfd.indexOf("dp") + 3;//
  dfd.remove(0, val);

  otHr = dfd.toInt();
  return (otHr);
}
int othr1() {
  int val = dfd.indexOf("dl") + 3;//
  dfd.remove(0, val);

  int  qtHr = dfd.toInt();
  return (qtHr);
}

int oftmin1() {
  char *ui = strtok(buff, "e")+1;//on time hr val
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}
int oftmin13() {
  char *ui = strtok(buff, "e");//on time hr val
  str = ui;
  str.remove(0, 5);
  oftMina = str.toInt();
  return (oftMina);
}
int ofthr()
{
  int val = dfd.indexOf("g") + 1;//
  dfd.remove(0, val);

  int tal = dfd.toInt();
  return (tal);

}
//****************************//
///USER INTERFACE MAIN SECTION///
void SerialCom() 
{
  String data_from_display = "";

  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }

  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());

  ///FIRST ZONE DRIP SETTINGS//

  if ((data_from_display.substring(0, 5) == "drips"))
  {
    int decimalplaces = 0;
    int divisor = 1;
    decimalplaces = data_from_display.substring(7, 8).toInt();
    for (int x = 0; x < (decimalplaces); x++) {
      divisor *= 10;
    }
    flowrate1 = float(data_from_display.substring(8).toInt()) / divisor;
    data_from_display = "";
    divisor = 1;
    EEPROM.update(Eepromprecp, flowrate1);
    driper = othr();
    dripline = othr1();

    Precipitation_Rate(flowrate1, driper, dripline);
    Hourly_Irrigation_Rate();

  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 5) == "etcal"))
  {

    int decimalplaces = 0;
    int divisor = 1;
    decimalplaces = data_from_display.substring(7, 8).toInt();
    for (int x = 0; x < (decimalplaces); x++) {
      divisor *= 10;
    }
    etvalue = float(data_from_display.substring(8).toInt()) / divisor;
    EEPROM.update(Eepromet, etvalue);
    Serial.println(etvalue);
    data_from_display = "";
    divisor = 1;
    DWR(etvalue, 0);
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 7) == "zone1on"))
  {


    Irrigation_Time();

    ZPumpStatus = PumpStatus = true;
    int ZoneAMin = Irrigation_time; 
    long minToMills = ZoneAMin * ms_per_hour  ;
    int shift_div = 0;
    if (DepthIrrMode_shift01 == true)
    {
     
      shift_div++;
    }
    if (DepthIrrMode_shift02 == true)
    {
    
      shift_div++;
    }
    if (DepthIrrMode_shift03 == true)
    {
      shift_div++;
    }
   shiftTimeDiv = minToMills / shift_div;
 Serial.println(minToMills);
 Serial.println(shiftTimeDiv);
 
   Zone01_MainLinePump.begin( shiftTimeDiv, ZPumpStatus);

  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 5) == "slt1a"))
  {
    DepthIrrMode_shift01 = true;

    Slot01_DIModeON_SetTime_Hr = oftmin13();
     Slot01_DIModeON_SetTime_min =  ofthr();
    Serial.println(Slot01_DIModeON_SetTime_Hr);
    Serial.println(Slot01_DIModeON_SetTime_min);
  }
   if ((data_from_display.substring(0, 5) == "slt1p"))
  {
    DepthIrrMode_shift01 = true;

     Slot01_DIModeON_SetTime_Hr = oftmin13();
      Slot01_DIModeON_SetTime_min =  ofthr();
    Slot01_DIModeON_SetTime_Hr=Slot01_DIModeON_SetTime_Hr+12;
    Serial.println(Slot01_DIModeON_SetTime_Hr);
    Serial.println(Slot01_DIModeON_SetTime_min);
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 5) == "slt2a"))
  {
     DepthIrrMode_shift02 = true;
     Slot02_DIModeON_SetTime_Hr = oftmin13();
    Slot02_DIModeON_SetTime_min =  ofthr();
    Serial.println(Slot02_DIModeON_SetTime_Hr);
    Serial.println(Slot02_DIModeON_SetTime_min);
  }
   if ((data_from_display.substring(0, 5) == "slt2p"))
  {
     DepthIrrMode_shift02 = true;
     Slot02_DIModeON_SetTime_Hr = oftmin13();
     Slot02_DIModeON_SetTime_min =  ofthr();
    Slot02_DIModeON_SetTime_Hr=Slot02_DIModeON_SetTime_Hr+12;
    Serial.println(Slot02_DIModeON_SetTime_Hr);
    Serial.println(Slot02_DIModeON_SetTime_min);
  }
  //****************************////****************************////****************************//

  if ((data_from_display.substring(0, 5) == "aratl"))
  {
    Zone_sizein_acre = oftmin13();
    Nutrient_tank = ofthr();

    Serial.println(Zone_sizein_acre);
    Serial.println(Nutrient_tank);
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 7) == "zonedos"))
  {
    nutreintstats = true;
    Urea_fertilizer = othr2();

    delay(500);
    per_nitrogen = othr();
    delay(500);
    Req_NutrientKg = othr1();
    delay(500);
    Nutrient_Cal(Urea_fertilizer / 100, per_nitrogen / 100, Zone_sizein_acre, Nutrient_tank, Req_NutrientKg / 100);
    NUTRIENTNEX();
  }
  //****************************////****************************////****************************//

  if ((data_from_display.substring(0, 7) == "zoneds1"))
  {
    pstats = true;
    MOP_fertilizer = othr2();
    delay(500);
    per_P = othr();
    delay(500);
    Req_P_Kg = othr1();
    delay(500);
    Nutrient_Cal(MOP_fertilizer / 100, per_P / 100, Zone_sizein_acre, Nutrient_tank, Req_P_Kg / 100);
    NUTRIENTNEX();
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 7) == "zoneds2"))
  {

    kstats = true;
    DOP_fertilizer  = othr2();
    delay(500);
    per_K = othr();
    delay(500);
    Req_K_Kg = othr1();
    delay(500);
    Nutrient_Cal(DOP_fertilizer / 100, per_K / 100, Zone_sizein_acre, Nutrient_tank, Req_K_Kg / 100);
    NUTRIENTNEX();
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 7) == "zoneds3"))
  {
    astats = true;
    Req_Acid_Kg  = othr2();

    Nutrient_Cal(0, 0, Zone_sizein_acre, Nutrient_tank, Req_Acid_Kg / 100);
    NUTRIENTNEX();
  }
  //****************************////****************************////****************************//
  if ((data_from_display.substring(0, 7) == "zoneds4"))
  {
    mstats = true;
    Req_MN_Kg  = othr2();

    Nutrient_Cal(0, 0, Zone_sizein_acre, Nutrient_tank, Req_MN_Kg / 100);
    NUTRIENTNEX();
  }
  //////////////////RTC////////////////////
  
  //****************************////****************************////****************************//
  sendData(data_from_display);
}
//*********USER INTERFACE SECTION BREAK*********//
///DUAL STATE BUTTON INTERFACE///
void sendData(String dfd)
{
  if (dfd == "NNON" )
  {
    AUTO_DoserA_Delay.start(DoserA_DelayTime);
    Serial.println("nitro on");
    Doser_A_Enable = true;
    feedBack = "NNON";
  }
  if (dfd == "NNOFF")
  {
    Serial.println("nitro off");
    Doser_A_Enable = false;
    feedBack = "NNON";
  }
  //*****//*****//****////****************************////****************************//
  if (dfd == "NPON" )
  {
     AUTO_DoserB_Delay.start(DoserB_DelayTime);
    Serial.println("phsprs on");
    Doser_B_Enable = true;
    feedBack = "NPON";
  }
  if (dfd == "NPOFF")
  {
    Serial.println("phsprs off");
    Doser_B_Enable = false;
    feedBack = "NPOFF";
  }
  //*****//*****//****////****************************////****************************//
  if (dfd == "NKON" )
  {
    AUTO_DoserC_Delay.start(DoserC_DelayTime);
    Serial.println("potassium on");
    Doser_C_Enable = true;
    feedBack = "NKON";
  }
  if (dfd == "NKOFF")
  {
    Serial.println("potasssium off");
    Doser_C_Enable = false;
    feedBack = "NKOFF";
  }
  //*****//*****//****////****************************////****************************//

  if (dfd == "DOON" )
  {
    Serial.println("doser on");
    Doser_bool = true;
    feedBack = "DOON";
  
    
  }
  if (dfd == "DOOFF")
  {
    Serial.println("doseer off");
    Doser_bool = false;
    feedBack = "DOOFF";
   
     
  }
  //*****//*****//****////****************************////****************************//
  if (dfd == "ACON" )
  {
    Serial.println("acid on");
    Doser_Acid_Enable = true;
    feedBack = "ACON";
  }
  if (dfd == "ACOFF")
  {
    Serial.println("acid off");
    Doser_Acid_Enable = false;
    feedBack = "ACOFF";
  }
  //*****//*****//****////****************************////****************************//
  if (dfd == "PHON" )
  {
    Serial.println("ph on");
    Doser_pH_Enable = true;
    feedBack = "PHON";
  }
  if (dfd == "PHOFF")
  {
    Serial.println("ph off");
    Doser_pH_Enable = false;
    feedBack = "PHOFF";
  }
  //****************************////****************************////****************************//
  /////////////////// MANUAL RELAYS////////////////////////

  if (dfd == "irron" )
  {
    Serial.println("irron");
    pinMode(irrigation, OUTPUT);
    digitalWrite(irrigation, LOW);

  }
  if (dfd == "irroff")
  {
    Serial.println("irroff");
    pinMode(irrigation, OUTPUT);
    digitalWrite(irrigation, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "safon" )
  {
    Serial.println("safon");
    pinMode(safetyvalve, OUTPUT);
    digitalWrite(safetyvalve, LOW);

  }
  if (dfd == "safoff")
  {
    Serial.println("safoff");
    pinMode(safetyvalve, OUTPUT);
    digitalWrite(safetyvalve, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "fpon" )
  {
    Serial.println("fpon");
    pinMode(zone1valve, OUTPUT);
    digitalWrite(zone1valve, LOW);

  }
  if (dfd == "fpoff")
  {
    Serial.println("fpoff");
    pinMode(zone1valve, OUTPUT);
    digitalWrite(zone1valve, HIGH);
  }

//****************************////****************************////****************************//
  if (dfd == "fson" )
  {
    Serial.println("fson");
    pinMode(zone2valve, OUTPUT);
    digitalWrite(zone2valve, LOW);

  }
  if (dfd == "fsoff")
  {
    Serial.println("fsoff");
    pinMode(zone2valve, OUTPUT);
    digitalWrite(zone2valve, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "z1on" )
  {
    Serial.println("z1on");
    pinMode(fertpump, OUTPUT);
    digitalWrite(fertpump, LOW);

  }
  if (dfd == "z1off")
  {
    Serial.println("z1off");
    pinMode(fertpump, OUTPUT);
    digitalWrite(fertpump, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "z2on" )
  {
    Serial.println("z2on");
    pinMode(fertsole, OUTPUT);
    digitalWrite(fertsole, LOW);

  }
  if (dfd == "z2off")
  {
    Serial.println("z2off");
    pinMode(fertsole, OUTPUT);
    digitalWrite(fertsole, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "mpon" )
  {
    Serial.println("mpon");
    pinMode(mixpump, OUTPUT);
    digitalWrite(mixpump, LOW);

  }
  if (dfd == "mpoff")
  {
    Serial.println("mpoff");
    pinMode(mixpump, OUTPUT);
    digitalWrite(mixpump, HIGH);
  } 
  //****************************////****************************////****************************//
  if (dfd == "nnon" )
  {
    Serial.println("nnon");
    pinMode(Dose_A_sol_pin, OUTPUT);
    digitalWrite(Dose_A_sol_pin, LOW);

  }
  if (dfd == "nnoff")
  {
    Serial.println("nnoff");
    pinMode(Dose_A_sol_pin, OUTPUT);
    digitalWrite(Dose_A_sol_pin, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "pnon" )
  {
    Serial.println("pnon");
    pinMode(Dose_B_sol_pin, OUTPUT);
    digitalWrite(Dose_B_sol_pin, LOW);

  }
  if (dfd == "pnoff")
  {
    Serial.println("pnoff");
    pinMode(Dose_B_sol_pin, OUTPUT);
    digitalWrite(Dose_B_sol_pin, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "knon" )
  {
    Serial.println("knon");
    pinMode(Dose_C_sol_pin, OUTPUT);
    digitalWrite(Dose_C_sol_pin, LOW);

  }
  if (dfd == "knoff")
  {
    Serial.println("knoff");
    pinMode(Dose_C_sol_pin, OUTPUT);
    digitalWrite(Dose_C_sol_pin, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "acnon" )
  {
    Serial.println("acnon");
    pinMode(Dose_Acid_sol_pin, OUTPUT);
    digitalWrite(Dose_Acid_sol_pin, LOW);

  }
  if (dfd == "acnoff")
  {
    Serial.println("acnoff");
    pinMode(Dose_Acid_sol_pin, OUTPUT);
    digitalWrite(Dose_Acid_sol_pin, HIGH);
  }
  //****************************////****************************////****************************//
  if (dfd == "mnon" )
  {
    Serial.println("mnon");
    pinMode(Dose_pHUp_sol_pin, OUTPUT);
    digitalWrite(Dose_pHUp_sol_pin, LOW);

  }
  if (dfd == "mnoff")
  {
    Serial.println("mnoff");
    pinMode(Dose_pHUp_sol_pin, OUTPUT);
    digitalWrite(Dose_pHUp_sol_pin, HIGH);
  }
  //****************************////****************************////****************************//
}
//***********DUAL STATE INTERFACE END***************//
