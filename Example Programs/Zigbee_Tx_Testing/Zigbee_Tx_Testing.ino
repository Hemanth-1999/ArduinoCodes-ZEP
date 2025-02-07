
#include <XBee.h>
#include <WiFi.h>

// set up the WiFi client
WiFiClient client;

// set up the XBee module
XBee xbee = XBee();
ZBTxRequest zbTx = ZBTxRequest(0xFFFF, ZB_BROADCAST_ADDRESS, NULL, 0);

// set the Zigbee channel to 15
const int zigbeeChannel = 15;

void setup() {
  // initialize the Serial port
  Serial.begin(9600);
  
  // initialize the XBee module
  xbee.begin(Serial);
  xbee.setChannel(zigbeeChannel);

  // connect to the WiFi network
  WiFi.begin("ssid", "password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  // send a message over Zigbee
  char msg[] = "Hello from Zigbee!";
  zbTx.setPayload((uint8_t *)msg, strlen(msg));
  xbee.send(zbTx);

  // send a message over WiFi
  if (client.connect("server", 80)) {
    client.println("Hello from WiFi!");
    client.stop();
  }

  delay(1000);
}
