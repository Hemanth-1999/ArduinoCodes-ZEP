///*****************************************SHEDULER PH AND EC *****************************************************
//!******************************************************************************
//! Name: SheduleDoser()                               *
//! Description: For Doser Shedule                     *
//! Param : DATE /TIME                                      *
//! Returns: void                                     *
//! Example:SheduleDoser(float pHAutoSetValue, float pHHys, float ECAutoSetValue, float ECHys )               *
//!******************************************************************************

void SheduleDoser(float pHAutoSetValue, float pHHys, float ECAutoSetValue, float ECHys ) {
  digitalWrite(LEDGREEN, HIGH);
  digitalWrite(TPUMPR, LOW);//h
  digitalWrite(MIXERR, LOW);

  pHSetpoint = pHAutoSetValue ;
  phSetHysteris = pHHys;
  pH = roundDecimalPoint(phValue, 2);

  ECSetpoint = ECAutoSetValue;
  ECHys = ECSetHysteris;
  EC = roundDecimalPoint(ecValue, 2);
  float HysterisMin = (pHSetpoint - phSetHysteris);
  float HysterisPlus = (pHSetpoint + phSetHysteris);
  ECHysterisMin = (ECSetpoint - ECSetHysteris);
  ECHysterisPlus = (ECSetpoint + ECSetHysteris);

  if (StopPHHys == false) {
    if (pH == pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }

        phDown = 0;
        phUp = 0;
        pinTime = pinLowTime;
      }
    }

    if (pH < HysterisMin) {
      digitalWrite(TPUMPR, LOW);//h
      digitalWrite(MIXERR, LOW);
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
      }
    }

    if (pH >= HysterisMin && pH < pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        digitalWrite(PHHIGH_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          //digitalWrite(PHLOW_STEP_PIN, HIGH);
          digitalWrite(PHHIGH_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(PHHIGH_STEP_PIN, LOW);
          delayMicroseconds(Speed);

          phUp = 1;
          phDown = 0;
          pinTime = pinHighTime;
        }
      }
    }

    if (pH > HysterisPlus) {
      digitalWrite(TPUMPR, LOW);//h
      digitalWrite(MIXERR, LOW);
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHLOW_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          digitalWrite(PHLOW_STEP_PIN, HIGH);
          //digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);

          phDown = 1;
          phUp = 0;
          pinTime = pinLowTime;
        }
      }
    }

    if (pH <= HysterisPlus && pH > pHSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        digitalWrite(PHLOW_DIR_PIN, LOW);
        //digitalWrite(PHLOW_DIR_PIN, LOW);
        for (int X = 0; X < 1000; X++) {
          digitalWrite(PHLOW_STEP_PIN, HIGH);
          //digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(PHLOW_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          phUp = 0;
          phDown = 1;
          pinTime = pinLowTime;
        }
      }
    }

    //********************EC DOSER ******************************
    if ((pH >= HysterisMin && pH <= HysterisPlus)) {
      if (EC == ECSetpoint) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis > pinTime) {
          previousMillis = currentMillis;
          phDown = 0;
          phUp = 0;
          pinTime = pinLowTime;
        }
      }
    }

    if (EC >= ECHysterisMin && EC <= ECHysterisPlus) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }

        Serial.println("EC EQL ");
        phUp = 0;
        phDown = 0;
        pinTime = pinLowTime;
      }
    }

    if (EC >= ECHysterisMin && EC < ECSetpoint) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;

        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }

        Serial.println("EC hys min EQL ");
        phUp = 1;
        phDown = 0;
        pinTime = pinHighTime;
      }
    }

    if (EC < ECHysterisMin) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        int ECRA =  ECRatioA * 230;
        digitalWrite(EC_A_DIR_PIN, LOW);
        for (int X = 0; X < ECRA; X++) {
          digitalWrite(EC_A_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(EC_A_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        int ECRB =  ECRatioB * 230;
        digitalWrite(EC_B_DIR_PIN, LOW);

        for (int X = 0; X < ECRB; X++) {
          digitalWrite(EC_B_STEP_PIN, HIGH);
          delayMicroseconds(Speed);
          digitalWrite(EC_B_STEP_PIN, LOW);
          delayMicroseconds(Speed);
        }
        Serial.println("A B ROTATE");

      }

    }

    if (EC > ECHysterisPlus) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > pinTime) {
        previousMillis = currentMillis;
        digitalWrite(LEDRED, HIGH);
        Serial.println("RO ON");
        //RO WATER ON
        digitalWrite( RORELAYR, HIGH);

        //        Serial.println("ph - pin is HIGH");
        phDown = 1;
        phUp = 0;
        pinTime = pinLowTime;

      }
    }

    if (EC <= ECHysterisPlus && EC > ECSetpoint) {

      Serial.println("EC hys plus EQL ");
    }
  }
}

result saveslot1() {
  return proceed;
}

void S1Read()
{
  pHs1 = EEPROM.readFloat(EeprompHs1);
  ECs1 = EEPROM.readFloat(EepromECs1);

}

void scheduleFun() {
  Serial.println("scheduleFun() is running...");
  digitalWrite(LEDGREEN, HIGH);
  digitalWrite(TPUMPR, HIGH);
  digitalWrite(MIXERR, HIGH);
  if (pHAvg == true && ECAvg == true) {
    static byte trigger = 1;
    dateandtime();
    Serial.print("RTC time");
    Serial.println(hourupg);
    Serial.println(minupg);
    Serial.print("Slot Set Time");

    Serial.println(S1Hr);
    Serial.println(S1Min);
    if (hourupg <= S1Hr && minupg <= S1Min && Resetfun == true) {
      saveslot1();
      S1Read();
      Serial.print("RTC time");
      Serial.println(hourupg);
      Serial.println(minupg);
      Serial.print("Slot Set Time");

      Serial.println(S1Hr);
      Serial.println(S1Min);

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs1;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs1;
      float ECHys =  ECSetHysteris;
      Serial.print("schedule func() phs1: ");
      Serial.println(pHs1);
      Serial.print("schedule func() phautosetvale: ");
      Serial.println(pHAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      Serial.println("PH SETPOINTS");
      Serial.println(pHAutoSetValue);
      Serial.println("PH HYS  SETPOINTS");
      Serial.println(pHHys);
      Serial.println("EC SETPOINTS");
      Serial.println(ECAutoSetValue);
      Serial.println("EC HYS  SETPOINTS");
      Serial.println(ECHys);
      Serial.println("Shift 1 Progressing");

    }

    if (hourupg <= S2Hr && minupg <= S2Min && Resetfun == true) {
      Serial.print("RTC time");
      Serial.println(hourupg);
      Serial.println(minupg);

      Serial.print("Slot Set Time");
      Serial.println(S2Hr);
      Serial.println(S2Min);

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);

      float pHAutoSetValue =  pHs2;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs2;
      float ECHys =  ECSetHysteris;

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );

    }

    if (hourupg <= S3Hr && minupg <= S3Min && Resetfun == true) {

      Serial.println(S3Min);
      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs3;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs3;
      float ECHys =  ECSetHysteris;

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );

    }

    if (hourupg <= S4Hr && minupg <= S4Min && Resetfun == true) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      float pHAutoSetValue =  pHs4;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs4;
      float ECHys =  ECSetHysteris;

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );

    }
    if (hourupg == S5Hr && minupg == S5Min && Resetfun == true) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      Serial.print("Slot Set Time");

      float pHAutoSetValue =  pHs5;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs5;
      float ECHys =  ECSetHysteris;

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );

    }

    if (hourupg == S6Hr && minupg == S6Min && Resetfun == true) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      Serial.println(S6Hr);
      Serial.println(S6Min);
      float pHAutoSetValue =  pHs6;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs6;
      float ECHys =  ECSetHysteris;
      Serial.print("schedule func() phs6: ");
      Serial.println(pHs6);
      Serial.print("schedule func() phautosetvale: ");
      Serial.println(pHAutoSetValue);
      Serial.print("schedule func() ec6: ");
      Serial.println(ECs6);
      Serial.print("schedule func() ECAutoSetValue: ");
      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      Serial.println("PH SHIFT 6 SETPOINTS");
      Serial.println(pHAutoSetValue);
      Serial.println("PH  HYS  SETPOINTS");
      Serial.println(pHHys);
      Serial.println("EC SHIFT 6 SETPOINTS");
      Serial.println(ECAutoSetValue);
      Serial.println("EC HYS  SETPOINTS");
      Serial.println(ECHys);

      Serial.println("Shift 6 Progressing");
    }

    if (hourupg == S7Hr && minupg == S7Min && Resetfun == true) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);
      Serial.println(S7Hr);
      Serial.println(S7Min);
      float pHAutoSetValue =  pHs7;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs7;
      float ECHys =  ECSetHysteris;
      Serial.print("schedule func() phs7: ");
      Serial.println(pHs7);
      Serial.print("schedule func() phautosetvale: ");
      Serial.println(pHAutoSetValue);
      Serial.print("schedule func() ec7: ");
      Serial.println(ECs7);
      Serial.print("schedule func() ECAutoSetValue: ");
      Serial.println(ECAutoSetValue);

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );
      Serial.println("PH SHIFT 7 SETPOINTS");
      Serial.println(pHAutoSetValue);
      Serial.println("PH  HYS  SETPOINTS");
      Serial.println(pHHys);
      Serial.println("EC SHIFT 7 SETPOINTS");
      Serial.println(ECAutoSetValue);
      Serial.println("EC HYS  SETPOINTS");
      Serial.println(ECHys);
      Serial.println("Shift 7 Progressing");
    }

    if (hourupg == S8Hr && minupg == S8Min && Resetfun == true) {

      digitalWrite(LEDGREEN, HIGH);
      digitalWrite(LEDBLUE, HIGH);

      float pHAutoSetValue =  pHs8;
      float pHHys =  phSetHysteris;
      float ECAutoSetValue =  ECs8;
      float ECHys =  ECSetHysteris;

      SheduleDoser(pHAutoSetValue, pHHys, ECAutoSetValue, ECHys );

    }
  }
}
