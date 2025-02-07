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
   if (Events_IRR == 21)
  {
    intializer(21);
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
      
      case 21:
      alertmode1 = "Auto and Sche";
      break;

  }
}
