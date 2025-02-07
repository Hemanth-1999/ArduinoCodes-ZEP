void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["did"] = ESP.getChipModel();
  TXCC["ts"] = formattedTime;
  JsonObject TXCC_ES_rtd = TXCC.createNestedObject("ES_rtd");

  int E_AT = random(100, 200);
  TXCC_ES_rtd["AT_v"] = "{temp: 905, hum: 20}";
//  TXCC_ES_rtd["Name"] = "Hemanth";

  char buffer[1024];

  serializeJson(doc, buffer);
  client.publish("axalyn", buffer);
  client.publish("telemetry", buffer);
  client.publish(sendtopic_CT, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}
