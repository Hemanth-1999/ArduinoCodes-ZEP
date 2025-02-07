void sendDataMQTT() {

  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  StaticJsonDocument<500> doc;
  doc["dn"] = "ES_NODE";
  doc["zone_id"] = "5805";
  doc["did"] = ESP.getChipModel();
  doc["ts"] = epochTime;
  JsonObject TXCC_ES_rtd = doc.createNestedObject("metrics");

  String E_AT = String(ES_AT, 2);
  TXCC_ES_rtd["AT_V"] = E_AT;
  String E_RH = String(ES_RH, 2);
  TXCC_ES_rtd["RH_v"] = E_RH;
  String E_VPD = String(VPDcal(), 2);
  TXCC_ES_rtd["VPD_v"] = E_VPD;
  String E_HI = String(HI, 2);
  TXCC_ES_rtd["HI_v"] = E_HI;
  String cco2 = String(co2, 2);
  TXCC_ES_rtd["Co2_v"] = cco2;
  String E_ETo = String(ETo, 2);
  TXCC_ES_rtd["ET_v"] = E_ETo;
  String E_BP = String(ES_BP, 2);
  TXCC_ES_rtd["BP_v"] = E_BP;
  String SR = String(solarRadiation, 2);
  TXCC_ES_rtd["SR_v"] = SR;
  String E_DP = String(dewpt, 2);
  TXCC_ES_rtd["DP_v"] = E_DP;
  TXCC_ES_rtd["SL_v"] = sl;

  char buffer[1024];

  serializeJson(doc, buffer);

  /*QoS: Set the QoS level based on how you want the message delivery to behave.
  QoS 0: At most once (fire-and-forget).
  QoS 1: At least once (acknowledged).
  QoS 2: Exactly once (highest level of service).*/

  // Publish to MQTT topics
  //client.publish("telemetry", tempstring);
  client.publish("axalyn", buffer);
  client.publish(sendtopic_CT, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("Publishing Data");
  delay(5000);

}
