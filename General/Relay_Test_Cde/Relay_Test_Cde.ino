#define switch0_on  18
#define switch1_on  19
#define switch2_on  21
#define switch3_on  22
#define switch4_on  23
#define switch5_on  34
#define switch6_on  35
#define switch7_on  25

#define relay0 9
#define relay1 26
#define relay2 27
#define relay3 14
#define relay4 12
#define relay5 13

void setup() {
  // put your setup code here, to run once:
  pinMode(switch0_on, INPUT);
  pinMode(switch1_on, INPUT);
  pinMode(switch2_on, INPUT);
  pinMode(switch3_on, INPUT);
  pinMode(switch4_on, INPUT);
  pinMode(switch5_on, INPUT);

  pinMode(relay0, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
}

void loop() {

//    if (digitalRead (switch0_on) == HIGH) {
//
//      digitalWrite(relay0, HIGH);
//  }

   if (digitalRead (switch0_on) == LOW) {

    digitalWrite(relay0, HIGH);

  }

  if (digitalRead (switch1_on) == HIGH) {

      digitalWrite(relay1, HIGH);
  }

  else if (digitalRead (switch1_on) == LOW) {

    digitalWrite(relay1, LOW);

  }

  if (digitalRead (switch2_on) == LOW) {

      digitalWrite(relay2, HIGH);
  }

  else if (digitalRead (switch2_on) == LOW) {

    digitalWrite(relay2, LOW);

  }

  if (digitalRead (switch3_on) == HIGH) {

      digitalWrite(relay3, HIGH);
  }

  else if (digitalRead (switch3_on) == LOW) {

    digitalWrite(relay3, LOW);

  }

  if (digitalRead (switch4_on) == HIGH) {

      digitalWrite(relay4, HIGH);
  }

  else if (digitalRead (switch4_on) == LOW) {

    digitalWrite(relay4, LOW);

  }

  if (digitalRead (switch5_on) == HIGH) {

      digitalWrite(relay5, HIGH);
  }

  else if (digitalRead (switch5_on) == LOW) {

    digitalWrite(relay5, LOW);

  }

}
