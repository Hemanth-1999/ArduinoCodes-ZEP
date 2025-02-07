// IoT Ladder Editor (0.0.2)
//
// Copyright (C) 2021 Leonardo Fernandes
//
// https://github.com/leofds/iot-ladder-editor
//
// Project: pjct1

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
//#include <Nextion.h> // Added Nextion library
#include <HardwareSerial.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define PIN_Q01 2
#define PIN_Q02 4
#define PIN_Q03 12
#define PIN_Q04 13
#define PIN_Q05 33
#define PIN_Q06 25

#define PIN_I01 14
#define PIN_I02 16
#define PIN_I03 17
#define PIN_I04 18
#define PIN_I05 19
#define PIN_I06 21
#define PIN_I07 22
#define PIN_I08 23

unsigned long SCpreviousMillis = 0;

const char* ssid = "Zeptogreens";
const char* password = "Zeptogreens@2023";

const char* broker_address = "192.168.0.189";
const uint16_t broker_port = 1883;
const char* device_id = "lRnGVKdgfNBhreVsJoqj";

const char* pub_topic = "plc/testcode/";
const char* sub_topic = "plc/testcode/";

const char* mqtt_username = "Zeptogreens";
const char* mqtt_password = "123456";


QueueHandle_t pubqueue;

typedef struct {
  char *data;
} PubMsg;

enum CONN_ST {
  CONN_ST_START,
  CONN_ST_WIFI_CONNECTING,
  CONN_ST_BROKER_CONNECTING,
  CONN_ST_BROKER_CONNECTED,
};

CONN_ST conn_st = CONN_ST_START;

void msgReceived(char* topic, byte* payload, unsigned int len);
void sendMsg(char *msg);

WiFiClient wiFiClient;

PubSubClient pubSubClient(wiFiClient); // Changed to use WiFiClient instead of WiFiClientSecure

// Timer struct and time variable
typedef struct {
  int32_t PRE;
  int32_t AC;
  int32_t B;
  int32_t DN;
  int32_t EN;
  uint64_t TT;
} LD_TIMER;

union {
  uint32_t p[2];
  uint64_t v;
} LD_TIME;

uint64_t telemetryTime;

uint64_t getTime() {
  return LD_TIME.v;
}

uint8_t LD_I1 = 0;
uint8_t LD_I4 = 0;
uint8_t LD_I2 = 0;
uint8_t LD_I3 = 0;
uint8_t LD_Q1 = 0;
uint8_t LD_I5 = 0;
uint8_t LD_I6 = 0;
uint8_t LD_Q2 = 0;
uint8_t LD_Q3 = 0;
uint8_t LD_I7 = 0;
uint8_t LD_Q4 = 0;
uint8_t LD_Q5 = 0;
uint8_t LD_I8 = 0;
uint8_t LD_Q6 = 0;

LD_TIMER LD_T1;
LD_TIMER LD_T2;
LD_TIMER LD_T3;
LD_TIMER LD_T4;

void TaskCom(void *pvParameters) {
  PubMsg msg;
  int status;

  for (;;) {
    if (xQueueReceive(pubqueue, &msg, (TickType_t)0) == pdPASS) {
      status = pubSubClient.publish(pub_topic, msg.data);
      if (status == 1) {
        Serial.println("Publish sent.");
      } else {
        Serial.println("Publish failed.");
      }
      free(msg.data);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void publishMessage(const char* msg) {
  size_t msgLen = strlen(msg);
  PubMsg pubMsg;
  pubMsg.data = (char*)malloc(msgLen + 1);
  strncpy(pubMsg.data, msg, msgLen);
  pubMsg.data[msgLen] = '\0';
  xQueueSend(pubqueue, &pubMsg, portMAX_DELAY);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Nextion
  rtc.begin();
  rtc.adjust(DateTime(2023, 2, 2, 4, 58, 0));

  pinMode(PIN_Q01, OUTPUT);
  pinMode(PIN_Q02, OUTPUT);
  pinMode(PIN_Q03, OUTPUT);
  pinMode(PIN_Q04, OUTPUT);
  pinMode(PIN_Q05, OUTPUT);
  pinMode(PIN_Q06, OUTPUT);

  pinMode(PIN_I01, INPUT);
  pinMode(PIN_I02, INPUT);
  pinMode(PIN_I03, INPUT);
  pinMode(PIN_I04, INPUT);
  pinMode(PIN_I05, INPUT);
  pinMode(PIN_I06, INPUT);
  pinMode(PIN_I07, INPUT);
  pinMode(PIN_I08, INPUT);

  pubqueue = xQueueCreate(10, sizeof(PubMsg));

  xTaskCreatePinnedToCore(
    TaskCom, "TaskCom", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  pubSubClient.setServer(broker_address, broker_port);
  pubSubClient.setCallback(msgReceived);

  while (!pubSubClient.connected()) {
    if (pubSubClient.connect(device_id, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker!");
      conn_st = CONN_ST_BROKER_CONNECTED;
      pubSubClient.subscribe(sub_topic);
    } else {
      Serial.print("Failed MQTT connection, rc=");
      Serial.println(pubSubClient.state());
      delay(2000);
    }
  }
}

/**************************    NEXTION - RTC     ***********************/
void timeSec() {
  //  ////Serial.println(dfd);
  val = dfd.indexOf("s") + 1; //clkd24m2y2022h2n58s30
  //  ////Serial.println(val);
  dfd.remove(0, val);
  //  Serial2.print("sec:");
  //  Serial2.println(dfd);
  uint8_t secRc = dfd.toInt();
  secsrc = secRc;
  ////Serial.print("Seconds: ");
  ////Serial.println(secRc);
}
void timeMin() {
  //  ////Serial.println(buff);
  char *cmn = strtok(buff, "s");
  String cSmn = cmn;
  //  ////Serial.println(cSmn);
  val = cSmn.indexOf("n") + 1; //    ////Serial.println(val);
  cSmn.remove(0, val);
  uint8_t minRc = cSmn.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  minrc = minRc;
  ////Serial.print("Minutes: ");
  ////Serial.println(minRc);
}
void timeHr() {
  //  ////Serial.println(buff);
  char *chr = strtok(buff, "n");
  String cShr = chr;
  val = cShr.indexOf("h") + 1; //    ////Serial.println(val);
  cShr.remove(0, val);
  uint8_t hrRc = cShr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  hourrc = hrRc;
  ////Serial.print("Hours: ");
  ////Serial.println(hrRc);

}
void timeDt() {
  //  ////Serial.println(buff);
  char *cdt = strtok(buff, "m");
  String cSdt = cdt;
  val = cSdt.indexOf("d") + 1; //    clkd24m2y2022h2n58s30
  cSdt.remove(0, val);
  //  ////Serial.println(str);
  uint8_t dtRc = cSdt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Daterc = dtRc;
  ////Serial.print("Date: ");
  ////Serial.println(Daterc);

}
void timeMnt() {
  char *cmnt = strtok(buff, "y");
  String cSmnt = cmnt;
  //  ////Serial.println(cSmnt);
  val = cSmnt.indexOf("m") + 1; //    ////Serial.println(val);
  cSmnt.remove(0, val);
  uint8_t mntRc = cSmnt.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Monthrc = mntRc;
  ////Serial.print("Month: ");
  ////Serial.println(Monthrc);

}
void timeYr() {
  char *cyr = strtok(buff, "h");
  String cSyr = cyr;
  val = cSyr.indexOf("y") + 1; //    ////Serial.println(val);
  cSyr.remove(0, val);
  int yrRc = cSyr.toInt(); //hourrc, minrc, secsrc, Daterc, Monthrc, Yearrc
  Yearrc = yrRc;
  ////Serial.print("Year: ");
  ////Serial.println(Yearrc);
}

void dateandtime() {
  DateTime now = rtc.now();

  hourupg = now.hour();
  minupg = now.minute();
  secslive = now.second();
  Datelive = now.day();
  Monthlive = now.month();
  Yearlive = now.year();

}

void readalldata() {
  EEPROM.write(200, hourrc);
  EEPROM.write(206, minrc);
  EEPROM.write(209, secsrc);
  EEPROM.write(212, Daterc);
  EEPROM.write(215, Monthrc);
  EEPROM.write(218, Yearrc);

  EEPROM.read(200);
  EEPROM.read(206);
  EEPROM.read(209);
  EEPROM.read(212);
  EEPROM.read(215);
  EEPROM.read(218);
}

void setTimeNex() {
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  runTimeNex();
}

void runTimeNex() {
  rtc.begin();
}

void meshpass() {
  char *pd = strtok(buff, ">");
  str = pd;
  int vale = str.indexOf("<") + 1;
  str.remove(0, vale);//meshpass
  String mstr = str;
  Serial.print("Mesh Pswd: ");
  Serial.println(mstr);
}

void prefix() {
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
  Serial.print("Prefix: ");
  Serial.println(str1);
}

void port() {
  int vale = dfd.indexOf(">") + 1;
  dfd.remove(0, vale);//port
  Serial.print("Port: ");
  Serial.println(dfd);
}

void SerialCom() {
  String data_from_display = "";
  String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
      Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());
  /************************************** WIFI - CREDENTIALS **************************/

  if ((dfd.substring(0, 4) == "sdpd")) {
    ssid = pwdname();
    Serial.print("User Name: ");
    Serial.println(str);
    str = ssid;
    password = pwd();
    Serial.print("Password: ");
    Serial.println(dfd);
    dfd = password;
    feedBack = "sdpd";
  }

  /**************************    NEXTION - RTC   ********************/
  if ((data_from_display.substring(0, 3) == "clk")) {
    timeMin();
    timeHr();
    timeSec();
    timeYr();
    timeMnt();
    timeDt();

    feedBack = "clk";
    dateandtime();
    readalldata();
    setTimeNex();

  }
  if ((data_from_display.substring(0, 6) == "settc")) {
    runTimeNex();
  }



  /**************************    MESH    ********************/
  if ((dfd.substring(0, 4) == "mesh")) {
    meshpass();
    prefix();
    port();
    feedBack = "mesh";
  }
  /************************************** TEMPERATURE **************************/

//  if ((dfd.substring(0, 4) == "temp")) {
//
//    Serial.println("ok");
//
//    sch10savetime();
//
//    float In_TempAutoSetpoint = vpdtemp_I();
//
//    SPtemp = In_TempAutoSetpoint ;
//
//    float Out_TempAutoSetpoint = vpdtemp_O();
//
//    feedBack = "tempat";
//
//  }

  //********************************************************************VPD*******************************
  //VPD Controller
//  if (dfd == "vpdoff") {
//    feedBack = "vpdon";
//    Serial.println("VPD  ON");
//    VpdautoModeOn = true;
//
//  }
//  if (dfd == "vpdon") {
//    asd();
//    VpdautoModeOn = false;
//    feedBack = "vpdoff";
//
//    Serial.println("VPD Off");
//    vpd_func = false;
//  }
  //********************************************************************End VPD*******************************

  //********************************************************************AUTO TEMP MODE*******************************

//  if (dfd == "tmpon") {
//
//
//    feedBack = "tmpon";
//    TemperatureAutoModeOn = true;
//    Serial.println("TEMP  ON");
//    temp_func = true;
//
//  }
//  if (dfd == "tmpoff") {
//
//
//    feedBack = "tmpoff";
//    TemperatureAutoModeOn = false;
//    asd();
//    Serial.println("TEMP  OFF");
//
//    temp_func = false;
//  }

//  sendData(data_from_display);
}

void callSerialCom() {
  if (Serial2.available()) {
    SerialCom();
  }
}

void  Nexval() {
  dateandtime();
  /////////////////////////////// Signal ////////////////////////

  Serial.println("nexval");
  Serial2.print("fdbk.txt=\"");
  Serial2.print(feedBack);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  ////////////////// wifi status /////////////
  Serial2.print("hstatus.txt=\"");
  Serial2.print(wifiStatus);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  /*************************** NODES VALUES *********************/

  Serial2.print("qo1.txt=\"");
  Serial2.print(LD_I1);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("qo2.txt=\"");
  Serial2.print(LD_I2);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("qo3.txt=\"");
  Serial2.print(LD_I3);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("qo4.txt=\"");
  Serial2.print(LD_I4);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("qo5.txt=\"");
  Serial2.print(LD_I5);
  Serial2.print("\"");
  Serial2.write(0x3ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("qo6.txt=\"");
  Serial2.print(LD_I6);
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  /**************** DATE & TIME ***********************/
  //Serial.print(hourupg);
  Serial2.print("hour.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("hourr.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //  Serial.print(minupg);
  Serial2.print("min.txt=\"");
  Serial2.print(minupg);//minupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //Serial.print(secslive);
  Serial2.print("sec.txt=\"");
  Serial2.print(secslive);//secslive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  ////////////  YEAR ////////

  //Serial.print(Datelive);
  Serial2.print("day.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("dayy.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  // Serial.print(Monthlive);
  Serial2.print("month.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("monthh.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  //Serial.print(Yearlive);
  Serial2.print("year.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  //    SerialU.println(yearch);

  Serial2.print("yearr.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = currentMillis;

    callSerialCom();
  }

  if (!pubSubClient.connected()) {
    if (conn_st == CONN_ST_BROKER_CONNECTED) {
      pubSubClient.disconnect();
      conn_st = CONN_ST_BROKER_CONNECTING;
    }

    if (conn_st == CONN_ST_BROKER_CONNECTING) {
      if (pubSubClient.connect(device_id, mqtt_username, mqtt_password)) {
        Serial.println("Connected to MQTT broker!");
        conn_st = CONN_ST_BROKER_CONNECTED;
        pubSubClient.subscribe(sub_topic);
        delay(100);
        Nexval();
      }
    }
  }

  pubSubClient.loop();

  LD_I1 = digitalRead(PIN_I01);
  LD_I4 = digitalRead(PIN_I04);
  LD_I2 = digitalRead(PIN_I02);
  LD_I3 = digitalRead(PIN_I03);
  LD_I5 = digitalRead(PIN_I05);
  LD_I6 = digitalRead(PIN_I06);
  LD_I7 = digitalRead(PIN_I07);
  LD_I8 = digitalRead(PIN_I08);

  // Ladder Logic
  if (LD_I1) {
    LD_T1.DN = 0;
    LD_T1.EN = 1;
    if (LD_T1.TT >= 1000) {
      LD_Q1 = 1;
    }
  } else {
    LD_T1.EN = 0;
    LD_T1.TT = 0;
    LD_T1.DN = 1;
    LD_Q1 = 0;
  }

  if (LD_I4) {
    LD_T2.DN = 0;
    LD_T2.EN = 1;
    if (LD_T2.TT >= 500) {
      LD_Q2 = 1;
    }
  } else {
    LD_T2.EN = 0;
    LD_T2.TT = 0;
    LD_T2.DN = 1;
    LD_Q2 = 0;
  }

  if (LD_I2) {
    LD_T3.DN = 0;
    LD_T3.EN = 1;
    if (LD_T3.TT >= 2000) {
      LD_Q3 = 1;
    }
  } else {
    LD_T3.EN = 0;
    LD_T3.TT = 0;
    LD_T3.DN = 1;
    LD_Q3 = 0;
  }

  if (LD_I3) {
    LD_T4.DN = 0;
    LD_T4.EN = 1;
    if (LD_T4.TT >= 3000) {
      LD_Q4 = 1;
    }
  } else {
    LD_T4.EN = 0;
    LD_T4.TT = 0;
    LD_T4.DN = 1;
    LD_Q4 = 0;
  }

  // JSON payload
  DynamicJsonDocument jsonPayload(256);
  jsonPayload["Q1"] = LD_Q1;
  jsonPayload["Q2"] = LD_Q2;
  jsonPayload["Q3"] = LD_Q3;
  jsonPayload["Q4"] = LD_Q4;
  jsonPayload["Q5"] = LD_Q5;
  jsonPayload["Q6"] = LD_Q6;

  String jsonString;
  serializeJson(jsonPayload, jsonString);

  publishMessage(jsonString.c_str());

  vTaskDelay(pdMS_TO_TICKS(100));
}

void msgReceived(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message received: ");
  Serial.println(topic);
  Serial.write(payload, len);
  Serial.println();

  // Process received message if needed
}

void sendMsg(char *msg) {
  Serial.print("Sending message: ");
  Serial.println(msg);
}
