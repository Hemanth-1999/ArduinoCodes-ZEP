void bt12PushCallback(void *ptr) {
  if (Serial3.available()) {

    DynamicJsonDocument VpdDoc(1024);
    DynamicJsonDocument VpdAlerts(1024);

    VpdDoc = VpdAlerts;

    if (!statusbt12) {
      statusbt12 = true;
      statusbt13 = false; // Ensure Temperature mode is off
      toggleState = true; // Ensure toggle state is consistent
      VPDMode();
      relayDelay.start(TOGGLE_TIME);
      vpd_mode = "1";
    } else {
      statusbt12 = false;
      relayDelay.stop();
      turnOffAllFans();
      vpd_mode = "0";
    }

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

    if (MESP_ID_Time_AvgVPD_v >= 0.1)
    {
      count1 --;


      if ((MESP_ID_Time_AvgVPD_v >= 0.0) && (MESP_ID_Time_AvgVPD_v <= 0.5))
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "8";

      }
      else if ((MESP_ID_Time_AvgVPD_v >= 0.6) && (MESP_ID_Time_AvgVPD_v <= 0.8))
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "9";

      }
      else if ((MESP_ID_Time_AvgVPD_v >= 1.5) && (MESP_ID_Time_AvgVPD_v <= 1.7))
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "10";

      }
      else if ((MESP_ID_Time_AvgVPD_v >= 1.8) && (MESP_ID_Time_AvgVPD_v <= 2.0))
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "9";

      }
      else if (MESP_ID_Time_AvgVPD_v >= 2.1)
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "8";

      }
      else if ((MESP_ID_Time_AvgVPD_v >= 0.9) && (MESP_ID_Time_AvgVPD_v <= 1.4))
      {

        VpdAlerts["Mode"] = "7";
        VpdAlerts["level"] = "11";

      }
    }

    Serial.println(RelayStatus);
    Serial.println("RelayStatus");

    serializeJson(VpdDoc, RelayStatus);
    serializeJson(VpdDoc, Serial);
    serializeJson(VpdDoc, Serial3);

  }
}

void bt13PushCallback(void *ptr) {
  if (Serial3.available()) {

    DynamicJsonDocument TempDoc(1024);
    DynamicJsonDocument Alerts(1024);

    TempDoc = Alerts;

    if (!statusbt13) {
      statusbt13 = true;
      statusbt12 = false; // Ensure VPD mode is off
      toggleState = true; // Ensure toggle state is consistent
      TempMode();
      relayDelay.start(TOGGLE_TIME);
      temp_mode = "1";
    } else {
      statusbt13 = false;
      relayDelay.stop();
      turnOffAllFans();
      temp_mode = "0";
    }

    if (count3 == 0)
    {
      StaticJsonDocument<200> clearr1;

      char tempstring2[500];

      serializeJson(clearr1, tempstring2);
      memset(tempstring2, 0, 500);

      count3 = 10;
    }

    if (MESP_ID_Time_AvgAT_v > 0)
    {
      count3--;

      if ((MESP_ID_Time_AvgAT_v >= 36) && (MESP_ID_Time_AvgAT_v <= 38))
      {

        Alerts["Mode"] = "12";
        Alerts["level"] = "10";

      }
      else if ((MESP_ID_Time_AvgAT_v >= 39) && (MESP_ID_Time_AvgAT_v <= 41))
      {

        Alerts["Mode"] = "12";
        Alerts["level"] = "9";

      }
      else if (MESP_ID_Time_AvgAT_v > 42)
      {

        Alerts["Mode"] = "12";
        Alerts["level"] = "8";

      }
      else if (MESP_ID_Time_AvgAT_v <= 35)
      {

        Alerts["Mode"] = "12";
        Alerts["level"] = "11";

      }
    }

    Serial.println(RelayStatus);
    Serial.println("RelayStatus");

    serializeJson(TempDoc, RelayStatus);
    serializeJson(TempDoc, Serial);
    serializeJson(TempDoc, Serial3);

  }
}
