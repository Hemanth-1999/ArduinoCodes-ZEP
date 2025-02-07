void configure_wifi_portal()
{
  if (wm_nonblocking) wm.setConfigPortalBlocking(false);
  new (&custom_mqtt_client_id) WiFiManagerParameter("client_id", "MQTT CLIENT ID","", 40);
  new (&custom_mqtt_server) WiFiManagerParameter("server", "MQTT BROKER","", 40);
  new (&custom_mqtt_port) WiFiManagerParameter("port", "MQTT PORT","", 6);
  new (&custom_mqtt_user) WiFiManagerParameter("user", "MQTT USERNAME", "", 40);
  new (&custom_mqtt_pass) WiFiManagerParameter("pass", "MQTT PASSWORD","", 20);
  new (&custom_mqtt_topic) WiFiManagerParameter("topic", "PUBLIS TOPIC","", 20);
  new (&custom_mqtt_farm) WiFiManagerParameter("farm", "FARM","", 40);
  new (&custom_mqtt_field) WiFiManagerParameter("field", "FIELD","", 40);
  new (&custom_mqtt_zone) WiFiManagerParameter("zone", "ZONE","", 40);
   wm.setSaveConfigCallback(saveConfigCallback);
   //add all your parameters here
  wm.addParameter(&custom_mqtt_client_id);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_pass);
  wm.addParameter(&custom_mqtt_topic);
  wm.addParameter(&custom_mqtt_farm);
  wm.addParameter(&custom_mqtt_field);
  wm.addParameter(&custom_mqtt_zone);
  
  std::vector<const char*>menu={"wifi","param","sep","restart","exit"};
  wm.setMenu(menu);
  wm.setConfigPortalTimeout(120);
  bool res;
  res = wm.autoConnect("CLIMATECONTROLL");
  if(res)
    Serial.println("connected...yeey :)");
  else
    Serial.println("FAILED TO CONNECT TO WIFI");
    
  read_from_file_status=false;
}

void saveConfigCallback () 
{
  Serial.println("CAPTIVE PORTAL CALL BACK");
  
   char* port;
    strcpy(mqttcredentials.Clientid, custom_mqtt_client_id.getValue());
    strcpy(mqttcredentials.Broker, custom_mqtt_server.getValue());
    strcpy(port, custom_mqtt_port.getValue());
    mqttcredentials.Port=atoi(port);
    strcpy(mqttcredentials.Username, custom_mqtt_user.getValue());
    strcpy(mqttcredentials.Password, custom_mqtt_pass.getValue());
    strcpy(mqttcredentials.Topic, custom_mqtt_topic.getValue());
    strcpy(mqttcredentials.Farm, custom_mqtt_farm.getValue());
    strcpy(mqttcredentials.Field, custom_mqtt_field.getValue());
    strcpy(mqttcredentials.Zone, custom_mqtt_zone.getValue());
    Serial.println(mqttcredentials.Clientid);
    Serial.println(mqttcredentials.Broker);
    Serial.println(mqttcredentials.Port);
    Serial.println(mqttcredentials.Username);
    Serial.println(mqttcredentials.Password);
    Serial.println(mqttcredentials.Topic);
    Serial.println(mqttcredentials.Farm);
    Serial.println(mqttcredentials.Field);
    Serial.println(mqttcredentials.Zone);

    DynamicJsonDocument doc(1024);
    
    doc["hostnamee"] = mqttcredentials.Username;
    doc["portt"] = mqttcredentials.Port;
    doc["clientidd"] = mqttcredentials.Clientid;
    doc["userr"] = mqttcredentials.Username;
    doc["passwordd"] = mqttcredentials.Password;
    doc["topicc"] = mqttcredentials.Topic;
    doc["farmm"] = mqttcredentials.Farm;
    doc["fieldd"] = mqttcredentials.Field;
    doc["zonee"] = mqttcredentials.Zone;

    // Open a file in SPIFFS for writing
    File file = SPIFFS.open("/params.json", "w");
    // Serialize the JSON object to a string and write it to the file
    serializeJson(doc, file);
    // Close the file
    file.close();
    
}
