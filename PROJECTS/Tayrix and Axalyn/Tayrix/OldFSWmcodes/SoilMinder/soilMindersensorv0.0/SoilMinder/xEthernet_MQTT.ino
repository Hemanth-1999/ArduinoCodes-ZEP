/**************************************************************
 * Function Name: WiFiEvent
 * Description: [Ethernet Setup Code]
 * Output: [Successful Ethernet Connection]
 **************************************************************/
void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}
/**************************************************************
 * Function Name: reconnect
 * Description: [MQTT Setup Code]
 * Output: [Successful MQTT Connection]
 **************************************************************/
void reconnect() 
{
  checkButton();
  
  if (!client.connected())
  {
   
    
     client.setServer(broker, mqttports);

    if (client.connect("Soil1", usernames, passwords)) {
      Serial.println("MQTT connected");

    }
    else {


      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    
      delay(3000);
    }

  }
}
/**************************************************************
 * Function Name: callback
 * Description: [Used for subscribing to topics from MQTT]
 * Output: [Successful MQTT subscribtion]
 **************************************************************/
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}
