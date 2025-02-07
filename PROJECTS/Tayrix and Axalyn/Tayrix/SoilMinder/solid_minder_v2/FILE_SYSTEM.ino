void FileSystemInitialize()
{
  while (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS file system");
    Serial.println("THIS PROGRAM WILL NOT WORK UNTILL IT IS CONNECTED TO FILE SYSTEM");
    delay(1000);
  }
}
void readsavedvalues()
{
  File file = SPIFFS.open("/params.json", "r");
  if (!file)
  {
    Serial.println("Failed to open params file");
    return;
  }

  // Parse the JSON data
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println("Failed to parse JSON data");
    return;
  }
  strcpy(MqttCredentials.Broker, doc["hostnamee"]);
  MqttCredentials.Port = doc["portt"];
  strcpy(MqttCredentials.ClientId, doc["deviceidd"]);
  strcpy(MqttCredentials.Username, doc["userr"]);
  strcpy(MqttCredentials.Password, doc["passwordd"]);
  strcpy(MqttCredentials.Topic1, "telemetry"); // assuming a constant topic
  strcpy(MqttCredentials.Farm, doc["farmm"]);
  strcpy(MqttCredentials.Field, doc["fieldd"]);
  strcpy(MqttCredentials.Zone, doc["zonee"]);
  file.close();
  Serial.print("FARM : "); Serial.println(MqttCredentials.Farm);
  Serial.print("FIELD : "); Serial.println(MqttCredentials.Field);
  Serial.print("ZONE : "); Serial.println(MqttCredentials.Zone);
  Serial.println("\n\n");

  Serial.print("BROKER : "); Serial.println(MqttCredentials.Broker);
  Serial.print("PORT : "); Serial.println(MqttCredentials.Port);
  Serial.print("USER : "); Serial.println(MqttCredentials.Username);
  Serial.print("PASS : "); Serial.println(MqttCredentials.Password);
  Serial.print("CLIENT ID : "); Serial.println(MqttCredentials.ClientId);

}
