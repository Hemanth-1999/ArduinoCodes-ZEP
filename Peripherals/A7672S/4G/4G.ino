#include <RadioLib.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

// LoRa
#define LORA_MOSI 27 // 23
#define LORA_MISO 19 // 19
#define LORA_SCK 5   // 18
#define LORA_CS 18   // 21

#define LORA_RST 14  // 22
#define LORA_DIO0 26  // 32
#define LORA_DIO1 14 // 35

#define FREQUENCY 915.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1, SPI, SPISettings());

// SIMCom A7672S Setup (SoftwareSerial for communicating with the module)
HardwareSerial mySerial2(2);
#define DEBUG true
#define IO_RXD2 17
#define IO_TXD2 16

const char* mqttServer = "164.52.223.248";
const char* mqttPort = "1883";
const char* mqttUser = "axalyn";
const char* mqttPassword = "cenaura@2024";
const char* mqttTopic = "axalyn";

String receivedLoRaData = "";

// Function Prototypes
void setupSIM();
void connectToMQTT();
void sendMQTTMessage(String topic, String message);
void reconnectMQTT();
void checkIncomingMessages();

void setup() {
  // Serial Monitor
  Serial.begin(9600);

  // SIM Module setup
  mySerial2.begin(115200, SERIAL_8N1, IO_RXD2, IO_TXD2); // Start SoftwareSerial for SIM module
  delay(1000);  // Allow time for initialization

  // LoRa Module setup
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // Initialize SIMCom Module
  if (!setupSIM()) {  // Assuming setupSIM returns a boolean or error code
    Serial.println(F("SIMCom initialization failed!"));
    while (true);  // Halt if SIM fails to initialize
  }

  // Initialize SX1276 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, OUTPUT_POWER, PREAMBLE_LEN, GAIN);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);  // Halt if LoRa fails to initialize
  }

  Serial.println(F("Waiting for incoming transmission ... "));

  // Connect to MQTT server
  if (!connectToMQTT()) {  // Assuming connectToMQTT returns a boolean or error code
    Serial.println(F("MQTT connection failed!"));
    while (true);  // Halt if MQTT connection fails
  }
}


void loop() {
  // LoRa receive logic
  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print data of the packet
    Serial.print(F("Data:\t\t\t"));
    Serial.println(str);

    // print RSSI (Received Signal Strength Indicator) of the last received packet
    Serial.print(F("RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print SNR (Signal-to-Noise Ratio) of the last received packet
    Serial.print(F("SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error of the last received packet
    Serial.print(F("Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));
    Serial.println();

    delay(5000);

    const char* jsonData = "{\"sensor\":\"BME280\", \"value\":23.5}"; // Replace with your actual JSON data
    sendMQTTPublishPacket(jsonData);
    delay(60000); // Send data every minute

  }

  // Check for MQTT connection and reconnect if necessary
  checkIncomingMessages();
}

// SIMCom A7672S initialization
bool setupSIM() {
    // Initialize the SIMCom module
    Serial.println(F("Initializing SIMCom module..."));

    // Add SIMCom initialization code here
    // For example, sending AT commands to ensure it's working
    // Example AT command check (pseudo code):
    mySerial2.println("AT");
    delay(100);
    
    if (mySerial2.available()) {
        String response = mySerial2.readString();
        if (response.indexOf("OK") != -1) {
            Serial.println(F("SIMCom module initialized successfully"));
            return true;  // SIM initialized successfully
        } else {
            Serial.println(F("SIMCom module initialization failed"));
            return false;  // SIM initialization failed
        }
    } else {
        Serial.println(F("No response from SIMCom module"));
        return false;  // No response
    }
}


// Function to send AT commands to SIMCom and wait for a response
// Function to send AT commands to SIMCom and wait for a response
void sendSIMCommand(String command, int delayTime) {
  mySerial2.println(command);
  delay(delayTime);
  while (mySerial2.available()) {
    Serial.write(mySerial2.read());
  }
}

// Connect to MQTT broker via SIMCom A7672S
void connectToMQTT() {
  sendSIMCommand("AT+CIPSHUT", 2000);
  sendSIMCommand("AT+CIPMUX=0", 1000);

  String connectCommand = "AT+CIPSTART=\"TCP\",\"" + String(mqttServer) + "\"," + mqttPort;
  sendSIMCommand(connectCommand, 5000);

  delay(2000);
  sendMQTTConnectPacket();
}

void sendMQTTConnectPacket() {
  String mqttPacket =
    String((char)0x10) + // Connect packet type
    String((char)0x00) + String((char)0x00) + // Remaining length placeholder
    "MQTT" + // Protocol Name
    String((char)0x04) + // Protocol Level (0x04 for MQTT 3.1.1)
    String((char)0xC2) + // Connect Flags (Username and Password)
    String((char)0x00) + String((char)0x3C) + // Keep Alive (60 seconds)
    "ABCDEF" + // Client ID (replace with your actual client ID)
    String(mqttUser) + // Username
    String(mqttPassword); // Password

  // Update the remaining length
  mqttPacket.setCharAt(1, mqttPacket.length() - 2); // Calculate and set length

  mySerial2("AT+CIPSEND", 2000);
  mySerial2.print(mqttPacket); // Send the MQTT connect packet
  mySerial2.write(0x1A); // End the message with Ctrl+Z
  delay(2000);
}

void sendMQTTPublishPacket(const char* jsonData) {
  String topic = mqttTopic;
  String payload = jsonData;
  int topicLength = topic.length();
  int payloadLength = payload.length();

  // Construct the MQTT publish packet
  String mqttPublishPacket =
    String("\x30") + // Publish packet type
    String((char)(2 + topicLength + payloadLength)) + // Remaining length
    topic + // Topic
    payload; // Payload

  // Send the packet
  mySerial2("AT+CIPSEND", 2000);
  mySerial2.print(mqttPublishPacket); // Send the MQTT publish packet
  mySerial2.write(0x1A); // End the message with Ctrl+Z
  delay(2000);
}

// Check incoming MQTT messages or reconnect if necessary
void checkIncomingMessages() {
  if (mySerial2.available()) {
    while (mySerial2.available()) {
      char c = mySerial2.read();
      Serial.print(c); // Print incoming messages or status updates
    }
  }
  reconnectMQTT(); // Reconnect if disconnected
}

// Reconnect to MQTT broker if connection is lost
void reconnectMQTT() {
  if (!mySerial2.available()) {
    Serial.println("Reconnecting to MQTT...");
    connectToMQTT();
  }
}
