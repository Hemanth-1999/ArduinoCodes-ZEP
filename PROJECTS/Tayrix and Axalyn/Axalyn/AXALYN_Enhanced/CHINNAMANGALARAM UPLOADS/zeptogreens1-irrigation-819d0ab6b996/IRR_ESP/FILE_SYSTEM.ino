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
  read_from_file_status=true;
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
    strcpy(mqttcredentials.Broker, doc["hostnamee"]);
    mqttcredentials.Port = doc["portt"];
    strcpy(mqttcredentials.Clientid, doc["clientidd"]);
    strcpy(mqttcredentials.Username, doc["userr"]);
    strcpy(mqttcredentials.Password, doc["passwordd"]);
    strcpy(mqttcredentials.Topic, doc["topicc"]); // assuming a constant topic
    strcpy(mqttcredentials.Farm, doc["farmm"]);
    strcpy(mqttcredentials.Field, doc["fieldd"]);
    strcpy(mqttcredentials.Zone, doc["zonee"]);
  file.close();
  Serial.print("FARM : ");Serial.println(mqttcredentials.Farm);
  Serial.print("FIELD : ");Serial.println(mqttcredentials.Field);
  Serial.print("ZONE : ");Serial.println(mqttcredentials.Zone);
  Serial.println("\n\n");
  
  Serial.print("BROKER : ");Serial.println(mqttcredentials.Broker);
  Serial.print("PORT : ");Serial.println(mqttcredentials.Port);
  Serial.print("USER : ");Serial.println(mqttcredentials.Username);
  Serial.print("PASS : ");Serial.println(mqttcredentials.Password);
  Serial.print("CLIENT ID : ");Serial.println(mqttcredentials.Clientid);
  
}
