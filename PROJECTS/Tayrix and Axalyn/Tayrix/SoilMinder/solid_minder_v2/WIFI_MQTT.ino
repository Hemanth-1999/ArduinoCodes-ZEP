void reconnect() 
{ 
//  espClient.setInsecure();
  while (!client.connected())
  {  
    //resetButtonstatus();
    Serial.print("CONNECTING TO MQTT...");
    if (client.connect(MqttCredentials.ClientId, MqttCredentials.Username, MqttCredentials.Password)) 
    {
      Serial.println("CONNECTED");
    }
    else 
    {
      Serial.println("FAILED");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(3000);
    }
  }
}

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
