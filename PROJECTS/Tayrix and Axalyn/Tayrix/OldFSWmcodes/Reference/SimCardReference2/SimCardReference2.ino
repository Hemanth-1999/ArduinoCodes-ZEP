#define SIM800L_IP5306_VERSION_20200811
#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG SerialMon
#include "utilities.h"
#define SerialMon Serial
#define SerialAT  Serial1
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER      1024
#include <TinyGsmClient.h>
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  60
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";
const char apn[]      = "jioNet"; // Your APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any
TinyGsmClient client(modem);
const int  port = 80;
void setupModem()
{
#ifdef MODEM_RST
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_RST, HIGH);
#endif
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_POWER_ON, HIGH);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LED_OFF);
}
void turnOffNetlight()
{
  SerialMon.println("Turning off SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=0");
}

void turnOnNetlight()
{
  SerialMon.println("Turning on SIM800 Red LED...");
  modem.sendAT("+CNETLIGHT=1");
}
void setup()
{
  SerialMon.begin(115200);
  delay(10);
  if (setupPMU() == false) {
    Serial.println("Setting power error");
  }
  setupModem();
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
}
void loop()
{
  SerialMon.println("Initializing modem...");
  modem.restart();
  turnOffNetlight();
  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem: ");
  SerialMon.println(modemInfo);
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork(240000L)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");
  digitalWrite(LED_GPIO, LED_ON);

  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }
  SerialMon.print(F("Connecting to APN: "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");
  SerialMon.print("Connecting to ");
  SerialMon.print(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");
  SerialMon.println("Performing HTTP GET request...");
  client.print(String("GET ") + resource + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");
  client.println();
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      char c = client.read();
      SerialMon.print(c);
      timeout = millis();
    }
  }
  SerialMon.println();
  client.stop();
  SerialMon.println(F("Server disconnected"));
  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));
#ifdef MODEM_DTR
  bool res;
  modem.sleepEnable();
  delay(100);
  res = modem.testAT();
  Serial.print("SIM800 Test AT result -> ");
  Serial.println(res);
  delay(1000);
  Serial.println("Use DTR Pin Wakeup");
  pinMode(MODEM_DTR, OUTPUT);
  digitalWrite(MODEM_DTR, LOW);
  res = modem.testAT();
  Serial.print("SIM800 Test AT result -> ");
  Serial.println(res);
#endif
#ifdef TEST_RING_RI_PIN
#ifdef MODEM_RI
  SerialAT.print("AT+CHFA=1\r\n");
  delay(2);
  SerialAT.print("AT+CRSL=100\r\n");
  delay(2);
  SerialAT.print("AT+CLVL=100\r\n");
  delay(2);
  SerialAT.print("AT+CLIP=1\r\n");
  delay(2);
  pinMode(MODEM_RI, INPUT);
  Serial.println("Wait for call in");
  while (digitalRead(MODEM_RI)) {
    Serial.print('.');
    delay(500);
  }
  Serial.println("call in ");
  delay(10000);
  SerialAT.println("ATA");
  delay(10000);
  SerialAT.println("ATH");
#endif
#endif
  int i = 3;
  while (i--) {
    digitalWrite(LED_GPIO, LED_ON);
    modem.sendAT("+SPWM=0,1000,80");
    delay(500);
    digitalWrite(LED_GPIO, LED_OFF);
    modem.sendAT("+SPWM=0,1000,0");
    delay(500);
  }
  modem.poweroff();
  SerialMon.println(F("Poweroff"));
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
