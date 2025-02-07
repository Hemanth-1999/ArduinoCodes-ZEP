/* Arduino Mega 2560 Equivalent Code */
#include <Arduino.h>
#include "Nextion.h"
#include <TaskScheduler.h>

// objects
Scheduler r;

// Constants
const int SENSOR_PIN = A0; // Input pin for measuring Vout
const int VOLTAGE_REF = 5; // Reference voltage for analog read
const float VoltageDividerRatio = 7.2; // Adjust based on your voltage divider
const float shuntResistor = 10;  // Value of the shunt resistor in ohms

// Global Variables
float sensorValue; // Variable to store value from analog read
float current; // Calculated current value

// Callback methods prototypes
void Parameters();

Task t1(500,  TASK_FOREVER, &Parameters, &r);

// Nextion Objects, ada 5 tombol
// (page id, component id, component name)
NexButton bt0 = NexButton(3, 17, "bt0");
NexButton b0 = NexButton(3, 16, "b0");
NexPage page1 = NexPage(3, 0, "ctrl");

// Declare variable global
bool statusbt0 = false;
bool statusb0 = false;
bool nexval = true;

// Register objects to the touch event list
NexTouch *nex_listen_list[] = {
  &page1,
  &bt0,
  &b0,
  NULL
};

/* Constants */
const uint8_t Cmd_End[3] = {0xFF, 0xFF, 0xFF}; // Command end sequence for Nextion

float Speed, Acceleation;

int Transport_Status, Vehicle_Status;

float Velocity_X, Velocity_Y, Velocity_Z;

int Err_Alrt, EstTime, Load, SftyInd, Signal, Service;

void switchToPage1() {
  Serial2.print("page ctrl");
  Serial2.write(Cmd_End, 3);
}

void page1PopCallback(void *ptr)
{
  dbSerialPrintln("page1PopCallback");
  page1.show();
}

//Saat tombol 1 ditekan,relay 1 menyala
void bt0PushCallback(void *ptr) {
  if (statusbt0 == false) {
    digitalWrite(8, LOW);
    statusbt0 = true;
  }
  else {
    digitalWrite(8, HIGH);
    statusbt0 = false;
  }
}

//Saat tombol 2 ditekan,relay 2 menyala
void b0PushCallback(void *ptr) {
  if (statusb0 == false) {
    digitalWrite(9, LOW);
    statusb0 = true;
  }
  else {
    digitalWrite(9, HIGH);
    statusb0 = false;
  }
}

void Parameters() {
  if (nexval == true) {

    Serial2.print("sp.txt=\"");
    Serial2.print("30");
    //    Serial2.print(Speed);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("acc.txt=\"");
    Serial2.print("50");
    //    Serial2.print(Acceleation);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("ts.txt=\"");
    Serial2.print("arrived");
    //    Serial2.print(Transport_Status);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("vi.txt=\"");
    Serial2.print("1265");
    //    Serial2.print(Vehicle_Status);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("vs.txt=\"");
    Serial2.print("Idle");
    //    Serial2.print(Vehicle_Status);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("vx.txt=\"");
    Serial2.print("0.5");
    //    Serial2.print(Velocity_X);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("vy.txt=\"");
    Serial2.print("1");
    //    Serial2.print(Velocity_Y);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("vz.txt=\"");
    Serial2.print("1.5");
    //    Serial2.print(Velocity_Z);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("err.txt=\"");
    Serial2.print("No supply");
    //    Serial2.print(Err_Alrt);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("et.txt=\"");
    Serial2.print("30");
    //    Serial2.print(EstTime);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("ld.txt=\"");
    Serial2.print("No load");
    //    Serial2.print(Load);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("si.txt=\"");
    Serial2.print("Supply");
    //    Serial2.print(SftyInd);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("sl.txt=\"");
    Serial2.print("Good");
    //    Serial2.print(Signal);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);

    Serial2.print("sv.txt=\"");
    Serial2.print("Good");
    //    Serial2.print(Service);
    Serial2.print("\"");
    Serial2.write(Cmd_End, 3);
  }
}

/* Function to send data to a numeric display */
void SendtoNum(const char *obj, uint16_t value) {
  char buf[30];
  int len = sprintf(buf, "%s=%u", obj, value);
  Serial2.write((uint8_t *)buf, len);
  Serial2.write(Cmd_End, 3); // End of command
}

/* Function to send data to a progress bar */
void SendtoPB(const char *obj, uint16_t value) {
  char buf[30];
  int len = sprintf(buf, "%s=%u", obj, value);
  Serial2.write((uint8_t *)buf, len);
  Serial2.write(Cmd_End, 3); // End of command
}

/* Function to send data to a speed gauge */
void SendtoSpeed(const char *obj, uint16_t value) {
  char buf[30];
  int len = sprintf(buf, "%s=%u", obj, value);
  Serial2.write((uint8_t *)buf, len);
  Serial2.write(Cmd_End, 3); // End of command
}

/* Setup function runs once at the beginning */
void setup() {
  Serial.begin(9600); // Initialize Serial2 communication for debugging or other purposes
  Serial2.begin(9600); // Initialize Serial2 communication with Nextion display
  // Note: Ensure this baud rate matches your Nextion display's configuration
  r.startNow();
  //Initialize Nextion Library
  nexInit();
  dbSerialPrintln("setup begin");

  // Register relay (output)
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Register the push/pop event callback function
  bt0.attachPush(bt0PushCallback, &bt0);
  b0.attachPush(b0PushCallback, &b0);
  page1.attachPop(page1PopCallback);

  dbSerialPrintln("setup end");

  delay(2000);
  //Matikan semua relay
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(500);

}

void BtteryStatus() {
  // Read a value from the INA169 board
  sensorValue = analogRead(SENSOR_PIN);

  // Sensor value (in volts) to the serial monitor
  Serial.print("sensorValue: ");
  Serial.print(sensorValue, 3);
  Serial.println(" V");

  // Convert the raw analog value (0-1023) to a voltage (assuming 5V reference)
  float vout = (sensorValue / 1023.0) * VOLTAGE_REF;
  float sourceVoltage = (sensorValue * VOLTAGE_REF) / 1023.0 * VoltageDividerRatio; // Adjust for divider

  // Output value (in volts) to the serial monitor
  Serial.print("SVout: ");
  Serial.print(sourceVoltage, 3);
  Serial.println(" V");

  Serial.print("Vout: ");
  Serial.print(vout, 3);
  Serial.println(" V");

  // Calculate the current (in amps) using the corrected INA169 formula
  // Amplified by 1000 in INA169, using 0.1Ω shunt resistor
  current = sourceVoltage / shuntResistor;  // Adjust the calculation for high current

  // Output value (in amps) to the serial monitor to 3 decimal places
  Serial.print("Current: ");
  Serial.print(current, 3);
  Serial.println(" A");

  // Convert the current value into a percentage based on the maximum current you want to represent
  // For example, if 3A is your maximum current, you would map it to 100%
  float maxCurrent = 2.0; // Set this to the maximum current you expect (in amps)
  int batteryPercentage = constrain((current / maxCurrent) * 100, 0, 100); // Ensure it stays within 0-100%

  Serial.print("BatteryPercentage: ");
  Serial.print(batteryPercentage);
  Serial.println(" %");
  Serial.println();

  // Update numeric display
  SendtoNum("n0.val", batteryPercentage);

  // Set color based on battery percentage
  if (batteryPercentage < 25) {
    SendtoPB("j0.pco", 63488); // Color range for 0-24 (red)
  }
  else if (batteryPercentage < 50) {
    SendtoPB("j0.pco", 64520); // Color range for 25-49 (yellow)
  }
  else {
    SendtoPB("j0.pco", 2016);  // Color range for 50-100 (green)
  }

  // Update the progress bar (or speed gauge) to reflect the current percentage
  SendtoSpeed("j0.val", batteryPercentage); // Update progress bar

  delay(300); // Small delay for readability/processing (can be replaced with millis() for non-blocking)
}

/* Loop function runs continuously */
void loop() {
  BtteryStatus();
  delay(1000);
  Parameters();
  //When push/pop event occured execute component in touch event list
  nexLoop(nex_listen_list);
}
