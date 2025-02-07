void eventmode2()
{
  StaticJsonDocument <500> doc1;
  if (((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true)) && ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true)))
  {
     JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "21";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
  }
  else if ((PumpAZ1bool == true) || (PumpBZ1bool == true) || (PumpDOZ1bool == true) || (PumpLBbool == true) || (PumpAZ1boolq == true) || (PumpAZ1boolqq == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "5";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }

   else if ((checkflag == true) || (checkflag1 == true) || (checkflag2 == true) || (checkflag3 == true) || (checkflag4 == true) | (checkflag5 == true) || (checkflag6 == true) || (checkflag7 == true) || (checkflag8 == true))
  {



    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["IRR"] = "6";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);


  }
}
