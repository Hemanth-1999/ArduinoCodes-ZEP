//**************************_________NEXTION - RTC___________***********************/
void timeSec() {

  val = dfd.indexOf("s") + 1;
  dfd.remove(0, val);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
}

int timeMin() {
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  val = cSmn.indexOf("n") + 1; //    Serial.println(val);
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
  return (minRc);
}

void timeHr() {
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1; //    Serial.println(val);
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;
}

void timeDt() {
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
}

void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  //  Serial.println(cSmnt);
  val = cSmnt.indexOf("m") + 1; //    Serial.println(val);
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Monthrc = mntRc;
  //  Serial.print("Month: ");
  //  Serial.println(Monthrc);
}

void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //    Serial.println(val);
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Yearrc = yrRc;
}

void dateandtime() {
  DateTime now = rtc.now();
  hourupg   = now.hour();//hourupg
  minupg    = now.minute();//minupg
  secslive  = now.second();//secslive
  Datelive  = now.day();//Datelive
  Monthlive = now.month();//Monthlive
  Yearlive  = now.year();//Yearlive
}
