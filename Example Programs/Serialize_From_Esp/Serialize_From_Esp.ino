
//Sample code how to Serialize


#include <ArduinoJson.h>

char EspData[1024];

void setup() {
  Serial.begin(9600);
}

void DatatoMega()
{
  DynamicJsonDocument Nodedoc(1024);
  
  // Add data to the JSON document
  Nodedoc["message"] = "Hello";
  
  serializeJson(Nodedoc, EspData);
  serializeJson(Nodedoc, Serial);
  
  // Send the JSON string over Serial
  Serial.println(EspData);

}

void loop() {

  DatatoMega();
  
  delay(1000);
}
