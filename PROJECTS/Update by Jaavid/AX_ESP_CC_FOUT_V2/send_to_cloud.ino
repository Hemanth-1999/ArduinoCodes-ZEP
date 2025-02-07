/*
THIS FUNCTION WILL SEND DATA IN JSON FORMAT TO MQTT.
THIS FUNCTION WILL READ DATA FROM GLOBAL VARIABLES AND ASSIGN TO JSON OBJECT.
WE WILL SEND THIS JSON MESSAGE TO CLOUD VIA MQTT.
*/
void tocloud()
{
  chipid = ESP.getChipId();
  String tempstring11;
  time_t epochTime = timeClient.getEpochTime();
  DynamicJsonDocument mydoc(1024);
  mydoc["dn"] = "climate controll";
  mydoc["d-id"] = chipid;
  mydoc["zone_id"] = 1;
  mydoc["ts"]=epochTime;
  JsonObject metrics = mydoc.createNestedObject("metrics");
  JsonObject Alerts = metrics.createNestedObject("Alerts");
  Alerts["Mode"] = alertmode1;
  Alerts["level"] = alertlevel;
  JsonObject MANUALstatus = metrics.createNestedObject("Manualstatus");
  MANUALstatus["FAN1"] =  Manualstatus_f1;
  MANUALstatus["FAN2"] =  Manualstatus_f2;
  MANUALstatus["FAN3"] =  Manualstatus_f3;
  MANUALstatus["FAN4"] =  Manualstatus_f4;
  MANUALstatus["FAN5"] =  Manualstatus_f5;
  MANUALstatus["FAN6"] =  Manualstatus_f6;
  MANUALstatus["FAN7"] =  Manualstatus_f7;
  MANUALstatus["FAN8"] =  Manualstatus_f8;
  MANUALstatus["CP"] =    Manualstatus_cp;
  MANUALstatus["FP"] = Manualstatus_fp;
  MANUALstatus["FC"] = Manualstatus_fs;
  MANUALstatus["CO2"] = Manualstatus_co;
  //MANUALstatus["IRR"] = Manualstatus_ir;
  //MANUALstatus["DO"] = Manualstatus_do;
  //MANUALstatus["DRIP"] =  Manualstatus_dr;
  //MANUALstatus["LIGHT"] = Manualstatus_pb;
  JsonObject EVENTS = metrics.createNestedObject("Events");
  EVENTS["TEMP"] =  eventogo ;
  EVENTS["VPD"] = eventogo1 ;
  EVENTS["IRR"] = eventogo2 ;
  JsonObject zone1 = metrics.createNestedObject("zone1");
  zone1["at_v1"]=70;
  zone1["rh_v1"]=255;
  zone1["vdp_v1"]=2;
  zone1["hi_v1"]=45;
  zone1["co2_v1"]=22;
  zone1["et_v1"]=3;
  zone1["bp_v1"]=0;
  zone1["sr_v1"]=3;
  zone1["dp_v1"]=2;
  zone1["sl_v1"]=1;
  JsonObject zone2 = metrics.createNestedObject("zone2");
  zone2["at_v2"]=70;
  zone2["rh_v2"]=255;
  zone2["vdp_v2"]=2;
  zone2["hi_v2"]=45;
  zone2["co2_v2"]=22;
  zone2["et_v2"]=3;
  zone2["bp_v2"]=0;
  zone2["sr_v2"]=3;
  zone2["dp_v2"]=2;
  zone2["sl_v2"]=1;
  JsonObject zone3 = metrics.createNestedObject("zone3");
  zone3["at_v3"]=70;
  zone3["rh_v3"]=255;
  zone3["vdp_v3"]=2;
  zone3["hi_v3"]=45;
  zone3["co2_v3"]=22;
  zone3["et_v3"]=3;
  zone3["bp_v3"]=0;
  zone3["sr_v3"]=3;
  zone3["dp_v3"]=2;
  zone3["sl_v3"]=1;
  JsonObject zone4 = metrics.createNestedObject("zone4");
  zone4["at_v4"]=30;
  zone4["rh_v4"]=45;
  zone4["vdp_v4"]=2;
  zone4["hi_v4"]=45;
  zone4["co2_v4"]=700;
  zone4["et_v4"]=3;
  zone4["bp_v4"]=0;
  zone4["sr_v4"]=255;
  zone4["dp_v4"]=2;
  zone4["sl_v4"]=1;
  serializeJson(mydoc, tempstring11);
  Serial.println("");
  serializeJson(mydoc, Serial);
  client.publish( mqttcredentials.Topic, tempstring11.c_str());
  
}
