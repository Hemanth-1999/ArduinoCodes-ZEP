//This is the default code present in all the shipped modules
// This code enables to publish test messages to the well known public broker by hivemq.com at http://www.hivemq.com/demos/websocket-client/
// Go to this webpage and click on Connect. Click on Add New Topic Subscription and type topic name as "4GLTE/testTopic"
// If the module is connected to Internet via 4G, test messages will be published by the module and these messages will be displayed on the screen at regular intervals
//refer AT command datasheet
//FOR VVM501 PRODUCT DETAILS VISIT www.vv-mobility.com

#define TINY_GSM_MODEM_SIM7600  // SIM7600 AT instruction is compatible with A7670
#define SerialAT Serial1
#define SerialMon Serial
#define TINY_GSM_USE_GPRS true

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <U8x8lib.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define RXD2 17
#define TXD2 16
#define powerPin 4
int rx = -1;

#define PIN_DTR     25
#define PWR_PIN     32

#define LED_PIN 2
#define sw 5
#define led 7
#define BUTTON_DEBOUNCE_DELAY 50 // 50 ms debounce time

// OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

int flag = 1;
int flag1 = 0;
int state = 0;

String res;
String rxString;

const char* topicName = "axalyn"; // or enter a topic name of your choice

struct MqttSettings
{
  char Broker[20] = "164.52.223.248"; //"pag.tayrix.com";
  int Port = 1883; //"30883";
  char ClientId[20] = "ESP32";
  char Username[60] =  "axalyn"; //"DEVICE_USERNAME@6b3331f5-ef48-4936-9394-ac368c7bd033";
  char Password[20] = "cenaura@2024"; //"DEVICE_PASSWORD";
  char Topic[20] = "axalyn"; //"telemetry";
  char Farm[20] = {'\0',};
  char Field[20] = {'\0',};
  char Zone[20] = {'\0',};
} MqttCredentials;

String payload = "temp: 30";  //test message

char yourMQTTServer[50];
char yourMQTTPort[20];
int yourMQTTPortNumber = 0;
int LED_BUILTIN = 2;  //Default LED Blink for Message Transmit Indication
int ledStatus = LOW;

//Set APN as per your sim card:
//Vi (Vodafone Idea) -> "vi.internet"
//Airtel -> airtelgprs.com
//Voda -> portalnmms
//BSNL -> bsnlnet
//jio -> jionet

const char apn[] = "www"; //APN automatically detects for 4G SIM, NO NEED TO ENTER, KEEP IT BLANK
const char gprsUser[] = "";
const char gprsPass[] = "";

void SIMCOM();
void DeviceNetwork();
void ReconnectToNetwork();

void modemPowerOn() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);
}
void modemPowerOff() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);
  digitalWrite(PWR_PIN, HIGH);
}
void modemRestart() {
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}
void SIMCOM() {
  //Blue LED on the board use as an indicator
  //If blinking: Modem not able to boot
  //If turned ON: connected to network
  //If turned OFF: Modem booted successfully but not connected to network, check your SIM, network coverage etc.

  digitalWrite(LED_PIN, LOW);

  Serial.println("Running SIMCOMATI command...");
  SerialAT.println("+SIMCOMATI"); //Get the module information
  modem.waitResponse(1000L, res);
  res.replace(GSM_NL "OK" GSM_NL, "");
  Serial.println(res);
  res = "";
  Serial.println();

  Serial.println("Preferred mode selection (GSM/LTE)...");
  SerialAT.println("+CNMP?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  res = "";

  Serial.println("Network mode connectivity testing (GSM, LTE or GSM/LTE)...");

  DeviceNetwork();

  digitalWrite(LED_PIN, HIGH); //Modem connected to network

  Serial.println();
  Serial.println("Device is connected to Sim network.");
  Serial.println();

  delay(1000);

  Serial.println("Testing the a sample MQTTS Call to pipe drive server...");
  //  Serial.println("NOTE: Please ensure to deploy an end point on Pipe Dream (pipedream.com) to test. Example: " + send_data_to_url);
  Serial.println();

}
void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(100);

  modemPowerOn();
  SerialAT.begin(115200, SERIAL_8N1, 16, 17);
  Serial.clearWriteError();
  Serial.println();
  Serial.println("Tayrix...");
  Serial.println();
  delay(2000);
  String res;
  Serial.println("Initializing Modem...");

  if (!modem.init()) {
    digitalWrite(LED_PIN, HIGH);
    modemRestart();
    delay(2000);
    Serial.println("Failed to restart modem, attempting to continue without restarting");
    digitalWrite(LED_PIN, LOW);
    return;
  }

  SIMCOM();

  u8x8.begin();

  // MQTT Broker setup
  mqtt.setServer(MqttCredentials.Broker, 1883);
  mqtt.setCallback(callback);

  // Connect to MQTT broker
  connectToMqtt(MqttCredentials.Broker, MqttCredentials.Port, MqttCredentials.Username, MqttCredentials.Password, MqttCredentials.Topic);

}

unsigned long lastPublishTime = 0; // for non-blocking delay
unsigned long lastDebounceTime = 0; // for button debouncing
int buttonState = HIGH; // Assume the button is not pressed

String a;

void loop() {
  // Check network connection
  if (!modem.isNetworkConnected()) {
    Serial.println("Network disconnected");
    if (!modem.waitForNetwork(60000L, true)) {
      Serial.println("Failed to reconnect to network");
      ReconnectToNetwork(); // Call the Reconnect function
      return; // Exit loop until reconnected
    }
    Serial.println("Network re-connected");

    // Check if GPRS is connected
    if (!modem.isGprsConnected()) {
      Serial.println("GPRS disconnected!");
      if (!modem.gprsConnect(apn)) {
        Serial.println("Failed to reconnect GPRS");
        ReconnectToNetwork(); // Call the Reconnect function
        return; // Exit loop until reconnected
      }
      Serial.println("GPRS reconnected");
    }
  }

  // Handle MQTT connection
  if (modem.gprsConnect(apn)) {
    if (!mqtt.connected()) {
      reconnect(); // Just in case we get disconnected from MQTT server
    }

    // Handle button input with debounce
    int reading = digitalRead(sw);
    if (reading != buttonState) {
      lastDebounceTime = millis(); // reset the debounce timer
    }

    if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
      if (reading != flag1) {
        
        // Publish Message
        flag1 = reading;
        digitalWrite(led, flag1 == LOW ? HIGH : LOW); // Toggle LED based on button press
        Serial.println("Publishing Message: LED OFF");
        
        u8x8.clearDisplay();
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        u8x8.setCursor(0, 4);
        u8x8.print("Publishing Data");
        delay(5000);
        
        // Construct and send MQTT Publish command
        mqtt.publish(topicName, flag1 == LOW ? "a" : "b");

        // Assuming you still want to keep the AT command logic for MQTT
        // Use a proper function to handle sending AT commands
        sendAtCommand("AT+CMQTTPUB=0,1,60"); // Acknowledgment
      }
    }

    mqtt.loop(); // Handle MQTT client tasks
  }

  // Receive Modem Response
  while (SerialAT.available() > 0) {
    delay(10);
    a = SerialAT.readString();
    processModemResponse(a); // Separate function to handle response
  }

  unsigned long rstresp = 0;
  unsigned long rstresp1 = millis();

  if (rstresp1 - rstresp > 500000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }

  delay(1000); // Optional: consider using a non-blocking delay method
}

void sendAtCommand(String command) {
  SerialAT.println(command);
  delay(1000); // You might want to replace this with a non-blocking approach
}

void processModemResponse(String response) {
  Serial.println(response);
  if (response.indexOf("PAYLOAD") != -1) {
    int new1 = response.indexOf("PAYLOAD");
    String new3 = response.substring(new1);
    int new4 = new3.indexOf('\n');
    String message = new3.substring(0, new4);
    message.trim(); // Trim whitespace

    Serial.print("Message is: ");
    Serial.println(message);

    if (message == "a") {
      state = 1;
      digitalWrite(led, HIGH);
      Serial.println("LED ON");
    } else if (message == "b") {
      state = 0;
      flag1 = 0;
      digitalWrite(led, LOW);
      Serial.println("LED OFF");
    }
  }
}
