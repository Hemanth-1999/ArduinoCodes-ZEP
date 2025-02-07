void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  valuedata = doc["data"];
}

void reconnect()
{

  while (!client.connected())
  {
    Serial.print("MQTT HOST NAME : ");
    Serial.println(hostname);
    Serial.print("MQTT PORT : ");
    Serial.println(port);
    Serial.print("MQTT USER NAME : ");
    Serial.println("DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033");
    Serial.print("MQTT PASSWORD : ");
    Serial.println(password);
    Serial.print("Attempting MQTT connection...");

    if (client.connect(clientId, "DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033", "DEVICE_PASSWORD"))
    {
      Serial.println("connected");
      client.subscribe(MQTT_SUBTOPIC);
      client.subscribe(sendtopic_WSMesh);
      client.subscribe(sendtopic_ESMesh);
      a = true;

    }
    else
    {
      a = false;

      Serial.print(hostname);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay(5000);  // wait 5sec and retry
      //ESP.restart();
    }
  }
}

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}
