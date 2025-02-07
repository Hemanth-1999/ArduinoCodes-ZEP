void SerialCom() 
{
  String data_from_display = "";
  delay(50);
  while (Serial2.available()) 
  {
    data_from_display += char(Serial2.read());
  }
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());//IF WE NEED TO USE CHARACTER ARRAY THEN USE THIS.
  
  //************************************* AUTO MODE **********************************************************************
  if((data_from_display.substring(0, 5) == "saoz1"))
  {
    Z1PAsavetime();//SAVES ON TIME
    Z1PAsavedue();// SAVES OFF TIME
    Serial.print("othr: ");Serial.println(Z1PASThr);//STORES ON TIME HOUR
    Serial.print("otMin: ");Serial.println(Z1PASTmin);//STORES ON TIME MINUTES
    Serial.print("oftHr: ");Serial.println(Z1PASPhr);//STORES OFF TIME HOURES
    Serial.print("oftMin: ");Serial.println(Z1PASPmin);//STORES OFF TIME IN MINUTES
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 6) == "slado")) 
  {
    Z1PAsavetimeq();
    Z1PAsavedueq();
    Serial.print("othr: ");Serial.println(Z1PASThrq);
    Serial.print("otMin: ");Serial.println(Z1PASTminq);

    Serial.print("oftHr: ");Serial.println(Z1PASPhrq);
    Serial.print("oftMin: ");Serial.println(Z1PASPminq);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "sladl")) 
  {
    Z1PAsavetimeqq();
    Z1PAsavedueqq();
    Serial.print("othr: ");Serial.println(Z1PASThrqq);
    Serial.print("otMin: ");Serial.println(Z1PASTminqq);

    Serial.print("oftHr: ");Serial.println(Z1PASPhrqq);
    Serial.print("oftMin: ");Serial.println(Z1PASPminqq);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "slaz1")) 
  {
    Z1PBsavetime();
    Z1PBsavedue();
    Serial.println(Z1PBSThr);
    Serial.println(Z1PBSTmin);
    Serial.println(Z1PBSPhr);
    Serial.println(Z1PBSPmin);
    feedBack = "saved";
  }
  //****************************************************PUMP DO****************************************
  if ((data_from_display.substring(0, 5) == "sdoz1")) 
  {
    Z1APsavetime();
    Z1APsavedue();
    Serial.println(Z1APSThr);
    Serial.println(Z1APSTmin);
    Serial.println(Z1APSPhr);
    Serial.println(Z1APSPmin);
    feedBack = "saved";
  }
  ////////////////////////IRRIGATION B////
  if ((data_from_display.substring(0, 5) == "slbz1")) 
  {
    LBPAsavedue();
    LBPAsavetime();
    Serial.println("SAVE IRR B Time");
    Serial.println(LBPASThr);
    Serial.println(LBPASTmin);
    Serial.println(LBPASPhr);
    Serial.println(LBPASPmin);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 4) == "A s1")) 
  {
    sch1savetime();
    sch1savedue();
    checkflag = true;
    Serial.println("slot 1 schedule time");
    Serial.println(sch1PSThr);
    Serial.println(sch1PSTmin);
    Serial.println(sch1PSThr1);
    Serial.println(sch1PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "az1s2"))
  {
    sch2savetime();
    sch2savedue();
    checkflag1 = true;
    Serial.println("slot 2 schedule time");

    Serial.println(sch2PSThr);

    Serial.println(sch2PSTmin);

    Serial.println(sch2PSThr1);

    Serial.println(sch2PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "az1s3"))
  {
    sch3savetime();
    sch3savedue();
    checkflag2 = true;
    Serial.println("slot 3 schedule time");
    Serial.println(sch3PSThr);
    Serial.println(sch3PSTmin);
    Serial.println(sch3PSThr1);
    Serial.println(sch3PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss1"))
  {
    sch4savetime();
    sch4savedue();
    checkflag3 = true;
    Serial.println("slot 4 schedule time");
    Serial.println(sch4PSThr);
    Serial.println(sch4PSTmin);
    Serial.println(sch4PSThr1);
    Serial.println(sch4PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss2"))
  {
    sch5savetime();
    sch5savedue();
    checkflag4 = true;
    Serial.println("slot 5 schedule time");
    Serial.println(sch5PSThr);
    Serial.println(sch5PSTmin);
    Serial.println(sch5PSThr1);
    Serial.println(sch5PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lbss3"))
  {
    sch6savetime();
    sch6savedue();
    checkflag5 = true;
    Serial.println("slot 6 schedule time");
    Serial.println(sch6PSThr);
    Serial.println(sch6PSTmin);
    Serial.println(sch6PSThr1);
    Serial.println(sch6PSTmin1);
    feedBack = "saved";
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if ((data_from_display.substring(0, 5) == "lass1"))
  {
    sch7savetime();
    sch7savedue();
    checkflag6 = true;
    Serial.println("slot 7 schedule time");
    Serial.println(sch7PSThr);
    Serial.println(sch7PSTmin);
    Serial.println(sch7PSThr1);
    Serial.println(sch7PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass2"))
  {
    sch8savetime();
    sch8savedue();
    checkflag7 = true;
    Serial.println("slot 8 schedule time");
    Serial.println(sch8PSThr);
    Serial.println(sch8PSTmin);
    Serial.println(sch8PSThr1);
    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  if ((data_from_display.substring(0, 5) == "lass3"))
  {
    sch8savetime();
    sch8savedue();
    checkflag8 = true;
    Serial.println("slot 9 schedule time");
    Serial.println(sch8PSThr);
    Serial.println(sch8PSTmin);
    Serial.println(sch8PSThr1);
    Serial.println(sch8PSTmin1);
    feedBack = "saved";
  }
  //***************************************WIFI CREDENTIALS******************
  if ((dfd.substring(0, 4) == "sdpd")) 
  {
    ssid = pwdname();
    Serial.print("User Name: ");
    Serial.println(ssid);
    password = pwd();
    Serial.print("Password: ");
    Serial.println(dfd);
    dfd = password;
    feedBack = "sdpd";
  }
  /**************************    NEXTION - RTC   ********************/
  if ((data_from_display.substring(0, 3) == "clk")) 
  {
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();
    update_rtc_time();
    feedBack = "clk";
  }
  //****************************************************SET DATE AND TIME***********************
  if ((data_from_display.substring(0, 6) == "settc")) 
  {
    update_rtc_time();
  }
  //*************************************************MANUAL MODE********************************************
  if (dfd == "pAz1On")
  {
    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, LOW);
    feedBack = "pAz1On";
    ZAApumpSt = "on";
    Serial.println("pump IRR is on");
  }
  if (dfd == "pAz1Off")
  {
    pinMode(Z1PUMPA, OUTPUT);
    digitalWrite(Z1PUMPA, HIGH);
    feedBack = "pAz1Off";
    Serial.println("pump  IRR is off");
    ZAApumpSt = "off";
  }

  if (dfd == "pdoaon")
  {
    pinMode(DOOA, OUTPUT);
    digitalWrite(DOOA, LOW);
    Serial.println("pump DOA is on");
  }
  if (dfd == "pdoaoff")
  {
    pinMode(DOOA, OUTPUT);
    digitalWrite(DOOA, HIGH);
    Serial.println("pump  doa is off");
  }
  if (dfd == "pdbon")
  {
    pinMode(DOOB, OUTPUT);
    digitalWrite(DOOB, LOW);
    Serial.println("pump DOB is on");
  }
  if (dfd == "pdboff")
  {
    pinMode(DOOB, OUTPUT);
    digitalWrite(DOOB, HIGH);
    Serial.println("pump DOB is off");
  }
  if (dfd == "pBz1On")
  {
    pinMode(Z1AIRPUMP, OUTPUT);
    digitalWrite(Z1AIRPUMP, LOW);
    Serial.println("Pump DO is on");
    feedBack = "pBz1On";
    ZBApumpSt = "on";
  }
  if (dfd == "pBz1Off")
  {
    pinMode(Z1AIRPUMP, OUTPUT);
    digitalWrite(Z1AIRPUMP, HIGH);

    Serial.println("Pump DO off");
    feedBack = "pBz1Off";
    ZBApumpSt = "off";
  }

  if (dfd == "laon")
  {
    pinMode(Z1PUMPB, OUTPUT);
    digitalWrite(Z1PUMPB, LOW);
    Serial.println("LIGHT A IS ON");
    feedBack = "laon";
    ZAAirSPumpSt = "on";
  }
  if (dfd == "laoff")
  {
    pinMode(Z1PUMPB, OUTPUT);
    digitalWrite(Z1PUMPB , HIGH);
    Serial.println("LIGHT A IS Off");
    feedBack = "laoff";
    ZAAirSPumpSt = "off";
  }

  if (dfd == "lbon")
  {
    pinMode(Z1SOLVALVE, OUTPUT);
    digitalWrite(Z1SOLVALVE, LOW);
    Serial.println("LIGHT B IS ON");
    feedBack = "lbon";
    ZAROSt = "on";
  }
  if (dfd == "lboff")
  {
    pinMode(Z1SOLVALVE, OUTPUT);
    digitalWrite(Z1SOLVALVE, HIGH);
    Serial.println("LIGHT B IS OFF");
    feedBack = "lboff";
    ZAROSt = "off";
  }
  if (dfd == "z1aon")
  {


    Serial.print("othr: ");
    Serial.println(Z1PASThr);

    Serial.print("otMin: ");
    Serial.println(Z1PASTmin);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhr);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPmin);
    long Z1PASThrconvOn = Z1PASThr * ms_per_hour;
    long Z1PASTminconvOn = Z1PASTmin * ms_per_min;
    long PumpAOnTimetotelOn = Z1PASThrconvOn + Z1PASTminconvOn;


    long Z1PASThrconvoff = Z1PASPhr * ms_per_hour;
    long Z1PASTminconvoff = Z1PASPmin * ms_per_min;
    long PumpAOnTimetoteloff = Z1PASThrconvoff + Z1PASTminconvoff;


    PumpAZ1.begin(PumpAOnTimetotelOn , PumpAOnTimetoteloff );

    Serial.println("PUMPA Z1 On");
    PumpAZ1bool = true;
    EepromReadZ1AP();
    feedBack = "z1aon";
  }
  if (dfd == "z1aoff")
  {
    irroff();
    PumpAZ1bool = false;
    Serial.println("PUMP Z1  Off");
    feedBack = "z1aoff";
  }

  if (dfd == "z1aonq")
  {


    Serial.print("othr: ");
    Serial.println(Z1PASThrq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminq);
    long Z1PASThrconvOnq = Z1PASThrq * ms_per_hour;
    long Z1PASTminconvOnq = Z1PASTminq * ms_per_min;
    long PumpAOnTimetotelOnq = Z1PASThrconvOnq + Z1PASTminconvOnq;


    long Z1PASThrconvoffq = Z1PASPhrq * ms_per_hour;
    long Z1PASTminconvoffq = Z1PASPminq * ms_per_min;
    long PumpAOnTimetoteloffq = Z1PASThrconvoffq + Z1PASTminconvoffq;


    PumpAZ1q.begin(PumpAOnTimetotelOnq , PumpAOnTimetoteloffq );

    Serial.println("PUMP do On");
    PumpAZ1boolq = true;
    EepromReadZ1APq();
    feedBack = "z1aon";
  }
  if (dfd == "z1aoffq")
  {

    PumpAZ1boolq = false;
    Serial.println("PUMP do Off");
    feedBack = "z1aoff";
  }


  if (dfd == "z1aonqq")
  {

    Serial.print("othr: ");
    Serial.println(Z1PASThrqq);

    Serial.print("otMin: ");
    Serial.println(Z1PASTminqq);

    Serial.print("oftHr: ");
    Serial.println(Z1PASPhrqq);

    Serial.print("oftMin: ");
    Serial.println(Z1PASPminqq);
    long Z1PASThrconvOnqq = Z1PASThrqq * ms_per_hour;
    long Z1PASTminconvOnqq = Z1PASTminqq * ms_per_min;
    long PumpAOnTimetotelOnqq = Z1PASThrconvOnqq + Z1PASTminconvOnqq;


    long Z1PASThrconvoffqq = Z1PASPhrqq * ms_per_hour;
    long Z1PASTminconvoffqq = Z1PASPminqq * ms_per_min;
    long PumpAOnTimetoteloffqq = Z1PASThrconvoffqq + Z1PASTminconvoffqq;


    PumpAZ1qq.begin(PumpAOnTimetotelOnqq , PumpAOnTimetoteloffqq );

    Serial.println("PUMPA do2 On");
    PumpAZ1boolqq = true;
    EepromReadZ1APqq();

  }
  if (dfd == "z1aoff")
  {
    PumpAZ1boolqq = false;
    Serial.println("PUMP do2  Off");
    feedBack = "z1aoff";
  }
  //*************************************************LIGHT A **********************************
  if (dfd == "alaon") {
    Serial.println("LIGHT A");

    Serial.println(Z1PBSThr);

    Serial.println(Z1PBSTmin);

    Serial.println(Z1PBSPhr);

    Serial.println(Z1PBSPmin);
    long Z1PBSThrconvOn = Z1PBSThr * ms_per_min;
    long Z1PBSTminconvOn = Z1PBSTmin * ms_per_sec;
    long PumpBOnTimetotelOn = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1PBSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1PBSPmin * ms_per_min;
    long PumpBOnTimetoteloff = Z1PBSThrconvoff + Z1PBSTminconvoff;

    PumpBZ1.begin(PumpBOnTimetotelOn , PumpBOnTimetoteloff);
    PumpBZ1bool = true;
    Serial.println("LIGHT A ON");
    EepromReadZ1PB();

    feedBack = "alaon";
  }
  if (dfd == "alaoff") 
  {
    dripoff();
    PumpBZ1bool = false;
    Serial.println("LIGHT A OFF ");
    feedBack = "alaoff";
  }
  //////////////////////////////////////// PUMP DO////////////////////////////////////////////
  if (dfd == "z1don") 
  {

    Serial.println(Z1APSThr);                  //Shift1Clockhr();

    Serial.println(Z1APSTmin);                //Shift1Clockmin();

    Serial.println(Z1APSPhr);                  //Shift1Clockhr();

    Serial.println(Z1APSPmin);
    long Z1PBSThrconvOn = Z1APSThr * ms_per_hour;
    long Z1PBSTminconvOn = Z1APSTmin * ms_per_min;
    long PumpDOOnTime = Z1PBSThrconvOn + Z1PBSTminconvOn;

    long Z1PBSThrconvoff = Z1APSPhr * ms_per_hour;
    long Z1PBSTminconvoff = Z1APSPmin * ms_per_min;
    long PumpDOOffTime = Z1PBSThrconvoff + Z1PBSTminconvoff;

    PumpDOZ1.begin(PumpDOOnTime , PumpDOOffTime);
    PumpDOZ1bool = true;
    Serial.println("PUMP DO ON");

    EepromReadZ1Air();

    feedBack = "z1don";
  }
  if (dfd == "z1doff") 
  {
    dooff();
    PumpDOZ1bool = false;

    Serial.println("PUMP DO Off");
    feedBack = "z1doff";
  }

  ///////////////////////////////////////LIGHT B////////////////////////////////////
  if (dfd == "albon") {

    Serial.println(LBPASThr);                  //Shift1Clockhr();

    Serial.println(LBPASTmin);                //Shift1Clockmin();

    Serial.println(LBPASPhr);                  //Shift1Clockhr();

    Serial.println(LBPASPmin);
    long z1LBSThrconvOn = LBPASThr * ms_per_hour;
    long z1LBSTminconvOn = LBPASTmin * ms_per_min;
    PumpLBOnTime = z1LBSThrconvOn + z1LBSTminconvOn;
    Serial.println("LBPASPmin");
    Serial.println(LBPASPmin);
    long z1LBSThrconvoff = LBPASPhr * ms_per_hour;

    long z1LBSTminconvoff = LBPASPmin * ms_per_min;
    PumpLBOffTime = z1LBSThrconvoff + z1LBSTminconvoff;
    Serial.println("PumpLBOnTime");                  //Shift1Clockhr();

    Serial.println(PumpLBOnTime);                //Shift1Clockmin();

    Serial.println("PumpLBOffTime");                  //Shift1Clockhr();

    Serial.println(PumpLBOffTime);

    PumpLB.begin(PumpLBOnTime , PumpLBOffTime);
    PumpLBbool = true;
    Serial.println("LIGHT B ON");


    feedBack = "albon";
  }

  if (dfd == "alboff")
  {
    irrboff();
    PumpLBbool = false;

    Serial.println("LIGHT B Off");
    feedBack = "alboff";
  }
}
