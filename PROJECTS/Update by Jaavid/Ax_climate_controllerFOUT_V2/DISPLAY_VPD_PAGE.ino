/*
IN DISPLAY
VPD PAGE HAS 
1)ATMOSPHERIC TEMPERATUE
2)RELATIVE HUMIDITY
3)VAPOUR PRESSURE DEFICIT (VPD)
2 BUTTONS.bt0 is for vpd and bt1 is for auto.
FIRST BUTTON IS 1 MEANS IT WILL SEND AT MEGA "vpdoff".
FIRST BUTTON IS 0 MEANS IT WILL SEND AT MEGA NOTHING.

SECOND BUTTON IS 1 MEANS IT WILL SEND AT MEGA "autooff"
SECOND BUTTON IS 0 MEANS IT WILL SEND AT MEGA "autoon"

WHEN FIRST BUTTON IS 0 AND SECOND BUTTON IS 1 THEN IT WILL SEND AT MEGA "vpdon"

*/

void vpdpage(bool stat)
{
  //THE DISPLAY SENDS "vpdoff"
  if(stat)//true
  {
    feedBack = "vpdon";
    Serial.println("VPD  ON");
    VpdautoModeOn = true;
  }
  //THE DISPLAY SENDS "vpdon"
  else
  {
    alloff();//THIS FUNCTION WILL OFF ALL RELAYS
    VpdautoModeOn = false;
    feedBack = "vpdoff";
    Serial.println("VPD Off");
    vpd_func = false;
  }
}
