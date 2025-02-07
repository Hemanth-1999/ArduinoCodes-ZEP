#include "Nextion.h"
#include <millisDelay.h>

// Pin assignments for the relays
#define ExFan1  A3
#define ExFan2  A2
#define ExFan3  30
#define ExFan4  28
#define ExFan5    8
#define ExFan6    9
#define ExFan7   10
#define ExFan8   11

// Read the current temperature and VPD
// For testing, consider setting fixed values:
float MESP_ID_Time_AvgAT_v = 28.5;  // Replace with actual or fixed value
float MESP_ID_Time_AvgVPD_v = 0.7;  // Replace with actual or fixed value

// Temperature setpoints
const float TEMP_SETPOINT = 25.0;
const float TEMP_LOW = TEMP_SETPOINT + 1.0;   // 26°C
const float TEMP_MID = TEMP_LOW + 2.0;        // 28°C
const float TEMP_HIGH = TEMP_MID + 2.0;       // 30°C

// VPD setpoints
const float VPD_LOW = 0.3;
const float VPD_MID = 0.6;
const float VPD_HIGH = 1.0;

NexPage page0 = NexPage(8, 0, "vpd1");
NexPage page1 = NexPage(9, 0, "temp");

// Nextion Objects
NexButton bt12 = NexButton(8, 5, "bt12");
NexButton bt13 = NexButton(9, 16, "bt13");

// Declare global variables
bool statusbt12 = false; // VPD mode status
bool statusbt13 = false; // Temperature mode status
bool toggleState = false; // Tracks which set of fans are on

String temp_mode;
String vpd_mode;

millisDelay relayDelay;       // MillisDelay object for non-blocking delays
const unsigned long TOGGLE_TIME = 5000; // Time in milliseconds for relay toggle

// Register objects to the touch event list
NexTouch *nex_listen_list[] = {
  &bt12,
  &bt13,
  NULL
};

void switchToPage0() {
  Serial.print("page vpd1");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void switchToPage1() {
  Serial.print("page temp");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

// Function to turn off all fans
void turnOffAllFans() {
  digitalWrite(ExFan1, HIGH);
  digitalWrite(ExFan2, HIGH);
  digitalWrite(ExFan3, HIGH);
  digitalWrite(ExFan4, HIGH);
  digitalWrite(ExFan5, HIGH);
  digitalWrite(ExFan6, HIGH);
  digitalWrite(ExFan7, HIGH);
  digitalWrite(ExFan8, HIGH);
}

// Function to turn on specific fans
void TempLow(int fan1, int fan2, int fan3 = -1, int fan4 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  if (fan3 != -1) digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
}

void TempMid(int fan1, int fan2, int fan3, int fan4 = -1, int fan5 = -1, int fan6 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
}

void TempHigh(int fan1, int fan2, int fan3, int fan4, int fan5 = -1, int fan6 = -1, int fan7 = -1, int fan8 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
  if (fan7 != -1) digitalWrite(fan7, LOW);
  if (fan8 != -1) digitalWrite(fan8, LOW);
}

// Function to handle fans based on VPD
void VPDMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgVPD_v <= VPD_LOW) {
      TempLow(ExFan2, ExFan4);
    } else if (MESP_ID_Time_AvgVPD_v <= VPD_MID) {
      TempMid(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgVPD_v <= VPD_HIGH) {
      TempHigh(ExFan2, ExFan4, ExFan6, ExFan8);
    }
  } else {
    // Alternate fans when toggleState is false
    if (MESP_ID_Time_AvgVPD_v <= VPD_LOW) {
      TempLow(ExFan1, ExFan3);
    } else if (MESP_ID_Time_AvgVPD_v <= VPD_MID) {
      TempMid(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgVPD_v <= VPD_HIGH) {
      TempHigh(ExFan1, ExFan3, ExFan5, ExFan7);
    }
  }
}

// Function to handle fans based on Temperature
void TempMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v <= TEMP_MID) {
      TempLow(ExFan1, ExFan3);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v <= TEMP_HIGH) {
      TempMid(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      TempHigh(ExFan1, ExFan3, ExFan5, ExFan7);
    }
  } else {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v <= TEMP_MID) {
      TempLow(ExFan2, ExFan4);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v <= TEMP_HIGH) {
      TempMid(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      TempHigh(ExFan2, ExFan4, ExFan6, ExFan8);
    }
  }
}

// Callback function when button bt12 is pressed (VPD mode)
void bt12PushCallback(void *ptr) {
  if (!statusbt12) {
    statusbt12 = true;
    statusbt13 = false; // Ensure Temperature mode is off
    VPDMode();
    relayDelay.start(TOGGLE_TIME);
    vpd_mode = "1";
  } else {
    statusbt12 = false;
    relayDelay.stop();
    turnOffAllFans();
    vpd_mode = "0";
  }
}

// Callback function when button bt13 is pressed (Temperature mode)
void bt13PushCallback(void *ptr) {
  if (!statusbt13) {
    statusbt13 = true;
    statusbt12 = false; // Ensure VPD mode is off
    TempMode();
    relayDelay.start(TOGGLE_TIME);
    temp_mode = "1";
  } else {
    statusbt13 = false;
    relayDelay.stop();
    turnOffAllFans();
    temp_mode = "0";
  }
}

void setup() {
  // Initialize pins
  pinMode(ExFan1, OUTPUT);
  pinMode(ExFan2, OUTPUT);
  pinMode(ExFan3, OUTPUT);
  pinMode(ExFan4, OUTPUT);
  pinMode(ExFan5, OUTPUT);
  pinMode(ExFan6, OUTPUT);
  pinMode(ExFan7, OUTPUT);
  pinMode(ExFan8, OUTPUT);

  turnOffAllFans();

  // Initialize Serial communication
  Serial.begin(9600);

  // Initialize Nextion display
  nexInit();

  // Attach button callbacks
  bt12.attachPush(bt12PushCallback, &bt12);
  bt13.attachPush(bt13PushCallback, &bt13);
}

void loop() {
  // Handle touch events from Nextion display
  nexLoop(nex_listen_list);

  // Apply fan control logic based on active mode
  if (statusbt12) { // VPD Mode
    if (relayDelay.justFinished()) {
      relayDelay.repeat(); // Restart the delay
      toggleState = !toggleState;  // Toggle between states
      VPDMode();
    }
  } else if (statusbt13) { // Temperature Mode
    if (relayDelay.justFinished()) {
      relayDelay.repeat(); // Restart the delay
      toggleState = !toggleState;  // Toggle between states
      TempMode();
    }
  }
}
