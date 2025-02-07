
#include <PubSubClient.h>
#include <ArduinoJson.h>




void setup()

{
  Serial.begin(115200);
}

void sendDataMQTT()

{
  
 DynamicJsonDocument doc(300);
 
 doc["id"] = 123;
 doc["name"] = "intensity";
 serializeJson(doc, Serial);
  
}

void loop()
{
  sendDataMQTT();
delay (3000);
}
