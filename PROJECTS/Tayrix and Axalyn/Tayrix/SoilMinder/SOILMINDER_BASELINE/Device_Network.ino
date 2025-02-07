void DeviceNetwork() {
  for (int i = 0; i <= 4; i++) {
    uint8_t network[] = {
      2,  /*Automatic*/
      13, /*GSM only*/
      38, /*LTE only*/
      51  /*GSM and LTE only*/
    };
    Serial.printf("Try %d method\n", network[i]);
    modem.setNetworkMode(network[i]);
    delay(3000);
    bool isConnected = false;
    int tryCount = 60;
    while (tryCount--) {
      String netoworkOperator = modem.getOperator();
      Serial.print("Operator: ");
      Serial.println(netoworkOperator);
      int16_t signal =  modem.getSignalQuality();
      Serial.print("Signal: ");
      Serial.println(signal);
      Serial.print("isNetworkConnected: ");
      isConnected = modem.isNetworkConnected();
      Serial.println( isConnected ? "CONNECTED" : "NOT CONNECTED YET");
      if (isConnected) {
        break;
      }
      delay(1000);
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    if (isConnected) {
      break;
    }
  }
}
