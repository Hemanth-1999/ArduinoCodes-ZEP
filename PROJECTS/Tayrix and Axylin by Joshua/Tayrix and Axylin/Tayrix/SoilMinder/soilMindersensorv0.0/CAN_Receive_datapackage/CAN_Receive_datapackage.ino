
#include <CAN.h>
#include <ArduinoJson.h>
#define TX_GPIO_NUM   21  // Connects to CTX
#define RX_GPIO_NUM   22  // Connects to CRX
const char MESSAGE_DELIMITER = '\n';  // Delimiter character for message end
const int MAX_MESSAGE_SIZE = 128; 
//==================================================================================//
void setup() {
  Serial.begin (115200);
  while (!Serial);
  delay (1000);
  Serial.println ("CAN Receiver/Receiver");
  // Set the pins
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  // start the CAN bus at 500 kbps
  if (!CAN.begin (500E3)) {
    Serial.println ("Starting CAN failed!");
    while (1);
  }
  else {
    Serial.println ("CAN Initialized");
  }
}
void loop()
{
   if (CAN.parsePacket()) {
    if (CAN.packetExtended()) {
      unsigned long canId = CAN.packetId();
      Serial.print("Received Extended ID: 0x");
      Serial.println(canId, HEX);
    } else {
      unsigned int canId = CAN.packetId();
      Serial.print("Received Standard ID: 0x");
      Serial.println(canId, HEX);
    }

    while (CAN.available()) {
      byte data = CAN.read();
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
