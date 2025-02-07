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
int steps = 10;

int speed1 = 30;
String direction1;

int speed2 = 30;
String direction2;

void plus() {
  pos++; //count steps
  Serial.println(pos);
  if (pos >= steps) {
    wheelStop();
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

void drive() {
  // {"direction1":"forward","direction2":"forward","steps":"300","speed1":"150","speed2":"150"}
  
  if (direction1 == "forward" && pos < steps) {
    wheelMoveForward(1); // Move wheel 1 forward
  } else if (direction1 == "backward" && pos < steps) {
    wheelMoveBackward(1); // Move wheel 1 backward
  } else {
    Serial.println("Stop Wheel 1");
    digitalWrite(m1_EL_Start_Stop, LOW);
  }

  if (direction2 == "forward" && pos < steps) {
    wheelMoveForward(2); // Move wheel 2 forward
  } else if (direction2 == "backward" && pos < steps) {
    wheelMoveBackward(2); // Move wheel 2 backward
  } else {
    Serial.println("Stop Wheel 2");
    digitalWrite(m2_EL_Start_Stop, LOW);
  }
}

void wheelStop() {
  digitalWrite(m1_EL_Start_Stop, LOW);
  digitalWrite(m2_EL_Start_Stop, LOW);  
}

void wheelMoveForward(int wheel) {
  if (wheel == 1) {
    analogWrite(m1_VR_speed, speed1);
    digitalWrite(m1_EL_Start_Stop, HIGH);
    digitalWrite(m1_ZF_Direction, HIGH);
  } else if (wheel == 2) {
    analogWrite(m2_VR_speed, speed2);
    digitalWrite(m2_EL_Start_Stop, HIGH);
    digitalWrite(m2_ZF_Direction, HIGH);
  }
}

void wheelMoveBackward(int wheel) {
  if (wheel == 1) {
    analogWrite(m1_VR_speed, speed1);
    digitalWrite(m1_EL_Start_Stop, HIGH);
    digitalWrite(m1_ZF_Direction, LOW);
  } else if (wheel == 2) {
    analogWrite(m2_VR_speed, speed2);
    digitalWrite(m2_EL_Start_Stop, HIGH);
    digitalWrite(m2_ZF_Direction, LOW);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, command);

    if (!error) {
      direction1 = doc["direction1"].as<String>();
      Serial.println(direction1);
      direction2 = doc["direction2"].as<String>();
      Serial.println(direction2);      
      steps = doc["steps"];
      Serial.println(steps);
      speed1 = doc["speed1"];
      Serial.println(speed1);
      speed2 = doc["speed2"];
      Serial.println(speed2);      
      drive();
    } else {
      Serial.println("Error parsing JSON: ");
      Serial.println(error.c_str());
    }
  }
}
