void printHexByte(byte b)
{
  Serial.print(b , HEX);
  Serial.print(':');
}

uint16_t GetModVal(byte val) {
  uint32_t startTime = 0;
  uint8_t  byteCount = 0;
  memset(buf, 0, sizeof(buf)); //empty incoming..
  //send request..
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (USE_CAST)
  mod.write(dataMod[val], sizeof(dataMod_B[val])); else
  mod.write(dataMod[val], sizeof(dataMod[val]));
  mod.flush();//wait for outgoing to be sent..
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  //recv response until timeout expires..
  Serial.print("Response in HEX: ");
  startTime = millis();
  while (millis() - startTime <= TIMEOUT) {
    if (mod.available() && byteCount < sizeof(buf)) {
      buf[byteCount++] = mod.read();
      printHexByte(buf[byteCount - 1]);
    }
  }
  Serial.println();
  //combine 2 byte into word..
  return (uint16_t)(buf[3] << 8 | buf[4]);
}

void getNPKdata()
{
  getNPKmoist();
  getNPKtemp();
  getNPKelec();
  getNPKph();
  getNPKnitr();
  getNPKphosp();
  getNPKpotass();
}

void getNPKmoist()
{
  NPK_moist=GetModVal(MOIST);
}
void getNPKtemp()
{
  NPK_temp=GetModVal(TEMP);
}
void getNPKelec()
{
  NPK_ec=GetModVal(EC);
}
void getNPKph()
{
  NPK_ph=GetModVal(PH);
}
void getNPKnitr()
{
  NPK_nitr=GetModVal(NITRO);
}
void getNPKphosp()
{
  NPK_phosp=GetModVal(PHOS);
}
void getNPKpotass()
{
  NPK_potass=GetModVal(POTA);
}
