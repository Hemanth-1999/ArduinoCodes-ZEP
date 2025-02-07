void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }

}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("CONNECTING TO MQTT...");
    if (client.connect(mqttcredentials.Clientid,mqttcredentials.Username,mqttcredentials.Password))
    {
      Serial.println("CONNECTERD");
    } 
    else 
    {
      Serial.println("FAILED");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
