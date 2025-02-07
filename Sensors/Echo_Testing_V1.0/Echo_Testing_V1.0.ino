#include <Arduino.h>

#define TAG "RS485_ECHO_APP"
#define ECHO_TEST_TXD 17
#define ECHO_TEST_RXD 16
#define ECHO_TEST_RE 2
#define ECHO_TEST_DE 4
#define BUF_SIZE  127
#define PACKET_READ_TICS   100//     (100 / portTICK_PERIOD_MS)
#define ECHO_READ_TOUT 3


byte data[BUF_SIZE];
int recvdCount = 0;

static void echo_send( const char* str, uint8_t length) {
  digitalWrite(ECHO_TEST_DE, HIGH);
  digitalWrite(ECHO_TEST_RE, HIGH);
  if (Serial2.write(str, length) != length) {
    ESP_LOGE(TAG, "Send data critical failure.");
    abort();
  } else Serial2.flush();

  digitalWrite(ECHO_TEST_DE, LOW);
  digitalWrite(ECHO_TEST_RE, LOW);

}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, ECHO_TEST_RXD, ECHO_TEST_TXD);
  pinMode(ECHO_TEST_RE, OUTPUT);
  pinMode(ECHO_TEST_DE, OUTPUT);
  echo_send("Start RS485 UART test.\r\n", 24);
  Serial.println("Ready..");
}



void loop() {

  unsigned long now = millis();

  if (Serial2.available()) {
    //read bytes till timeout..
    recvdCount = 0;
    memset(data, 0, sizeof(data));
    while (millis() - now < PACKET_READ_TICS) {
      if (Serial2.available()) {
        byte b = Serial2.read();
        data[recvdCount] = b;
        recvdCount++;
        if (recvdCount >= sizeof(data)) {
          //error, potential overflow
          recvdCount = 0;
        }
      }
    }
    if (recvdCount > 0) {
      echo_send( "\r\n", 2);
      char prefix[] = "RS485 Received: [";
      echo_send( prefix, (sizeof(prefix) - 1));
      ESP_LOGI(TAG, "Received %u bytes:", recvdCount);
      Serial.print("Esp Received: ");
      for (int i = 0; i < recvdCount; i++) {
        Serial.printf("0x%.2X ", (uint8_t)data[i]);
        Serial.print("; Echo Sent: ");
        echo_send( (const char*)&data[i], 1);
        if (data[i] == '\r') {
          echo_send( "\n", 1);
        }
      }
      echo_send( "]\r\n", 3);

    } else {
      echo_send( ".", 1);
      Serial.println(".");
    }
  }


}
