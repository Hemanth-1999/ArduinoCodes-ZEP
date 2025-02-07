void configure_wifi_portal()
{
  WiFiManagerParameter custom_mqtt_client_id("client_id", "MQTT CLIENT ID","", 40);
  WiFiManagerParameter custom_mqtt_server("server", "MQTT BROKER","", 40);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT PORT","", 6);
  WiFiManagerParameter custom_mqtt_user("user", "MQTT USERNAME", "", 40);
  WiFiManagerParameter custom_mqtt_pass("pass", "MQTT PASSWORD","", 20);
  WiFiManagerParameter custom_mqtt_topic("topic", "PUBLIS TOPIC","", 20);
  WiFiManagerParameter custom_mqtt_farm("farm", "FARM","", 40);
  WiFiManagerParameter custom_mqtt_field("field", "FIELD","", 40);
  WiFiManagerParameter custom_mqtt_zone("zone", "ZONE","", 40);
   WiFiManager wifiManager;
   wifiManager.setSaveConfigCallback(saveConfigCallback);
   //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_client_id);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);
  wifiManager.addParameter(&custom_mqtt_topic);
  wifiManager.addParameter(&custom_mqtt_farm);
  wifiManager.addParameter(&custom_mqtt_field);
  wifiManager.addParameter(&custom_mqtt_zone);
  std::vector<const char*>menu={"wifi","param","sep","restart","exit"};
  wifiManager.setMenu(menu);
  wifiManager.setConfigPortalTimeout(120);//SET TIME TO CLOSE THE PORTAL
  wifiManager.autoConnect("CLIMATECONTROLL");
  Serial.println("connected...yeey :)");
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
//    Serial.println(mqttcredentials.Clientid);
//    Serial.println(mqttcredentials.Broker);
//    Serial.println(mqttcredentials.Port);
//    Serial.println(mqttcredentials.Username);
//    Serial.println(mqttcredentials.Password);
//    Serial.println(mqttcredentials.Topic);
//    Serial.println(mqttcredentials.Farm);
//    Serial.println(mqttcredentials.Field);
//    Serial.println(mqttcredentials.Zone);
}
void saveConfigCallback () 
{
  //Serial.println("Should save config");
  //shouldSaveConfig = true;
}
