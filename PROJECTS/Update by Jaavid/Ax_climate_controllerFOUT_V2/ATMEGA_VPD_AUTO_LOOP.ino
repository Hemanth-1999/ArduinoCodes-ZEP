//***************************************************VPD AUTO MODE***********************************************
void vpd_control(float MESP_ID_Time_AvgVPD_v)
{
  StaticJsonDocument <500> doc3;

  if (count1 == 0)
  {

    StaticJsonDocument<200> clearr;
    doc3 = clearr;
    char tempstring1[500];
    serializeJson(clearr, tempstring1);
    memset(tempstring1, 0, 500);

    count1 = 20;
  }
  
  float vpd = MESP_ID_Time_AvgVPD_v;
 
  if (MESP_ID_Time_AvgVPD_v >= 0.1)
  {
    count1 --;


    if ((MESP_ID_Time_AvgVPD_v >= 0.0) && (MESP_ID_Time_AvgVPD_v <= 0.5))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";

      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


    }
    else if ((MESP_ID_Time_AvgVPD_v >= 0.6) && (MESP_ID_Time_AvgVPD_v <= 0.8))
    {

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.5) && (MESP_ID_Time_AvgVPD_v <= 1.7))
    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "10";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);
    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.8) && (MESP_ID_Time_AvgVPD_v <= 2.0))

    {


      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "9";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);




    }
    else if (MESP_ID_Time_AvgVPD_v >= 2.1)
    {

      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "8";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);


    }

    else if ((MESP_ID_Time_AvgVPD_v >= 0.9) && (MESP_ID_Time_AvgVPD_v <= 1.4))
    {
      JsonObject Alerts = doc3.createNestedObject("Alerts");
      Alerts["Mode"] = "7";
      Alerts["level"] = "11";
      serializeJson(doc3, Serial);
      serializeJson(doc3, Serial3);

    }
  }
  Serial.println(MESP_ID_Time_AvgVPD_v);
  Serial.println("VPD SETPOINT");


  if (MESP_ID_Time_AvgVPD_v != 0) 
  {
    if (MESP_ID_Time_AvgVPD_v < 0.3) //0.41<=0.4 // relayState = LOW; // turn it on
    { 
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
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

      }
      else if ((mills >= 1205000) && (mills <= 2400000))
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


      }

    }


    else if ((MESP_ID_Time_AvgVPD_v > 0.31)  &&   (MESP_ID_Time_AvgVPD_v <= 0.49)) { //0.41<=0.4
      // relayState = LOW; // turn it on


      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
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
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
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
      }
    }
    else if ((MESP_ID_Time_AvgVPD_v > 0.21) && (MESP_ID_Time_AvgVPD_v < 0.30)) {

      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
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
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
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
      }
    }
    else if ((MESP_ID_Time_AvgVPD_v > 0.01) && (MESP_ID_Time_AvgVPD_v < 0.20)) {


      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
      {
        Serial.println(" FIRST STATE TIME ");
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
      }
      else if ((mills >= 1205000) && (mills <= 2400000))
      {
        Serial.println(" SECOND STATE TIME ");
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
      }
    }

    else if ((MESP_ID_Time_AvgVPD_v >= 1.51) && (MESP_ID_Time_AvgVPD_v <= 1.69)) {
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
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
      else if ((mills >= 1205000) && (mills <= 2400000))
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
        digitalWrite(pad1, LOW);
      }
    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.70) && (MESP_ID_Time_AvgVPD_v <= 1.89)) {

      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
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
      else if ((mills >= 1205000) && (mills <= 2400000))
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
        digitalWrite(pad1, LOW);
      }

    }
    else if ((MESP_ID_Time_AvgVPD_v >= 1.90)) {
      unsigned long mills = millis();
      if ((mills >= 0) && (mills <= 1200000))
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
      else if ((mills >= 1205000) && (mills <= 2400000))
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
        digitalWrite(pad1, LOW);

      }

    }

    else if (MESP_ID_Time_AvgVPD_v >= 0.5 && (MESP_ID_Time_AvgVPD_v <= 1.5))
    {

      Serial.println("nothing to do");
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
