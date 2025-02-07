/**************************************************************
   Function Name: Startup
   DescriBbWption: [Main Setup page for wifi connection and userinterface for user defined paramaters]
   Input: [Wifi Credentials, MQTT IP address, port, username,password,devicename,farm,field,zone ]
   Output: [Generates a wifi web page to enter all those inputs listed above]
 **************************************************************/
void startup()
{
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  int customFieldLength = 90;
  new (&Mqttaddress) WiFiManagerParameter("MQTTADD", "MQTT IP", "", customFieldLength);
  new (&MqttPort) WiFiManagerParameter("MQTTPORT", "PORT", "", customFieldLength );
  new (&Mqttusername) WiFiManagerParameter("MQTTUSERNAME", "MQTTUSERNAME", "", customFieldLength);
  new (&Mqttpassword) WiFiManagerParameter("MQTTPASSWORD", "MQTTPASSWORD", "", customFieldLength);
  new (&devicename) WiFiManagerParameter("DEVICENAME", "DEVICENAME", "", customFieldLength);
  new (&farm) WiFiManagerParameter("FARM", "FARM", "", customFieldLength);
  new (&zone) WiFiManagerParameter("ZONE", "ZONE", "", customFieldLength);
  wm.addParameter(&Mqttaddress);
  wm.addParameter(&MqttPort);
  wm.addParameter(&Mqttusername);
  wm.addParameter(&Mqttpassword);
  wm.addParameter(&devicename);
  wm.addParameter(&farm);
  wm.addParameter(&zone);
  wm.setSaveParamsCallback(saveParamCallback);
  std::vector<const char *> menu = {"wifi", "param", "sep", "restart", "exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(500); // auto close configportal after n seconds
  bool res;
  res = wm.autoConnect("WeatherMinder", "Weather@1999"); // password protected ap
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
/**************************************************************
   Function Name: saveParamCallback
   Description: [This function is used for saving the user credentials and parameters into flash memory in Json Format]
   Output: [Saved credentials]
 **************************************************************/

void saveParamCallback()
{
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println(" mqtt address = " + getParam("MQTTADD"));
  Serial.println(" mqtt port = " + getParam("MQTTPORT"));
  Serial.println(" mqtt uername = " + getParam("MQTTUSERNAME"));
  Serial.println(" mqtt password = " + getParam("MQTTPASSWORD"));
  Serial.println(" device name = " + getParam("DEVICENAME"));
  Serial.println(" farm = " + getParam("FARM"));
  Serial.println(" zone = " + getParam("ZONE"));

  strcpy(hostnamee, Mqttaddress.getValue());
  strcpy(portt, MqttPort.getValue());
  strcpy(userr, Mqttusername.getValue());
  strcpy(passwordd, Mqttpassword.getValue());
  strcpy(deviceidd, devicename.getValue());
  strcpy(farmm, farm.getValue());
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
  doc["zonee"] = zonee;

  // Open a file in SPIFFS for writing
  File file = SPIFFS.open("/params.json", "w");

  // Serialize the JSON object to a string and write it to the file
  serializeJson(doc, file);

  // Close the file
  file.close();

}
/**************************************************************
   Function Name: readsavedvalues
   Description: [This function is used for reading the user credentials and parameters from flash memory in Json Format]
   Output: [Accesing Saved credentials]
 **************************************************************/
void readsavedvalues()
{
  File file = SPIFFS.open("/params.json", "r");
  if (!file) {
    Serial.println("Failed to open params file");
    return;
  }

  // Parse the JSON data
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println("Failed to parse JSON data");
    return;
  }


  String hostnameefromf = doc["hostnamee"];
  String porttfromf = doc["portt"];
  String userrfromf = doc["userr"];
  String passworddfromf = doc["passwordd"];
  String devicesa = doc["deviceidd"];
  String farmq = doc["farmm"];
  String zonew = doc["zonee"];


  file.close();

  hostnameefromf.toCharArray(broker, 100);
  mqttports = porttfromf.toInt();
  userrfromf.toCharArray(usernames, 100);
  passworddfromf.toCharArray(passwords, 100);
  devicesa.toCharArray(dev, 100);
  farmq.toCharArray(frm, 100);
  zonew.toCharArray(zne, 100);

  Serial.print("broker:  ");
  Serial.println(broker);
  Serial.print("port:  ");
  Serial.println(mqttports);
  Serial.print("user:  ");
  Serial.println(usernames);
  Serial.print("pass:  ");
  Serial.println(passwords);
  Serial.print("device:  ");
  Serial.println(dev);
  Serial.print("farm:  ");
  Serial.println(frm);
  Serial.print("zone:  ");
  Serial.println(zne);

  String combined_string = devicesa + "/" + farmq +  "/" + zonew;
  Serial.println(combined_string);

}
/**************************************************************
   Function Name: checkButton
   Description: [checks if user want to change any type of credentials it is activated when button pin is pressed more than 3 seconds]
   Output: [Providing user wifi wb page for changes]
 **************************************************************/
void checkButton()
{
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if ( digitalRead(TRIGGER_PIN) == LOW ) {
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if ( digitalRead(TRIGGER_PIN) == LOW ) {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }

    }
  }
}
String getParam(String name) {
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}
