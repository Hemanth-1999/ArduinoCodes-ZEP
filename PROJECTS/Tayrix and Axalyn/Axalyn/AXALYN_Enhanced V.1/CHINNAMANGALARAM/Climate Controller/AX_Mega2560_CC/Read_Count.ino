void manualread()
{
  f1 = digitalRead(ExFan1);
  f2 = digitalRead(ExFan2);
  f3 = digitalRead(ExFan3);
  f4 = digitalRead(ExFan4);
  f5 = digitalRead(ExFan5);
  f6 = digitalRead(ExFan6);
  f7 = digitalRead(ExFan7);
  f8 = digitalRead(ExFan8);
  cp = digitalRead(pad1);
  co = digitalRead(Co2Gen);
  fp = digitalRead(Fogger);
  fs = digitalRead(FoggerSol);

  storeprvstate = "";
  storeprvstate = f1 + f2 +  f3 +  f4 + f5 + f6 +  f7 +  f8 +  cp +  fp + fs +  co +  ir +  doo +  dr +  pb;

  if (storeprvstate != checkstate)
  {
    checkstate = storeprvstate;
    counterm = 5;
    Serial.println("No of Relays");
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
  if (Serial3.available()) {

    DynamicJsonDocument Manualstatus(1024);

    Manualstatus["f1"] = f1;
    Manualstatus["f2"] = f2;
    Manualstatus["f3"] = f3;
    Manualstatus["f4"] = f4;
    Manualstatus["f5"] = f5;
    Manualstatus["f6"] = f6;
    Manualstatus["f7"] = f7;
    Manualstatus["f8"] = f8;
    Manualstatus["cp"] = cp;
    Manualstatus["co"] = co;
    Manualstatus["fp"] = fp;
    Manualstatus["fs"] = fs;

    Serial.println("Publish Esp:");
    serializeJson(Manualstatus, RelayStatus);
    serializeJson(Manualstatus, Serial);
    serializeJson(Manualstatus, Serial3);
    Serial.println(RelayStatus);
  }
}
