void manualstatus()
{
//  if (Serial.available() > 0) {

    receivedData = Serial.readStringUntil('\n');

    // Debug: Print the received data
    Serial.print("Received data: ");
    Serial.println(receivedData);

    if (receivedData.length() > 0) {
      lastData = receivedData;  // Store the last received payload

      // Allocate a JsonDocument to deserialize the JSON data
      StaticJsonDocument<1024> Manualstatus;
      deserializeJson(Manualstatus, Serial);
      DeserializationError error = deserializeJson(Manualstatus, Serial);

      if (!error) {

        Manualstatus_f1 = Manualstatus["f1"];
        Manualstatus_f2 = Manualstatus["f2"];
        Manualstatus_f3 = Manualstatus["f3"];
        Manualstatus_f4 = Manualstatus["f4"];
        Manualstatus_f5 = Manualstatus["f5"];
        Manualstatus_f6 = Manualstatus["f6"];
        Manualstatus_f7 = Manualstatus["f7"];
        Manualstatus_f8 = Manualstatus["f8"];
        Manualstatus_cp = Manualstatus["cp"];
        Manualstatus_co = Manualstatus["co"];
        Manualstatus_fp = Manualstatus["fp"];
        Manualstatus_fs = Manualstatus["fs"];
        Manualstatus_ir = Manualstatus["ir"];
        Manualstatus_do = Manualstatus["do"];
        Manualstatus_dr = Manualstatus["dr"];
        Manualstatus_pb = Manualstatus["pb"];

        // Debugging: Print the extracted values
        Serial.println("New data received and parsed:");
        Serial.print("Fan1: "); Serial.println(Manualstatus_f1);
        Serial.print("Fan2: "); Serial.println(Manualstatus_f2);
        Serial.print("Fan3: "); Serial.println(Manualstatus_f3);
        Serial.print("Fan4: "); Serial.println(Manualstatus_f4);
        Serial.print("Fan5: "); Serial.println(Manualstatus_f5);
        Serial.print("Fan6: "); Serial.println(Manualstatus_f6);
        Serial.print("Fan7: "); Serial.println(Manualstatus_f7);
        Serial.print("Fan8: "); Serial.println(Manualstatus_f8);       
      } else {

        return;
      }
    }
    // If no new data has arrived, keep using the last received data
    if (lastData.length() > 0) {
      Serial.println("Data from mega:");
      Serial.println(lastData);
    } else {
      Serial.println("No data received yet.");
    }
//  }
}
