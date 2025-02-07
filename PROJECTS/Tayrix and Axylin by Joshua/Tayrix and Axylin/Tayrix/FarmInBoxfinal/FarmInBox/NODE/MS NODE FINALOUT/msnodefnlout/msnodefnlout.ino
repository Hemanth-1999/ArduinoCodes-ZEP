#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <Wire.h>
#include <OneWire.h>//https://github.com/PaulStoffregen/OneWire/tree/v2.3.2
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define ss 17
#define rst 25
#define dio0 39
# define analogInPin2 A6//34
# define analogInPin1 A7//35
//17.3850° N, 78.4867° E
float latitude= 17.3850 , longitude= 78.4867;
String  lat_str , lng_str;

Adafruit_BME280 bme; // I2C

unsigned long delayTime;


TinyGPSPlus gps;
HardwareSerial SerialGPS(1);



String outgoing;              // outgoing messsage

byte msgCount = 0;            // count of outgoing messsages
byte MasterNode = 0xFF;
byte Node1 = 0xBB;
 int rep=0;
int AirValue = 590;   //you need to replace this value with Value_1
int WaterValue = 300;  //you need to replace this value with Value_2

int soilMoistureValue = 0;
int soilmoisturepercent = 0;

int AirValue_2 = 590;   //you need to replace this value with Value_1
int WaterValue_2 = 300;  //you need to replace this value with Value_2
int soilMoistureValue_2 = 0;
int soilmoisturepercent_2 = 0;
 int soil1 ;
  int soil2 ;
float temperature;
float Pressure;
float humidity;

float temperatureF;
float temperatureC;

String Mymessage = "";
String incoming = "";

int chipId;

const int oneWireBus = 4; // GPIO where the DS18B20 is connected to

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void GPS() {
  
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      
      if (gps.location.isValid())
      {
        
        latitude = gps.location.lat();
        lat_str = (latitude , 6);
        longitude = gps.location.lng();
        lng_str = (longitude , 6);
        Serial.print("Latitude = ");
        Serial.println(lat_str);
        Serial.print("Longitude = ");
        Serial.println(lng_str);
      }
    }
  }
}




void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient addresss
  byte sender = LoRa.read();            // sender addresss
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.println(incoming);
  Serial.print("INCOMING: ");
  int Val = incoming.toInt();
  if (Val == 10)

    Mymessage = Mymessage  + chipId + "," + soilMoistureValue + "," + soil1 + "," + soilMoistureValue + "," + soil1 + "," + temperature + "," + Pressure + "," + humidity  + "," + temperatureC + "," + temperatureF + "," + lat_str + "," + lng_str;    
  sendMessage(Mymessage, MasterNode, Node1);
  delay(100);
  Mymessage = "";
}


void sendMessage(String outgoing, byte MasterNode, byte Node1) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination addresss
  LoRa.write(Node1);             // add sender addresss
  LoRa.write(msgCount);                 // add messsage ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;  // increment messsage ID
  Serial.print("SENDING: ");
  Serial.println(outgoing);
}

void setup() {
  Serial.begin(115200);  // initialize serial
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  sensors.begin(); // Start the DS18B20 sensor
  Serial.println(F("BME280 test"));

  Serial.println();

  while (!Serial);

  Serial.println("LoRa Node1");

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  bool status;

  // default settings
  // (you can also passs in a Wire library object like &Wire2)
  status = bme.begin(0x76);
//  if (!status) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");
//    while (1);
//  }


  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  Serial.print("Chip ID: "); Serial.println(chipId);

}

int GetSoilVal(int pin) {
  delay(2000);
  soilMoistureValue = analogRead(pin);//35

  soilmoisturepercent = map(soilMoistureValue, 3500, 1500, 0, 100);

  return soilmoisturepercent;
}
void loop()
{
  onReceive(LoRa.parsePacket());
 
  if(rep<=10)
  {
    
    GPS();
    Serial.println("rep");
    Serial.println(rep);
    rep++;
    delay(1000);
  }
//   soil1 = GetSoilVal(A7);
//  soil2 = GetSoilVal(A6);


  temperature = bme.readTemperature();
  Pressure = (bme.readPressure() / 100.0F);
  humidity = bme.readHumidity();

  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

  temperatureF = sensors.getTempFByIndex(0);


  
}
