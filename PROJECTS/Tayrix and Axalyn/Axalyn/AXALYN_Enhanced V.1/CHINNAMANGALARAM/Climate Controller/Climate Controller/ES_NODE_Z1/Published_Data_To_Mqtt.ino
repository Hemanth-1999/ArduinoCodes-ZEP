void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  DynamicJsonDocument doc(1024);
  //  JsonObject TXCC = doc.createNestedObject("txcc_es");
  doc["dn"] = "CLIMATECONTROLLER_ESP";
  doc["zone_id"] = "5805";
  doc["did"] = ESP.getChipModel();
  doc["ts"] = epochTime;
  JsonObject TXCC_ES_rtd = doc.createNestedObject("metrics");
  String E_AT = String(ES_AT, 2);
  TXCC_ES_rtd["AT_V"] = E_AT;
  String E_RH = String(ES_RH, 2);
  TXCC_ES_rtd["RH_v"] = E_RH;
  TXCC_ES_rtd["VPD_v"] = VPDcal();
  String E_HI = String(HI, 2);
  TXCC_ES_rtd["HI_v"] = E_HI;
  String cco2 = String(co2, 2);
  TXCC_ES_rtd["Co2_v"] = cco2;
  String E_ETo = String(ETo, 2);
  TXCC_ES_rtd["ET_v"] = E_ETo;
  String E_BP = String(ES_BP, 2);
  TXCC_ES_rtd["BP_v"] = E_BP;
  String UV_intensity = String(uvIntensity, 2);
  TXCC_ES_rtd["SR_v"] = UV_intensity;
  String E_DP = String(dewpt, 2);
  TXCC_ES_rtd["DP_v"] = E_DP;
  TXCC_ES_rtd["SL_v"] = sl;

  char buffer[1024];
  char tempstring[1024];

  serializeJson(doc, tempstring);

  // Publish to MQTT topics
  //client.publish(sendtopic_Mesh, buffer);
  client.publish("axalyn", tempstring);
  client.publish(sendtopic_CT, tempstring);
  client.publish(sendtopic_Node, tempstring);

  // Output JSON data to Serial for debugging

  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");
  memset(tempstring, 0, 500);
}
