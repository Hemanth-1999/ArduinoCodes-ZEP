#include <BME280I2C.h>
#include <Wire.h>
#include<ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define TIP_SENSOR_PIN 4
#define CALIBRATE_FACTOR 60
#define RAIN_BUCKET_SIZE 120
#define DWELL_TIME 40

const unsigned long oneHour = 3600000UL;
unsigned long lastTipTime;
unsigned long lastRainTime; //Used for rainRate calculation
int rainBucket [RAIN_BUCKET_SIZE + 1]; /* 24 hours x 5 Days = 120 hours */
int rainRate = 0;
int rainWindow = 72;         //default rain window in hours.
volatile int wasTippedBuffer = 0;
int rainSensorThreshold = 50; //default rain sensor sensitivity in hundredths.
int state = 0;
int oldState = 2; //Setting the default to something other than 1 or 0
int lastRainRate = 0;
int lastMeasure = 0;
bool gotTime = false;
int lastHour;
int currentHour;
int  hour;

int direction;
float speed_mph;

int UVOUT = 35; //Output from the sensor
int REF_3V3 = 34; //3.3V power

#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];

int RXPin = 16, TXPin = 17;

// Replace the next variables with your SSID/Password combination
const char* ssid = "TayrixPrecision";
const char* password = "Tayrix@123";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.100.5";
const char* mqtt_server = "192.168.100.5";
const char* mqttUsername = "svaasafarm";
const char* mqttpsw = "svaasafarmmqtt";

BME280I2C bme; // I2C
                                  
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

float uvIntensity;

float temp(NAN);
float hum(NAN);
float  pres(NAN);

float latitude , longitude;
String  lat_str , lng_str;

const char* WHNode = "WHSNODE";

char tempString[1024];

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
uint32_t rainTotal(uint8_t hours)
{
  uint32_t total = 0;
  for (uint8_t i = 0; i <= hours; i++)
  {
    total += rainBucket [i];
  }
  return total;
}
void sensorTipped()
{
  unsigned long thisTipTime = millis();
  if (thisTipTime - lastTipTime > 100UL)
  {
    rainBucket[0] += CALIBRATE_FACTOR; // adds CALIBRATE_FACTOR hundredths of unit each tip
    wasTippedBuffer++;
  }
  lastTipTime = thisTipTime;
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUsername, mqttpsw)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //  client.publish(ZoneNSensor);
      // ... and resubscribe
      //client.subscribe();

    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void UVRead() {




  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;

  uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level

  Serial.print("output: ");
  Serial.print(refLevel);

  Serial.print("ML8511 output: ");
  Serial.print(uvLevel);

  Serial.print(" / ML8511 voltage: ");
  Serial.print(outputVoltage);

  Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.print(uvIntensity);


  Serial.println();
  delay(1200);
}
void speedcal() {

  int sensorpim = analogRead(32);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorpim * (5.0 / 4095.0);
  // print out the value you read:
  Serial.println(voltage);

  float wind_speed = mapfloat(voltage, 0.0, 2, 0, 32.4);
  speed_mph = ((wind_speed * 3600) / 1609.344);
  Serial.print("Wind Speed =");
  Serial.print(wind_speed);
  Serial.println("m/s");
  Serial.print(speed_mph);
  Serial.println("mph");
  delay(3000);
}

void directioncal() 
{

  int sensorValue = analogRead(33);
  float volt = sensorValue * 10 / 4095;
  direction = map(sensorValue, 0, 4095, 0, 360);
  // print out the value you read:
  Serial.print("ADC : ");
  Serial.println(sensorValue);
  Serial.print("Voltage : ");
  Serial.println(volt);
  Serial.print("Direction : ");
  Serial.println(direction);
  delay(300);

  if (direction < 11.25)
    Serial.println("N");
  else if (direction < 33.75)
    Serial.println("NNE");
  else if (direction < 56.25)
    Serial.println("NE");
  else if (direction < 78.75)
    Serial.println("ENE");
  else if (direction < 101.25)
    Serial.println("E");
  else if (direction < 123.25)
    Serial.println("ESE");
  else if (direction < 146)
    Serial.println("SE");
  else if (direction < 168.75)
    Serial.println("SSE");
  else if (direction < 191.25)
    Serial.println("S");
  else if (direction < 213.75)
    Serial.println("SSW");
  else if (direction < 236.75)
    Serial.println("SW");
  else if (direction < 258.75)
    Serial.println("WSW");
  else if (direction < 281)
    Serial.println("W");
  else if (direction < 303.75)
    Serial.println("WNW");
  else if (direction < 326)
    Serial.println("NW");
  else if (direction < 348.75)
    Serial.println("NNW");
  else
    Serial.println("N");

}
void raincal() {

  // let's constantly check to see if the rain in the past rainWindow hours is greater than rainSensorThreshold
  //
  uint16_t measure = 0; // Check to see if we need to show sensor tripped in this block
  for (uint8_t i = 0; i < rainWindow; i++)
  {
    measure += rainBucket [i];
    if (measure != lastMeasure)
    {
      //      DEBUG_PRINT(F("measure value (total rainBucket within rainWindow): "));
      //      DEBUG_PRINTLN(measure);
      lastMeasure = measure;
    }
  }
  //
  state = (measure >= (rainSensorThreshold * 100));
  if (state != oldState)
  {

    Serial.print(F("New Sensor State... Sensor: "));
    Serial.println(state ? "Tripped" : "Not Tripped");
    oldState = state;
  }
  //
  unsigned long tipDelay = millis() - lastRainTime;
  if (wasTippedBuffer) // if was tipped, then update the 24hour total and transmit to Vera
  {
    Serial.println(F("Sensor Tipped"));
    Serial.print(F("rainBucket [0] value: "));
    Serial.println(rainBucket [0]);
    ((float)rainTotal(currentHour) / 100, 1); //Calculate the total rain for the day

    wasTippedBuffer--;
    rainRate = ((oneHour) / tipDelay);
    if (rainRate != lastRainRate)
    {


      Serial.print(F("RainRate= "));
      Serial.println(rainRate);
      lastRainRate = rainRate;
    }
    lastRainTime = lastTipTime;
  }
  //

  if (currentHour != lastHour)
  {
    Serial.println(F("One hour elapsed."));
    ((float)rainTotal(currentHour) / 100, 1); // send today's rainfall

    Serial.print(F("rainBucket[0] = "));
    Serial.println(rainBucket[0]);
    for (int16_t i = RAIN_BUCKET_SIZE - 1; i >= 0; i--)//cascade an hour of values back into the array
    {
      rainBucket [i + 1] = rainBucket [i];
    }
    rainBucket[0] = 0;
  }


}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // initialize serial
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(TIP_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt(TIP_SENSOR_PIN), sensorTipped, FALLING);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

  client.publish(WHNode, tempString);
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also passs in a Wire library object like &Wire2)
  while (!Serial) {} // Wait

  Wire.begin();
//
//  while (!bme.begin())
//  {
//    Serial.println("Could not find BME280 sensor!");
//    delay(1000);
//  }

//  switch (bme.chipModel())
//  {
//    case BME280::ChipModel_BME280:
//      Serial.println("Found BME280 sensor! Success.");
//      break;
//    case BME280::ChipModel_BMP280:
//      Serial.println("Found BMP280 sensor! No Humidity available.");
//      break;
//    default:
//      Serial.println("Found UNKNOWN sensor! Error!");
//  }

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


void loop() {

  json();
  bme.read(pres, temp, hum);
//  raincal();
  UVRead();
//  speedcal();
//  directioncal();

  if (!client.connected()) {
    reconnect();

  }
  client.loop();
  delay(100);

 while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
  latitude = gps.location.lat();
  lat_str = String(latitude , 6);
  longitude = gps.location.lng();
  lng_str = String(longitude , 6);
  Serial.print("Latitude = ");
  Serial.println(lat_str);
  Serial.print("Longitude = ");
  Serial.println(lng_str);
     }
   }
 }
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);

}
void json() {
  DynamicJsonDocument doc1(1024);

  doc1["WS"] = speed_mph;
  doc1["WD"] = direction;
  doc1["uv"] = uvIntensity;
  doc1["TEMP"] = temp;
  doc1["HUM"] = hum;
  doc1["PRE"] = pres;
  doc1["LAT"] = lat_str;
  doc1["LON"] = lng_str;

  serializeJson(doc1, Serial);
  Serial.println();

  DynamicJsonDocument doc3(1024);
  doc3["metrics"] = doc1;
  serializeJson(doc3, tempString);
  client.publish(WHNode, tempString);
  memset(tempString, 0, 1024);
}
