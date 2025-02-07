#include<WiFi.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <SPIFFS.h>
#include <FS.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include"MapFloat.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "CO2Sensor.h"
#include <AsyncTCP.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#include<EEPROM.h>
#include <cmath> // Include cmath for mathematical functions

// Assumed environmental values:
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
float temp(NAN), hum(NAN), pres(NAN);

BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_hPa);
EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);
WiFiManager wifiManager;
WiFiClient espClient;
//WiFiClientSecure espClient; // This clientsecure is for ditto
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

/// Total environmental calculation declerations
float t4, h4, p4, alti, dewpt, uvIntensity;
float Tme, dn1, dn, num1, enum1, enum2, expon, pdenom, powercal, top1, top2, num2, num3, denom2, powercalc2, expnum1, expnum2, expcal;
float expnum3, expnum4, expcal2, ETcal, Es, RHcal, RHcal2, num5, num6, numcalc, fdenom1, fdenom2, fdenom3, fdenom4, fnum1, fnum2, fnum3, fnum4, fnum5, fnum6, fnum7, fnum8, fnum9, fnum10;
float Tmean;
float U2, P, Ea, delta, gama, ETmax, ETmin;
float Tmax, Tmin;
float h;//h=8.2;//height of the measurement above the ground surface
float T;//t=7.7;//air temp
float z = 840; //3.6;//elevation above sealevel//
//The elevation of the land surface varies from the low point of −418 m at the Dead Sea, to a maximum altitude of 8,848 m at the top of Mount Everest.
//The mean Height of land above sea level is 840 m.”

float Un = 2.4; ////measured wind speed z m above the ground surface
float RHmax, RHmin;
float Rn;//UV//=5.9;//net radiation flux
float G;//=HI//4.5;//sensible heat flux into the soil
float AirTemp;
float LeafTemp;
float voltage, outputvoltage;

int battry = 100;

uint8_t x;
uint8_t y;

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

float ES_BP, ES_AT, ES_RH, RH, ETo, HI;

CO2Sensor co2Sensor(15, 0.99, 100);
const char* co2 = "0.0";

int sl, ET;
float VPD, p1, p2;
int uvLevel;
int refLevel;

float VPDcal();

// Function prototypes
float calculateSolarAltitude(int currentHour, int currentMinute, int dayOfYear);
float estimateSolarRadiation(float altitudeAngle);

void saveConfig();
void loadConfig();
void saveConfigCallback();
void initializeWifiManager();
void wifiInfo();

void scanI2CDevices();
void BMEData();
void OLED();
void page1();
void page2();
void page3();
void page4();
void ETcalcul();
void OLEDJson();
void Ra02LoRa();
void LoRaData1();
void LoRaData2();
void LoRafailed();

char mqttServer[40] = "164.52.223.248"; //"pag.axalyn.com"; //
char port_str[40] = "1883"; //"30883"; // string representation of the port
uint16_t mqttPort = atoi(port_str); // convert port string to integer
char mqttUserName[60] = "axalyn"; //"DEVICE_USERNAME@6c458335-0fdc-40d9-8ab1-7606dabdb887"; //"Zeptogreens";//"zepto";//"Zeptogreens";
char mqttPwd[40] = "cenaura@2024"; //"DEVICE_PASSWORD"; // "12345";
char clientID[40] = "username0001";

String timestamp;
String message1;
String message2;
String message3;
String message4;
String message5;
String message6;
String message7;
String message8;
String message9;
String message10;
String receivedRssi;

bool a;
bool shouldSaveConfig = false;

const char* Rectopic = "esp/test"; //publish topic
const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT = "Chinnamangalaram/GH1/ES/ZoneA/ClimateData"; //publish topic
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
const char* sendalert = "test/package/PFC";

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;

// Global variables for I2C scanning
unsigned long lastI2CScan = 0;
unsigned long I2CScanInterval = 30000; // Interval for I2C scan (30 seconds)

// Initialize the display (default I2C bus)
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

// Initialize U8G2 with your specific display parameters
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

// OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

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

  Wire.begin();

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

    u8x8.begin();

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    client.setServer(mqttServer, mqttPort); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
  }  else {
    Serial.println("No Wifi");
  }
}

// Define variables for OLED timing
unsigned long oledDisplayStart = 0;
bool isDisplayingOLED = false;

void loop()
{
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

    if (!isDisplayingOLED) {

      isDisplayingOLED = true;
      oledDisplayStart = nowon;  // Record the start time of OLED display

      OLED();

      timeClient.update();

      time_t epochTime = timeClient.getEpochTime();
      struct tm *ptm = gmtime(&epochTime);

      int currentHour = ptm->tm_hour;
      int currentMinute = ptm->tm_min;
      int dayOfYear = ptm->tm_yday + 1;

      Serial.print("VPD: ");
      Serial.print(VPDcal());
      Serial.println(" kPa");

      altitudeAngle = calculateSolarAltitude(currentHour, currentMinute, dayOfYear);

      Serial.print("Altitude Angle: ");
      Serial.print(altitudeAngle);
      Serial.println("°");

      Serial.print("Solar Radiation: ");
      Serial.print(solarRadiation);
      Serial.println(" W/m²");

      averageDailyRadiation = calculateDailyAverageSolarRadiation();

      Serial.print("Avg Solar Rad: ");
      Serial.print(averageDailyRadiation);
      Serial.println(" Wh/m²"); // Assuming you want the result in Wh/m²
      Serial.println();

      delay(2000); // Adjust delay if necessary

    }

    // Check if 5 seconds have passed for OLED display
    if (nowon - oledDisplayStart >= 5000) {
      isDisplayingOLED = false;  // Stop displaying OLED after 5 seconds
    }
    
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      if (client.connected()) {
        sendDataToMqtt();
      }
    }
    client.loop();
  }

  // Perform I2C scan at intervals
  if (millis() - lastI2CScan > I2CScanInterval) {
    lastI2CScan = millis();
    scanI2CDevices();
  }

  unsigned long rstresp = 0;
  unsigned long rstresp1 = millis();

  if (rstresp1 - rstresp > 500000) {
    Serial.println("....");
    rstresp = rstresp1;
    ESP.restart();
  }
}
