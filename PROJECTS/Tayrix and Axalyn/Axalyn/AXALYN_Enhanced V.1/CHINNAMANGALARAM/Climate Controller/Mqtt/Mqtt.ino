#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <U8x8lib.h>
#include <SPIFFS.h>
#include <FS.h>
#include<EEPROM.h>
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include<WiFi.h>
#include "MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#define MQ2pin 34

// Assumed environmental values:
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
float temp(NAN), hum(NAN), pres(NAN);

BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_hPa);
EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

// OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManager wifiManager;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);

const size_t bufferSize = 100; // Adjust buffer size as per your requirement
char mqttServer[40] = "164.52.223.248"; //"pag.axalyn.com"; //
char port_str[40] = "1883"; //"30883"; // string representation of the port
uint16_t mqttPort = atoi(port_str); // convert port string to integer
char mqttUserName[60] = "axalyn"; // "DEVICE_USERNAME@6c458335-0fdc-40d9-8ab1-7606dabdb887"; //"Zeptogreens";//"zepto";//"Zeptogreens";
char mqttPwd[40] = "cenaura@2024"; // "DEVICE_PASSWORD"; // "12345";
char clientID[40] = "username0001";
String timestamp;

WiFiClient espClient;
PubSubClient client(espClient);

BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_16,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
unsigned long mtime = 0;

const int WATCHDOG_TIMEOUT = 30;  // Timeout in seconds

const long interval = 3000;  // Interval at which to perform action (in milliseconds)
const long pageInterval = 5000;  // Interval to switch pages (5 seconds)

unsigned long pageChangeMillis = 0;

int currentPage = 1;

String msgStr = "";
const char* topic1 = "esp/test"; //publish topic
const char* Rectopic = "esp/test"; //publish topic

const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT =  "Saifarm/GH2/PFC/ZoneA/PFCCONTROL/nodeside";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";

int sl, ET;

float ES_BP, ES_AT, ES_RH, RH, ETo, HI;
float t4, h4, p4, alti, dewpt, uvIntensity;

int battery = 100;

float radiation;
float altitudeAngle;
float solarRadiation;
float averageDailyRadiation;

const float latitude = 17.388333;  // Your latitude
const float longitude = 78.230630; // Your longitude
const float maxRadiation = 1000.0; // Max solar radiation at zenith (clear sky conditions)

// Define M_PI if it's not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int p;
int co2 = 0;

bool a;
bool shouldSaveConfig = false;

float VPDcal();
float VPD, p1, p2;

float AirTemp;
float LeafTemp;

// Function prototypes
float calculateSolarAltitude(int currentHour, int currentMinute, int dayOfYear);
float estimateSolarRadiation(float altitudeAngle);

void saveConfig();
void loadConfig();
void saveConfigCallback();
void initializeWifiManager();
void wifiInfo();

void BMEData();
void ETcalcul();

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}

void setup() {

  Serial.begin(115200);

  while (!Serial) {}  // Wait

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    // Handle the failure appropriately, possibly halt or retry
    while (true) {
      delay(1000); // Delay indefinitely if SPIFFS fails to mount
    }
  }

  // Save configuration if needed
  if (shouldSaveConfig) {
    Serial.println("Saving updated configuration...");
    saveConfig();
  }

  loadConfig();   // Load configuration settings from SPIFFS or EEPROM

  initializeWifiManager();   // Set up WiFiManager and MQTT configuration

  wifiManager.setTimeout(180);

  wifiInfo();

  Wire.begin(25, 26);

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    switch (bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
    }

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    u8x8.begin();

    client.setServer(mqttServer, mqttPort); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
  }
  else {
    Serial.println("No Wifi");

  }

}

unsigned long currentMillis = millis();
unsigned long lastMsg = 0;
unsigned long lastMsg2 = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop() {
  // Retry loop if WiFi connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to reconnect to WiFi...");

    // Attempt to reconnect to Wi-Fi
    WiFi.reconnect();  // Try to reconnect to the last connected Wi-Fi network
    Serial.println("Reconnection failed, retrying...");
    delay(5000);  // Wait 5 seconds before retrying
    return;  // Skip the rest of the loop if not connected

    // If connected
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to WiFi."); \
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();

    if (!client.connected()) {
      reconnect();
    }
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;

      timeClient.update();

      time_t epochTime = timeClient.getEpochTime();
      struct tm *ptm = gmtime(&epochTime);

      int currentHour = ptm->tm_hour;
      int currentMinute = ptm->tm_min;
      int dayOfYear = ptm->tm_yday + 1;

      printBME280Data(&Serial);
      
      BMEData();    // Ensure sensor data is read before displaying
      ETcalcul();   // Update ET calculation

      Serial.print("VPD: ");
      Serial.print(VPDcal());
      Serial.println(" kPa");

      altitudeAngle = calculateSolarAltitude(currentHour, currentMinute, dayOfYear);

      Serial.print("Altitude Angle: ");
      Serial.print(altitudeAngle);
      Serial.println("°");

      solarRadiation = estimateSolarRadiation(altitudeAngle);

      Serial.print("Solar Radiation: ");
      Serial.print(solarRadiation);
      Serial.println(" W/m²");

      averageDailyRadiation = calculateDailyAverageSolarRadiation();

      Serial.print("Avg Solar Rad: ");
      Serial.print(averageDailyRadiation);
      Serial.println(" Wh/m²"); // Assuming you want the result in Wh/m²

      delay(2000); // Adjust delay if necessary

    }
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      if (client.connected()) {
        sendDataMQTT();
      }

    }

    client.loop();
  }
  
  unsigned long rstresp1 = millis();
  if (rstresp1 - rstresp > 500000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }
}
