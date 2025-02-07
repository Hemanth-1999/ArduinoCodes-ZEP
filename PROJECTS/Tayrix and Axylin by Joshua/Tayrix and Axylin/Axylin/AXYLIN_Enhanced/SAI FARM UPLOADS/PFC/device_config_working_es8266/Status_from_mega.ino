void eventstatus()
{

  DynamicJsonDocument event(500);

  DeserializationError error = deserializeJson(event, Serial);

  if (error) {
    return;
  }

  Events_TEMP = event["Events"]["TEMP"];
  Events_VPD = event["Events"]["VPD"];
  Events_IRR = event["Events"]["IRR"];

  if (Events_TEMP == 3)
  {
    intializer(3);
    intializer(1);
  }

  if (Events_VPD == 4)
  {
    intializer(4);
    intializer(1);
  }
  if (Events_IRR == 5)
  {
    intializer(5);
  }
  if (Events_IRR == 6)
  {
    intializer(6);
  }

}
void manualstatus()
{
  DynamicJsonDocument relaystatus(500);

  DeserializationError error = deserializeJson(relaystatus, Serial);

  if (error) {

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
void alert()
{
  DynamicJsonDocument alertfrom(500);
  DeserializationError error = deserializeJson(alertfrom, Serial);
  if (error) {
    return;
  }

  Alerts_Mode = alertfrom["Alerts"]["Mode"];
  Alerts_level = alertfrom["Alerts"]["level"];
  if (Alerts_Mode == 7)
  {
    intializer(7);
  }
  if (Alerts_Mode == 12)
  {
    intializer(12);
  }
  if (Alerts_level == 8)
  {
    intializer(8);

  }
  if (Alerts_level == 9)
  {
    intializer(9);
  }
  if (Alerts_level == 10)
  {
    intializer(10);
  }
  if (Alerts_level == 11)
  {
    intializer(11);

  }
}
void intializer(int toperform)
{
  switch (toperform)

  {
    case 1:
      if (Events_TEMP == 0)
      {
        eventogo = "null";
      }
      if (Events_VPD == 0)
      {
        eventogo1 = "null";
      }
      if (Events_IRR == 0)
      {
        eventogo2 = "null";
      }
      break;

    case 3:
      eventogo = "Autotemp";
      break;


    case 4:
      eventogo1 = "Autovpd";
      break;

    case 5:
      eventogo2 = "AutoIrr";
      break;

    case 6:
      eventogo2 = "ScheIrr";
      break;

    case 7:
      alertmode1 = "VPD";
      break;

    case 8:
      alertlevel = "Critical";
      break;

    case 9:
      alertlevel = "moderate";
      break;

    case 10:
      alertlevel = "low";
      break;
    case 11:
      alertlevel = "normal";
      break;

    case 12:
      alertmode1 = "Temperature";
      break;

  }
}
