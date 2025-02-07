#include<WiFi.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <FS.h>
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
#include <U8g2lib.h>
#include<EEPROM.h>

#define UVOUT 32
#define  REF_3V3 33

// Assumed environmental values:
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
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

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

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

int battry;
float ES_BP, ES_AT, ES_RH, RH, ETo, HI;

CO2Sensor co2Sensor(15, 0.99, 100);
int co2;//=co2Sensor.read();

int sl, ET;
float VPD, p1, p2;
int uvLevel;
int refLevel;
int averageAnalogRead(int);

float VPDcal();

// Function prototypes
void saveConfig();
void loadConfig();
void saveConfigCallback();
void initializeWifiManager();
void wifiInfo();

void BMEData();
void OLED();
void page1();
void page2();
void page3();
void page4();
void ETcalcul();
void UVA();

char mqttServer[40] = "164.52.223.248";// "192.168.1.107";//"192.168.1.183";//"192.168.1.102";
char port_str[40] = "1883"; // string representation of the port
uint16_t mqttPort = atoi(port_str); // convert port string to integer
char mqttUserName[40] = "axalyn"; //"Zeptogreens";//"zepto";//"Zeptogreens";
char mqttPwd[40] = "cenaura@2024"; // "12345";
char clientID[40] = "username0001"; // client id username+0001
String timestamp;

bool a;
bool shouldSaveConfig = false;

const char* Rectopic = "esp/test"; //publish topic
const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT = "Chinnamangalaram/GH1/ES/ZoneA/ClimateData"; //publish topic
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
const char* sendalert = "test/package/PFC";

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

// Create custom TwoWire instance for OLED
TwoWire I2C_bus1 = TwoWire(0);

//////////////////////////////////////////////////////////////////
void setup()
{
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

  // Initialize default I2C bus for BME280
  Wire.begin(25, 26); // SDA, SCL

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

    Serial.print("Assumed outdoor temperature: "); Serial.print(outdoorTemp);
    Serial.print("°C\nAssumed reduced sea level Pressure: "); Serial.print(referencePressure);
    Serial.print("hPa\nAssumed barometer altitude: "); Serial.print(barometerAltitude);
    Serial.println("m\n***************************************");

    // Initialize the custom I2C bus for OLED
    I2C_bus1.begin(21, 22); // SDA, SCL for OLED

    u8g2.begin();

    timeClient.begin();
    timeClient.setTimeOffset(19800);

    client.setServer(mqttServer, 1883); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
  }
  else {

    Serial.println("No Wifi");

  }

}

//////////////////////////////////////////////////////////////////
void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();

    if (!client.connected()) {
      reconnect();
    }
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;
      OLED();
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
