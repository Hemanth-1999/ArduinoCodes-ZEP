/*
USER WILL ENTER DATE AND TIME IN DATE&TIME PAGE OF DISPLAY AND HE WILL PRESS SAVE BUTTON IN DISPLAY.
THEN DISPLAY WILL SEND " clkd24m2y2022h2n58s30 " THIS MESSAGE TO AT MEGA VIA SERIAL2.
AS SOON AS AT MEGA READS "clk" THEN read_date_time_display() IS CALLED.
*/
void read_date_time_display()
{
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();
    feedBack = "clk";
}
/*
THIS FUNCTION BELONGS TO RTC MODULE.
THIS FRUNCTION WILL SET DATE AND TIME MANUALLY BY THE USER TO RTC MODULE.
GENERALLY WE ARE SETTING DATE AND TIME WHICH IS RED AND STORED IN GLOBAL VARIABLES FROM DISPLAY.
*/
void update_rtc_time()
{
  if(!rtc.begin())
  {
    Serial.println("SORRY WE COULD NOT FIND RTC MODULE SO WE ARE UNABLE TO ADD DATE AND TIME MANUALLY.");
    return;
  }
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  Serial.println("WE HAVE UPDATED THE TIME AND DATE");
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY MINUTES FROM DISPLAY MESSAGE AND ASSIGN IT TO minrc which is global variable
*/
void timeMin() 
{
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  int val = cSmn.indexOf("n") + 1; //    clkd24m2y2022h2n58s30
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); 
  minrc = minRc;
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY SECONDS FROM DISPLAY MESSAGE AND ASSIGN IT TO secsrc which is global variable
*/
void timeSec() 
{
  int val = dfd.indexOf("s") + 1; //    clkd24m2y2022h2n58s30
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY hours FROM DISPLAY MESSAGE AND ASSIGN IT TO   hourrc   which is global variable
*/
void timeHr() 
{
  char *chr = strtok(buff, "n");
  String cShr = chr;
  int val = cShr.indexOf("h") + 1; //    clkd24m2y2022h2n58s30
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); 
  hourrc = hrRc;
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY date FROM DISPLAY MESSAGE AND ASSIGN IT TO   Daterc   which is global variable
*/
void timeDt() 
{
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  int val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  //  ////Serial.println(str);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY   month    FROM DISPLAY MESSAGE AND ASSIGN IT TO   Monthrc   which is global variable
*/
void timeMnt() 
{
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  int val = cSmnt.indexOf("m") + 1; //    clkd24m2y2022h2n58s30
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); 
  Monthrc = mntRc;
}
/*
INSIDE TIME AND DATE PAGE ON PRESSING SAVE BUTTON. DISPLAY WILL SEND THE TIME AND DATE ENTERED BY USER IN DISPLAY TO AT MEGA. the format is clkd24m2y2022h2n58s30.
THIS FUNCTION IS USED TO READ ONLY   year    FROM DISPLAY MESSAGE AND ASSIGN IT TO   Yearrc   which is global variable
*/
void timeYr() 
{
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  int val = cSyr.indexOf("y") + 1; //    clkd24m2y2022h2n58s30
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); 
  Yearrc = yrRc;
}
/*
THIS FUNCTION WILL READ DATE AND TIME FROM RTC MODULE
*/
void read_date_time_rtc()
{
  if (!rtc.begin()) 
  {
    Serial.println("UNABLE TO FIND RTC MODULE, SO YOU CAN NOT READ DATE AND TIME FROM RTC MODULE");
    return;
  }
  // Read the current time from RTC
  DateTime now = rtc.now();
  Rtc_hour = now.hour();
  Rtc_min = now.minute();
  Rtc_sec = now.second();
  Rtc_day = now.day();
  Rtc_month = now.month();
  Rtc_year = now.year();
  Serial.println("DATE AND TIME HAS BEEN READ FROM RTC MODULE");
}
/*
THIS FUNCTION WILL UPDATE DATE AND TIME IN DISPLAY
*/
void update_display_time()
{
  Serial2.print("hourr.txt=\"");
  Serial2.print(Rtc_hour);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("min.txt=\"");
  Serial2.print(Rtc_min);//minupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("sec.txt=\"");
  Serial2.print(Rtc_sec);//secslive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("dayy.txt=\"");
  Serial2.print(Rtc_day);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

   
  Serial2.print("month.txt=\"");
  Serial2.print(Rtc_month);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  

  //Serial.print(Yearlive);
  Serial2.print("yearr.txt=\"");
  Serial2.print(Rtc_year);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
}
