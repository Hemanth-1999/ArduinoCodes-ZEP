
#include <XBee.h>
#include <WiFi.h>

// set up the WiFi server
WiFiServer server(80);

// set up the XBee module
XBee xbee = XBee();
ZBRxResponse zbRx = ZBRxResponse();

// set the Zigbee channel to 15
const int zigbeeChannel = 15;

void setup() {
  // initialize the Serial port
  Serial.begin(9600);
  
  // initialize the XBee module
  xbee.begin(Serial);
  xbee.setChannel(zigbeeChannel);

  // start the WiFi server
  server.begin();
}

void loop() {
  // check for incoming Zigbee messages
  if (xbee.readPacket()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(zbRx);
      char msg[zbRx.getDataLength()+1];
      memcpy(msg, zbRx.getData(), zbRx.getDataLength());
      msg[zbRx.getDataLength()] = '\0';
      Serial.print("Zigbee message: ");
      Serial.println(msg);
    }
  }

  // check for incoming WiFi messages
  WiFiClient client = server.available();
  if (client) {
    String msg = client.readStringUntil('\r');
    client.stop();
    Serial.print("WiFi message: ");
    Serial.println(msg);
  }

  delay(100);
}
