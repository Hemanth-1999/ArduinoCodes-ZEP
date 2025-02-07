void processData() {
  if (!newData) {
    return; // If no new data, return
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, receivedPayload);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Ensure data exists and is valid
  if (doc.containsKey("metrics")) {
    JsonObject txcc_es = doc["metrics"];

    if (txcc_es.containsKey("AT_V") &&
        txcc_es.containsKey("RH_v") &&
        txcc_es.containsKey("BP_v") &&
        txcc_es.containsKey("VPD_v") &&
        txcc_es.containsKey("HI_v") &&
        txcc_es.containsKey("Co2_v") &&
        txcc_es.containsKey("ET_v") &&
        txcc_es.containsKey("DP_v") &&
        txcc_es.containsKey("SL_v") &&
        txcc_es.containsKey("SR_v")) {

      ES_rtd_AT_v = txcc_es["AT_V"];
      ES_rtd_RH_v = txcc_es["RH_v"];
      ES_rtd_BP_v = txcc_es["BP_v"];
      ES_rtd_VPD_v = txcc_es["VPD_v"];
      ES_rtd_HI_v = txcc_es["HI_v"];
      ES_rtd_Co2_v = txcc_es["Co2_v"];
      ES_rtd_ET_v = txcc_es["ET_v"];
      ES_rtd_DP_v = txcc_es["DP_v"];
      ES_rtd_SL_v = txcc_es["SL_v"];
      ES_rtd_SR_v = txcc_es["SR_v"];

      // Calculate averages (if you need to do this based on multiple readings, you should implement averaging logic)
      double avg_AT_v = ES_rtd_AT_v;
      double avg_RH_v = ES_rtd_RH_v;
      double avg_BP_v = ES_rtd_BP_v;
      double avg_VPD_v = ES_rtd_VPD_v;
      double avg_HI_v = ES_rtd_HI_v;
      double avg_Co2_v = ES_rtd_Co2_v;
      double avg_ET_v = ES_rtd_ET_v;
      double avg_DP_v = ES_rtd_DP_v;
      double avg_SL_v = ES_rtd_SL_v;
      double avg_SR_v = ES_rtd_SR_v;

      // Serialize the averaged data to JSON
      avgDoc["AT_V_avg"] = avg_AT_v;
      avgDoc["RH_v_avg"] = avg_RH_v;
      avgDoc["VPD_v_avg"] = avg_VPD_v;
      avgDoc["HI_v_avg"] = avg_HI_v;
      avgDoc["Co2_v_avg"] = avg_Co2_v;
      avgDoc["ET_v_avg"] = avg_ET_v;
      avgDoc["BP_v_avg"] = avg_BP_v;
      avgDoc["SR_v_avg"] = avg_SR_v;
      avgDoc["DP_v_avg"] = avg_DP_v;
      avgDoc["SL_v_avg"] = avg_SL_v;

    }

  } else {
    Serial.println("No metrics in received JSON");
  }

  newData = false; // Reset the flag after processing

  RAT.addValue(ES_rtd_AT_v);
  RAH.addValue(ES_rtd_RH_v);
  RAHI.addValue(ES_rtd_HI_v);
  RAVPD.addValue(ES_rtd_VPD_v);
  RACO2.addValue(ES_rtd_Co2_v);
  RAET.addValue(ES_rtd_ET_v);
  RABP.addValue(ES_rtd_BP_v);
  RASOL.addValue(ES_rtd_SL_v);
  RADEW.addValue(ES_rtd_DP_v);

  Ax_es_ES_rtd_AT_v_Avg = RAT.getAverage();
  Ax_es_ES_rtd_RH_v_Avg = RAH.getAverage();
  Ax_es_ES_rtd_HI_v_Avg = RAHI.getAverage();
  Ax_es_ES_rtd_VPD_v_Avg = RAVPD.getAverage();
  Ax_es_ES_rtd_Co2_v = RACO2.getAverage();
  Ax_es_ES_rtd_ET_v = RAET.getAverage();
  Ax_es_ES_rtd_BP_v = RABP.getAverage();
  Ax_es_ES_rtd_SR_v = RASOL.getAverage();
  Ax_es_ES_rtd_DP_v = RADEW.getAverage();
}
