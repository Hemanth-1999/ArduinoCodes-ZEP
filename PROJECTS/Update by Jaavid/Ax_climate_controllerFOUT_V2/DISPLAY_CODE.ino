void callSerialCom() 
{
  if (Serial2.available()) 
  {
   SerialCom("");
  }
}

void SerialCom(String frommob) 
{
  String data_from_display = "";//IT IS USED TO READ DATA FROM DISPLAY.
  delay(50);
  while (Serial2.available()) 
  {
    data_from_display += char(Serial2.read());
  }
  dfd = data_from_display;//STORING DISPLAY DATA TO GLOBAL VARIABLE.
  dfd.toCharArray(buff, dfd.length());//IF WE NEED TO USE CHARACTER ARRAY THEN USE THIS.
 /************************************** WIFI PAGE - CREDENTIALS **************************/
  #ifdef TEST_DISPLAY_WIFI_PAGE
  if ((data_from_display.substring(0, 4) == "sdpd")) 
  {
    ssid_password();//THIS WILL READ SSID AND PASSWORD FROM WIFI PAGE OF DISPLAY.
  } 
  #endif
   //**************************    MESH    ********************
//  if ((dfd.substring(0, 4) == "mesh")) 
//  {
//    #ifdef TEST_DISPLAY_MESH
//    meshpass();
//    prefix();
//    port();
//    feedBack = "mesh";
//     #endif
//  }

  //************************************** TEMPERATURE PAGE CREDENTIALS **************************
  #ifdef TEST_DISPLAY_TEMP_PAGE
  if ((dfd.substring(0, 4) == "temp")) 
  {
    readtempvalues();//THIS WILL READ INDORE AND OUTDORE TEMPERATURES FROM TEMPERATURE PAGE OF DIAPLAY.
  }
  #endif
   //********************************************************************AUTO TEMP MODE*******************************
                                      // NOTE WE ARE STILL INSIDE TEMPERATURE PAGE ONLY
  #ifdef TEST_DISPLAY_AUTO_TEMP
  if (dfd == "tmpon") 
  {
    autotenpmode(true);
  }
  if (dfd == "tmpoff") 
  {
    autotenpmode(false);
  }
  #endif
  //***************************************************** VPD PAGE CREDENTIALS *******************************
  #ifdef TEST_DISPLAY_VPD_PAGE
  if (dfd == "vpdoff") //this is on touch event
  {
    vpdpage(true);
  }
  if (dfd == "vpdon") //this is on touch event
  {
    vpdpage(false);
  }
  #endif
  //***************************************MANUAL PAGE CREDENTIALS******************************
  #ifdef TEST_DISPLAY_MANUAL_MODE
  
  if (dfd == "f1on" )
  {
    manual(1);
  }
  else if(dfd == "f1off")  
  {
     manual(2);
  }
  else if(dfd =="f2on")
  {
     manual(3);
  }
  else if(dfd  == "f2off")
  {
     manual(4);
  }
  else if(dfd == "f3on")
  {
     manual(5);
  }
  else if(dfd == "f3off")
  {
     manual(6);
  }
  else if(dfd == "f4on")
  {
     manual(7);
  }
  else if(dfd == "f4off")
  {
     manual(8);
  }
  else if(dfd == "f5on")
  {
     manual(9);
  }
  else if(dfd == "f5off")
  {
     manual(10);
  }
  else if(dfd == "f6on")
  {
     manual(11);
  }
  else if(dfd == "f6off")
  {
     manual(12);
  }
  else if(dfd == "f7on")
  {
     manual(13);
  }
  else if(dfd == "f7off")
  {
     manual(14);
  }
  else if(dfd == "f8on")
  {
     manual(15);
  }
  else if(dfd == "f8off")
  {
     manual(16);
  }
  else if(dfd == "cpon")
  {
     manual(17);
  }
  else if(dfd == "cpoff")
  {
     manual(18);
  } 
  else if(dfd == "fpon")
  {
     manual(19);
  }
  else if(dfd == "fpoff")
  {
     manual(20);
  }
  else if(dfd == "fcon")
  {
     manual(21);
  }
  else if(dfd == "fcoff")
  {
     manual(22);
  }
  else if(dfd == "co2on")
  {
     manual(23);
  }
  else if(dfd == "co2off")
  {
     manual(24);
  }
 #endif
 
 //********************************************************DISPLAY TIME AND DATE ***************************
 
 #ifdef TEST_DISPLAT_TIME
  if ((data_from_display.substring(0, 3) == "clk"))//enters this if condition when save button is pressed in display
  {
     read_date_time_display();//WHEN SAVE BUTTON IS PRESSED ON DISPLAY. THIS FUN IS CALLED SO IT WILL EXTRACT TIME AND DATE FROM DISPLAY.
     update_rtc_time();//UPDATE TIME AND DATE OF RTC MODULE TO DATE AND TIME RECEIVED FROM DISPLAY.
  }
  if ((data_from_display.substring(0, 6) == "settc"))//enters this if condition when set button is pressed on display
  {
    read_date_time_rtc();//THIS FUN WILL READ DATE AND TIME FROM RTC MODULE
    delay(100);
    update_display_time();//THIS FUNCTION WILL WRITE DATE AND TIME TO DISPLAY, DATE AND TIME READ FROM RTC MODULE WILL BE UPDATED TO DISPLAY.
  }
 
 #endif

 
}
