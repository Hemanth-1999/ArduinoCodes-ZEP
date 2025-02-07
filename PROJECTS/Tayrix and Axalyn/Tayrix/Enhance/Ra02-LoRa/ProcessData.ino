String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void processData() {
  if (newData == false) {
    AT_l = getValue(receivedMessage, ',', 0);
    RH_l = getValue(receivedMessage, ',', 1);
    BP_l = getValue(receivedMessage, ',', 2);

    HI_l = getValue(receivedMessage, ',', 3);
    DP_l = getValue(receivedMessage, ',', 4);
    SL_l = getValue(receivedMessage, ',', 5);

    Co2_l = getValue(receivedMessage, ',', 6);
    ET_l = getValue(receivedMessage, ',', 7);
    SR_l = getValue(receivedMessage, ',', 8);

    VPD_l = getValue(receivedMessage, ',', 9);

    AT = AT_l.toInt();
    RH = RH_l.toInt();
    BP = BP_l.toInt();

    HI = HI_l.toInt();
    DP = DP_l.toInt();
    SL = SL_l.toInt();

    Co2 = Co2_l.toInt();
    ET  = ET_l.toInt();
    SR  = SR_l.toInt();

    VPD  = VPD_l.toInt();
    receivedMessage = "";

    Serial.print("temperature: ");
    Serial.println(AT);
  }
}
