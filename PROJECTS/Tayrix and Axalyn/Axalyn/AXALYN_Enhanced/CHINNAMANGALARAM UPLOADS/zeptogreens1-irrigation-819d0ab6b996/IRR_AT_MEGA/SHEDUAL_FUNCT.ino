void scheduleFun() 
{


  if (checkflag == true)
  {
    Serial.println(sch1PSThr);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin);                //Shift1Clockmin();

    Serial.println(sch1PSThr1);                  //Shift1Clockhr();

    Serial.println(sch1PSTmin1);

    Serial.println(Rtc_hour);
    Serial.println(Rtc_min);


    int S1Hr = sch1PSThr;
    int S1Min = sch1PSTmin;
    int S2Hr = sch1PSThr1;
    int S2Min = sch1PSTmin1;



    read_date_time_rtc();


    if ((Rtc_hour == S1Hr)  &&  (Rtc_min  ==  S1Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((Rtc_hour == S2Hr)  &&  (Rtc_min  ==  S2Min))
    {

      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);

    }
  }
  if (checkflag1 == true)
  {




    int S3Hr = sch2PSThr;
    int S3Min = sch2PSTmin;
    int S4Hr = sch2PSThr1;
    int S4Min = sch2PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S3Hr)  &&  (Rtc_min  ==  S3Min)) 
    {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((Rtc_hour == S4Hr)  &&  (Rtc_min  ==  S4Min)) 
    {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);
    }
  }


  if (checkflag2 == true)
  {



    int S5Hr = sch3PSThr;
    int S5Min = sch3PSTmin;
    int S6Hr = sch3PSThr1;
    int S6Min = sch3PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S5Hr)  &&  (Rtc_min  ==  S5Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, LOW);
    }
    else if ((Rtc_hour == S6Hr)  &&  (Rtc_min  ==  S6Min)) {
      pinMode(Z1PUMPA, OUTPUT);
      digitalWrite(Z1PUMPA, HIGH);
    }


  }


  if (checkflag3 == true)
  {



    int S7Hr = sch4PSThr;
    int S7Min = sch4PSTmin;
    int S8Hr = sch4PSThr1;
    int S8Min = sch4PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S7Hr)  &&  (Rtc_min  ==  S7Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((Rtc_hour == S8Hr)  &&  (Rtc_min  ==  S8Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }

  if (checkflag4 == true)
  {



    int S9Hr = sch5PSThr;
    int S9Min = sch5PSTmin;
    int S10Hr = sch5PSThr1;
    int S10Min = sch5PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S9Hr)  &&  (Rtc_min  ==  S9Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((Rtc_hour == S10Hr)  &&  (Rtc_min  ==  S10Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }

  if (checkflag5 == true)
  {



    int S11Hr = sch6PSThr;
    int S11Min = sch6PSTmin;
    int S12Hr = sch6PSThr1;
    int S12Min = sch6PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S11Hr)  &&  (Rtc_min  ==  S11Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, LOW);
    }
    else if ((Rtc_hour == S12Hr)  &&  (Rtc_min  ==  S12Min)) {
      pinMode(Z1SOLVALVE, OUTPUT);
      digitalWrite(Z1SOLVALVE, HIGH);
    }


  }
  if (checkflag6 == true)
  {



    int S13Hr = sch7PSThr;
    int S13Min = sch7PSTmin;
    int S14Hr = sch7PSThr1;
    int S14Min = sch7PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S13Hr)  &&  (Rtc_min  ==  S13Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((Rtc_hour == S14Hr)  &&  (Rtc_min  ==  S14Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }


  }
  if (checkflag7 == true)
  {



    int S15Hr = sch8PSThr;
    int S15Min = sch8PSTmin;
    int S16Hr = sch8PSThr1;
    int S16Min = sch8PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S15Hr)  &&  (Rtc_min  ==  S15Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((Rtc_hour == S16Hr)  &&  (Rtc_min  ==  S16Min)) {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }


  }
  if (checkflag8 == true)
  {



    int S17Hr = sch9PSThr;
    int S17Min = sch9PSTmin;
    int S18Hr = sch9PSThr1;
    int S18Min = sch9PSTmin1;


    read_date_time_rtc();

    if ((Rtc_hour == S17Hr)  &&  (Rtc_min  ==  S17Min)) 
    {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, LOW);
    }
    else if ((Rtc_hour == S18Hr)  &&  (Rtc_min  ==  S18Min)) 
    {
      pinMode(Z1AIRPUMP, OUTPUT);
      digitalWrite(Z1AIRPUMP, HIGH);
    }
  }
}
