/**************************    WIFI - CLOUD     ***********************/
String pwd(void) {
  int val = dfd.indexOf("<") + 1;
  dfd.remove(0, val);
  return (dfd);
}

String pwdname(void) {
  char *pd = strtok(buff, "<");
  str = pd;
  str.remove(0, 4);
  str = str;
  return (str);
}

/**************************    MESH     ***********************/
void port(void) {
  int val = dfd.indexOf(">") + 1;
  dfd.remove(0, val);//port
}

void meshpass() {
  char *pd = strtok(buff, ">");
  str = pd;
  int val = str.indexOf("<") + 1;
  str.remove(0, val);//meshpass
  str = str;

}

void prefix() {
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
}

int tank() {

  int val = dfd.indexOf("z") + 1;
  dfd.remove(0, val);
  otMin = dfd.toInt();
  return (otMin);
}

int sendhour() {

  hr = strtok(buff, "min");
  str = hr;
  val = str.indexOf("hr") + 3;
  str.remove(0, val);
  shr = str.toInt();

  return (shr);

}

int sendmin() {

  val = dfd.indexOf("min") + 3; //
  dfd.remove(0, val);
  smn = dfd.toInt();

  return (smn);

}

int oftmin() {
  char *ui = strtok(buff, "z");
  str = ui;
  str.remove(0, 5);
  oftMin = str.toInt();
  return (oftMin);
}

void setTimeNex() {
  Serial.println("setTimeNex() is running...");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

void runTimeNex() {
  Serial.println("runTimeNex() is running...");
  rtc.begin();
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
}

/************************************** HyspH **************************/
void sendHph() {
  hph = strtok(buff, "b");
  String str = hph;
  //    Serial.println(str);
  val = str.indexOf("a") + 2;
  str.remove(0, val);
  pHHys = float(str.toInt()) / 10;
  Serial.print("HyspH = ");
  Serial.println(pHHys);
  dummyPHHYS = pHHys;
  if (pHHys == 0) {

    pHHys = 0.1;
    Serial.print("auto HyspH = ");
    Serial.println(pHHys);
  }
}

/************************************** HysEC **************************/
void sendHec() {
  val = dfd.indexOf("b") + 2;
  //Serial.println(val);
  dfd.remove(0, val);
  ECHys = float(dfd.toInt()) / 10;
  Serial.print("HysEc = ");
  Serial.println(ECHys);
}

/******************* SERIALCOM *******************************/
void SerialCom() {
  Serial.println("Serialcom() is running...");
  String data_from_display = "";
  delay(60);
  //   String dfd;           //use for setpoints but will effect schedule mode
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
  dfd = data_from_display;
  //    Serial.println(data_from_display);
  dfd.toCharArray(buff, dfd.length());

  /**************************    SCHEDULE MODE     ***********************/

  if (dfd.substring(0, 3) == "sl1") {

    sendec1();
    sendph1();
    Shift1Clock();
    Resetfun = true;
    feedBack = "sl1";

  }

  if (dfd.substring(0, 3) == "sl2") {

    sendec2();
    sendph2();
    Shift2Clock();
    Resetfun = true;
    feedBack = "sl2";

  }

  if (dfd.substring(0, 3) == "sl3") {

    sendec3();
    sendph3();
    Shift3Clock();
    Resetfun = true;
    feedBack = "sl3";

  }

  if (dfd.substring(0, 3) == "sl4") {

    sendec4();
    sendph4();
    Shift4Clock();
    Resetfun = true;
    feedBack = "sl4";

  }

  if (dfd.substring(0, 3) == "sl5") {

    sendec5();
    sendph5();
    Shift5Clock();
    Resetfun = true;
    feedBack = "sl5";

  }

  if (dfd.substring(0, 3) == "sl6") {

    sendec6();
    sendph6();
    Shift6Clock();
    Resetfun = true;
    feedBack = "sl6";

  }

  /**************************    AUTO MODE     ***********************/

  if (dfd.substring(0, 3) == "atm") {

    startph = true;

    val = dfd.indexOf("ph") + 2;
    dfd.remove(0, val);
    savpH = float(dfd.toInt());

    val = dfd.indexOf("ec") + 2;
    dfd.remove(0, val);
    savEc = float(dfd.toInt());

    feedBack = "atm";
  }

  if ((data_from_display.substring(0, 5) == "amOn")) {

    startph = true;
    autoboolcheck = true;
    AutoFun();
    digitalWrite(TPUMPR, LOW);
    digitalWrite(MIXERR, LOW);
    Calibration();
    Serial.println("Auto Mode is Enabled");
    feedBack = "amOn";

  } else if ((data_from_display.substring(0, 5) == "amOff")) {

    startph = false;
    autoboolcheck = false ;
    digitalWrite(TPUMPR, HIGH);
    digitalWrite(MIXERR, HIGH);
    Serial.println("Auto Mode is Disabled");
    feedBack = "amOff";

  }

  /**************************    MANUAL MODE     **************************/

  if ((data_from_display.substring(0, 3) == "phl")) {
    val = data_from_display.indexOf("l") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    phplus = data_from_display.toInt();
    Serial.print("pH+ = ");
    Serial.println(phplus);
    manualdosemin();
    feedBack = "phl";
  }

  if ((data_from_display.substring(0, 3) == "phm")) {
    val = data_from_display.indexOf("m") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    phmin = data_from_display.toInt();
    Serial.print("ph min= ");
    Serial.println(phmin);
    //      Serial.println(String(value.valLong));
    manualdoseplus();
    feedBack = "phm";
  }

  if ((data_from_display.substring(0, 3) == "nta")) {
    val = data_from_display.indexOf("a") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    ECA = data_from_display.toInt();
    Serial.print("Nutrient A = ");
    Serial.println(ECA);
    //Serial.println(String(value.valLong));
    manualdoseEcA();
    feedBack = "nta";
  }
  if ((data_from_display.substring(0, 3) == "ntb")) {
    val = data_from_display.indexOf("b") + 1; //    Serial.println(val);
    data_from_display.remove(0, val);
    ECB = data_from_display.toInt();
    Serial.print("Nutrient B = ");
    Serial.println(ECB);
    //      data_from_display = "";
    manualdoseEcB();
    feedBack = "ntb";
  }

  /**************************    SET POINTS   ********************/

  if (data_from_display.substring(0, 3) == "spt") {
    sendHec();
    sendHph();
    feedBack = "spt";
  }

  /**************************    TANK MANAGEMENT   ********************/

  if ((data_from_display.substring(0, 5) == "tkmst")) {

    Serial.print("Tank Height= ");
    tankHeight = tank();
    Serial.println(tankHeight);

    oftmin();
    Serial.print("Set Level= ");
    tanksetpoint = oftmin();
    Serial.println(tanksetpoint);
    feedBack = "tkmst";
  }

  /**************************    WIFI   ********************/

  if (dfd.substring(0, 4) == "sdpd") {

    ssid = pwdname();

    password = pwd();

    feedBack = "sdpd";

  }

  /**************************    MESH    ********************/

  if ((dfd.substring(0, 4) == "mesh")) {
    prefix();
    meshpass();
    port();
    feedBack = "mesh";
  }

  /**************************    CLOUD    ********************/

  if ((dfd.substring(0, 4) == "clod")) {

    ssid = pwdname();
    Serial.print("Cloud SSID: ");
    Serial.println(ssid);

    password = pwd();
    Serial.print("Cloud Password: ");
    Serial.println(password);
    //      dfd = password;
    feedBack = "clod";
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
    setTimeNex();//setTimeNex
  }

  if ((data_from_display.substring(0, 6) == "setrtc")) {
    runTimeNex();//runTimeNex
  }

  if ((data_from_display.substring(0, 3) == "rat")) {

  }
  sendData(data_from_display);  /////  UNITS, DEBUG & SPEED RATIO ////
}

void sendData(String dfd) {

  /********************************* SETTINGS - UNITS **************************/

  if (dfd == "TdsOn") {
    Serial.println("TDS is On");
    var = 1;
    feedBack = "TdsOn";
  }

  if (dfd == "EccOn") {
    Serial.println("Ec is On");
    var = 0;
    feedBack = "EccOn";
  }

  if (dfd == "CfOn") {
    Serial.println("CF is On");
    var = 2;
    feedBack = "CfOn";
  }

  if (dfd == "CelOn") {
    Serial.println("Celsius is On");
    unitTemp = nutrientTemperature;
    feedBack = "CelOn";
  }

  if (dfd == "FarOn") {
    Serial.println("Fahrenheit is On");
    unitTemp = Fahrenheit;
    feedBack = "FarOn";
  }

  /********************************* SETTINGS - MIX RATIO  **************************/
  if (dfd == "v1On") {
    Serial.println("Version 1 is On");
    feedBack = "v1On";
  }

  if (dfd == "v2On") {
    Serial.println("Version 2 is On");
    feedBack = "v2On";
  }

  if (dfd == "v3On") {
    Serial.println("Version 3 is On");
    feedBack = "v3On";
  }

  if (dfd == "Low") {
    Speed = 1000;
    Serial.println("Low Flow Rate");
    feedBack = "Low";
  }

  if (dfd == "Medium") {
    Speed = 750;
    Serial.println("Medium Flow Rate");
    feedBack = "Medium";
  }

  if (dfd == "High") {
    Speed = 500;
    Serial.println("High Flow Rate");
    feedBack = "High";
  }

  /********************************* DEBUG **************************/

  if (dfd == "SpOn") {
    //    TPRelay = HIGH;
    digitalWrite(TPUMPR, LOW);
    Serial.println("Sub Pump is On");
    sampumpSt = "on";
    feedBack = "SpOn";
  }
  if (dfd == "SpOff") {
    //    TPRelay = LOW;
    digitalWrite(TPUMPR, HIGH);
    Serial.println("Sub Pump is Off");
    sampumpSt = "off";
    feedBack = "SpOff";
  }
  if (dfd == "MixOn") {
    //    MRelay = HIGH;
    digitalWrite(MIXERR, LOW);
    Serial.println("Mixer is On");
    mixerSt = "on";
    feedBack = "MixOn";
  }
  if (dfd == "MixOff") {
    //    MRelay = LOW;
    digitalWrite(MIXERR, HIGH);
    Serial.println("Mixer is Off");
    mixerSt = "off";
    feedBack = "MixOff";
  }
  if (dfd == "HtOn") {
    //    HRelay = HIGH;
    digitalWrite(HEATERR, LOW);
    Serial.println("Heater is On");
    feedBack = "HtOn";
    HeaterSt = "on";
  }
  if (dfd == "HtOff") {
    //    HRelay = LOW;
    digitalWrite(HEATERR, HIGH);
    Serial.println("Heater is Off");
    feedBack = "HtOff";
    HeaterSt = "off";
  }
  if (dfd == "ClOn") {
    //    RRelay = HIGH;
    digitalWrite(RORELAYR, LOW);
    Serial.println("Cooler is On");
    feedBack = "ClOn";
    RoSt = "on";
  }
  if (dfd == "ClOff") {
    //    RRelay = LOW;
    digitalWrite(RORELAYR, HIGH);
    Serial.println("Cooler is Off");
    feedBack = "ClOff";
    RoSt = "off";
  }

  /************************************** WATER LEVEL SENSOR **************************/

  if (dfd == "TkMgOn") {
    waterlevelnt = true;
    waterlevel_sensor = true;
    Serial.println("Tank Management On");
    feedBack = "TkMgOn";

  }

  if (dfd == "TkMgOff") {
    waterlevelnt = false;
    Serial.println("Tank Management Off");
    feedBack = "TkMgOff";

  }

}

void nexVal() {
  Serial.println("nexVal() is running...");
  dateandtime();
  switch (var) {
    case 0:
      unitConduct = ECValueavrg;
      //        Serial.println(ECValueavrg);
      break;
    case 1:
      unitConduct = nutrientTDS;
      //        Serial.println(nutrientTDS);
      break;
    case 2:
      unitConduct = nutrientCF;
      break;
    default:
      unitConduct = ECValueavrg;
      break;
  }
  /**************** WIFI ***********************/
  Serial2.print("hstatus.txt=\"");
  Serial2.print(wifiStatus);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  /**************** WATER LEVEL SENSOR ***********************/
  Serial2.print("readtank.txt=\"");
  Serial2.print(waterconlevel);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  /****************  TEMPERATURE  ***********************/
  Serial2.print("WaterTemp.txt=\"");
  Serial2.print(unitTemp); // unitTemp = nutrientTemperature(or) Fahrenheit
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  /**************** CALIBRATION ***********************/
  Serial2.print("phreading.txt=\"");
  Serial2.print(savpH);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("ecreading.txt=\"");
  Serial2.print(savEc);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("pHvalue.txt=\"");
  Serial2.print(pH);
  //  Serial2.print("7");
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("ECvalue.txt=\"");
  Serial2.print(unitConduct); // unitConduct = nutrientCF (or)nutrientTDS nutrientEC /ecValue
  //  Serial2.print("7");
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("DOvalue.txt=\"");
  Serial2.print(DO);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);


  /**************** DATE & TIME ***********************/
  //Serial.print(hourupg);
  Serial2.print("hour.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  //    Serial.println("time: ");
  //    Serial.println(hourupg);
  Serial2.print("hourr.txt=\"");
  Serial2.print(hourupg);//hourupg
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  //  Serial.println(minupg);
  Serial2.print("min.txt=\"");
  Serial2.print(minupg);//minupg
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  //Serial.print(secslive);
  Serial2.print("sec.txt=\"");
  Serial2.print(secslive);//secslive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  ////////////  YEAR ////////

  //Serial.print(Datelive);
  Serial2.print("day.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("dayy.txt=\"");
  Serial2.print(Datelive);//Datelive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  // Serial.print(Monthlive);
  Serial2.print("month.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("monthh.txt=\"");
  Serial2.print(Monthlive);//Monthlive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("year.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  Serial2.print("yearr.txt=\"");
  Serial2.print(Yearlive);//Yearlive
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

  /**************** PH AND EC VALUES *****************/
  Serial2.print("readph.txt=\"");
  Serial2.print(pH_Value);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);


  Serial2.print("readec.txt=\"");
  Serial2.print(Ec_Value);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  /**************** FEEDBACK ***********/
  Serial2.print("fdbk.txt=\"");
  Serial2.print(feedBack);
  Serial2.print("\"");
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);

}
