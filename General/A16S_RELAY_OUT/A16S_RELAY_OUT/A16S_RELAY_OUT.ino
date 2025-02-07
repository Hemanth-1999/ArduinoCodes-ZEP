#include "Arduino.h"
#include "PCF8574.h"

// Set i2c address
PCF8574 pcf8574_1(0x24, 4, 5);
PCF8574 pcf8574_2(0x25, 4, 5);

void setup()
{
  Serial.begin(115200);
  //  delay(1000);

  // Set pinMode to OUTPUT
  pcf8574_1.pinMode(P0, OUTPUT);
  pcf8574_1.pinMode(P1, OUTPUT);
  pcf8574_1.pinMode(P2, OUTPUT);
  pcf8574_1.pinMode(P3, OUTPUT);
  pcf8574_1.pinMode(P4, OUTPUT);
  pcf8574_1.pinMode(P5, OUTPUT);
  pcf8574_1.pinMode(P6, OUTPUT);
  pcf8574_1.pinMode(P7, OUTPUT);

  pcf8574_2.pinMode(P0, OUTPUT);
  pcf8574_2.pinMode(P1, OUTPUT);
  pcf8574_2.pinMode(P2, OUTPUT);
  pcf8574_2.pinMode(P3, OUTPUT);
  pcf8574_2.pinMode(P4, OUTPUT);
  pcf8574_2.pinMode(P5, OUTPUT);
  pcf8574_2.pinMode(P6, OUTPUT);
  pcf8574_2.pinMode(P7, OUTPUT);


  Serial.print("Init pcf8574_1...");
  if (pcf8574_1.begin()) {
    Serial.println("PCF8574_1_OK");
  } else {
    Serial.println("PCF8574_1_KO");
  }

  Serial.print("Init pcf8574_2...");
  if (pcf8574_2.begin()) {
    Serial.println("PCF8574_2_OK");
  } else {
    Serial.println("PCF8574_2_KO");
  }


}

void loop()
{
  for (int i = 0; i <= 7; i++)
  {
    pcf8574_1.digitalWrite(i, LOW);
    delay(500);
  }
  for (int j = 0; j <= 7; j++)
  {
    pcf8574_2.digitalWrite(j, LOW);
    delay(500);
  }
  for (int i = 0; i <= 7; i++)
  {
    pcf8574_1.digitalWrite(i, HIGH);
    delay(500);
  }
  for (int j = 0; j <= 7; j++)
  {
    pcf8574_2.digitalWrite(j, HIGH);
    delay(500);
  }

}
