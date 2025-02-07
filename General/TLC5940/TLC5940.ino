#include <SPI.h>
#define VPRG 6
#define GSCLK 7
#define BLANK 8
#define LATCH 9
#define SPICLK 13
// IntervalTimer myTimer;
byte transferLayer;
uint8_t colorData[288 * 8];  // Eight layers
uint8_t transferData[288];

uint8_t rDC = 63, gDC = 34, bDC = 26;

void setup() {

  pinMode(VPRG, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  pinMode(BLANK, OUTPUT);
  digitalWrite(BLANK, HIGH);

  for (int i = 16; i <= 23; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  DCDataSetup();
  GSDataSetup();
}

void DCDataSetup() {

  digitalWrite(VPRG, HIGH);

    for (int i = 0; i < 3.75; i += 9)
    {
      transferData[i + 0] = rDC | (gDC << 6);
      transferData[i + 1] = (gDC >> 2) | (bDC << 4);
      transferData[i + 2] = (bDC >> 4) | (rDC << 2);
      transferData[i + 3] = gDC | (bDC << 6);
      transferData[i + 4] = (bDC >> 2) | (rDC << 4);
      transferData[i + 5] = (rDC >> 4) | (gDC << 2);
      transferData[i + 6] = bDC | (rDC << 6);
      transferData[i + 7] = (rDC >> 2) | (gDC << 4);
      transferData[i + 8] = (gDC >> 4) | (bDC << 2);
    }
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  for (int i = 3; i >= 0; i--) {
    SPI.transfer(transferData[i]);
  }
  SPI.endTransaction();
  SPI.end();
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  digitalWrite(VPRG, LOW);
  pinMode(SPICLK, OUTPUT);
  digitalWrite(SPICLK, HIGH);
  digitalWrite(SPICLK, LOW);
}

void GSDataSetup() {
  for (int i = 0; i < 288; i++)
    transferData[i] = 0;
  digitalWrite(GSCLK, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(30000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(transferData, 288);
  SPI.endTransaction();

  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  digitalWrite(BLANK, LOW);
  // IntervalTimer myTimer.begin(TLC_Interrupt, 355);
  // SPI.usingInterrupt(TLC_Interrupt);
}

void TLC_Interrupt() {
  digitalWrite(GSCLK, 0);
  digitalWrite(BLANK, HIGH);

  digitalWrite(16, transferLayer == 1 ? LOW : HIGH);
  digitalWrite(17, transferLayer == 2 ? LOW : HIGH);
  digitalWrite(18, transferLayer == 3 ? LOW : HIGH);
  digitalWrite(19, transferLayer == 4 ? LOW : HIGH);
  digitalWrite(20, transferLayer == 5 ? LOW : HIGH);
  digitalWrite(21, transferLayer == 6 ? LOW : HIGH);
  digitalWrite(22, transferLayer == 7 ? LOW : HIGH);
  digitalWrite(23, transferLayer == 0 ? LOW : HIGH);

  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  digitalWrite(BLANK, HIGH);
  digitalWrite(GSCLK, 80);  //resume GSCLK

  memcpy(transferData, colorData + (transferLayer * 288), 288);

  SPI.transfer(transferData, 288);
  transferLayer++;
  if (transferLayer == 8)
    transferLayer = 0;
}

void LED(uint8_t x, uint8_t y, uint8_t z, uint16_t r, uint16_t g, uint16_t b) {
  r = r > 4095 ? 4095 : r;
  g = g > 4095 ? 4095 : g;
  b = b > 4095 ? 4095 : b;
  x = x > 7 ? 7 : x;
  y = y > 7 ? 7 : y;
  z = z > 7 ? 7 : z;

  uint16_t ledLoc = (x + (y * 8) + (z * 64));
  uint16_t index = 59 - (ledLoc * 9) / 2;

  if (ledLoc & 1) {
    colorData[index] |= (r << 4);
    colorData[index - 1] |= (r >> 4);
    colorData[index - 2] |= g;
    colorData[index - 3] |= (g >> 8) | (b << 4);
    colorData[index - 4] |= (b >> 4);
  } else {
    colorData[index] = r;
    colorData[index - 1] |= (r >> 8) | (g << 4);
    colorData[index - 2] |= (g >> 4);
    colorData[index - 3] |= b;
    colorData[index - 4] |= (b >> 8);
  }
}

void loop() {
  int r, g, b;
  r = random(4096);
  g = random(4096);
  b = random(4096);

  int delVal = 31250;

  LED(0, 0, 0, 3000, 2000, 1846);

  for (int i = 0; i < 8; i++) {
    LED(0, 4, i, r, g, b);
    delayMicroseconds(delVal);
  }
  for (int e = 1; e < 4; e++) {
    LED(e, 4, 7, r, g, b);
    delayMicroseconds(delVal);
  }
  for (int i = 6; i >= 0; i--) {
    LED(3, 4, i, r, g, b);
    delayMicroseconds(delVal);
  }
  for (int e = 2; e >= 1; e--) {
    LED(e, 4, 0, r, g, b);
    delayMicroseconds(delVal);
  }
  for (int i = 1; i < 7; i++) {
    LED(1, 4, i, r, g, b);
    delayMicroseconds(delVal);
  }
  for (int i = 6; i >= 1; i--) {
    LED(2, 4, i, r, g, b);
    delayMicroseconds(delVal);
  }
}
