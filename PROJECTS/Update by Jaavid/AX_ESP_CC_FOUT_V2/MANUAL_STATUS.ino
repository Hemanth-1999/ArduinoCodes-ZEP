/*
WE RECEIVE DATA FROM ATMEGA VIA SERIAL. MESSAGE FORMAT IS JSON
THIS JSON MESSAGE CONTAINS DATA REGARDING ALL FANS,COLLING PADS ON OR OFF STATUS.
THIS FUNCTION WILL READ THE STATUS AND ASSIGN IT TO GLOBAL VARIABLES.
*/
void manualstatus()
{
  DynamicJsonDocument relaystatus(500);
  DeserializationError error = deserializeJson(relaystatus, Serial);
  if (error) 
  {
    Serial.println("FAILED TO DESERILIZE MANUAL STATUS JSON DATA");
    return;
  }
  JsonObject Manualstatus = relaystatus["Manualstatus"];
  Manualstatus_f1 = Manualstatus["f1"];
  Manualstatus_f2 = Manualstatus["f2"];
  Manualstatus_f3 = Manualstatus["f3"];
  Manualstatus_f4 = Manualstatus["f4"];
  Manualstatus_f5 = Manualstatus["f5"];
  Manualstatus_f6 = Manualstatus["f6"];
  Manualstatus_f7 = Manualstatus["f7"];
  Manualstatus_f8 = Manualstatus["f8"];
  Manualstatus_cp = Manualstatus["cp"];
  Manualstatus_co = Manualstatus["co"];
  Manualstatus_fp = Manualstatus["fp"];
  Manualstatus_fs = Manualstatus["fs"];
  Manualstatus_ir = Manualstatus["ir"];
  Manualstatus_do = Manualstatus["do"];
  Manualstatus_dr = Manualstatus["dr"];
  Manualstatus_pb = Manualstatus["pb"];
}
