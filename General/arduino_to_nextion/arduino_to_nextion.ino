void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Hello World");
  
}

void loop()
{
void digitalWrite(uint8_t, uint8_t);
    Serial2.print("kai.val=" + String(LOW));    
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(2000);
void digitalWrite(uint8_t, uint8_t);
    Serial2.print("kai.val=" + String(HIGH));
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(2000);
}
