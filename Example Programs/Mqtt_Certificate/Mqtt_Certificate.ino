
//#include <ESP8266WiFi.h>
#include <Arduino.h>

#include <FS.h>
#include <SPIFFS.h>
#include <mqtt_client.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <U8g2lib.h>
#include<EEPROM.h>
#include<Wire.h>
#include<WiFi.h>
#include "MapFloat.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#define MQ2pin 34
#include "certs.h"


// Define the size of EEPROM for each certificate
#define CERT_SIZE 1024

// EEPROM address for storing the certificate
#define CERT_ADDR 0

String cert;

// Update the following with your actual paths and filenames
const char* certificate = \
                          "-----BEGIN cert_filename-----\n" \
                          "MIIB4zCCAYmgAwIBAgIUDvfsevHpF7ObReAAmGXXHHsAXD0wCgYIKoZIzj0EAwIw\n" \
                          "UjELMAkGA1UEBhMCQ0ExDzANBgNVBAcMBk90dGF3YTEUMBIGA1UECgwLRWNsaXBz\n" \
                          "ZSBJb1QxDTALBgNVBAsMBEhvbm8xDTALBgNVBAMMBHJvb3QwHhcNMjMwNjI0MDkw\n" \
                          "NjAwWhcNMjQwNjIzMDkwNjAwWjBQMQswCQYDVQQGEwJDQTEPMA0GA1UEBwwGT3R0\n" \
                          "YXdhMRQwEgYDVQQKDAtFY2xpcHNlIElvVDENMAsGA1UECwwESG9ubzELMAkGA1UE\n" \
                          "AwwCY2EwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQphoV7ZJZg71VRld66PKiW\n" \
                          "MHYatfL71/xughhSsHfe4GYNA6h9pz6Qb0e9h8Kj6tLTFNThvZ0E502oy6e/A5tq\n" \
                          "oz8wPTAdBgNVHQ4EFgQUbAyRRCU3YCp7Zq1TbGeW/fagqHEwDwYDVR0TAQH/BAUw\n" \
                          "AwEB/zALBgNVHQ8EBAMCAQYwCgYIKoZIzj0EAwIDSAAwRQIhAJaqUu4QSLK3pwCd\n" \
                          "NLmrKHol6dXqaKSoFUKYVdASsFsRAiBvX8XHIdRk/AekcDX4AejDg1W5EbvP5r1t\n" \
                          "KiEsV1l1Tw==\n" \
                          "-----END cert_filename-----\n" \
                          "-----BEGIN cert_filename-----\n" \
                          "MIICeTCCAh+gAwIBAgIUfwZ93GxLHl4pKx+jUiyh/bXYkSkwCgYIKoZIzj0EAwIw\n" \
                          "UjELMAkGA1UEBhMCQ0ExDzANBgNVBAcMBk90dGF3YTEUMBIGA1UECgwLRWNsaXBz\n" \
                          "ZSBJb1QxDTALBgNVBAsMBEhvbm8xDTALBgNVBAMMBHJvb3QwHhcNMjMwNjI0MDkw\n" \
                          "NjAwWhcNMjQwNjIzMDkwNjAwWjBSMQswCQYDVQQGEwJDQTEPMA0GA1UEBwwGT3R0\n" \
                          "YXdhMRQwEgYDVQQKDAtFY2xpcHNlIElvVDENMAsGA1UECwwESG9ubzENMAsGA1UE\n" \
                          "AwwEcm9vdDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABAgp8l1Ao38qj7kizHpb\n" \
                          "5hBdid6mLc3JY67K11hocPN5z/V2SZ01Az+wo2HYtRem58KbAQNOBTvI2vDx0EUu\n" \
                          "d22jgdIwgc8wHQYDVR0OBBYEFD7oR5QKFx4rb18HZ+62kQ1x66cUMIGPBgNVHSME\n" \
                          "gYcwgYSAFD7oR5QKFx4rb18HZ+62kQ1x66cUoVakVDBSMQswCQYDVQQGEwJDQTEP\n" \
                          "MA0GA1UEBwwGT3R0YXdhMRQwEgYDVQQKDAtFY2xpcHNlIElvVDENMAsGA1UECwwE\n" \
                          "SG9ubzENMAsGA1UEAwwEcm9vdIIUfwZ93GxLHl4pKx+jUiyh/bXYkSkwDwYDVR0T\n" \
                          "AQH/BAUwAwEB/zALBgNVHQ8EBAMCAQYwCgYIKoZIzj0EAwIDSAAwRQIgEXB+MLWl\n" \
                          "LiuwXQ7w6PmcDhf646XAl3Nfo8YMSziVPIUCIQDDUfiiAnrniyFjlMKHDKk1GI/J\n" \
                          "UT6Pq7oL2KbKP37rNA==\n" \
                          "-----END cert_filename-----\n" \
                          "-----BEGIN cert_filename-----\n" \
                          "MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
                          "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
                          "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \
                          "WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
                          "RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
                          "AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
                          "R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
                          "sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
                          "NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
                          "Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
                          "/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
                          "AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
                          "Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
                          "FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
                          "AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
                          "Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
                          "gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
                          "PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \
                          "ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
                          "CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
                          "lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
                          "avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
                          "yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
                          "yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
                          "hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
                          "HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
                          "MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
                          "nLRbwHOoq7hHwg==\n" \
                          "-----END cert_filename-----\n" \
                          "-----BEGIN cert_filename-----\n" \
                          "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
                          "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
                          "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
                          "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
                          "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
                          "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
                          "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
                          "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
                          "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
                          "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
                          "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
                          "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
                          "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
                          "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
                          "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
                          "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
                          "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
                          "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
                          "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
                          "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
                          "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
                          "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
                          "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
                          "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
                          "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
                          "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
                          "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
                          "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
                          "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
                          "-----END cert_filename-----\n";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("mqttServer", "mqtt server", "", 40);

uint32_t chipid;
const char* mqttServer = "pag.tayrix.com"; //"192.168.1.38";
const int mqttPort = 30883;  //1883;
const char* mqttUserName = "demo-device@org.eclipse.packages.c2e"; //"tayrix"; //"Zeptogreens";
const char* mqttPwd = "demo-secret"; //"zepto@2023"; // "12345";
const char* clientID = "username0001"; // client id username+0001
String timestamp;

WiFiClientSecure espClient = WiFiClientSecure();

PubSubClient client(espClient);

//parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 5000;
unsigned long mtime = 0;

// TEST OPTION FLAGS
bool TEST_CP         = false; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 90; // test cp timeout

bool TEST_NET        = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND   = true; // enable on demand
int  ONDDEMANDPIN    = 0; // gpio for button
bool WMISBLOCKING    = false; // use blocking or non blocking mode

String msgStr = "";
const char* topic1 = "esp/test"; //publish topic
const char* Rectopic = "esp/test"; //publish topic

const char* sendtopic_Mesh = "account/Zepto/clientid/appid/climatedata/ES";
const char* sendtopic_CT =  "Saifarm/GH2/PFC/ZoneA/PFCCONTROL/nodeside";
const char* sendtopic_Node = "Chinnamangalaram/GH1/ZoneA/Node";
#define batteryvolt 34

const int pagecount = 4;
int p;
int chipId;
int sensorvalue;
float voltage, outputvoltage;
int battry;
bool a;

void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings
void batterypercent();
int averageAnalogRead(int);

void saveWifiCallback() {
  Serial.println("[CALLBACK] saveCallback fired");
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");

}

void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
}

float tempdec;

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  float refanalog = analogRead(pinToRead);
  float DCvalue = mapFloat( refanalog, 0, 1023, 0, 5);

  runningValue /= numberOfReadings;
  return (runningValue);
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
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


}

void sendDataMQTT() {
  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  uint32_t ES_did = chipId;

  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["did"] = ES_did;
  TXCC["ts"] = formattedTime;
  JsonObject TXCC_ES_rtd = TXCC.createNestedObject("ES_rtd");

  int E_AT = random(100, 200);
  TXCC_ES_rtd["AT_v"] = E_AT;
  //  TXCC_ES_rtd["Name"] = "Hemanth";

  char buffer[1024];

  serializeJson(doc, buffer);
  client.publish(sendtopic_Mesh, buffer);
  client.publish(sendtopic_CT, buffer);
  client.publish(sendtopic_Node, buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

void wifiInfo() {
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)wm.getWiFiSSID());
  Serial.println("PASS: " + (String)wm.getWiFiPass());
}

void reconnect() {
  while (!client.connected()) {
    // ESP.restart();
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");

      a = true;

    }
    else {
      a = false;

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay(5000);  // wait 5sec and retry
      ESP.restart();

    }

  }

}

void writeCertificateToEEPROM() {
  for (int i = 0; i < CERT_SIZE; ++i) {
    char c = certificate[i];
    EEPROM.write(CERT_ADDR + i, c);
    if (c == '\0') {
      break;  // Reached the end of the cert_filename string
    }
  }
  EEPROM.commit();  // Commit the changes to EEPROM
}

String readCertificateFromEEPROM() {
  for (int i = 0; i < CERT_SIZE; ++i) {
    char c = EEPROM.read(CERT_ADDR + i);
    if (c == '\0') {
      break;  // Reached the end of the stored string
    }
    certificate += c;
  }
  return certificate;
}

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}

String storedCert = readCertificateFromEEPROM();

void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();
  pinMode(34, INPUT);

  WiFiManager wifiManager;

  wifiManager.setTimeout(180);

  if (!wifiManager.autoConnect("Tayrix Sensor Wi-Fi")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);

    ESP.restart();
    delay(5000);
  }

  Serial.println("connected...yeey :)");

  // Function to write the certificate to EEPROM
  if (storedCert) {
    // If the certificate is valid, print it
    Serial.print("Stored Certificate: ");
    Serial.println(String(storedCert));

  } else {
    // If the certificate is not valid, print an error message
    Serial.println("No valid certificate found in EEPROM");
  }

  wifiInfo();

  if (WiFi.status() == WL_CONNECTED) {

    timeClient.begin();
    timeClient.setTimeOffset(19800);
    espClient.setCACert(storedCert.c_str());

    client.setServer(mqttServer, mqttPort); //setting MQTT server
    client.setCallback(callback); //defining function which will be called when message is recieved.
    client.subscribe(Rectopic);//
  }
  else {
    Serial.println("No Wifi");

  }

}

unsigned long lastMsg = 0;
unsigned long lastMsg01 = 0;
unsigned long rstresp = 0;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    wm.process();

    if (!client.connected()) {
      reconnect();
    }
    unsigned long nowon = millis();
    if (nowon - lastMsg01 > 1000) {
      lastMsg01 = nowon;
    }
    unsigned long now1 = millis();

    // Read cert_filename before checking the client connection
    String cert_filename = readCertificateFromEEPROM();

    if (now1 - lastMsg > 5000) {
      lastMsg = now1;

      if (client.connected()) {
        // Use the cert variable as needed in your MQTT operations
        Serial.println(cert_filename);
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
