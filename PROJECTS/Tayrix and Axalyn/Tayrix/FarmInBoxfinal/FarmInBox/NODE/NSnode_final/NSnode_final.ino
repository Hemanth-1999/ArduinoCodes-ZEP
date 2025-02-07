#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define ss 6
#define rst 4
#define dio0 5
#define RE 7
#define DE 8
long int minn;
#include <TinyGPS.h>
float lat=17.3850 , lon=78.4867 ;
TinyGPS gps;
bool modbool = false;
String outgoing;
byte msgCount = 0;
byte MasterNode = 0xFF;
byte Node2 = 0xCC;
String Mymessage = "";
String incoming = "";
String latitude;
String longitude; 
byte val1, val2, val3;
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];
SoftwareSerial mod(2, 3);
SoftwareSerial gpsSerial(9, 10);
int chipId ;
void onReceive(int packetSize) {

  if (packetSize == 0) return;          // if there's no packet, return
  Serial.println("we are here");

  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();

  }

  Serial.println(incoming);
  Serial.print("INCOMING: ");
  int Val = incoming.toInt();
  if (Val == 20)

  {
    Mymessage = Mymessage  + chipId + "," +  val1 + "," +val2 + "," +val3+ "," +latitude + "," +longitude ;
    sendMessage(Mymessage, MasterNode, Node2);
    delay(100);
    Mymessage = "";
  }

}
void sendMessage(String outgoing, byte MasterNode, byte Node2) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node2);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;  // increment message ID
  Serial.print("SENDING: ");
  Serial.println(outgoing);
}

void setup()
{
  Serial.begin(115200);

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  while (!Serial);
  Serial.println("LoRa Node2");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  bool status;


  chipId = 1232;
  Serial.print("Chip ID: "); Serial.println(chipId);
}

void loop()

{

  onReceive(LoRa.parsePacket());
  
 unsigned long maxx = millis();
  if(maxx-minn>4000)
  {
    minn=maxx;
 
  val1 = nitrogen();
  delay(150);
  val2 = phosphorous();
  delay(150);
  val3 = potassium();
  delay(150);
  }
  
 if(modbool==false)
 {
  callserial();
 }

}

byte nitrogen() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(nitro, sizeof(nitro)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}
void callserial()
{
  if (modbool == false)
  {
    gpsSerial.begin(9600);
    while (gpsSerial.available()) { // check for gps data
      if (gps.encode(gpsSerial.read())) // encode gps data
      {
        gps.f_get_position(&lat, &lon);
      }
    }
    latitude = String(lat, 6);
    longitude = String(lon, 6);
    Serial.println(latitude + ";" + longitude);
    delay(1000);
  
  }
  if(((lat>0)||(lon>0))&&(modbool ==false))
  {
    modbool = true;
    mod.begin(9600);
  }
}
