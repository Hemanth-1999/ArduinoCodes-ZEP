void funtimer()
{
  eventmode();
}
void eventmode()
{
  StaticJsonDocument<500> doc1;

  if (TemperatureAutoModeOn == true)
  {

    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["TEMP"] = "3";

    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);
  }

  if (VpdautoModeOn == true)
  {
    JsonObject EVENTSTATUS = doc1.createNestedObject("Events");
    EVENTSTATUS["VPD"] = "4";
    serializeJson(doc1, Serial);
    serializeJson(doc1, Serial3);

  }
}


/*
THIS FUNCTION WILL TAKE AN ARGUMENT OF FLOAT TYPE SPtemp.
THIS SPtemp VALUE IS INITIALLY ZEOR INITIALLY, WHEN USER ENTERS ANY VALUE IN INDOOR TEMPERATURE IN DISPLAY IN TEMPERATUR PAGE AND PRESS SET. SPtemp VARIABLE WILL BE UPDATED.
IT DOES NOT CARE ABOUT ON/OFF BUTTON STATUS IN TEMPERATURE PAGE.(i dont konw 100%)

THIS FUNCTION WILL TAKE TEMPRATURE ENTERED ON DISPLAY(INDOOR TEMP).
IT WILL ADD 1 TO IT AND STORE IN ATempHysteris.
NOW IT WILL ADD 2 TO TEMPRATURE ENTERED ON DISPLAY(INDOOR TEMP) AND STORE IT IN ATempLow
NOW IT WILL ADD 4 TO TEMPRATURE ENTERED ON DISPLAY(INDOOR TEMP) AND STORE IT IN ATempMid
NOW IT WILL ADD 6 TO TEMPRATURE ENTERED ON DISPLAY(INDOOR TEMP) AND STORE IT IN ATemphIGH
float ATemp = _AT(THE INDOOR NODE TEMPERATURE SENT BY ESP8266)

NOW THIS FUNCTION DEFINED FEW CONDITIONS:

  1)THE INDORE NODE TEMPERATURS SEND BY ESP8266 MUST NOT BE ZERO. IT CAN BE POSITIVE NUBER OR NEGATIVE NUMBER BUT NOT EQUAL TO ZERO.
    IF IT IS EQUAL TO ZERO THEN FUNCTION WILL END.
    1.1)THE TEMP BY ESP8266 IS GRATER THAN TEMP IN DISPLAY +1(ATempHysteris)
        1.1.1)THE TEMP BY ESP8266 IS GREATER THAN THE TEMP IN DISPLAY +6(ATempHigh)
              IF CONDITION 1.1.1) IS FALSE THEN IT WILL CHECK NEXT CONDITION.
        1.1.2)THE TEMP BY ESP8266 IS GREATER THAN THE TEMP IN DISPLAY +4(ATempMid) AND LESS THE TEMP IN DISPLAY+6(ATempHigh)
              IF CONDITION 1.1.2) IS FALSE THEN IT WILL CHECK NEXT CONDITION.
        1.1.3)THE TEMP BY ESP8266 IS GREATER THAN THE TEMP IN DISPLAY +2(ATempLow) AND LESS THE TEMP IN DISPLAY+4(ATempMid) AND LESS THE TEMP IN DISPLAY+6(ATempHigh)
              IF CONDITION 1.1.3) IS FALSE THEN IT WILL CHECK NEXT CONDITION.
        1.1.4)THE TEMP BY ESP8266 IS GREATER THAN THE TEMP IN DISPLAY (SPtemp) AND LESS THAN ATempHigh,ATempMid,ATempLow
              IF CONDITION 1.1.4) IS FALSE THEN THERE ARE NO OTHERE CONDITIONS IT WILL GO TO NEXT STATEMENT THAT IS 1.2).
    IF CONDITION 1.1) IS FALSE THEN THERE IS NO ELSE CASE IT WILL GO FOR NEXT STATEMENT
        
    1.2)THE TEMP BY ESP8266 IS LESS THAN TEMP IN DISPLAY (SPtemp)
*/
void AutoTemp_control(float SPtemp) 
{
  StaticJsonDocument <500> doc3;

  if (count3 == 0)
  {
    StaticJsonDocument<200> clearr1;
    doc3 = clearr1;
    char tempstring2[500];
    serializeJson(clearr1, tempstring2);
    memset(tempstring2, 0, 500);

    count3 = 10;
  }


  float ATempHysteris = SPtemp + 1;

  float ATemp = _AT;
  
  if (ATemp > 0)
  {
    count3--;


    if ((ATemp >= 36) && (ATemp <= 38))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if ((ATemp >= 39) && (ATemp <= 41))
    {




      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);



    }
    else if (ATemp > 42)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

    }
    else if (ATemp <= 35)
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "12";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




    }
  }
  Serial.println(ATemp);

  Serial.println(SPtemp);

  Serial.println(ATempHysteris);
  if (ATemp != 0) 
  {

    if ( ATemp >= ATempHysteris) 
    {  // 33>=26  //31.80>=26w


      Serial.println(" we need  fans to turn on");
      Serial.println(ATemp);
      //    Serial.println(ATempHysteris);
      float ATempLow =  ATempHysteris + 2; //26
      float ATempMid = ATempLow + 2; //28
      float ATempHigh = ATempMid + 2; //30



      Serial.println("Modes");
      Serial.println(ATempHigh);
      Serial.println(ATempMid);
      Serial.println(ATempLow);
      if (ATemp >= ATempHigh) //32<31                 // Temp=33    32<33//31.80>=32 fail
      {
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))//THIS IS TRUE IF MILLIS IS GREATER THAN ZERO AND LESS THAN OR EQUAL TO 20 MINUTES
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, LOW);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);

          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))//TRUE IF MILLIS IS GREATER THAN 20MINUTES AND LESS THAN 40 MINUTES.
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, LOW);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, LOW);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, HIGH);
        }
      }
      else if ((ATemp >= ATempMid) && (ATemp < ATempHigh)) //31>30&& 31<32        //Temp=31  31>30  && 31<32//31.80>=30 && 31.80<32
      {

        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 60000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 61000) && (mills <= 120000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, LOW);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);

        }
      }
      else  if ((ATemp >= ATempLow) && (ATemp < ATempMid)) //Temp=29 //31.80>=28 && 31.80<30
      {
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
      }
      else if ((ATemp >= SPtemp) && (ATemp < ATempLow)) 
      { //Temp=27 31.80>=26 31.80<28
        unsigned long mills = millis();
        if ((mills >= 0) && (mills <= 1200000))
        {
          Serial.println(" FIRST STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, HIGH);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, LOW);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, LOW);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, HIGH);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
        else if ((mills >= 1205000) && (mills <= 2400000))
        {
          Serial.println(" SECOND STATE TIME ");
          Serial.println( mills );
          pinMode(ExFan1, OUTPUT);
          digitalWrite(ExFan1, HIGH);
          pinMode(ExFan2, OUTPUT);
          digitalWrite(ExFan2, HIGH);
          pinMode(ExFan3, OUTPUT);
          digitalWrite(ExFan3, LOW);
          pinMode(ExFan4, OUTPUT);
          digitalWrite(ExFan4, HIGH);
          pinMode(ExFan5, OUTPUT);
          digitalWrite(ExFan5, HIGH);
          pinMode(ExFan6, OUTPUT);
          digitalWrite(ExFan6, HIGH);
          pinMode(ExFan7, OUTPUT);
          digitalWrite(ExFan7, LOW);
          pinMode(ExFan8, OUTPUT);
          digitalWrite(ExFan8, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, LOW);
        }
      }
    }

    if (ATemp < SPtemp) 
    {
      //31.80<26
      Serial.println(ATemp);
      Serial.println("All OFF");
      //relayState = HIGH;
      pinMode(ExFan1, OUTPUT);
      digitalWrite(ExFan1, HIGH);
      pinMode(ExFan2, OUTPUT);
      digitalWrite(ExFan2, HIGH);
      pinMode(ExFan3, OUTPUT);
      digitalWrite(ExFan3, HIGH);
      pinMode(ExFan4, OUTPUT);
      digitalWrite(ExFan4, HIGH);
      pinMode(ExFan5, OUTPUT);
      digitalWrite(ExFan5, HIGH);
      pinMode(ExFan6, OUTPUT);
      digitalWrite(ExFan6, HIGH);
      pinMode(ExFan7, OUTPUT);
      digitalWrite(ExFan7, HIGH);
      pinMode(ExFan8, OUTPUT);
      digitalWrite(ExFan8, HIGH);
      pinMode(pad1, OUTPUT);
      digitalWrite(pad1, HIGH);
    }
  }
}
