void callback(char* topic, byte* payload, unsigned int length) {
  receivedPayload = "";
  for (int i = 0; i < length; i++) {
    receivedPayload += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(receivedPayload);
  newData = true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    if (client.connect(clientId, user, password))
    {
      client.subscribe(sendtopic_WSMesh);
      //   client.subscribe(sendtopic_ESMesh);
      client.subscribe("axalyn");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      ESP.restart();
    }
  }
}

void HandleMqttState() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
