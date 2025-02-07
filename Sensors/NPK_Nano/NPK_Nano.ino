#include <SoftwareSerial.h>
#include <Wire.h>

// Define RS485 pins for RE and DE to switch between transmit and receive mode
#define RS485_RE 8
#define RS485_DE 7

// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

// A byte array to store NPK values
byte values[11];

// SoftwareSerial object to communicate with the RS485 module
SoftwareSerial modbus(2, 3); // RX, TX

void setup() {
  // Start serial communication with the computer
  Serial.begin(9600);

  // Start serial communication with the RS485 module
  modbus.begin(9600);

  // Set RS485 pins as outputs
  pinMode(RS485_RE, OUTPUT);
  pinMode(RS485_DE, OUTPUT);

  // Turn off RS485 receiver and transmitter initially
  digitalWrite(RS485_RE, LOW);
  digitalWrite(RS485_DE, LOW);

  // Wait for the RS485 module to initialize
  delay(500);
}

void loop() {

  Serial.print("Nitrogen: ");
  Serial.print(readValue(nitro, sizeof(nitro)));
  Serial.println(" mg/kg");

  Serial.print("Phosphorous: ");
  Serial.print(readValue(phos, sizeof(phos)));
  Serial.println(" mg/kg");

  Serial.print("Potassium: ");
  Serial.print(readValue(pota, sizeof(pota)));
  Serial.println(" mg/kg");


  // Wait for 2 seconds before reading values again
  delay(2000);
}


// Sends a Modbus RTU request and reads the response to get a value
//byte readValue(const byte* request)
byte readValue(const byte* request, byte requestSize) {
  // Set RS485 module to transmit mode
  digitalWrite(RS485_RE, HIGH);
  digitalWrite(RS485_DE, HIGH);

  // Send Modbus RTU request to the device
  modbus.write(request, requestSize);

  // Set RS485 module to receive mode
  digitalWrite(RS485_RE, LOW);
  digitalWrite(RS485_DE, LOW);

  // Wait for the response to be received
  delay(10);

  // Read the response into the values array
  byte responseLength = modbus.available();
  for (byte i = 0; i < responseLength; i++) {
    values[i] = modbus.read();
  }

  // Return the value from the response
  return values[3] << 8 | values[4];
}
