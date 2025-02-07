/**************************************************************
 * Function Name: Startup
 * Description: [Main Setup page for wifi connection and userinterface for user defined paramaters]
 * Input: [Wifi Credentials, MQTT IP address, port, username,password,devicename,farm,field,zone ]
 * Output: [Generates a wifi web page to enter all those inputs listed above]
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
  new (&SerialLock) WiFiManagerParameter("SerialLock", "SerialLock", "", customFieldLength);

  wm.addParameter(&Mqttaddress);
  wm.addParameter(&MqttPort);
  wm.addParameter(&Mqttusername);
  wm.addParameter(&Mqttpassword);
  wm.addParameter(&devicename);
  wm.addParameter(&farm);
  wm.addParameter(&zone);
  wm.addParameter(&SerialLock);
  wm.setSaveParamsCallback(saveParamCallback);
  std::vector<const char *> menu = {"wifi","restart","update","info", "exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  bool res;
  res = wm.autoConnect("FertigationMinder", "Soil@1999"); // password protected ap
  Serial.println("connected...yeey :)");
}
/**************************************************************
 * Function Name: saveParamCallback
 * Description: [This function is used for saving the user credentials and parameters into flash memory in Json Format]
 * Output: [Saved credentials]
 **************************************************************/

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println(" mqtt address = " + getParam("MQTTADD"));
  Serial.println(" mqtt port = " + getParam("MQTTPORT"));
  Serial.println(" mqtt uername = " + getParam("MQTTUSERNAME"));
  Serial.println(" mqtt password = " + getParam("MQTTPASSWORD"));
  Serial.println(" device name = " + getParam("DEVICENAME"));
  Serial.println(" farm = " + getParam("FARM"));
  Serial.println(" zone = " + getParam("ZONE"));
  Serial.println(" seriallock = " + getParam("SerialLock"));

  strcpy("cea.axalyn.com", Mqttaddress.getValue());
  strcpy("1883", MqttPort.getValue());
  strcpy("axalyn", Mqttusername.getValue());
  strcpy("cenaura@2024", Mqttpassword.getValue());
  strcpy(deviceidd, devicename.getValue());
  strcpy(farmm, farm.getValue());
  strcpy(zonee, zone.getValue());
  strcpy(seriallock, SerialLock.getValue());
  Serial.println("cea.axalyn.com");
  Serial.println("1883");
  Serial.println("axalyn");
  Serial.println("cenaura@2024");
  Serial.println(deviceidd);
  Serial.println(farmm);
  Serial.println(zonee);
  Serial.println(seriallock);

  DynamicJsonDocument doc(1024);
  doc["hostname"] = "cea.axalyn.com";
  doc["port"] = "1883";
  doc["user"] = "axalyn";
  doc["passwordd"] = "cenaura@2024";
  doc["deviceidd"] = "deviceidd";
  doc["farmm"] = farmm;
  doc["zonee"] = zonee;
  doc["SERIAL"] = seriallock;

  // Open a file in SPIFFS for writing
  File file = SPIFFS.open("/params.json", "w");

  // Serialize the JSON object to a string and write it to the file
  serializeJson(doc, file);

  // Close the file
  file.close();

}
/**************************************************************
 * Function Name: readsavedvalues
 * Description: [This function is used for reading the user credentials and parameters from flash memory in Json Format]
 * Output: [Accesing Saved credentials]
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
  String Seriall = doc["SERIAL"];


  file.close();

  hostnameefromf.toCharArray(broker, 100);
  mqttports = porttfromf.toInt();
  userrfromf.toCharArray(usernames, 100);
  passworddfromf.toCharArray(passwords, 100);
  devicesa.toCharArray(dev, 100);
  farmq.toCharArray(frm, 100);
  zonew.toCharArray(zne, 100);
  Seriall.toCharArray(SSerial, 100);


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
 Serial.print("SerialLock:  ");
  Serial.println(SSerial);

  String combined_string = devicesa + "/" + farmq + "/" +  zonew;
  Serial.println(combined_string);
  
}
String getParam(String name) {
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}
/**************************************************************
 * Function Name: checkButton
 * Description: [checks if user want to change any type of credentials it is activated when button pin is pressed more than 3 seconds]
 * Output: [Providing user wifi wb page for changes]
 **************************************************************/
void checkButton() {
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
