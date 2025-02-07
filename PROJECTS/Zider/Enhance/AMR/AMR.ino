#include <ArduinoJson.h>

//#######################################
// GPIO mappings for Arduino Mega 2560
//#######################################

int m1_EL_Start_Stop = 5; // EL
int m1_Signal_hall = 4;   // Signal - Hall sensor
int m1_ZF_Direction = 3;  // ZF
int m1_VR_speed = 2;      // VR

int m2_EL_Start_Stop = 11; // EL
int m2_Signal_hall = 10;   // Signal - Hall sensor
int m2_ZF_Direction = 9;  // ZF
int m2_VR_speed = 8;      // VR

int pos = 0;
int steps = 0;
int speed1 = 0;
int speed2 = 0;
String direction1;
String direction2;

void plus() {
  pos++; //count steps
  Serial.println(pos);
  if (pos >= steps) {
    wheel1_Stop();
    wheel2_Stop();
    pos = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //wheel 1 - Setup pin mode
  pinMode(m1_EL_Start_Stop, OUTPUT); //stop/start - EL
  pinMode(m1_Signal_hall, INPUT);    //plus       - Signal
  pinMode(m1_ZF_Direction, OUTPUT);  //direction  - ZF
  //wheel 2 - Setup pin mode
  pinMode(m2_EL_Start_Stop, OUTPUT); //stop/start - EL
  pinMode(m2_Signal_hall, INPUT);    //plus       - Signal
  pinMode(m2_ZF_Direction, OUTPUT);  //direction  - ZF
  //Hall sensor detection - Count steps
  attachInterrupt(digitalPinToInterrupt(m1_Signal_hall), plus, CHANGE);
  attachInterrupt(digitalPinToInterrupt(m2_Signal_hall), plus, CHANGE);
}

unsigned long lastMsg = 0;
unsigned long lastMsg1 = 0;
unsigned long lastMsg2 = 0;
unsigned long lastMsg3 = 0;
unsigned long lastMsg4 = 0;
unsigned long lastMsg5 = 0;

void wheel1_Stop() {
  analogWrite(m1_VR_speed, speed1);
  digitalWrite(m1_EL_Start_Stop, LOW);
}

void wheel2_Stop() {
  analogWrite(m2_VR_speed, speed1);
  digitalWrite(m2_EL_Start_Stop, LOW);
}

void wheel1_Move_Forward() {
  analogWrite(m1_VR_speed, speed1);
  digitalWrite(m1_EL_Start_Stop, HIGH);
  digitalWrite(m1_ZF_Direction, HIGH);
}

void wheel2_Move_Forward() {
  analogWrite(m2_VR_speed, speed2);
  digitalWrite(m2_EL_Start_Stop, HIGH);
  digitalWrite(m2_ZF_Direction, HIGH);
}

void wheel1_Move_Backward() {
  analogWrite(m1_VR_speed, speed1);
  digitalWrite(m1_EL_Start_Stop, HIGH);
  digitalWrite(m1_ZF_Direction, LOW);
}

void wheel2_Move_Backward() {
  analogWrite(m2_VR_speed, speed2);
  digitalWrite(m2_EL_Start_Stop, HIGH);
  digitalWrite(m2_ZF_Direction, LOW);
}

void drive() {
  // {"direction1":"forward","direction2":"forward","steps":"0","speed1":"50","speed2":"50"}
  // {"direction":"backword","steps":"30","speed":"50"}
  // {"direction":"stop","steps":"0","speed":"0"}--

  if (direction1 == "forward" && pos < steps) {
    unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) {
      lastMsg = now1;
      wheel1_Move_Forward();
    }
  } else if (direction1 == "backward" && pos < steps) {
    unsigned long now2 = millis();
    if (now2 - lastMsg1 > 5000) {
      lastMsg1 = now2;
      wheel1_Move_Backward();
    }
  } else if (direction1 == "stop" && pos < steps) {
    unsigned long now3 = millis();
    if (now3 - lastMsg2 > 5000) {
      lastMsg2 = now3;
      wheel1_Stop();
    }
  }

  if (direction2 == "backward" && pos < steps) {
    unsigned long now4 = millis();
    if (now4 - lastMsg3 > 5000) {
      lastMsg3 = now4;
      wheel2_Move_Forward();
    }
  } else if (direction2 == "forward" && pos < steps) {
    unsigned long now5 = millis();
    if (now5 - lastMsg4 > 5000) {
      lastMsg4 = now5;
      wheel2_Move_Backward();
    }
  } else if (direction2 == "stop" && pos < steps) {
    unsigned long now6 = millis();
    if (now6 - lastMsg5 > 5000) {
      lastMsg5 = now6;
      wheel2_Stop();
    }
  }
}

unsigned long lastMsg01 = 0;

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, command);

    if (!error) {
      direction1 = doc["direction1"].as<String>();
      Serial.println("direction1:");
      Serial.println(direction1);
      direction2 = doc["direction2"].as<String>();
      Serial.println("direction2:");
      Serial.println(direction2);
      Serial.println();
      steps = doc["steps"];
      Serial.println("steps:");
      Serial.println(steps);
      Serial.println();
      speed1 = doc["speed1"];
      Serial.println("speed1:");
      Serial.println(speed1);
      speed2 = doc["speed2"];
      Serial.println("speed2:");
      Serial.println(speed2);
      unsigned long nowon = millis();
      if (nowon - lastMsg01 > 1000) {
        lastMsg01 = nowon;
        drive();
      }
    } else {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    }
  }
}
