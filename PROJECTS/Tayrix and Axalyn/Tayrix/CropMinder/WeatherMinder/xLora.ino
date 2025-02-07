void onReceive(int packetSize) 
{
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient addresss
  byte sender = LoRa.read();            // sender addresss
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.println(incoming);
  Serial.print("INCOMING: ");
  int Val = incoming.toInt();
  if (Val == 20)
  Mymessage = Mymessage +  AT+ "," + VPD + "," + ET + "," + RH + "," + BP + "," + SL + "," + DP;
  sendMessage(Mymessage, MonolineMaster, WeatherMinder);
  delay(100);
  Mymessage = "";
}
void sendMessage(String outgoing, byte Destination, byte Sender) 
{
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
}
