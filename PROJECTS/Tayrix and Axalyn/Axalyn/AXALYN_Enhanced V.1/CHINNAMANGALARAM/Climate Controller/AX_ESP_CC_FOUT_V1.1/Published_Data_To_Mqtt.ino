void tocloud()
{
  String formattedTime = timeClient.getFormattedTime();

  time_t epochTime = timeClient.getEpochTime();
  DynamicJsonDocument mydoc(1024);
  mydoc["dn"] = "CC_ESP";
  mydoc["zone_id"] = "5805";
  mydoc["did"] = ESP.getChipId();
  mydoc["ts"] = epochTime;
  JsonObject PFCDATA = mydoc.createNestedObject("Events");
  JsonObject Alerts = PFCDATA.createNestedObject("Alerts");
  Alerts["Mode"] = alertmode1;
  Alerts["level"] = alertlevel;
  JsonObject MANUALstatus = PFCDATA.createNestedObject("Manualstatus");
  MANUALstatus["FAN1"] = String(Manualstatus_f1);
  MANUALstatus["FAN2"] = String(Manualstatus_f2);
  MANUALstatus["FAN3"] = String(Manualstatus_f3);
  MANUALstatus["FAN4"] = String(Manualstatus_f4);
  MANUALstatus["FAN5"] = String(Manualstatus_f5);
  MANUALstatus["FAN6"] = String(Manualstatus_f6);
  MANUALstatus["FAN7"] = String(Manualstatus_f7);
  MANUALstatus["FAN8"] = String(Manualstatus_f8);
  MANUALstatus["CP"] = String(Manualstatus_cp);
  MANUALstatus["FP"] = String(Manualstatus_fp);
  MANUALstatus["FC"] = String(Manualstatus_fs);
  MANUALstatus["CO2"] = String(Manualstatus_co);
  MANUALstatus["IRR"] = String(Manualstatus_ir);
  MANUALstatus["DO"] = String(Manualstatus_do);
  MANUALstatus["DRIP"] = String(Manualstatus_dr);
  MANUALstatus["LIGHT"] = String(Manualstatus_pb);
  JsonObject EVENTS = PFCDATA.createNestedObject("Events");
  EVENTS["TEMP"] =  eventogo ;
  EVENTS["VPD"] = eventogo1 ;
  EVENTS["IRR"] = eventogo2 ;

  serializeJson(mydoc, tempstring11);
  client.publish("axalyn", tempstring11);

  // Output JSON data to Serial for debugging

  serializeJson(mydoc, Serial);
  Serial.println();
  Serial.println("-----------------------");
  memset(tempstring11, 0, 500);
}
