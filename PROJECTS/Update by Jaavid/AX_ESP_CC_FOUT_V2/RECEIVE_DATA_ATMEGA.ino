/*
THIS FUNCTION IS USED TO RECEIVE MESSAGE FROM ATMEGA VIA SERIAAL.
MESSAGE FORMAT IS JSON
*/
void alert()
{
  DynamicJsonDocument alertfrom(500);
  DeserializationError error = deserializeJson(alertfrom, Serial);
  if (error) 
  {
      Serial.println("FAILED TO DESERIALIZED ALERT MESSAGE");
    return;
  }
  Alerts_Mode = alertfrom["Alerts"]["Mode"];
  Alerts_level = alertfrom["Alerts"]["level"];
  if (Alerts_Mode == 7)
  {
    intializer(7);//THIS MEANS ATMEGA IS IN VPD
  }
  if (Alerts_Mode == 12)
  {
    intializer(12);//THIS MEANS AT MEGA IS IN TEMPERATER MODE
  }
  if (Alerts_level == 8)
  {
    intializer(8);//THIS MEANS AT MEGA IS IN SOME MODE, NOW IT IS IN CRIRICAL LEVEL IN THE MODE

  }
  if (Alerts_level == 9)
  {
    intializer(9);//THIS MEANS AT MEGA IS IN SOME MODE, NOW IT IS FINE IN THAT MODE
  }
  if (Alerts_level == 10)
  {
    intializer(10);//THIS MEANS AT MEGA IS IN SOME MODE, NOW IT IS IN LOW LEVEL IN THAT MODE
  }
  if (Alerts_level == 11)
  {
    intializer(11);//THIS MEANS AT MEGA IS IN SOME MODE, NOW IT IS NORMAL IN THAT MODE

  }
  
}

/*

*/
void eventstatus()
{

  DynamicJsonDocument event(500);

  DeserializationError error = deserializeJson(event, Serial);

  if (error) 
  {
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


/*
THIS FUNCTION RECEIVES AN INTEGER AS AN ARGUMENT.
THIS FUNCTION HANDLES CERTAIN TASKS BASED ON THE ARGUMENT.
THIS WILL SAY US ATMEGA IS IN MANUAL MODE OR VPD MODE OR AUTO MODE.
THIS FUNCTION WILL UPDATE THE MODE AND LEVEL OF AT MEGA TO GLOBAL VARIABLES.
SO WE CAN SEND MESSAGE TO MQTT ABOUT THE ATMEGA ALERT MODE AND LEVEL USING THESE GLOBAL VARIABLES.
*/
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
