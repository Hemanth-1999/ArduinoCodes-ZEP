void NPKData() {

//  Serial.println("Moisture: ");
  val1 = GetModVal(MOIST);
  Val1 = val1 * 0.1;
//  Serial.print(Val1);
//  Serial.println(" %");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("Temperature: ");
  val2 = GetModVal(TEMP);
  Val2 = val2 * 0.1;
//  Serial.print(Val2);
//  Serial.println(" *C");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("EC: ");
  val3 = GetModVal(EC);
//  Serial.print(val3);
//  Serial.println(" us/cm");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("Ph: ");
  val4 = GetModVal(PH);
  Val4 = val4 * 0.1;
//  Serial.print(Val4);
//  Serial.println(" ph");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("Nitrogen (N): ");
  val5 = GetModVal(NITRO);
//  Serial.print(val5);
//  Serial.println(" mg/kg\n");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("Phosphorous (P): ");
  val6 = GetModVal(PHOS);
//  Serial.print(val6);
//  Serial.println(" mg/kg\n");
//  Serial.println("-----");
  delay(1000);

//  Serial.println("Potassium (K): ");
  val7 = GetModVal(POTA);
//  Serial.print(val7);
//  Serial.println(" mg/kg\n");
//  Serial.println("-----");

  delay(5000);
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
//  Serial.print("Response in HEX: ");
  startTime = millis();
  while (millis() - startTime <= TIMEOUT) {
    if (mod.available() && byteCount < sizeof(buf)) {
      buf[byteCount++] = mod.read();
      printHexByte(buf[byteCount - 1]);
    }
  }
//  Serial.println();
  //combine 2 byte into word..
  return (uint16_t)(buf[3] << 8 | buf[4]);
}

void printHexByte(byte b)
{
//  Serial.print(b , HEX);
//  Serial.print(':');
}
