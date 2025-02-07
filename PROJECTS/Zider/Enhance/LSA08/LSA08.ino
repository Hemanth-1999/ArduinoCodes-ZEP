#define RX_PIN 16 // Define the RX pin for Serial2 (change as needed)
#define TX_PIN 17 // Define the TX pin for Serial2 (change as needed)
#define LSA08_ERROR 255 // Error code for no data

// Function to read the LSA08 sensor data
unsigned char Read_LSA08(unsigned char address, float* angle) {
    unsigned char buffer[8];  // Buffer to hold received data
    int index = 0;            // Index for buffer
    unsigned long startTime = millis(); // Start time for timeout

    // Wait for data with a 2-second timeout
    while (Serial2.available() == 0) {
        if (millis() - startTime > 2000) {  // Timeout after 2 seconds
            Serial.println("No data received within the timeout period.");
            return LSA08_ERROR;  // Return error code
        }
    }

    // Read available data into the buffer
    while (Serial2.available()) {
        buffer[index++] = Serial2.read();  // Read byte into buffer
        if (index >= sizeof(buffer)) {
            break;  // Prevent buffer overflow
        }
    }

    // Debugging: print the received data in HEX format
    Serial.print("Received data: ");
    for (int i = 0; i < index; i++) {
        Serial.print(buffer[i], HEX); // Print in HEX format
        Serial.print(" ");
    }
    Serial.println();

    // Check if the first byte indicates an error
    if (buffer[0] != LSA08_ERROR) {
        *angle = ((float)buffer[1] + ((float)buffer[2] / 100.0));  // Adjust based on sensor data format
        return buffer[0];  // Return status code
    }

    return LSA08_ERROR;  // Return error if data is invalid
}

void setup() {
    Serial.begin(115200); // Initialize Serial for debugging
    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize Serial2 with LSA08 sensor baud rate

    Serial.println("Starting LSA08 sensor reading...");
}

void loop() {
    float angle = 0;  // Variable to hold the angle position
    unsigned char status = Read_LSA08(0x01, &angle);  // Example: address is 0x01 (adjust as per your sensor)

    if (status != LSA08_ERROR) {
        Serial.print("Line Position: ");
        Serial.println(angle);  // Print the line position
    } else {
        Serial.println("Error reading sensor.");  // Error message if reading fails
    }

    delay(1000); // Wait for 1 second before the next reading
}
