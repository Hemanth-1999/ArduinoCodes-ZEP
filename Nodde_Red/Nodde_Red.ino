#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "ZEPTOGREENS 2G";
const char* password = "Newzepto@20";

const char* broker_address = "broker.hivemq.com";
const uint16_t broker_port = 1883;
const char* device_id = "qwerty12345";

const char* mqtt_username = "Zeptogreens";
const char* mqtt_password = "123456";

const char* topic = "ISA95/Ixorio/Calterio/plc/data";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(broker_address, broker_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Generate random data for 10 parameters
  float floorTemperature = random(1000, 1201) / 10.0;  // Degree Celsius
  float coolingRate = random(50, 150) / 10.0;  // Cooling rate
  float airPressure = random(800, 1200) / 100.0;  // Bar
  String vibrationStatus = (random(3) == 0) ? "High" : (random(2) == 0) ? "Moderate" : "Low";
  int humidity = random(30, 70);  // Percentage
  float powerConsumption = random(50, 200) / 10.0;  // kW units
  int conveyorSpeed = random(2500, 3001);  // RPM
  String machineStatus = (random(2) == 0) ? "Running" : "Stopped";
  String alarms = (random(2) == 0) ? "ON" : "OFF";
  int numWorkers = random(28, 31);  // Number of workers

  // Print the generated data
  Serial.print("Floor Temperature: ");
  Serial.print(floorTemperature, 1);
  Serial.println(" °C");

  Serial.print("Cooling Rate: ");
  Serial.println(coolingRate, 1);

  Serial.print("Air Pressure: ");
  Serial.print(airPressure, 2);
  Serial.println(" Pa");

  Serial.print("Vibration Status: ");
  Serial.println(vibrationStatus);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Power Consumption: ");
  Serial.print(powerConsumption, 1);
  Serial.println(" kW");

  Serial.print("Conveyor Speed: ");
  Serial.println(conveyorSpeed);
  Serial.println("Rpm");


  Serial.print("Machine Status: ");
  Serial.println(machineStatus);

  Serial.print("Alarms: ");
  Serial.println(alarms);

  Serial.print("Number of Workers: ");
  Serial.println(numWorkers);

  // Publish the data to the MQTT topic
  DynamicJsonDocument jsonDocument(256);
  jsonDocument["FloorTemperature"] = floorTemperature;
  jsonDocument["CoolingRate"] = coolingRate;
  jsonDocument["AirPressure"] = airPressure;
  jsonDocument["VibrationStatus"] = vibrationStatus;
  jsonDocument["Humidity"] = humidity;
  jsonDocument["PowerConsumption"] = powerConsumption;
  jsonDocument["ConveyorSpeed"] = conveyorSpeed;
  jsonDocument["MachineStatus"] = machineStatus;
  jsonDocument["Alarms"] = alarms;
  jsonDocument["NumWorkers"] = numWorkers;

  char jsonBuffer[256];
  serializeJson(jsonDocument, jsonBuffer);

  client.publish(topic, jsonBuffer);

  delay(1000);  // Adjust the delay as needed
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(device_id, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds");
      delay(5000);
    }
  }
}
