/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#define relay1 12
#define relay2 13
#define relay3 14
#define relay4 15

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
}

void loop() {
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  digitalWrite(relay1, LOW);
  Serial.println("relay1 on");
  digitalWrite(relay2, LOW);
  Serial.println("relay2 on");
  //  digitalWrite(relay3, LOW);
  //  Serial.println("relay3 on");
  //  digitalWrite(relay4, LOW);
  //  Serial.println("relay4 on");
  delay(5000);

  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay1, HIGH);
  Serial.println("relay1 off");
  digitalWrite(relay2, HIGH);
  Serial.println("relay2 off");
  delay(5000);
}
