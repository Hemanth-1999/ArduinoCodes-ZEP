/*
IN THIS FUNCTION WE HAVE MQTT AND SSID.
HERE WE NEED TO ONLY ENTER MQTT CREDENTIALS AND PRESS SAVE AND PRESS EXIT. WE DO NOT WANT USER TO ENTER SSID AND PASSWORD.

*/
void ConfigPortal()
{
  Serial.println("\n Starting");
  //wm.resetSettings();
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  int customFieldLength = 40;
  new (&Mqttaddress) WiFiManagerParameter("MQTTADD", "MQTT IP", "", customFieldLength);
  new (&MqttPort) WiFiManagerParameter("MQTTPORT", "PORT", "", customFieldLength );
  new (&Mqttusername) WiFiManagerParameter("MQTTUSERNAME", "MQTTUSERNAME", "", customFieldLength);
  new (&Mqttpassword) WiFiManagerParameter("MQTTPASSWORD", "MQTTPASSWORD", "", customFieldLength);
  new (&devicename) WiFiManagerParameter("DEVICENAME", "DEVICENAME", "", customFieldLength);
  new (&farm) WiFiManagerParameter("FARM", "FARM", "", customFieldLength);
  new (&field) WiFiManagerParameter("FIELD", "FIELD", "", customFieldLength);
  new (&zone) WiFiManagerParameter("ZONE", "ZONE", "", customFieldLength);
  wm.addParameter(&Mqttaddress);
  wm.addParameter(&MqttPort);
  wm.addParameter(&Mqttusername);
  wm.addParameter(&Mqttpassword);
  wm.addParameter(&devicename);
  wm.addParameter(&farm);
  wm.addParameter(&field);
  wm.addParameter(&zone);
  wm.setSaveParamsCallback(saveParamCallback);
  #ifdef GSM_ENABLE
  std::vector<const char *> menu = {"param", "sep", "restart", "exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(120);//seconds
  #else
  std::vector<const char *> menu = {"wifi","param", "sep", "restart", "exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(120);
  #endif
  bool res;
  res = wm.autoConnect("SoilMinder"); // password protected ap
   Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void saveParamCallback() 
{
  char hostnamee[40] ;
  char portt[6] ;
  char userr[40];
  char passwordd[40];
  char deviceidd[20];
  char farmm[20];
  char fieldd[20];
  char zonee[20];

  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println(" mqtt address = " + getParam("MQTTADD"));
  Serial.println(" mqtt port = " + getParam("MQTTPORT"));
  Serial.println(" mqtt uername = " + getParam("MQTTUSERNAME"));
  Serial.println(" mqtt password = " + getParam("MQTTPASSWORD"));
  Serial.println(" device name = " + getParam("DEVICENAME"));
  Serial.println(" farm = " + getParam("FARM"));
  Serial.println(" field = " + getParam("FIELD"));
  Serial.println(" zone = " + getParam("ZONE"));

  strcpy(hostnamee, Mqttaddress.getValue());
  strcpy(portt, MqttPort.getValue());
  strcpy(userr, Mqttusername.getValue());
  strcpy(passwordd, Mqttpassword.getValue());
  strcpy(deviceidd, devicename.getValue());
  strcpy(farmm, farm.getValue());
  strcpy(fieldd, field.getValue());
  strcpy(zonee, zone.getValue());
  Serial.println(hostnamee);
  Serial.println(portt);
  Serial.println(userr);
  Serial.println(passwordd);
  Serial.println(deviceidd);
  Serial.println(farmm);
  Serial.println(fieldd);
  Serial.println(zonee);



  DynamicJsonDocument doc(1024);
  doc["hostnamee"] = hostnamee;
  doc["portt"] = portt;
  doc["userr"] = userr;
  doc["passwordd"] = passwordd;
  doc["deviceidd"] = deviceidd;
  doc["farmm"] = farmm;
  doc["fieldd"] = fieldd;
  doc["zonee"] = zonee;

  // Open a file in SPIFFS for writing
  File file = SPIFFS.open("/params.json", "w");

  // Serialize the JSON object to a string and write it to the file
  serializeJson(doc, file);

  // Close the file
  file.close();

}

String getParam(String name) 
{
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) 
  {
    value = wm.server->arg(name);
  }
  return value;
}
