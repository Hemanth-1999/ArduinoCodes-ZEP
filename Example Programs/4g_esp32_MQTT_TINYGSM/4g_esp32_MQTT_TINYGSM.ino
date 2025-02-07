#define TINY_GSM_MODEM_SIM7600  // SIM7600 AT instruction is compatible with A7670
#define SerialAT Serial1
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true
#include <TinyGsmClient.h>
#include <PubSubClient.h>

#define RXD2 17    // VVM501 MODULE RXD INTERNALLY CONNECTED
#define TXD2 16    // VVM501 MODULE TXD INTERNALLY CONNECTED
#define powerPin 4  // VVM501 MODULE ESP32 PIN D4 CONNECTED TO POWER PIN OF A7670C CHIPSET, INTERNALLY CONNECTED

int LED_BUILTIN = 2;
int ledStatus = LOW;

char mqttServer[40] = "164.52.223.248"; // MQTT Server IP
char port_str[40] = "1883"; // string representation of the port
uint16_t mqttPort = atoi(port_str); // convert port string to integer
char mqttUserName[60] = "axalyn"; // MQTT Username
char mqttPwd[40] = "cenaura@2024"; // MQTT Password
char clientID[40] = "username0001";

const char *broker         = "broker.hivemq.com"; // MQTT Broker
const char *led_on_off     = "axalyn"; // Subscribe topic for LED control
const char *message        = "axalyn"; // Publish topic for messages
const char *topic = "axalyn"; // Publish topic for LED status

const char apn[] = ""; // APN automatically detects for 4G SIM, keep it blank

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);
PubSubClient mqtt(client);

/*
Developed by Nandu, Anandu, Unnikrishnan 
Company: Elementz Engineers Guild Pvt Ltd
*/
#define sw 5
#define led 7
int flag = 1;
int flag1 = 0;
int state=0;
String Publish = "led/publish"; //Publish Topic
String Subscribe = "led/subscribe"; //Subscribe Topic

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  delay(100);
  digitalWrite(powerPin, HIGH);
  delay(1000);
  digitalWrite(powerPin, LOW);

  Serial.println("\nConfiguring VVM501 Module. Kindly wait");

  delay(10000);

  SerialAT.begin(115200, SERIAL_8N1, RXD2, TXD2);

  // Restart the modem
  SerialAT.println("AT+CFUN=1"); // Full functionality
  delay(2000); // Wait for a couple of seconds

  // Check response
  if (SerialAT.available()) {
    String response = SerialAT.readString();
    Serial.print("Restart response: ");
    Serial.println(response);
  } else {
    Serial.println("No response from modem during restart");
  }

  Serial.println("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println("Failed to connect to the network");
    return;
  }
  Serial.println("Network connected");

  // GPRS connection parameters
  if (!modem.gprsConnect(apn)) {
    Serial.println("Failed to connect to GPRS");
    return;
  }
  Serial.println("GPRS connected");

  // MQTT Broker setup
  mqtt.setServer(broker, 1883);
  mqtt.setCallback(callback);

  // Connect to MQTT broker
  connectToMqtt();
}

void loop() {
  if (!mqtt.connected()) {
    reconnect(); // Just in case we get disconnected from MQTT server
  }
  mqtt.publish(message, "HELLO FROM VVM501 ESP32 4G LTE MODULE");
  Serial.println("HELLO FROM VVM501 ESP32 4G LTE MODULE");
  delay(1000);
  SerialAT.println("AT+CMQTTPAYLOAD=0,1"); //Payload length
  delay(1000);
  SerialAT.println("a"); //Payload message
  delay(1000);
  SerialAT.println("AT+CMQTTPUB=0,1,60"); //Acknowledgment
  delay(1000);

  // Check network connection
  if (!modem.isNetworkConnected()) {
    Serial.println("Network disconnected");
    if (!modem.waitForNetwork(180000L, true)) {
      Serial.println("Failed to reconnect to network");
      return;
    }
    Serial.println("Network re-connected");

    if (!modem.isGprsConnected()) {
      Serial.println("GPRS disconnected!");
      if (!modem.gprsConnect(apn)) {
        Serial.println("Failed to reconnect GPRS");
        return;
      }
      Serial.println("GPRS reconnected");
    }
  }

  mqtt.loop();
  delay(100);
}

void connectToMqtt() {
  while (!mqtt.connected()) { // Loop until connected to MQTT server
    Serial.println("Attempting MQTT connection...");

    // Start MQTT service
    SerialAT.println("ATE0");
    delay(2000);
    SerialAT.println("AT+CMQTTSTART"); //Establishing MQTT Connection
    delay(2000);
    String response = waitForResponse(2000);
    Serial.println("Got: " + response);

    if (response.indexOf("OK") == -1) {
      Serial.println("Failed to start MQTT service");
      delay(5000);
      continue;
    }

    // Set client ID
    SerialAT.println("AT+CMQTTACCQ=0,\"elementz/123\"");
    String response1 = waitForResponse(2000);
    Serial.println("Got: " + response1);

    if (response1.indexOf("OK") == -1) {
      Serial.println("Error setting client ID");
      delay(5000);
      continue;
    }

    // Connect to MQTT Broker
    // Convert mqttPwd (uint16_t) to a string
    String mqttPwdStr = String(mqttPwd);  // Converts uint16_t to String

    // Build the entire AT command as one string
    String mqttConnectCommand = "AT+CMQTTCONNECT=0,\"tcp://";
    mqttConnectCommand += mqttServer;      // Add MQTT server
    mqttConnectCommand += ":";
    mqttConnectCommand += mqttPort;        // Add MQTT port
    mqttConnectCommand += "\",90,1,\"";
    mqttConnectCommand += mqttUserName;    // Add MQTT username
    mqttConnectCommand += "\",\"";
    mqttConnectCommand += mqttPwdStr;      // Add MQTT password (converted from uint16_t to string)
    mqttConnectCommand += "\"";

    // Send the entire command at once
    SerialAT.println(mqttConnectCommand);

    // Optionally, print the command to Serial for debugging purposes
    Serial.println("Sent command: " + mqttConnectCommand);

    String response2 = waitForResponse(5000);
    Serial.println("Got: " + response2);

    // Check connection status
    if (mqtt.connected()) {
      Serial.println("Connected to Broker");
      SerialAT.println("AT+CMQTTSUBTOPIC=0,9,1"); //AT Command for Setting up the Subscribe Topic Name
      delay(2000);
      SerialAT.println(topic); //Topic Name
      delay(2000);
      SerialAT.println("AT+CMQTTSUB=0,4,1,1"); //Length of message
      delay(2000);
      SerialAT.println("HAII"); //message
      delay(2000);
      Serial.println("Done");
      mqtt.subscribe(topic);   //Subscribe to Learning Mode Topic
      break; // Exit loop if connected
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // Will attempt connection again in 5 seconds
    }
  }
}

String waitForResponse(unsigned long timeout) {
  String response = "";
  unsigned long startTime = millis();
  while (millis() - startTime < timeout) {
    while (SerialAT.available()) {
      char c = SerialAT.read();
      response += c;
      delay(5); // Small delay to allow buffer to fill
    }
  }
  return response;
}

void reconnect() {
  connectToMqtt();
}

void callback(char* topic, byte* payload, unsigned int length) { // MQTT callback function
  if (String(topic) == led_on_off) {
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
    Serial.print("ledStatus:");
    Serial.println(ledStatus);
    mqtt.publish(topic, ledStatus ? "1" : "0");
    Serial.println("Incoming message");
  } else {
    Serial.println("No matching topic");
  }
}
