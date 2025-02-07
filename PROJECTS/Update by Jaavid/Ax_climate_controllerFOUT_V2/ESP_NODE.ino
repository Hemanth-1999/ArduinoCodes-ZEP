/*
  ESP 32 NODE WILL SEND SOME DATA TO ESP8266 OF CLIMATE CONTROLL.
  CLIMATE CONTROLer ESP8266 MUST RECEIVE AND PROCESS THIS INCOMING DATA.
  THIS INCOMING DATA IS FROM MQTT, IT IS OF JSOIN FORMAT.
  NOW THIS JSON DATA IS SENT TO ATMEGA VIA SERIAL3.
  THEREFORE NODE ESP32 WILL SEND VIA MQTT TO ESP8266. ESP8266 WILL SEND DATA TO ATMEGA VIA SERIAL3.

  WHAT I DO NOT UNDERSTAND IS THIS RETERIVED JSON VALUES ARE USED TO DISPLAY ON THE DISPLAY.
*/
void FromEspData()
{
    // Parse the JSON string into a JSON document
    DynamicJsonDocument Nodedoc(1024);
    deserializeJson(Nodedoc, Serial3);
    DeserializationError error = deserializeJson(Nodedoc, Serial3);
    
    // Check for parsing errors
    if (error) {
      Serial.print("Parsing failed: ");
      Serial.println(error.c_str());
      return;
    }
    
    String message = Nodedoc["Data sent to Mega"];
    
    // Print the received message
    Serial.print("Received message: ");
    Serial.println(message);

}
