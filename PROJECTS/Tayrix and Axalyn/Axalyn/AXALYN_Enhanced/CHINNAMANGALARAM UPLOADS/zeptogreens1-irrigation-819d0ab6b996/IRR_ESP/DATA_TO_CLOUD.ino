void tocloud()
{
  String tempstring11="";
  
  chipid = ESP.getChipId();
  time_t epochTime = timeClient.getEpochTime();
  
  DynamicJsonDocument doc(1024);
  doc["dn"] = "doser";
  doc["d-id"] = chipid;
  doc["zone_id"] = 1;
  doc["ts"]=epochTime;
  JsonObject PFCDATA = doc.createNestedObject("metrics");
  JsonObject Alerts = PFCDATA.createNestedObject("alerts");
  Alerts["Mode"] = alertmode1;
  Alerts["level"] = alertlevel;
  JsonObject MANUALstatus = PFCDATA.createNestedObject("manualstatus");
   MANUALstatus["irz1a"] =  Manualstatus_irz1a;
  MANUALstatus["irz1b"] =  Manualstatus_irz1b;
  MANUALstatus["irz2a"] =  Manualstatus_irz2a;
  MANUALstatus["irz2b"] =  Manualstatus_irz2b;
  MANUALstatus["doa"] =  Manualstatus_doa;
  MANUALstatus["dob"] =  Manualstatus_dob;

  JsonObject EVENTS = PFCDATA.createNestedObject("events");
  EVENTS["IRR"] = eventogo2 ;

  serializeJson(doc, tempstring11);

  client.publish( "telemetry", tempstring11.c_str());

}
