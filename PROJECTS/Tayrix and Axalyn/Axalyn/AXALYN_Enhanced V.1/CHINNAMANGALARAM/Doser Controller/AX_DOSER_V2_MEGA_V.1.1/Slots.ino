/***************************** SLOT1-pH *************************/
void sendph1() {
  pHs1 = float(str.toInt()) / 100;
  Serial.print("Slot1 - pH = ");
  Serial.println(pHs1);

  Serial.print(" eeprom save phs1= ");
  Serial.println(pHs1);
}

/******************************** SLOT1-Ec ************************/
void sendec1() {
  ECs1 = float(dfd.toInt()) / 100;
  Serial.print("Slot2 - Ec = ");
  Serial.println(ECs1);
  EEPROM.updateFloat(EepromECs1, ECs1);

  Serial.print(" eeprom save phs2= ");
  Serial.println(ECs1);
}

/***************************** SLOT2-pH *************************/
void sendph2() {
  pHs2 = float(str.toInt()) / 100;
  Serial.print("Slot2 - pH = ");
  Serial.println(pHs2);

  Serial.print(" eeprom save phs2= ");
  Serial.println(pHs2);
}

/******************************** SLOT2-Ec ************************/
void sendec2() {
  ECs2 = float(dfd.toInt()) / 100;
  Serial.print("Slot2 - Ec = ");
  Serial.println(ECs2);
  EEPROM.updateFloat(EepromECs2, ECs2);
  //  S2Read();
  Serial.print(" eeprom save phs2= ");
  Serial.println(ECs2);
}

/***************************** SLOT3-pH *************************/
void sendph3() {
  pHs3 = float(str.toInt()) / 100;
  Serial.print("Slot3 - pH = ");
  Serial.println(pHs3);

  //S3Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(pHs3);
}

/**************************** SLOT3-Ec *************************/
void sendec3() {
  ECs3 = float(dfd.toInt()) / 100;
  Serial.print("Slot3 - Ec = ");
  Serial.println(ECs3);
  EEPROM.updateFloat(EepromECs3, ECs3);
  //S3Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(ECs3);
}

/*************************** SLOT4-pH *************************/
void sendph4() {
  pHs4 = float(str.toInt()) / 100;
  Serial.print("Slot4 - pH = ");
  Serial.println(pHs4);
  EEPROM.updateFloat(EeprompHs4, pHs4);
  //S4Read();
  Serial.print(" eeprom save phs3= ");
  Serial.println(pHs3);
}

/************************** SLOT4-Ec *************************/
void sendec4() {
  ECs4 = float(dfd.toInt()) / 100;
  Serial.print("Slot4 - Ec = ");
  Serial.println(ECs4);
  EEPROM.updateFloat(EepromECs4, ECs4);
  //S4Read();
  Serial.print(" eeprom save phs4= ");
  Serial.println(ECs4);
}

/************************** SLOT5-pH ***********************/
void sendph5() {
  pHs5 = float(str.toInt()) / 100;
  Serial.print("Slot5 - pH = ");
  Serial.println(pHs5);
  EEPROM.updateFloat(EeprompHs5, pHs5);
  //S4Read();
  Serial.print(" eeprom save phs5= ");
  Serial.println(pHs5);
}

/************************* SLOT5-Ec **********************/
void sendec5() {
  ECs5 = float(dfd.toInt()) / 100;
  Serial.print("Slot5 - Ec = ");
  Serial.println(ECs5);
  EEPROM.updateFloat(EepromECs5, ECs5);
  //S4Read();
  Serial.print(" eeprom save phs5= ");
  Serial.println(ECs5);
}

/*********************** SLOT6-pH *********************/
void sendph6() {
  pHs6 = float(str.toInt()) / 100;
  Serial.print("Slot6 - pH = ");
  Serial.println(pHs6);
  EEPROM.updateFloat(EeprompHs6, pHs6);
  //S4Read();
  Serial.print(" eeprom save phs6= ");
  Serial.println(pHs6);
}

/********************** SLOT6-Ec *********************/
void sendec6() {
  ECs6 = float(dfd.toInt()) / 100;
  Serial.print("Slot6 - Ec = ");
  Serial.println(ECs6);
  EEPROM.updateFloat(EepromECs6, ECs6);
  //S4Read();
  Serial.print(" eeprom save phs6= ");
  Serial.println(ECs6);
}
