/*Multiple LoRa Communication
  example code by miliohm.com */

#include <SPI.h>
#include <LoRa.h>

String inString = "";
int val = 0;
const long frequency = 915E6;
int led = 13;
unsigned long int millisBefore;

int turn = 1;
boolean statusRepeater = 0;

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }
}

void loop()
{
  if (statusRepeater == 0) {
    if (millis() - millisBefore < 1000) {
      if (turn == 1) {
        Serial.println("Send Request 1");
        SendRequest("C1");
        waitForAnswer();
        turn = 2;
      }
      waitForAnswer();
    } else if ((millis() - millisBefore > 1000) && (millis() - millisBefore < 2000)) { //jika waktu kurang dari 4 detik
      if (turn == 2) {
        Serial.println("Send Request 2");
        SendRequest("G2");
        waitForAnswer();
        turn = 3;
      }
    } else if (millis() - millisBefore > 6000) {
      if (turn == 3) {
        Serial.println("Send Request 3");
        SendRequest("G3");
        waitForAnswer();
        SendRequest("G1");
        waitForAnswer();
        millisBefore = millis();
        turn = 1;
      }
    }
  }
}

void waitForAnswer()
{
   // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      val = inString.toInt();       
    }
    inString = "";     
    LoRa.packetRssi();    
  }
      
  Serial.println(val);  
}

void SendRequest(String request) { //fungsi untuk Send request dengan variable parameter request
  String dataTotal = request;
  int dataLength = dataTotal.length(); dataLength ++;
  uint8_t total[dataLength];
  dataTotal.toCharArray(total, dataLength);
  LoRa.send(total, dataLength);
  LoRa.waitPacketSent();
}
