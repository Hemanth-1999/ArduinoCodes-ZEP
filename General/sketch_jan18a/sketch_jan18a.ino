#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include<EEPROM.h>
#include<Wire.h>
#include<WiFi.h>

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);

uint32_t chipid;
//const char* ssid = "Zeptogreens";//Kumar_Xstream
//const char* password =  "Zeptogreens20";//703664703664
const char* mqttServer = "192.168.1.180"; //"192.168.1.166";// "192.168.1.107";//"192.168.1.183";//"192.168.1.102";
const int mqttPort = 1883;
const char* mqttUserName = "Zeptogreens"; //"Zeptogreens";//"zepto";//"Zeptogreens";
const char* mqttPwd = "123456"; // "12345";
const char* clientID = "username0001"; // client id username+0001
String timestamp;

WiFiClient espClient;
PubSubClient client(espClient);

String msgStr = "";
const char* topic1 = "esp/test"; //publish topic
const char* Rectopic = "esp/test"; //publish topic

const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata"; //publish topic
//const char* sendtopic_Mesh ="account-name/PD1/W1/A1/Climate";
const char* sendtopic_RGB = "RGB";
const char* sendtopic_IR = "IR";
const char* sendtopic_UV = "UV";
const char* sendalert = "test/package/PFC";

void sendDataMQTT()
{
  // create an object
JsonObject object = doc.to<JsonObject>();
object["hello"] = "world";

// serialize the object and send the result to Serial
serializeJson(doc, Serial);
}
