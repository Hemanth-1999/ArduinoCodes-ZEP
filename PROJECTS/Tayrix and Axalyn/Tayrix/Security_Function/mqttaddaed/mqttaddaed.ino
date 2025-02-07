#include <WiFi.h>
#include <PubSubClient.h>
#include <AESLib.h>
#include <ArduinoJson.h>

const char* ssid = "Zeptogreens";
const char* password = "Zepto@23";
const char* unlockKey = "Your_Unlock_Key";
bool serialPortLocked = true;
const char* mqttBroker = "192.168.0.102";
const int mqttPort = 1883;
const char* mqttUser = "Zeptogreens";
const char* mqttPassword = "123456";
const char* mqttTopic = "Tayrix/data";
WiFiClient espClient;
PubSubClient mqttClient(espClient);
byte key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
byte cypher[600]; // Increase the buffer size to store the encrypted data.
byte decryptedtext[600]; // Buffer size for decrypted data, adjust it as needed.
AES aes;

void setupWifi()
{
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to Wi-Fi. IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMQTT(PubSubClient& mqttClient)
{
  mqttClient.setServer(mqttBroker, mqttPort);
  // Set the MQTT broker credentials if required.
  // mqttClient.setCredentials(mqttUser, mqttPassword);
}

void reconnectMQTT(PubSubClient& mqttClient)
{
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Client",mqttUser,mqttPassword))
    {
      Serial.println("Connected to MQTT broker.");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void jsonpackage()
{
  DynamicJsonDocument doc(500);
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["AT"] = random(20, 90);
  TXCC["RH"] = random(20, 90);
  TXCC["BP"] = random(20, 90);
  TXCC["SL"] = random(20, 90);
  TXCC["VPD"] = random(20, 90);
  TXCC["DP"] = random(20, 90);
  TXCC["ET"] = random(20, 90);

  size_t jsonSize = measureJson(doc); // Measure the JSON data size without writing to a buffer.
  byte buffer[jsonSize + 16 - (jsonSize % 16)]; // Add padding to the buffer to make it a multiple of 16 bytes.
  memset(buffer, 0, sizeof(buffer)); // Initialize buffer to 0.
  serializeJson(doc, buffer, jsonSize); // Serialize JSON into the buffer.

  encryption(buffer, sizeof(buffer));

  // Encode the encrypted data in Base64
  size_t encodedSize = base64_enc_len(sizeof(cypher));
  char base64Encoded[encodedSize];
  base64_encode(base64Encoded, (const char*)cypher, sizeof(cypher));
   Serial.println(base64Encoded);
  // Publish the Base64 encoded data to MQTT
  mqttClient.publish(mqttTopic, base64Encoded);
  Serial.println("Base64 encoded data published to MQTT topic.");
}

void encryption(const byte* plaintext, size_t size)
{
  aes.set_key(key, sizeof(key)); // Setting Key for AES

  Serial.println("Before Encryption:");
  for (size_t i = 0; i < size; i++)
  {
    Serial.write(plaintext[i]);
  }

  // Encrypt the JSON data in blocks of 16 bytes.
  Serial.print("");
   Serial.println("After encryption:::->");
  Serial.print("{ ");
for (size_t i = 0; i < size; i += 16)
{
  aes.encrypt(plaintext + i, cypher + i); // Store the encrypted data in the cypher buffer.
  for (size_t j = 0; j < 16; j++)
  {
    Serial.print("0x");
    if (cypher[i + j] < 0x10) {
      Serial.print("0"); // Print leading zero if the value is less than 0x10 (single-digit value).
    }
    Serial.print(cypher[i + j], HEX);
    Serial.print(", ");
  }
}
Serial.println("0x00 }");


  // Decryption: Decrypt the encrypted data in blocks of 16 bytes.
  Serial.println("After Decryption (in hexadecimal):");
  for (size_t i = 0; i < size; i += 16)
  {
    aes.decrypt(cypher + i, decryptedtext + i);
    for (size_t j = 0; j < 16; j++)
    {
      Serial.print(decryptedtext[i + j], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("Actual Data after Decryption:");
  for (size_t i = 0; i < size; i++)
  {
    Serial.write(decryptedtext[i]);
  }
  Serial.println();
}



void setup()
{
  Serial.begin(9600);

  setupWifi();
  setupMQTT(mqttClient);
}

void loop()
{
    if (serialPortLocked) {
    unlockSerialPort();
  }
  if (!mqttClient.connected())
  {
    reconnectMQTT(mqttClient);
  }

  mqttClient.loop();

  jsonpackage();
  delay(3000);
}
void unlockSerialPort() {
  Serial.println("Enter the unlock key:");
  while (Serial.available() == 0) {
    // Wait for input
  }

  String inputKey = Serial.readStringUntil('\n');
  inputKey.trim();

  if (inputKey == unlockKey) {
    Serial.println("Serial port unlocked.");
    serialPortLocked = false;
  } else {
    Serial.println("Incorrect key. Try again.");
    Serial.flush();
  }
}
