#include <U8g2lib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include<ArduinoJson.h>
#include <SPI.h>              
#include <LoRa.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);


#define ss 18 //GPIO 5
#define rst 14  //GPIO 14
#define dio0 26   //GPIO 2
#define MSG_BUFFER_SIZE (1024)
char msg[MSG_BUFFER_SIZE];

// Replace the next variables with your SSID/Password combination
const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.0.186";
const char* mqttUsername = "Zeptogreens";
const char* mqttpsw = "123456";

const char* ZoneNSensor = "MSNode";
const char* ZoneSoilsensor = "NPKNode";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
//char msg[50];
int value = 0;

byte MasterNode = 0xFF;
byte Node1 = 0xBB;
byte Node2 = 0xCC;
byte Node3 = 0xDD;

const int pageCount = 8;
void MS2value();
void DSTEMP();
void MSsensor();
void MSsensorBME();
void MSsensorgps();
void npknode();
void NPKGPS();
void npktemp();
void (*pages[pageCount])() = { MSsensor, MS2value, MSsensorBME, DSTEMP, MSsensorgps, npknode, npktemp, NPKGPS};
int duration [pageCount] = { 1000, 1000, 1000, 1000, 2000};
int battry = random(10, 100);
String SenderNode = "";
String outgoing;              // outgoing message


byte msgCount = 0;            // count of outgoing messages
String incoming = "";

// Tracks the time since last event fired
unsigned long previousMillis = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0;
unsigned long currentMillis = 0;
int interval = 1 ; // updated every 1 second
int Secs = 0;

int temperature;
int humidity;
int pressure;

int soilmoisturepercent;
int soilMoistureValue;
int soilMoistureValue_2;
int soilmoisturepercent_2;

int temperatureC;
int temperatureF;

int Ni;
int Ph;
int Ko;
int Battery;
 int p;

float Latitude;
float Longitude;
float Altitude;
String  lat_str , lng_str;


int MS_chipId;
int NPK_chipId;
int chipId;
bool a;

void Display();

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
      // Subscribe
      //       client.subscribe(ZoneNSensor);
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

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
  LoRa.write(MasterNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
char tempString[1024];
void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  if ( sender == 0XBB )
    SenderNode = "Node1:";
  if ( sender == 0XCC )
    SenderNode = "Node2:";
  if ( sender == 0XDD )
    SenderNode = "Node3:";
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length


  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }



  if ( sender == 0XBB )
  {
    Serial.println("NODE 1");

    
    String C = getValue(incoming, ',', 0);

    String v = getValue(incoming, ',', 1); // Soil Moisture Value
    String p = getValue(incoming, ',', 2); // Soil Moisture percentage
    String v_2 = getValue(incoming, ',', 3); // Soil Moisture Value2
    String p_2 = getValue(incoming, ',', 4); // Soil Moisture percent2

    String t = getValue(incoming, ',', 5); // Temperature
    String pr = getValue(incoming, ',', 6); //pressure
    String h = getValue(incoming, ',', 7); // Humidity

    String t_c = getValue(incoming, ',', 8);
    String la = getValue(incoming, ',', 9);
    String lo = getValue(incoming, ',', 10);
    String al = getValue(incoming, ',', 11);

    MS_chipId = C.toInt();

    soilMoistureValue = v.toInt();
    soilmoisturepercent = p.toInt();
    soilMoistureValue_2 = v_2.toInt();
    soilmoisturepercent_2 = p_2.toInt();

    temperature = t.toInt();
    pressure = pr.toInt();
    humidity = h.toInt();

    temperatureC = t_c.toInt();

    incoming = "";

    Serial.println("");
    Serial.print("chipId =  ");
    Serial.print(MS_chipId);
    Serial.println("");

    Serial.print("soilMoistureValue = ");
    Serial.print(soilMoistureValue);
    Serial.println("");

    Serial.print("soilMoistureValue = ");
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    Serial.println("");

    Serial.print("soilMoistureValue_2= ");
    Serial.print(soilMoistureValue_2);
    Serial.println("");

    Serial.print("soilMoistureValue_2= ");
    Serial.print(soilmoisturepercent_2);
    Serial.println("%");
    Serial.println("");

    Serial.print("temperatureC= ");
    Serial.print(temperatureC);
    Serial.println("ºC");
//    Serial.print("temperatureF= ");
//    Serial.print(temperatureF);
//    Serial.println("ºF");

    Serial.print("temperature = ");
    Serial.print(temperature);
    Serial.println(" C ");

    Serial.print("humidity = ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("pressure = ");
    Serial.print(pressure);
    Serial.println(" bar ");
    Serial.println("");

    Serial.print("Latitude = ");
    Serial.println(Latitude);

    Serial.print("Longitude = ");
    Serial.println(Longitude);


    DynamicJsonDocument doc(500);
    doc["id"] = MS_chipId;
    doc["SMV"] = soilMoistureValue;
    doc["SMP"] = soilmoisturepercent;
    doc["SMV_2"] = soilMoistureValue_2;
    doc["SMP_2"] = soilmoisturepercent_2;
    doc["TEMPC"] = temperatureC;
    doc["Temp"] = temperature;
    doc["Humi"] = humidity;
    doc["PRE"] = pressure;
    doc["Lat"] = Latitude;
    doc["Lon"] = Longitude;
    doc["Alt"] = Altitude;

    serializeJsonPretty(doc, Serial);
    Serial.println();
    DynamicJsonDocument doc2(1024);
    doc2["MSNODE"] = doc;
    serializeJson(doc2, tempString);
    client.publish(ZoneNSensor, tempString);
    memset(tempString, 0, 1024);


  }


  if ( sender == 0XCC ) {

    Serial.println("NODE 2");

    String C = getValue(incoming, ',', 0);

    String N = getValue(incoming, ',', 1);
    String P = getValue(incoming, ',', 2);
    String K = getValue(incoming, ',', 3);

    String t = getValue(incoming, ',', 4); // Temperature
    String pr = getValue(incoming, ',', 5); //pressure
    String h = getValue(incoming, ',', 6); // Humidity

    String la = getValue(incoming, ',', 7);
    String lo = getValue(incoming, ',', 8);


    String B = getValue(incoming, ',', 9);

    NPK_chipId = C.toInt();

    Ni = N.toInt();
    Ph = P.toInt();
    Ko = K.toInt();

    temperature = t.toInt();
    pressure = pr.toInt();
    humidity = h.toInt();

    Battery  = B.toInt();
    lat_str  = la.toInt();
    lng_str  = lo.toInt();
    incoming = "";

    Serial.println("");
    Serial.print("chipId =  ");
    Serial.print(NPK_chipId);
    Serial.println("");

    Serial.print("Nitrogen: ");
    Serial.print(Ni);
    Serial.println(" mg/kg");

    Serial.print("Phosphorous: ");
    Serial.print(Ph);
    Serial.println(" mg/kg");

    Serial.print("Potassium: ");
    Serial.print(Ko);
    Serial.println(" mg/kg");
    Serial.println("");


    Serial.print("temperature = ");
    Serial.print(temperature);
    Serial.println(" C ");

    Serial.print("humidity = ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("pressure = ");
    Serial.print(pressure);
    Serial.println(" bar ");
    Serial.println("");

    Serial.print("Latitude = ");
    Serial.println(Latitude);

    Serial.print("Longitude = ");
    Serial.println(Longitude);


    Serial.print("Battery: ");
    Serial.print(Battery);
    Serial.println("%");

   
    DynamicJsonDocument doc1(1024);
    doc1["id"] = NPK_chipId;
    doc1["NI"] = Ni;
    doc1["PH"] = Ph;
    doc1["POT"] = Ko;
    doc1["TEMP"] = temperature;
    doc1["PRE"] = pressure;
    doc1["HUMI"] = humidity;   
    doc1["LAT"] = Latitude;
    doc1["LON"] = Longitude;

    doc1["BAT"] = Battery;
    serializeJsonPretty(doc1, Serial);
    Serial.println();

    DynamicJsonDocument doc3(1024);
    doc3["NPKNODE"] = doc1;
    serializeJson(doc3, tempString);
    memset(tempString, 0, 1024);

  }

  if ( sender == 0XDD )
  {
    Serial.println("NODE 3");

    
    String CC = getValue(incoming, ',', 0);

    String vv = getValue(incoming, ',', 1); // Soil Moisture Value
    String pp = getValue(incoming, ',', 2); // Soil Moisture percentage
    String v_22 = getValue(incoming, ',', 3); // Soil Moisture Value2
    String p_22 = getValue(incoming, ',', 4); // Soil Moisture percent2

    String tt = getValue(incoming, ',', 5); // Temperature
    String prr = getValue(incoming, ',', 6); //pressure
    String hh = getValue(incoming, ',', 7); // Humidity

    String t_cc = getValue(incoming, ',', 8);
    String laa = getValue(incoming, ',', 9);
    String loo = getValue(incoming, ',', 10);
    String all = getValue(incoming, ',', 11);

    MS_chipId = CC.toInt();

    soilMoistureValue = vv.toInt();
    soilmoisturepercent = pp.toInt();
    soilMoistureValue_2 = v_22.toInt();
    soilmoisturepercent_2 = p_22.toInt();

    temperature = tt.toInt();
    pressure = prr.toInt();
    humidity = hh.toInt();

    temperatureC = t_cc.toInt();

    incoming = "";

    Serial.println("");
    Serial.print("chipId =  ");
    Serial.print(MS_chipId);
    Serial.println("");

    Serial.print("soilMoistureValue = ");
    Serial.print(soilMoistureValue);
    Serial.println("");

    Serial.print("soilMoistureValue = ");
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    Serial.println("");

    Serial.print("soilMoistureValue_2= ");
    Serial.print(soilMoistureValue_2);
    Serial.println("");

    Serial.print("soilMoistureValue_2= ");
    Serial.print(soilmoisturepercent_2);
    Serial.println("%");
    Serial.println("");

    Serial.print("temperatureC= ");
    Serial.print(temperatureC);
    Serial.println("ºC");
//    Serial.print("temperatureF= ");
//    Serial.print(temperatureF);
//    Serial.println("ºF");

    Serial.print("temperature = ");
    Serial.print(temperature);
    Serial.println(" C ");

    Serial.print("humidity = ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("pressure = ");
    Serial.print(pressure);
    Serial.println(" bar ");
    Serial.println("");

    Serial.print("Latitude = ");
    Serial.println(Latitude);

    Serial.print("Longitude = ");
    Serial.println(Longitude);


    DynamicJsonDocument doc(500);
    doc["id"] = MS_chipId;
    doc["SMV"] = soilMoistureValue;
    doc["SMP"] = soilmoisturepercent;
    doc["SMV_2"] = soilMoistureValue_2;
    doc["SMP_2"] = soilmoisturepercent_2;
    doc["TEMPC"] = temperatureC;
    doc["Temp"] = temperature;
    doc["Humi"] = humidity;
    doc["PRE"] = pressure;
    doc["Lat"] = Latitude;
    doc["Lon"] = Longitude;
    doc["Alt"] = Altitude;

    serializeJsonPretty(doc, Serial);
    Serial.println();
    DynamicJsonDocument doc2(1024);
    doc2["MSNODE"] = doc;
    serializeJson(doc2, tempString);
    client.publish(ZoneNSensor, tempString);
    memset(tempString, 0, 1024);


  }
  Display();
}
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void Display() {
  u8g2.firstPage();

  do {
    (*pages[p])();
  } while ( u8g2.nextPage() );
  delay(duration[p]);
  p = p + 1;
  if (p == pageCount)
    p = 0;

}
void MSsensor() {

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(MS_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setCursor(0, 30);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("SMV: "));
  u8g2.print(soilMoistureValue);
  u8g2.setCursor(0, 45);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("SMP: "));
  u8g2.setCursor(30, 45);
  u8g2.print(soilmoisturepercent);
  u8g2.print(" %");

}
void MS2value() {

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(MS_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setCursor(0, 30);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("SMV_2: "));
  u8g2.print(soilMoistureValue_2);


  u8g2.setCursor(0, 45);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("SMP_2: "));
  u8g2.setCursor(40, 45);
  u8g2.print(soilmoisturepercent_2);
  u8g2.print(" %");


}

void DSTEMP() {

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(MS_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setCursor(0, 30);
  u8g2.print(F("TEMPC: "));
  u8g2.setCursor(40, 30);
  u8g2.print(temperatureC);
  u8g2.print(" ºC");

//  u8g2.setCursor(0, 45);
//  u8g2.print(F("TEMPF: "));
//  u8g2.setCursor(40, 45);
//  u8g2.print(temperatureF);
//  u8g2.print(" ºF");

}

void MSsensorBME() {

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(MS_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }



  u8g2.setCursor(0, 30);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("TEMP:  "));
  u8g2.setCursor(30, 30);
  u8g2.print(temperature);
  u8g2.print(" *C");
  u8g2.setCursor(0, 45);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("PRE:  "));
  u8g2.setCursor(30, 45);
  u8g2.print(pressure);
  u8g2.print(" hPa");
  u8g2.setCursor(0, 60);
  u8g2.print(F("HUMI: "));
  u8g2.setCursor(30, 60);
  u8g2.print(humidity);
  u8g2.print(" %");

}
void MSsensorgps() {

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(MS_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }
  u8g2.setCursor(0, 30);
  u8g2.print(F("LAT: "));
  u8g2.print(Latitude);
  u8g2.setCursor(0, 45);
  u8g2.print(F("LON: "));
  u8g2.print(Longitude);
  u8g2.setCursor(0, 60);
  u8g2.print(F("ALT: "));
  u8g2.print(Altitude);

}


void npknode() {


  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(NPK_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setCursor(0, 30);
  u8g2.print(F("NIT: "));
  u8g2.setCursor(30, 30);
  u8g2.print(Ni);
  u8g2.println(" mg/kg");
  u8g2.setCursor(0, 45);
  u8g2.print(F("PHR: "));
  u8g2.setCursor(30, 45);
  u8g2.print(Ph);
  u8g2.println(" mg/kg");
  u8g2.setCursor(0, 60);
  u8g2.print(F("POT: "));
  u8g2.setCursor(30, 60);
  u8g2.print(Ko);
  u8g2.println(" mg/kg");
}

void NPKGPS() {


  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(NPK_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }
  u8g2.setCursor(0, 30);
  u8g2.print(F("LAT: "));
  u8g2.print(Latitude);
  u8g2.setCursor(0, 45);
  u8g2.print(F("LON: "));
  u8g2.print(Longitude);
  u8g2.setCursor(0, 60);
  u8g2.print(F("ALT: "));
  u8g2.print(Altitude);

}

void npktemp(){

  u8g2.setFont(u8g2_font_siji_t_6x10);

  u8g2.setCursor(0, 15);
  u8g2.print(NPK_chipId);
  if (a > 0)
  {
    u8g2.setCursor(66, 14);

    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();

  u8g2.setCursor(86, 14);

  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    //  u8g2.drawFrame(98,0,20,14);
    //  u8g2.drawVLine(103,0,12);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);

    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }
  u8g2.setCursor(0, 30);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("TEMP:  "));
  u8g2.setCursor(30, 30);
  u8g2.print(temperature);
  u8g2.print(" *C");
  u8g2.setCursor(0, 45);
  u8g2.setFont(u8g2_font_siji_t_6x10);
  u8g2.print(F("PRE:  "));
  u8g2.setCursor(30, 45);
  u8g2.print(pressure);
  u8g2.print(" hPa");
  u8g2.setCursor(0, 60);
  u8g2.print(F("HUMI: "));
  u8g2.setCursor(30, 60);
  u8g2.print(humidity);
  u8g2.print(" %");


  
}


void setup() {

  Serial.begin(115200);
  //    u8g2.setFont(u8g2_font_cu12_tr);
  u8g2.begin();
  while (!Serial);
  Serial.println("LoRa Master Node");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  StaticJsonDocument<500> doc;
 

}
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  currentMillis = millis();
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval) {
    Secs = Secs + 1;
    // Serial.println(Secs);
    if ( Secs >= 62 )
    {
      Secs = 0;
    }
    if ( (Secs >= 1) && (Secs <= 20) )
    {

      String message = "10";
      sendMessage(message, MasterNode, Node1);
      Serial.println(message);

    }

    if ( (Secs >= 21 ) && (Secs <= 60))
    {

      String message = "20";
      sendMessage(message, MasterNode, Node2);
      Serial.println(message);

    }

    if ( (Secs >= 61 ) && (Secs <= 80))
    {

      String message = "30";
      sendMessage(message, MasterNode, Node3);
      Serial.println(message);

    }

    previoussecs = currentsecs;

  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());


}
