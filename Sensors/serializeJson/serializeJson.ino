#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Adafruit_TCS34725.h"
#include <SparkFunTSL2561.h>
#define UVOUT 15
#define REF_3V3  4

//mosquitto_sub -d -u Zeptogreens -P 123456 -t

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
SFE_TSL2561 light;

uint16_t r, g, b, c, colorTemp, RGBlux;
double lux;    // Resulting lux value
boolean good;  // True if neither sensor is saturated
unsigned int data0, data1;
boolean gain;
unsigned int ms;

//Use the 3.3V power pin as a reference to get a very accurate output value from sensor
bool a;
char Sensordata[1024];
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";
const char* mqtt_server = "192.168.0.173";
const char* mqttUsername = "Zeptogreens";
const char* mqttpsw = "123456";
const char* msg = "Sensor Values";

WiFiClient espClient;
PubSubClient client(espClient);

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
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUsername, mqttpsw)) {
      Serial.println("connected");
      a = true;
    } else {
      a = false;
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void sensor() {

  int uvLevel  = analogRead(UVOUT);
  int refLevel = analogRead(REF_3V3);

  float outputVoltage = 3.3 / refLevel * uvLevel;

  float uvIntensity;

  DynamicJsonDocument doc(500);
  JsonObject Ws = doc.createNestedObject("Sensordata");

  Ws["RGBlux"]                    =    RGBlux;
  Ws["R"]                         =    r;
  Ws["G"]                         =    g;
  Ws["B"]                         =    b;
  Ws["C"]                         =    c;
  Ws["Lum"]                       =    lux;
  Ws["IR"]                        =    data0;
  Ws["Visible"]                   =    data1;
  Ws["output"]                    =    refLevel;
  Ws["ML8511 output:"]            =    uvLevel;
  Ws["UV Intensity (mW/cm^2):"]   =    uvIntensity;

  serializeJson(doc, Serial);
  Serial.println(" ");
  serializeJson(doc, Sensordata);
  client.publish(msg, Sensordata);
}

void setup() {

  Serial.println(msg);
  Serial.begin(115200);
  delay(100);
  setup_wifi();
  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }

  else

  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  Serial.println("TSL2561 example sketch");

  light.begin();

  gain = 0;

  unsigned char time = 2;

  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);

  Serial.println("Powerup...");
  light.setPowerUp();

  {
    pinMode(UVOUT, INPUT);
    pinMode(REF_3V3, INPUT);
    Serial.println("ML8511 example");
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void printError(byte error) {
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(3000);
  delay(ms);
  sensor();

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  RGBlux = tcs.calculateLux(r, g, b);

  if (light.getData(data0, data1))
  {

    good = light.getLux(gain, ms, data0, data1, lux);

  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }

}
