void timeSec() 
{
  int val = dfd.indexOf("s") + 1;
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}
void timeMin() 
{

  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  int val = cSmn.indexOf("n") + 1;
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
}
void timeHr() 
{
  char *chr = strtok(buff, "n");
  String cShr = chr;
  int val = cShr.indexOf("h") + 1;
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt();
  hourrc = hrRc;
}

void timeDt() 
{
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  int val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
}
void timeMnt() 
{
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;

  int val = cSmnt.indexOf("m") + 1;
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt();
  Monthrc = mntRc;

}
void timeYr() 
{
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  int val = cSyr.indexOf("y") + 1; //
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt();
  Yearrc = yrRc;
}

//****************************************************THIS FUNCTION IS USED TO SEND DATE AND TIME TO DISPLAY.
void update_display_time()
{
  Serial2.print("hourr.txt=\"");
  Serial2.print(Rtc_hour);//Rtc_hour
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("min.txt=\"");
  Serial2.print(Rtc_min);//Rtc_min
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

   
  Serial2.print("t0.txt=\"");
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
