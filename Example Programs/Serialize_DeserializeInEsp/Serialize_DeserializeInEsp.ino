#include <ArduinoJson.h>

void setup() {
    Serial.begin(9600); // Initialize Serial communication with Mega
}

void loop() {
    // Part 1: Serialize and send data to Mega

    // Create a JSON document
    StaticJsonDocument<200> docToSend;

    // Add data to the JSON object
    docToSend["temperature"] = 25.5;
    docToSend["humidity"] = 60.2;

    // Serialize JSON to a string
    String jsonToSend;
    serializeJson(docToSend, jsonToSend);

    // Send JSON string over Serial
    Serial.println(jsonToSend);

    // Part 2: Receive and deserialize data from Mega

    if (Serial.available()) {
        // Read the incoming JSON string
        String jsonReceived = Serial.readStringUntil('\n');

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

    delay(1000); // Delay for 1 second
}
