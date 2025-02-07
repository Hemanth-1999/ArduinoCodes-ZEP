void sendnodedata()
{ 
  DynamicJsonDocument metricdoc(1024);
  DynamicJsonDocument Nodedoc(1024);
  JsonObject ESNODE = Nodedoc.createNestedObject("ESNODE");
  ESNODE["ES_AT"] =  txcc_es_ES_rtd_AT_v;
  ESNODE["ES_RH"] =  txcc_es_ES_rtd_RH_v;
  ESNODE["ES_VPD"] =  txcc_es_ES_rtd_VPD_v;
  ESNODE["ES_HI"] =  txcc_es_ES_rtd_HI_v;
  ESNODE["ES_CO2"] =  txcc_es_ES_rtd_Co2_v;
  ESNODE["ES_ET"] =  txcc_es_ES_rtd_ET_v;
  ESNODE["ES_SR"] =  txcc_es_ES_rtd_SR_v;
  ESNODE["ES_BP"] =  txcc_es_ES_rtd_BP_v;
  ESNODE["ES_DP"] =  txcc_es_ES_rtd_DP_v;
  ESNODE["ES_SL"] =  txcc_es_ES_rtd_SL_v;
  JsonObject WSNODE = Nodedoc.createNestedObject("WSNODE");
  WSNODE["WS_AT"] =  Ws_AT_v;
  WSNODE["WS_RH"] =  Ws_RH_v;
  WSNODE["WS_VPD"] =  Ws_VPD_v;
  WSNODE["WS_HI"] =  Ws_HI_v;
  WSNODE["WS_ET"] =  Ws_ET_v;
  WSNODE["WS_SR"] =  Ws_SR_v;
  WSNODE["WS_BP"] =  Ws_BP_v;
  WSNODE["WS_DP"] =  Ws_DP_v;
  WSNODE["WS_SL"] =  Ws_SL_v;
  serializeJson(Nodedoc, tempstring1);
  Serial.println("");
  serializeJson(Nodedoc, Serial);
  memset(tempstring1, 0, 500);

  JsonObject METRIC = metricdoc.createNestedObject("metrics");
  METRIC["AT"] =  txcc_es_ES_rtd_AT_v;
  METRIC["RH"] =  txcc_es_ES_rtd_RH_v;
  METRIC["VPD"] =  txcc_es_ES_rtd_VPD_v;
  METRIC["HI"] =  txcc_es_ES_rtd_HI_v;
  METRIC["Co2"] =  txcc_es_ES_rtd_Co2_v;
  METRIC["Ev"] =  txcc_es_ES_rtd_ET_v;
  METRIC["Solar"] =  txcc_es_ES_rtd_SR_v;
  METRIC["BaPa"] =  txcc_es_ES_rtd_BP_v;
  METRIC["Dew"] =  txcc_es_ES_rtd_DP_v;
  METRIC["SeaL"] =  txcc_es_ES_rtd_SL_v;
  serializeJson(metricdoc, tempstring2);
  Serial.println("");
 client.publish(MQTT_PUBTOPIC, tempstring2);
 

  memset(tempstring2, 0, 500);

}
