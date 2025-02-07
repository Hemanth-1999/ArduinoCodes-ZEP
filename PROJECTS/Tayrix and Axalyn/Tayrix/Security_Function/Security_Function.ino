#include <AESLib.h>
#include <ArduinoJson.h>

byte key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
byte cypher[16]; // AES block size is 16 bytes for AES-128.
byte decryptedtext[600]; // Buffer size for decrypted data, adjust it as needed.
AES aes;

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
  Serial.println("----------");
  delay(1000); // Add a small delay to avoid confusion in the output.
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
  Serial.println("After Encryption (in hexadecimal):");
  for (size_t i = 0; i < size; i += 16)
  {
    aes.encrypt(plaintext + i, cypher + i); // Store the encrypted data in the cypher buffer.
    for (size_t j = 0; j < 16; j++)
    {
      Serial.print(cypher[i + j], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  // Decryption: Decrypt the encrypted data in blocks of 16 bytes.
  for (size_t i = 0; i < size; i += 16)
  {
    aes.decrypt(cypher + i, decryptedtext + i);
  }

  Serial.println("After Decryption:");
  for (size_t i = 0; i < size; i++)
  {
    Serial.write(decryptedtext[i]);
  }
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  jsonpackage();
  delay(3000);
}
