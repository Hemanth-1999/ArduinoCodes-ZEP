/**************************************************************
 * Function Name: Lora Setup
 * Description: [for receving sensor data from SoilMINDER]
 **************************************************************/
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
  int recipient = LoRa.read();          // recipient addresss
  byte sender = LoRa.read();
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  Serial.println(incoming);
  Serial.print("INCOMING: ");
  if (sender == 0xBB)
  {
    Serial.println("From SoilMinder");
    String N = getValue(incoming, ',', 0);
    String P = getValue(incoming, ',', 1);
    String K = getValue(incoming, ',', 2);
    String SM = getValue(incoming, ',', 3);
    String ST = getValue(incoming, ',', 4);
    String PH = getValue(incoming, ',', 5);
    String EC = getValue(incoming, ',', 6);
    Nvalue = N.toInt();
    Pvalue = P.toInt();
    Kvalue = K.toInt();
    SMvalue = SM.toInt();
    STvalue = ST.toInt();
    PHvalue = PH.toInt();
    ECvalue = EC.toInt();
  }
  esp_task_wdt_reset();
}
void sendMessage(String outgoing, byte Destination, byte Sender) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(Destination);              // add destination addresss
  LoRa.write(Sender);             // add sender addresss
  LoRa.write(msgCount);                 // add messsage ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;  // increment messsage ID
  Serial.print("SENDING: ");
  Serial.println(outgoing);
  esp_task_wdt_reset();
}
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
