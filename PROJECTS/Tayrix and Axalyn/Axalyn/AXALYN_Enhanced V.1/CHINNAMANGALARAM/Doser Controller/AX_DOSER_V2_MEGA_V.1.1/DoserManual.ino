result manualdosemin() {
  digitalWrite(LEDBLUE, HIGH);
  int RPHUP =  PHUPR * 230;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pinTime) {
    previousMillis = currentMillis;
    digitalWrite(PHHIGH_DIR_PIN, LOW);
    // Motor rotate with ml input
    for ( int y = 0; y < phplus; y++) {
      for ( int x = 0; x < RPHUP; x++) //(onemlphdown * phmin) //420
      {
        digitalWrite(PHHIGH_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PHHIGH_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        Serial.println("PHHIGH_STEP_Speed: ");
        Serial.println(Speed);
      }
    }
    pinTime = pinLowTime;
  }
}

result manualdoseplus() {
  digitalWrite(LEDBLUE, HIGH);
  int RPHDOWN =  PHDOWNR * 230;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > pinTime) {
    previousMillis = currentMillis;
    digitalWrite(PHLOW_DIR_PIN, LOW);
    for ( int y = 0; y < phmin ; y++) {
      for (int x = 0; x < RPHDOWN ; x++) //(onemlphup * phplus)390
      {
        digitalWrite(PHLOW_STEP_PIN, HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PHLOW_STEP_PIN, LOW);
        delayMicroseconds(Speed);
        Serial.println("PHLOW_STEP_Speed: ");
        Serial.println(Speed);
      }

    }
    pinTime = pinLowTime;
  }
}

int ECRA;
result manualdoseEcA() {
  Serial.println("ecaaa");
  digitalWrite(LEDBLUE, HIGH);
  int ECRA =  ECRatioA * 230;
  digitalWrite(EC_A_DIR_PIN, LOW);

  // Motor rotate with ml input
  for ( int z = 0; z <  ECA; z++) {
    for (int x = 0; x < ECRA; x++)//375
    {
      digitalWrite(EC_A_STEP_PIN, HIGH);
      delayMicroseconds(Speed);
      digitalWrite(EC_A_STEP_PIN, LOW);
      delayMicroseconds(Speed);
      Serial.println("EC_A_STEP_Speed: ");
      Serial.println(Speed);
    }
  }
}

int ECRB;
result manualdoseEcB() {
  digitalWrite(LEDBLUE, HIGH);
  int ECRB =  ECRatioB * 240;
  digitalWrite(EC_B_DIR_PIN, LOW);
  digitalWrite(EC_C_DIR_PIN, LOW);
  for ( int b = 0; b <  ECB; b++) {
    // Spin motor slowly
    for (int x = 0; x < ECRB; x++) //375
    {
      digitalWrite(EC_B_STEP_PIN, HIGH);
      delayMicroseconds(Speed);
      digitalWrite(EC_B_STEP_PIN, LOW);
      delayMicroseconds(Speed);
      Serial.println("EC_B_STEP_Speed: ");
      Serial.println(Speed);
    }
  }
}
