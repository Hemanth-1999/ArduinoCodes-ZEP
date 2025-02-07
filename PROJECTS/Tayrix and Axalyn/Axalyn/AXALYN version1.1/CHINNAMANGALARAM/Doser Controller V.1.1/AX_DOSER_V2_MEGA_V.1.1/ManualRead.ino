String checkstate ;
String storeprvstate = "";
String set_point_from_app ;

int tp ;
int ht ;
int mx ;
int counterm;

const char* Relaystate;

void manualread()
{
  Serial.println("manualread() is running...");
  tp = analogRead(TPUMPR);
  ht = analogRead(HEATERR);
  mx = analogRead(MIXERR);

  storeprvstate = "";
  storeprvstate = tp + ht + mx ;

  if (storeprvstate != checkstate)
  {
    checkstate = storeprvstate;
    counterm = 5;
    Serial.print("Relay status: ");
    Serial.println(checkstate);
    Serial.println();
    counterun();
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
  Manualstatus["TPUMPR"] = tp;
  Manualstatus["HEATERR"] = ht;
  Manualstatus["MIXERR"] = mx;
  serializeJson(Manualdoc1, Serial);
  serializeJson(Manualdoc1, Serial3);
  Serial.println(".....");
}

void eventmode()
{
  Serial.println("eventmode() is running...");
  StaticJsonDocument <500> doc1;

  if ((autoboolcheck == true) && (scheboolcheck == true))
  {
    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["Dosing"] = "1";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
  }
  else if (autoboolcheck == true)
  {

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["Dosing"] = "2";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);

  }

  else if (scheboolcheck == true)
  {

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["Dosing"] = "3";
    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);

  }

}

void funtimer()
{
  eventmode();
}
