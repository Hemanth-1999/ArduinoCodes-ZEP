// Define control signal pins
const int brkPin = 5;
const int enPin  = 6;
const int frPin  = 9;
const int svPin  = 10;

void setup() {
  // Initialize control signal pins
  pinMode(brkPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(frPin, OUTPUT);
  pinMode(svPin, OUTPUT);
  

}

void loop() {
  // Example control sequences
  // Set motor speed (0-255)
  analogWrite(svPin, 128); // 50% speed
  delay(2000); // Run for 2 seconds
  digitalWrite(brkPin, LOW); // BRK and COM disconnected -> brake
  delay(2000); // Brake for 2 seconds
  digitalWrite(enPin, HIGH);   // EN and COM disconnected -> motor stop

}
