int value = 0;
float voltage;
float accvol;
float perc;

String data_from_display = "";
String dfd;
String str;
String feedBack;

char buff[2000];

void setup() {
  Serial.begin(9600);
}

void Voltage() {
  value = analogRead(13);
  voltage = value * 5.0 / 1023;
  accvol = voltage - 1.53;
  perc = map(voltage, 3.6, 4.2, 0, 100);
  Serial.print("Voltage= ");
  Serial.println(accvol);
  Serial.print("Battery level= ");
  Serial.print(perc);
  Serial.println(" %");
  delay(500);
}

void port() {
  int vale = dfd.indexOf(">") + 1;
  dfd.remove(0, vale);//port
  Serial.print("Port: ");
  Serial.println(dfd);
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

void SerialCom(String frommob) 
{
  String data_from_display = "";
  //       String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  //    Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());

  if ((data_from_display.substring(0, 6) == "settc")) {
//    runTimeNex();
  }

  /**************************    MESH    ********************/
  if ((dfd.substring(0, 4) == "mesh")) {
    meshpass();
    prefix();
    port();
    feedBack = "mesh";
  }

//  sendData(data_from_display);
}

void callSerialCom() {
  if (Serial2.available()) {
   SerialCom("");
  }
}

void nexval() {
  if (accvol > 0) {

    Serial2.print("at.txt=\"");
    Serial2.print(accvol);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  }

}

unsigned long SCpreviousMillis = 0;
unsigned long NXpreviousMillis = 0;

void loop() {
  unsigned long SCcurrentMillis = millis();
  if (SCcurrentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = SCcurrentMillis;
    callSerialCom();
  }

  unsigned long NXcurrentMillis = millis();
  if (NXcurrentMillis - NXpreviousMillis >= 30000) {
    NXpreviousMillis = NXcurrentMillis;
    nexval();
  }
}
