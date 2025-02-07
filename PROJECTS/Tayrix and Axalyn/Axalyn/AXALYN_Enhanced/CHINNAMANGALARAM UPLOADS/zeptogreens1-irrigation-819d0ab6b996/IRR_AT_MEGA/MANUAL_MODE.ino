void manualread()
{
  //Z1PUMPA    36 //Z1PUMPA
  //Z1PUMPB    38 //Z1PUMPB
  //Z1AIRPUMP  23 //Z2PUMPA
  //Z1SOLVALVE 25 //Z2PUMPB
  //DOOA       32 // Z1DO
  //DOOB       27 // Z2DO

  irz1a = digitalRead(Z1PUMPA);
  irz1b = digitalRead(Z1PUMPB);
  irz2a = digitalRead(Z1AIRPUMP);
  irz2b = digitalRead(Z1SOLVALVE);
  doa = digitalRead(DOOA);
  dob = digitalRead(DOOB);

  String storeprvstate = "";
  String checkstate="";
  storeprvstate =   irz1a +  irz1b +  irz2a +  irz2b + doa + dob;
 
  if (storeprvstate != checkstate)
  {

    checkstate = storeprvstate;
    counterm = 5;
    Serial.println(checkstate);

  }
}
void counterun()
{

  if (counterm > 0)
  {
    runmanual();
    counterm --;
  }

}

void runmanual()
{
  StaticJsonDocument <500> Manualdoc1;
  JsonObject Manualstatus = Manualdoc1.createNestedObject("Manualstatus");

  Manualstatus["irz1a"] = irz1a;
  Manualstatus["irz1b"] = irz1b;
  Manualstatus["irz2a"] = irz2a;
  Manualstatus["irz2b"] = irz2b;
  Manualstatus["doa"] = doa;
  Manualstatus["dob"] = dob;
  serializeJson(Manualdoc1, Serial);
  serializeJson(Manualdoc1, Serial3);
  Serial.println(".....");

}
