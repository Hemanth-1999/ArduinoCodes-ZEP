int othr() {
  ////Serial.println(dfd);//off time min value
  int val = dfd.indexOf("g") + 1;//
  //  ////Serial.println(val);
  dfd.remove(0, val);
  //  ////Serial.println(dfd);
  otHr = dfd.toInt();
  return (otHr);
  //  ////Serial.println(otHr);
}

int otmin() {
  char *nt = strtok(buff, "g");//off time hr val
  str = nt;
  int val = str.indexOf("f") + 1;
  str.remove(0, val);
  otMin = str.toInt();
  return (otMin);
  //  ////Serial.println(otMin);
}
int ofthr() {
  char *fr = strtok(buff, "f");
  str = fr;
  int val = str.indexOf("e") + 1; //  on time min val
  str.remove(0, val);
  oftHr = str.toInt();
  return (oftHr);
}
int oftmin() {
  char *ui = strtok(buff, "e");//on time hr val
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}

void SerialCom() {
  String data_from_display = "";
  //       String dfd;           //use for setpoints but will effect schedule mode
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  //    Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());

  /************************************** TEMPERATURE **************************/

  if ((dfd.substring(0, 4) == "temp")) {

    //funtion

  }



  if (dfd == "IrrMode_DueOn")
  {

    int Z1_IRR_hr = 00;
    int Z1_IRR_min = 01;

    Serial.print("Z1_IRR_hr: ");
    Serial.println(Z1_IRR_hr);
    Serial.print("Z1_IRR_min: ");
    Serial.println(Z1_IRR_min);

    Serial.print("Due switch Start");
    long Due_IRR_minToMilliSec = 1;
    long Z1_IRR_OnTimeHr = Z1_IRR_hr * ms_per_hour;
    long Z1_IRR_OnTimeMIn = Z1_IRR_min * ms_per_min;
    long ZoneA_TotelOnTime = Z1_IRR_OnTimeHr + Z1_IRR_OnTimeMIn;

    DurationIrrbool = true;
    Due_IRR.begin( ZoneA_TotelOnTime, ZPumpStatus); // RelayPIN,ONTime,RelayStatus

    Serial.println("Irr Z1 On");
    ZPumpStatus = true;

    feedBack = "z1aon";
  }
  if (dfd == "IrrMode_DueOff")
  {
    DurationIrrbool = true;
    Serial.println("PUMP Z1  Off");
    feedBack = "z1aoff";
  }

  if (dfd == "IrrMode_QtyOn") {
    feedBack = "QtyOn";
    IrrMode_Qty_bool=true;
    Serial.println("IrrMode_Qty  ON");

  }
  if (dfd == "IrrMode_QtyOff") {
    IrrMode_Qty_bool=false;
    feedBack = "QtyOff";
    Serial.println("IrrMode_Qty Off");

  }
}

void sendDataToNextion(String dfd) {
  if (dfd == "f1on" ) {
    feedBack = "f1on";
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
    Serial.println("f1 On");
  }
  if (dfd == "f1off") {
    feedBack = "f1off";
    digitalWrite(12, HIGH);
    Serial.println("f1 Off");
  }
  if (dfd == "f2on") {
    feedBack = "f2on";
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
    Serial.println("f2 On");
  }
}
