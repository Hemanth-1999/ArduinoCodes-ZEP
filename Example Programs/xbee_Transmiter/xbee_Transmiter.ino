
#include <HardwareSerial.h>
HardwareSerial SerialXBee(1);
int BUTTON = 5;
boolean toggle = false;
void setup()
{
Serial.begin(9600);
pinMode(BUTTON, INPUT_PULLUP);
XBee.begin(9600);
}
void loop()
{
if (digitalRead(BUTTON) == LOW && toggle)
{
Serial.println("Turn on LED");
toggle = false;
XBee.write('1');
delay(1000);
}
else if (digitalRead(BUTTON) == LOW && !toggle)
{
Serial.println("Turn off LED");
toggle = true;
XBee.write('0');
delay(1000);
}
}
