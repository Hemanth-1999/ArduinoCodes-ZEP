void DoserAuto() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  Serial.println("DoserAuto() is running.");
  EepromReadPHCal();
  EepromReadEC();
  digitalWrite(LEDGREEN, HIGH);

  Serial.println(pHHys);
  Serial.println("pH automatic start");
  Serial.print("pH SETPOINT : ");
  Serial.println(pHAutoSP);
  Serial.print("EC SETPOINT : ");
  Serial.println(ECAutoSP);
  Serial.println(pHHys);
  //Sampler pump and mixer relay ON...
  digitalWrite(TPUMPR, LOW);
  digitalWrite(MIXERR, LOW);

  pHSetpoint = pHAutoSP ;
  phSetHysteris = pHHys;
  pH = roundDecimalPoint(phValueavrg, 2);

  Serial.print("PH SENSOR : ");
  Serial.println(pH);
  Serial.print("EC HYS : ");
  Serial.println(ECHys);

  float  ECSetpoint = ECAutoSP;
  //  ECAutoSP =ECSetpoint;
  ECSetHysteris = ECHys;
  Serial.print("EC setpoint : ");
  Serial.println(ECSetpoint);
  EC = roundDecimalPoint(ecValue, 2);
  Serial.print("EC SENSOR : ");
  Serial.println(EC);

  float HysterisMin = (pHSetpoint - phSetHysteris);
  float HysterisPlus = (pHSetpoint + phSetHysteris);
  Serial.print("PH HYS MIN : ");
  Serial.println(HysterisMin);
  Serial.print("PH HYS MAX : ");
  Serial.println(HysterisPlus);

  float ECHysterisMin = (ECSetpoint - ECSetHysteris);
  float ECHysterisPlus = (ECSetpoint + ECSetHysteris);
  Serial.print("EC HYS MIN : ");
  Serial.println(ECHysterisMin);
  Serial.print("EC HYS MAX : ");
  Serial.println(ECHysterisPlus);

  //  if (StopPHHys == false) {
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
        Serial.println("PH == pHSetpoint");
      }
      Serial.println("ph Max pump and Min pump are OFF");
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
      Serial.println("ph UP pump is HIGH");
    }
  }

  if (pH >= HysterisMin && pH < pHSetpoint) {
    Serial.println("ph EQL2");
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
      Serial.println("ph DOWN pump is HIGH HysterisPlus");
    }
  }

  if (pH <= HysterisPlus && pH > pHSetpoint) {
    Serial.println("PH EQL");
  }
  //********************EC DOSER ******************************
  if ((pH >= HysterisMin && pH <= HysterisPlus)) {
    Serial.println("PH EQL & EC Correction START");
  }
  //EC FUNtion
  if (EC == ECSetpoint) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      Serial.println("EC SETPoit EQL");
      phDown = 0;
      phUp = 0;
      pinTime = pinLowTime;
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
      Serial.println("PH & EC EQL");
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
      Serial.println("EC EQL");
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

    }
    Serial.println("EC A and B  HIGH");
  }

  //0.19>0.30
  if (EC > ECHysterisPlus) {
    // if (ECHysterisPlus  > EC) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > pinTime) {
      previousMillis = currentMillis;
      digitalWrite(LEDRED, HIGH);
      //RO WATER ON
      if (Tankfull = true) {
        digitalWrite( RORELAYR, HIGH);
      } else {
        Serial.print("TankFull");

      }
      Serial.println("EC RO Relay ON");
      phDown = 1;
      phUp = 0;
      pinTime = pinLowTime;

    }
  }

  if (EC <= ECHysterisPlus && EC > ECSetpoint) {
    Serial.println("EC EQL");
  }
}

/************************************** AUTO FUNCTION **************************/

void AutoFun() {

  if (pHAvg == true && ECAvg == true) {

    pHsp =  pHAutoSP;//auto mode value
    dummyPHHYS =  phSetHysteris;
    phSetHysteris = pHHys;

    Serial.println("PH Auto Setpoint");
    Serial.println(pHsp);
    Serial.println("PHyes Auto Setpoint");
    Serial.println(phSetHysteris);
    Serial.println(pHHys); //dummyPHHYS
    Serial.println(dummyPHHYS);

    ECsp = ECAutoSP;
    ECSetHysteris = ECHys;

    Serial.println("EC Auto Setpoint");
    Serial.println(ECsp);
    Serial.println(ECSetHysteris);
    DoserAuto();
  }
}
