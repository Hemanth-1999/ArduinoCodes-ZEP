// Constants
const int ina169OutputPin = A0;  // Analog pin connected to the OUT pin of INA169
const float shuntResistorValue = 10;  // Value of the shunt resistor in ohms
const float voltageReference = 5.0;     // Reference voltage of the Arduino (5V for most boards)

// Variables
float outputVoltage = 0.0;  // Output voltage from INA169
float current = 0.0;        // Measured current in amperes

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Print startup message
  Serial.println("INA169 Current Measurement");
}

void loop() {
  // Read the analog voltage from the INA169 OUT pin
  int analogValue = analogRead(ina169OutputPin);

  // Convert the analog reading to a voltage
  outputVoltage = (analogValue / 1023.0) * voltageReference;

  // Calculate the current based on the output voltage
  // Current (I) = Output Voltage / Shunt Resistor Value
  current = outputVoltage / shuntResistorValue;

  // Print the measured current to the Serial Monitor
  Serial.print("Output Voltage: ");
  Serial.print(outputVoltage, 6);  // Print with 6 decimal places for precision
  Serial.println(" V");
  Serial.print("Measured Current: ");
  Serial.print(current, 6);        // Print the current with precision
  Serial.println(" A");

  // Wait a bit before taking the next reading
  delay(1000);  // 1 second delay
}
