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
//     client.setServer("cea.axalyn.com", "1883");
    if (client.connect("Fertigation", "axalyn", "cenaura@2024")) {
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
void callbackk(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}
/**************************************************************
 * Function Name: ToCloud
 * Description: [Creating Json package and sending sensor data to cloud]
 * Output: [data to cloud]
 **************************************************************/
void ToCloud()
{  
  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["AT"] = ATemp;
  TXCC["RH"] = Ahum;
  TXCC["BP"] = Abp;
  TXCC["SL"] = Asl;
  TXCC["VPD"] = Avpd;
  TXCC["DP"] = Adp;
  TXCC["ET"] = Aet;
   char buffer[1024];
  serializeJson(doc, buffer);
  client.publish("axalyn", buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");
  
}
