#include "Arduino.h"
#include "PCF8574.h"

// Set i2c address

PCF8574 pcf8574_1(0x24, 4, 5);
PCF8574 pcf8574_2(0x25, 4, 5);

PCF8574 pcf8574_IN1(0x22, 4, 5);
PCF8574 pcf8574_IN2(0x23, 4, 5);

void setup()
{
  Serial.begin(115200);
  //  delay(1000);

  // Set pinMode to OUTPUT
  for (int i = 0; i <= 7; i++)
  {
    pcf8574_IN1.pinMode(i, INPUT);
  }
  for (int i = 0; i <= 7; i++)
  {
    pcf8574_IN2.pinMode(i, INPUT);
  }

  for (int i = 0; i <= 7; i++)
  {
    pcf8574_1.pinMode(i, OUTPUT);
  }

  for (int i = 0; i <= 7; i++)
  {
    pcf8574_2.pinMode(i, OUTPUT);
  }

  pcf8574_1.begin();
  pcf8574_2.begin();
  pcf8574_IN1.begin();
  pcf8574_IN2.begin();
  /*pcf8574_IN1.pinMode(P0, INPUT);
    pcf8574_IN1.pinMode(P1, INPUT);
    pcf8574_IN1.pinMode(P2, INPUT);
    pcf8574_IN1.pinMode(P3, INPUT);
    pcf8574_IN1.pinMode(P4, INPUT);
    pcf8574_IN1.pinMode(P5, INPUT);
    pcf8574_IN1.pinMode(P6, INPUT);
    pcf8574_IN1.pinMode(P7, INPUT);

    pcf8574_IN2.pinMode(P0, INPUT);
    pcf8574_IN2.pinMode(P1, INPUT);
    pcf8574_IN2.pinMode(P2, INPUT);
    pcf8574_IN2.pinMode(P3, INPUT);
    pcf8574_IN2.pinMode(P4, INPUT);
    pcf8574_IN2.pinMode(P5, INPUT);
    pcf8574_IN2.pinMode(P6, INPUT);
    pcf8574_IN2.pinMode(P7, INPUT);

    pcf8574_IN1.begin();
    pcf8574_IN2.begin();
    /*Serial.print("Init pcf8574_IN1...");
    if (pcf8574_IN1.begin()){
    Serial.println("pcf8574_IN1_OK");
    }else{
    Serial.println("pcf8574_IN1_KO");
    }

    Serial.print("Init pcf8574_IN2...");
    if (pcf8574_IN2.begin()){
    Serial.println("pcf8574_IN2_OK");
    }else{
    Serial.println("pcf8574_IN2_KO");
    }*/


}

void loop() {
  for (int i = 0; i <= 7; i++)
  {
    if (pcf8574_IN1.digitalRead(i) == LOW)
    {
      Serial.printf("KEY  %d PRESSED\n", i + 1);
      pcf8574_1.digitalWrite(i, LOW);
    } else pcf8574_1.digitalWrite(i, HIGH);
    delay(10);
  }


  for (int i = 0; i <= 7; i++)
  {
    if (pcf8574_IN2.digitalRead(i) == LOW)
    {
      Serial.printf("KEY %d PRESSED\n", i + 9);
      pcf8574_2.digitalWrite(i, LOW);
    } else pcf8574_2.digitalWrite(i, HIGH);
    delay(10);
  }

}
