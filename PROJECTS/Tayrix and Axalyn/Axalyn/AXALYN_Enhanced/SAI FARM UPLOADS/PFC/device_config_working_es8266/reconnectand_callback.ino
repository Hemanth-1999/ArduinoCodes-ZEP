void mqttconnection()
{
  
  while (!client.connected()) {
     checkButton();

  
   if (client.connect("ESP32Client", usernames, passwords))
    {
     Serial.println("connected");
     client.subscribe(MQTT_SUBTOPIC);
     client.subscribe(MQTT_SUBTOPIC1);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());     
      delay(5000);
    }

}
}
void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);
   
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
   
  if(JsonObject txcc_es = doc["txcc_es"])
  {
long txcc_es_did = txcc_es["did"]; // 14806448
const char* txcc_es_ts = txcc_es["ts"]; // "18:00:40"

JsonObject txcc_es_ES_rtd = txcc_es["ES_rtd"];
 txcc_es_ES_rtd_AT_v = txcc_es_ES_rtd["AT_v"]; // 29.60000038
 txcc_es_ES_rtd_RH_v = txcc_es_ES_rtd["RH_v"]; // 33.5
 txcc_es_ES_rtd_VPD_v = txcc_es_ES_rtd["VPD_v"]; // 2.700000048
 txcc_es_ES_rtd_HI_v = txcc_es_ES_rtd["HI_v"]; // 28.70000076
 txcc_es_ES_rtd_Co2_v = txcc_es_ES_rtd["Co2_v"]; // 0
 txcc_es_ES_rtd_ET_v = txcc_es_ES_rtd["ET_v"]; // 88.90000153
 txcc_es_ES_rtd_BP_v = txcc_es_ES_rtd["BP_v"]; // 941.2000122
 txcc_es_ES_rtd_SR_v = txcc_es_ES_rtd["SR_v"]; // 3028.771973
 txcc_es_ES_rtd_DP_v = txcc_es_ES_rtd["DP_v"]; // 11.89999962
 txcc_es_ES_rtd_SL_v = txcc_es_ES_rtd["SL_v"]; // 1130
  }
}
