/*
whenever we receive mqtt message this function will be called.
if it find metrics in json then it will read all key values and store in variables and do 
average if other mqtt message comes and store the average values to global variables.
*/
void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  
  if(strstr(topic,sendtopic_app)!=NULL)
  {
    message_from_app(payload,length);
  }
  else
  {
    for (int i = 0; i < length; i++) 
    {
      Serial.print((char)payload[i]);
    }
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
      //client.subscribe(sendtopic_WSMesh);//to receive data from outside node.
      //client.subscribe(sendtopic_ESMesh);//to receive data from inside nodes.
      client.subscribe(sendtopic_app);//to receive data from app

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
