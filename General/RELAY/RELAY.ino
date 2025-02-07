#include <DS3231.h>

int Relay = 4;

DS3231  rtc(SDA, SCL);
Time t;

unsigned long OnHour = 14; //SET TIME TO ON RELAY (24 HOUR FORMAT)
unsigned long OnMin = 43;
unsigned long OffHour = 14; //SET TIME TO OFF RELAY
unsigned long OffMin = 44;

void setup() {
  Serial.begin(115200);
  rtc.begin();
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
}

void loop() {
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hr(s), ");
  Serial.print(t.min);
  Serial.print(" min(s), ");
  Serial.print(t.sec);
  Serial.print(" sec(s)");
  Serial.println(" ");
  delay (1000);
  
  if(t.hour == OnHour && t.min == OnMin){
    digitalWrite(Relay,LOW);
    Serial.println("LIGHT ON");
    }
    
    else if(t.hour == OffHour && t.min == OffMin){
      digitalWrite(Relay,HIGH);
      Serial.println("LIGHT OFF");
    }
}
