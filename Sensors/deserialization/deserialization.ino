#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RunningAverage.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "millisDelay.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiClient espClient;
PubSubClient client(espClient);
millisDelay getvalue;
millisDelay rlvalue;

int Sensordata_RGBlux ; // 331
int Sensordata_R ; // 638
int Sensordata_G; // 591
int Sensordata_B ; // 539
int Sensordata_C ; // 1257
double Sensordata_Lum ; // 33.73167527
int Sensordata_IR; // 291
int Sensordata_Visible ; // 144
bool Sensordata_good ; // true
int Sensordata_output ; // 0
int Sensordata_ML8511_output_ ;
int Sensordata_UV_Intensity_mW_cm_2_ ;

///DECLARATION
uint32_t chipid;
const char* eventogo;
const char* eventogo1;
const char* eventogo2;
const char* alertmode1;
const char* alertlevel;
const char* valuedata;
char tempstring1[1024];
char tempstring2[1024];
char tempstring11[1024];
char RELAYSTRING[1024];

///MQTT SETUP
const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";
const char* broker = "192.168.0.173";
const int port = 1883;
const char* mqttUser = "Zeptogreens";
const char* mqttPassword = "123456";
const char* FDSFDS = "Sensor Values";

void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  valuedata = doc["data"];

  if (JsonObject Sensordata = doc["Sensordata"])
  {
    Sensordata_RGBlux = Sensordata["RGBlux"]; // 331
    Sensordata_R = Sensordata["R"]; // 638
    Sensordata_G = Sensordata["G"]; // 591
    Sensordata_B = Sensordata["B"]; // 539
    Sensordata_C = Sensordata["C"]; // 1257
    Sensordata_Lum = Sensordata["Lum"]; // 33.73167527
    Sensordata_IR = Sensordata["IR"]; // 291
    Sensordata_Visible = Sensordata["Visible"]; // 144
    Sensordata_output = Sensordata["output"]; // 0
    Sensordata_ML8511_output_ = Sensordata["ML8511 output:"];
    Sensordata_UV_Intensity_mW_cm_2_ = Sensordata["UV Intensity (mW/cm^2):"];
    Serial.println(" ");
    Serial.println("RGBlux "); Serial.println(Sensordata_RGBlux);
    Serial.println("R "); Serial.println(Sensordata_R);
    Serial.println("G "); Serial.println(Sensordata_G);
    Serial.println("B "); Serial.println(Sensordata_B);
    Serial.println("C "); Serial.println(Sensordata_C);

    Serial.println("Lum "); Serial.println(Sensordata_Lum);
    Serial.println("IR "); Serial.println(Sensordata_IR);
    Serial.println("Visible "); Serial.println(Sensordata_Visible);
    Serial.println("output "); Serial.println(Sensordata_output);

    Serial.println("ML8511 output: "); Serial.println(Sensordata_ML8511_output_);
    Serial.println("UV Intensity (mW/cm^2): "); Serial.println(Sensordata_UV_Intensity_mW_cm_2_);
  }
}
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      client.subscribe(FDSFDS);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup()
{

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    yield();
  }
  Serial.println("Connected to the WiFi network");

  timeClient.begin();
  timeClient.setTimeOffset(19800);

  client.setServer(broker, port);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  timeClient.update();
  client.loop();

}
