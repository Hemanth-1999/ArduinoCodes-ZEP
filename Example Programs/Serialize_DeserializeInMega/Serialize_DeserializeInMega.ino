#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);  // Initialize Serial communication with the PC
  Serial3.begin(9600); // Initialize Serial3 communication with ESP
}

void loop() {
  // Part 1: Receive and deserialize data from ESP

  if (Serial3.available()) {
    // Read the incoming JSON string
    String jsonReceived = Serial3.readStringUntil('\n');

    // Create a JSON document to hold the deserialized data
    StaticJsonDocument<200> docReceived;

    // Deserialize the JSON string into the JSON document
    DeserializationError error = deserializeJson(docReceived, jsonReceived);

    // Check if there were any errors in deserialization
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // Extract values from the JSON object
    float receivedTemp = docReceived["temperature"];
    float receivedHumidity = docReceived["humidity"];

    // Use these values as needed
    Serial.print("Received Temperature: ");
    Serial.println(receivedTemp);
    Serial.print("Received Humidity: ");
    Serial.println(receivedHumidity);
  }

  // Part 2: Serialize and send data to ESP

  // Create a JSON document
  StaticJsonDocument<200> docToSend;

  // Add data to the JSON object
  docToSend["temperature"] = 26.3;
  docToSend["humidity"] = 55.1;

  // Serialize JSON to a string
  String jsonToSend;
  serializeJson(docToSend, jsonToSend);

  // Send JSON string over Serial3
  Serial3.println(jsonToSend);

  delay(1000); // Delay for 1 second
}
