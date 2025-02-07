
//Sample code how to Deserialize

#include <ArduinoJson.h>

char MegaData[1024];

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);  // Use Serial3 for communication with ESP32
}

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
    
    String message = Nodedoc["message"];
    
    // Print the received message
    Serial.print("Received message: ");
    Serial.println(message);   
  }

void loop() {
    
FromEspData();

}
