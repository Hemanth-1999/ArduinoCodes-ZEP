#include <Arduino.h>

#define TAG      "RS485_MODB_APP"
#define mod      Serial2
#define DI       17
#define RO       16
#define RE       5
#define DE       4
#define BUF_SIZE 20
#define TIMEOUT  500
#define USE_CAST false

#define MOIST 0
#define TEMP  1
#define EC    2
#define PH    3
#define NITRO 4
#define PHOS  5
#define POTA  6


//all together.. 7 in 1, first 7 registers from device address 1..
const byte dataMod[7][8] = {
  {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A},//moist
  {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA},//temp
  {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA},//ec
  {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A},//ph
  {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xc5, 0xcb},//nitro
  {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0b},//phos
  {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b} //pot
};//pot

//same but address 255 (broadcast) any connected sensor
//should respond, the sensor will replace the 255 with their
//currently configured address, change USE_CAST to true to use
const byte dataMod_B[7][8] = {
  {0xFF, 0x03, 0x00, 0x00, 0x00, 0x01, 0x91, 0xD4},//moist
  {0xFF, 0x03, 0x00, 0x01, 0x00, 0x01, 0xC0, 0x14},//temp
  {0xFF, 0x03, 0x00, 0x02, 0x00, 0x01, 0x30, 0x14},//ec
  {0xFF, 0x03, 0x00, 0x03, 0x00, 0x01, 0x61, 0xD4},//ph
  {0xFF, 0x03, 0x00, 0x04, 0x00, 0x01, 0xD0, 0x15},//nitro
  {0xFF, 0x03, 0x00, 0x05, 0x00, 0x01, 0x81, 0xD5},//phos
  {0xFF, 0x03, 0x00, 0x06, 0x00, 0x01, 0x71, 0xD5} //pot
};

//incoming buffer
byte buf[BUF_SIZE];

void setup() {
  Serial.begin(115200);
  //check different bauds could be 4800
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  Serial.println("Ready..");
}

void loop() {
  uint16_t val1;
  uint16_t val2;
  uint16_t val3;
  uint16_t val4;
  uint16_t val5;
  uint16_t val6;
  uint16_t val7;       

  Serial.println("Moisture: ");
  val1 = GetModVal(MOIST);
  float Val1 = val1 * 0.1;
  Serial.print(Val1);
  Serial.println(" %");
  Serial.println("-----");
  delay(1000);

  Serial.println("Temperature: ");
  val2 = GetModVal(TEMP);
  float Val2 = val2 * 0.1;
  Serial.print(Val2);
  Serial.println(" *C");
  Serial.println("-----");
  delay(1000);

  Serial.println("EC: ");
  val3 = GetModVal(EC);
  Serial.print(val3);
  Serial.println(" us/cm");
  Serial.println("-----");
  delay(1000);

  Serial.println("Ph: ");
  val4 = GetModVal(PH);
  float Val4 = val4 * 0.1;
  Serial.print(Val4);
  Serial.println(" ph");
  Serial.println("-----");
  delay(1000);

  Serial.println("Nitrogen (N): ");
  val5 = GetModVal(NITRO);
  Serial.print(val5);
  Serial.println(" mg/kg\n");
  Serial.println("-----");
  delay(1000);

  Serial.println("Phosphorous (P): ");
  val6 = GetModVal(PHOS);
  Serial.print(val6);
  Serial.println(" mg/kg\n");
  Serial.println("-----");
  delay(1000);

  Serial.println("Potassium (K): ");
  val7 = GetModVal(POTA);
  Serial.print(val7);
  Serial.println(" mg/kg\n");
  Serial.println("-----");

  delay(5000);

}

uint16_t GetModVal(byte val) {
  uint32_t startTime = 0;
  uint8_t  byteCount = 0;
  memset(buf, 0, sizeof(buf)); //empty incoming..
  //send request..
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (USE_CAST)
  mod.write(dataMod[val], sizeof(dataMod_B[val])); else
  mod.write(dataMod[val], sizeof(dataMod[val]));
  mod.flush();//wait for outgoing to be sent..
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  //recv response until timeout expires..
  Serial.print("Response in HEX: ");
  startTime = millis();
  while (millis() - startTime <= TIMEOUT) {
    if (mod.available() && byteCount < sizeof(buf)) {
      buf[byteCount++] = mod.read();
      printHexByte(buf[byteCount - 1]);
    }
  }
  Serial.println();
  //combine 2 byte into word..
  return (uint16_t)(buf[3] << 8 | buf[4]);
}

void printHexByte(byte b)
{
  Serial.print(b , HEX);
  Serial.print(':');
}
