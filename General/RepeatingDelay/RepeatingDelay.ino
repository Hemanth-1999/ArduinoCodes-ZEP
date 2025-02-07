//#include <DS3231.h>
#include <Time.h>
#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000

long Zone1HrOn  = 18;
long Zone1MinOn = 1;

long Zone1HrOnconvOn  = Zone1HrOn * ms_per_hour;
long Zone1MinOnconvOn = Zone1MinOn * ms_per_min;
long Zone1TotalOnTime = Zone1HrOnconvOn + Zone1MinOnconvOn;

//long Zone1HrOnconvOff  = Zone1HrOff * ms_per_hour;
//long Zone1MinOnconvOff = Zone1MinOff * ms_per_min;
//long Zone1TotalOffTime = Zone1HrOnconvOff + Zone1MinOnconvOff;

int Light1 = 5;
int Light2 = 6;
int Light3 = 7;
int Light4 = 8;

//DS3231  rtc(SDA, SCL);
//Time t;

unsigned long LightOnSec = 15000; // 20 sec
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
bool ledOn = false; // keep track of the Light state

void setup() {
  Serial.begin(115200);
//  rtc.begin();
  pinMode(Light1, OUTPUT);
  digitalWrite(Light1, LOW);
  pinMode(Light2, OUTPUT);
  digitalWrite(Light2, LOW);
  pinMode(Light3, OUTPUT);
  digitalWrite(Light3, LOW);
  pinMode(Light4, OUTPUT);
  digitalWrite(Light4, LOW);

  //     start delay
  delayStart = millis();
  delayRunning = true;
}

//*******************************************//////////// AUTO MODE ////////////********************************************************************

void checkToggleLight() {
  //   check if delay has timed out
  if (delayRunning && ((millis() - delayStart) >= Zone1MinOnconvOn)) {
    delayStart += Zone1MinOnconvOn; // this prevents drift in the delays
    // toggle the Light
    ledOn = !ledOn;
    if (Light1 == HIGH) {
      digitalWrite(Light1, HIGH);
      Serial.println("Light1 OFF");// turn Light on
    } else {
      digitalWrite(Light1, LOW);
      Serial.println("Light1 ON");// turn Light off
    }

    if (Light2 == HIGH) {
      digitalWrite(Light2, HIGH);
      Serial.println("Light2 OFF");// turn Light on
    } else {
      digitalWrite(Light2, LOW);
      Serial.println("Light2 ON");// turn Light off
    }

    if (Light3 == HIGH) {
      digitalWrite(Light3, HIGH);
      Serial.println("Light3 OFF");// turn Light on
    } else {
      digitalWrite(Light3, LOW);
      Serial.println("Light3 ON");// turn Light off
    }

    if (Light4 == HIGH) {
      digitalWrite(Light4, HIGH);
      Serial.println("Light4 OFF");// turn Light on
    } else {
      digitalWrite(Light4, LOW);
      Serial.println("Light4 ON");// turn Light off
    }
  }

}

void loop() {
//  t = rtc.getTime();
//  Serial.print(t.hour);
//  Serial.print(" hr(s), ");
//  Serial.print(t.min);
//  Serial.print(" min(s), ");
//  Serial.print(t.sec);
//  Serial.print(" sec(s)");
//  Serial.println(" ");
  delay (1000);
  checkToggleLight();// call to toggle Light based on timer
}
