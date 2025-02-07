#include "Nextion.h"
#include <Arduino.h>
#include <MemoryFree.h>
#include <ArduinoJson.h>

#define ExFan1  A3
#define ExFan2  A2
#define ExFan3  30
#define ExFan4  28

//Relay-2
#define ExFan5    8
#define ExFan6    9
#define ExFan7   10
#define ExFan8   11

//Relay-3
#define pad1      A4
#define Fogger    A6
#define FoggerSol A5
#define Co2Gen    A7

NexPage page0 = NexPage(8, 0, "vpd1");
NexPage page1 = NexPage(9, 0, "temp");
NexPage page2 = NexPage(10, 0, "manual1");
NexPage page3 = NexPage(11, 0, "manual2");

// Nextion Objects, ada 5 tombol
// (page id, component id, component name)
NexButton bt0 = NexButton(10, 1, "bt0");
NexButton bt1 = NexButton(10, 2, "bt1");
NexButton bt2 = NexButton(10, 3, "bt2");
NexButton bt3 = NexButton(10, 4, "bt3");
NexButton bt4 = NexButton(10, 5, "bt4");
NexButton bt5 = NexButton(10, 30, "bt5");
NexButton bt6 = NexButton(11, 31, "bt6");
NexButton bt7 = NexButton(11, 1, "bt7");
NexButton bt8 = NexButton(10, 5, "bt8");
NexButton bt9 = NexButton(10, 2, "bt9");
NexButton bt10 = NexButton(10, 3, "bt10");
NexButton bt11 = NexButton(10, 4, "bt11");
NexButton bt12 = NexButton(8, 5, "bt12");
NexButton bt13 = NexButton(9, 16, "bt13");

// Declare variable global
bool statusbt0 = false;
bool statusbt1 = false;
bool statusbt2 = false;
bool statusbt3 = false;
bool statusbt4 = false;
bool statusbt5 = false;
bool statusbt6 = false;
bool statusbt7 = false;
bool statusbt8 = false;
bool statusbt9 = false;
bool statusbt10 = false;
bool statusbt11 = false;
bool statusbt12 = false;
bool statusbt13 = false;

String r1;
String r2;
String r3;
String r4;
String r5;
String r6;
String r7;
String r8;
String r9;
String r10;
String r11;
String r12;

// Register objects to the touch event list
NexTouch *nex_listen_list[] = {
  &bt0,
  &bt1,
  &bt2,
  &bt3,
  &bt4,
  &bt5,
  &bt6,
  &bt7,
  &bt8,
  &bt9,
  &bt10,
  &bt11,
  &bt12,
  &bt13,
  NULL
};

char buff[2000];
bool temp_func;
extern volatile unsigned long timer0_millis;
int count1 = 5;
int count3 = 10;
int count4 = 20;
String feedBack;
float SPtemp;
float ATempHysteris = SPtemp + 1;
String dfd;
float MESP_ID_Time_AvgAT_v, MESP_ID_Time_AvgRH_v, MESP_ID_Time_AvgHI_v, MESP_ID_Time_AvgVPD_v, MESP_ID_Time_EvTr_v, MESP_ID_Time_BaPr_v, MESP_ID_Time_SoRa_v,   MESP_ID_Time_DePt_v;
int MESP_ID_Time_SeL_v, MESP_ID_Time_ntl_v, MESP_ID_Time_co2_v ;

//Saat tombol 1 ditekan,relay 1 menyala
void bt0PushCallback(void *ptr) {
  if (statusbt0 == false) {
    digitalWrite(A2, LOW);
    statusbt0 = true;
    //set button color RED
    bt0.Set_background_color_bco(5411);

    //set for QR Code
    r1 = "1";
  }
  else {
    digitalWrite(A2, HIGH);
    statusbt0 = false;
    //reset button color
    bt0.Set_background_color_bco(57408);

    //set for QR Code
    r1 = "0";
  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt1PushCallback(void *ptr) {
  if (statusbt1 == false) {
    digitalWrite(A3, LOW);
    statusbt1 = true;
    //set button color RED
    bt1.Set_background_color_bco(5411);

    //set for QR Code
    r2 = "1";
  }
  else {
    digitalWrite(A3, HIGH);
    statusbt1 = false;
    //reset button color
    bt1.Set_background_color_bco(57408);

    //set for QR Code
    r2 = "0";
  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt2PushCallback(void *ptr) {
  if (statusbt2 == false) {
    digitalWrite(28, LOW);
    statusbt2 = true;
    //set button color RED
    bt2.Set_background_color_bco(5411);

    //set for QR Code
    r3 = "1";
  }
  else {
    digitalWrite(28, HIGH);
    statusbt2 = false;
    //reset button color
    bt2.Set_background_color_bco(57408);

    //set for QR Code
    r3 = "0";
  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void bt3PushCallback(void *ptr) {
  if (statusbt3 == false) {
    digitalWrite(30, LOW);
    statusbt3 = true;
    //set button color RED
    bt3.Set_background_color_bco(5411);

    //set for QR Code
    r4 = "1";
  }
  else {
    digitalWrite(30, HIGH);
    statusbt3 = false;
    //reset button color
    bt3.Set_background_color_bco(57408);

    //set for QR Code
    r4 = "0";
  }

}

void bt5PushCallback(void *ptr) {
  if (statusbt4 == false) {
    digitalWrite(8, LOW);
    statusbt4 = true;
    //set button color RED
    bt5.Set_background_color_bco(5411);

    //set for QR Code
    r5 = "1";
  }
  else {
    digitalWrite(8, HIGH);
    statusbt4 = false;
    //reset button color
    bt5.Set_background_color_bco(57408);

    //set for QR Code
    r5 = "0";
  }

}

//Saat tombol reset ditekan, semua relay mati
//void bt4PushCallback(void *ptr) {
//  digitalWrite(A2, HIGH);
//  digitalWrite(A3, HIGH);
//  digitalWrite(8, HIGH);
//  digitalWrite(28, HIGH);
//  digitalWrite(30, HIGH);
//  statusbt0 = false;
//  statusbt1 = false;
//  statusbt2 = false;
//  statusbt3 = false;
//  statusbt4 = false;
//
//  //reset button color
//  bt0.Set_background_color_bco(48631);
//  bt1.Set_background_color_bco(48631);
//  bt2.Set_background_color_bco(48631);
//  bt3.Set_background_color_bco(48631);
//  bt5.Set_background_color_bco(48631);
//
//}

void bt13PushCallback(void *ptr) {
  if (Serial3.available()) {
    if (statusbt13 == false) {
      statusbt13 = true;
//      TemperatureAutoModeOn = true;

      if ((dfd.substring(0, 4) == "temp")) {

        float In_TempAutoSetpoint = temp_I();

        SPtemp = In_TempAutoSetpoint ;

        float Out_TempAutoSetpoint = temp_O();

        feedBack = "tempat";

      }

      StaticJsonDocument <500> doc3;

      if (count3 == 0)
      {
        StaticJsonDocument<200> clearr1;
        doc3 = clearr1;
        char tempstring2[500];
        serializeJson(clearr1, tempstring2);
        memset(tempstring2, 0, 500);

        count3 = 10;
      }

      if (MESP_ID_Time_AvgAT_v > 0)
      {
        count3--;


        if ((MESP_ID_Time_AvgAT_v >= 36) && (MESP_ID_Time_AvgAT_v <= 38))
        {


          JsonObject Alerts = doc3.createNestedObject("Alerts");
          Alerts["Mode"] = "12";
          Alerts["level"] = "10";
          serializeJson(doc3, Serial);
          serializeJson(doc3, Serial3);



        }
        else if ((MESP_ID_Time_AvgAT_v >= 39) && (MESP_ID_Time_AvgAT_v <= 41))
        {

          JsonObject Alerts = doc3.createNestedObject("Alerts");
          Alerts["Mode"] = "12";
          Alerts["level"] = "9";
          serializeJson(doc3, Serial);
          serializeJson(doc3, Serial3);

        }
        else if (MESP_ID_Time_AvgAT_v > 42)
        {

          JsonObject Alerts = doc3.createNestedObject("Alerts");
          Alerts["Mode"] = "12";
          Alerts["level"] = "8";
          serializeJson(doc3, Serial);
          serializeJson(doc3, Serial3);

        }
        else if (MESP_ID_Time_AvgAT_v <= 35)
        {

          JsonObject Alerts = doc3.createNestedObject("Alerts");
          Alerts["Mode"] = "12";
          Alerts["level"] = "11";
          serializeJson(doc3, Serial);
          serializeJson(doc3, Serial3);

        }
      }
      Serial.println(MESP_ID_Time_AvgAT_v);

      Serial.println(SPtemp);

      Serial.println(ATempHysteris);
      if (MESP_ID_Time_AvgAT_v != 0) {

        if ( MESP_ID_Time_AvgAT_v >= ATempHysteris) {  // 33>=26  //31.80>=26w


          Serial.println(" we need  fans to turn on");
          Serial.println(MESP_ID_Time_AvgAT_v);
          //    Serial.println(ATempHysteris);
          float ATempLow =  ATempHysteris; //26
          float ATempMid = ATempLow + 2; //28
          float ATempHigh = ATempMid + 2; //30

          Serial.println("Modes");
          Serial.println(ATempHigh);
          Serial.println(ATempMid);
          Serial.println(ATempLow);
          if (MESP_ID_Time_AvgAT_v >= ATempHigh) //32<31                 // Temp=33    32<33//31.80>=32 fail
          {
            unsigned long mills = millis();
            if ((mills >= 0) && (mills <= 1200000))
            {
              Serial.println(" FIRST STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, LOW);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, LOW);
              pinMode(30, OUTPUT);
              digitalWrite(30, LOW);
              pinMode(28, OUTPUT);
              digitalWrite(28, LOW);
              pinMode(8, OUTPUT);
              digitalWrite(8, LOW);
              pinMode(9, OUTPUT);
              digitalWrite(9, LOW);
              pinMode(10, OUTPUT);
              digitalWrite(10, HIGH);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }
            else if ((mills >= 1205000) && (mills <= 2400000))
            {
              Serial.println(" SECOND STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, LOW);
              pinMode(28, OUTPUT);
              digitalWrite(28, LOW);
              pinMode(8, OUTPUT);
              digitalWrite(8, LOW);
              pinMode(9, OUTPUT);
              digitalWrite(9, LOW);
              pinMode(10, OUTPUT);
              digitalWrite(10, LOW);
              pinMode(11, OUTPUT);
              digitalWrite(11, LOW);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, HIGH);
            }

            else if (mills >= 2405000)
            {
              noInterrupts ();
              timer0_millis = 0;
              interrupts ();
            }
          }
          else if ((MESP_ID_Time_AvgAT_v >= ATempMid) && (MESP_ID_Time_AvgAT_v < ATempHigh)) //31>30&& 31<32        //Temp=31  31>30  && 31<32//31.80>=30 && 31.80<32
          {

            unsigned long mills = millis();
            if ((mills >= 0) && (mills <= 60000))
            {
              Serial.println(" FIRST STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, LOW);
              pinMode(30, OUTPUT);
              digitalWrite(30, HIGH);
              pinMode(28, OUTPUT);
              digitalWrite(28, LOW);
              pinMode(8, OUTPUT);
              digitalWrite(8, HIGH);
              pinMode(9, OUTPUT);
              digitalWrite(9, LOW);
              pinMode(10, OUTPUT);
              digitalWrite(10, HIGH);
              pinMode(11, OUTPUT);
              digitalWrite(11, LOW);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }
            else if ((mills >= 61000) && (mills <= 120000))
            {
              Serial.println(" SECOND STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, LOW);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, LOW);
              pinMode(28, OUTPUT);
              digitalWrite(28, HIGH);
              pinMode(8, OUTPUT);
              digitalWrite(8, LOW);
              pinMode(9, OUTPUT);
              digitalWrite(9, HIGH);
              pinMode(10, OUTPUT);
              digitalWrite(10, LOW);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);

            }


            else if (mills >= 125000)
            {
              noInterrupts ();
              timer0_millis = 0;
              interrupts ();
            }
          }
          else  if ((MESP_ID_Time_AvgAT_v >= ATempLow) && (MESP_ID_Time_AvgAT_v < ATempMid)) //Temp=29 //31.80>=28 && 31.80<30
          {
            unsigned long mills = millis();
            if ((mills >= 0) && (mills <= 1200000))
            {
              Serial.println(" FIRST STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, HIGH);
              pinMode(28, OUTPUT);
              digitalWrite(28, LOW);
              pinMode(8, OUTPUT);
              digitalWrite(8, HIGH);
              pinMode(9, OUTPUT);
              digitalWrite(9, LOW);
              pinMode(10, OUTPUT);
              digitalWrite(10, HIGH);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }
            else if ((mills >= 1205000) && (mills <= 2400000))
            {
              Serial.println(" SECOND STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, LOW);
              pinMode(28, OUTPUT);
              digitalWrite(28, HIGH);
              pinMode(8, OUTPUT);
              digitalWrite(8, HIGH);
              pinMode(9, OUTPUT);
              digitalWrite(9, HIGH);
              pinMode(10, OUTPUT);
              digitalWrite(10, LOW);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }


            else if (mills >= 2405000)
            {
              noInterrupts ();
              timer0_millis = 0;
              interrupts ();
            }
          }
          else if ((MESP_ID_Time_AvgAT_v >= SPtemp) && (MESP_ID_Time_AvgAT_v < 26)) { //Temp=27 31.80>=26 31.80<28
            unsigned long mills = millis();
            if ((mills >= 0) && (mills <= 1200000))
            {
              Serial.println(" FIRST STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, HIGH);
              pinMode(28, OUTPUT);
              digitalWrite(28, LOW);
              pinMode(8, OUTPUT);
              digitalWrite(8, HIGH);
              pinMode(9, OUTPUT);
              digitalWrite(9, LOW);
              pinMode(10, OUTPUT);
              digitalWrite(10, HIGH);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }
            else if ((mills >= 1205000) && (mills <= 2400000))
            {
              Serial.println(" SECOND STATE TIME ");
              Serial.println( mills );
              pinMode(A3, OUTPUT);
              digitalWrite(A3, HIGH);
              pinMode(A2, OUTPUT);
              digitalWrite(A2, HIGH);
              pinMode(30, OUTPUT);
              digitalWrite(30, LOW);
              pinMode(28, OUTPUT);
              digitalWrite(28, HIGH);
              pinMode(8, OUTPUT);
              digitalWrite(8, HIGH);
              pinMode(9, OUTPUT);
              digitalWrite(9, HIGH);
              pinMode(10, OUTPUT);
              digitalWrite(10, LOW);
              pinMode(11, OUTPUT);
              digitalWrite(11, HIGH);
              pinMode(pad1, OUTPUT);
              digitalWrite(pad1, LOW);
            }


            else if (mills >= 2405000)
            {
              noInterrupts ();
              timer0_millis = 0;
              interrupts ();
            }
          }
        }

        if (MESP_ID_Time_AvgAT_v < SPtemp) {
          //31.80<26
          Serial.println(MESP_ID_Time_AvgAT_v);
          Serial.println("All OFF");
          //relayState = HIGH;
          pinMode(A3, OUTPUT);
          digitalWrite(A3, HIGH);
          pinMode(A2, OUTPUT);
          digitalWrite(A2, HIGH);
          pinMode(30, OUTPUT);
          digitalWrite(30, HIGH);
          pinMode(28, OUTPUT);
          digitalWrite(28, HIGH);
          pinMode(8, OUTPUT);
          digitalWrite(8, HIGH);
          pinMode(9, OUTPUT);
          digitalWrite(9, HIGH);
          pinMode(10, OUTPUT);
          digitalWrite(10, HIGH);
          pinMode(11, OUTPUT);
          digitalWrite(11, HIGH);
          pinMode(pad1, OUTPUT);
          digitalWrite(pad1, HIGH);
        }
      }
      Serial.println("TEMP");
//      funtimer();
      Serial.println("TEMP  ON");
      temp_func = true;
    }
    else {
      digitalWrite(A3, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(30, HIGH);
      digitalWrite(28, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(A4, HIGH);
      digitalWrite(A6, HIGH);
      digitalWrite(A5, HIGH);
      digitalWrite(A7, HIGH);
      statusbt13 = false;
//      TemperatureAutoModeOn = false;
      Serial.println("TEMP  OFF");
      temp_func = false;
    }
  }
}

float temp_I() {
  char *ui = strtok(buff, "o");
  str = ui;
  int vale = dfd.indexOf("it") + 3;
  str.remove(0, vale);
  float i_temp = float(str.toInt()) / 10;
  return (i_temp);

}
float temp_O() {//tempit10ot10
  Serial.println(dfd);
  int vale = dfd.indexOf("ot") + 3;
  dfd.remove(0, vale);
  Serial.println(dfd);
  float o_temp = float(dfd.toInt()) / 10;
  return (o_temp);

}

void SerialCom(String frommob)
{
  String data_from_display = "";
  delay(50);
  while (Serial2.available()) {
    data_from_display += char(Serial2.read());
  }
      Serial.println(data_from_display);
  dfd = data_from_display;
  dfd.toCharArray(buff, dfd.length());
  sendData(data_from_display);
}

void setup() {
  Serial.begin(9600);
  //Initialize Nextion Library
  nexInit();

  // Register relay (output)
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(pad1, OUTPUT);
  pinMode(Fogger, OUTPUT);
  pinMode(FoggerSol, OUTPUT);
  pinMode(Co2Gen, OUTPUT);

  // Register the push/pop event callback function
  bt0.attachPush(bt0PushCallback, &bt0);
  bt1.attachPush(bt1PushCallback, &bt1);
  bt2.attachPush(bt2PushCallback, &bt2);
  bt3.attachPush(bt3PushCallback, &bt3);
  bt4.attachPush(bt4PushCallback, &bt4);
  bt5.attachPush(bt5PushCallback, &bt5);
  bt13.attachPush(bt5PushCallback, &bt13);

  delay(2000);
  //Matikan semua relay
  digitalWrite(A3, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(pad1, HIGH);
  digitalWrite(Fogger, HIGH);
  digitalWrite(FoggerSol, HIGH);
  digitalWrite(Co2Gen, HIGH);
  delay(500);

}

void loop() {
  //When push/pop event occured execute component in touch event list
  if (Serial3.available()) {
    ////////////************** Serialcom loop Mode***********///////////
    //When push/pop event occured execute component in touch event list

    //////////************** VPD loop Mode***********///////////
    if (VpdautoModeOn == true ) {

      unsigned long VPcurrentMillis = millis();
      if (VPcurrentMillis - VPpreviousMillis >= 10000) {
        VPpreviousMillis = VPcurrentMillis;
      }
    }

    String receivedPayload = Serial3.readStringUntil('\n');
    DynamicJsonDocument avgDoc(1024);
    deserializeJson(avgDoc, Serial3);
    DeserializationError error = deserializeJson(avgDoc, Serial3);

    if (error) {

      return;
    }

    float MESP_ID_Time_AvgAT_v = avgDoc["AT_V_avg"].as<float>();
    float MESP_ID_Time_AvgRH_v = avgDoc["RH_v_avg"].as<float>();
    float MESP_ID_Time_AvgVPD_v = avgDoc["VPD_v_avg"].as<float>();
    float MESP_ID_Time_AvgHI_v = avgDoc["HI_v_avg"].as<float>();
    int MESP_ID_Time_co2_v = avgDoc["Co2_v_avg"].as<int>();
    float MESP_ID_Time_EvTr_v = avgDoc["ET_v_avg"].as<float>();
    float MESP_ID_Time_BaPr_v = avgDoc["BP_v_avg"].as<float>();
    float MESP_ID_Time_SoRa_v = avgDoc["SR_v_avg"].as<float>();
    float MESP_ID_Time_DePt_v = avgDoc["DP_v_avg"].as<float>();
    int MESP_ID_Time_SeL_v = avgDoc["SL_v_avg"].as<int>();
    nexLoop(nex_listen_list);
    callSerialCom();
    Serial.println("Data From Esp: ");
    Serial.print("AT:");
    Serial.println(MESP_ID_Time_AvgAT_v);
    Serial.print("RH:");
    Serial.println(MESP_ID_Time_AvgRH_v);
    Serial.print("VPD:");
    Serial.println(MESP_ID_Time_AvgVPD_v);
    Serial.print("HI:");
    Serial.println(MESP_ID_Time_AvgHI_v);
    Serial.print("Co2:");
    Serial.println(MESP_ID_Time_co2_v);
    Serial.print("ET:");
    Serial.println(MESP_ID_Time_EvTr_v);
    Serial.print("BP:");
    Serial.println(MESP_ID_Time_BaPr_v);
    Serial.print("SR:");
    Serial.println(MESP_ID_Time_SoRa_v);
    Serial.print("DP:");
    Serial.println(MESP_ID_Time_DePt_v);
    Serial.print("SL:");
    Serial.println(MESP_ID_Time_SeL_v);

    if (MESP_ID_Time_AvgAT_v > 0)
    {

      Serial2.print("at.txt=\"");
      Serial2.print(MESP_ID_Time_AvgAT_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.print("rh.txt=\"");
      Serial2.print(MESP_ID_Time_AvgRH_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("vpd.txt=\"");
      Serial2.print(MESP_ID_Time_AvgVPD_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      if (MESP_ID_Time_EvTr_v > 0)
      {
        Serial2.print("ev.txt=\"");
        Serial2.print(MESP_ID_Time_EvTr_v);
        Serial2.print("\"");
        Serial2.write(0x03ff);
        Serial2.write(0x03ff);
        Serial2.write(0x03ff);
      }

      Serial2.print("bp.txt=\"");
      Serial2.print(MESP_ID_Time_BaPr_v);
      Serial2.print("\"");
      Serial2.write(0x3ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("sl.txt=\"");
      Serial2.print(MESP_ID_Time_SeL_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("co.txt=\"");
      Serial2.print(MESP_ID_Time_co2_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("dpp.txt=\"");
      Serial2.print(MESP_ID_Time_DePt_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("sr.txt=\"");
      Serial2.print(MESP_ID_Time_SoRa_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);

      Serial2.print("hi.txt=\"");
      Serial2.print(MESP_ID_Time_AvgHI_v);
      Serial2.print("\"");
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
      Serial2.write(0x03ff);
    }

  }
}
