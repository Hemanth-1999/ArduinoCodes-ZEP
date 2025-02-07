/* Arduino Mega 2560 Equivalent Code */
#include <Arduino.h>
#include "Nextion.h"
#include <TaskScheduler.h>

// objects
Scheduler r;

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

void Nexval() {
  for (int value = 0; value <= 100; value++) {
    SendtoNum("n0.val", value); // Update numeric display

    Parameters();
    //    t1.enable();

    // Set color based on value
    if (value < 25) {
      SendtoPB("j0.pco", 63488); // Color range for 0-24
    }
    else if (value < 50) {
      SendtoPB("j0.pco", 64520); // Color range for 25-49
    }
    else {
      SendtoPB("j0.pco", 2016);  // Color range for 50-100
    }

    float val_p = ((float)value / 100.0) * 100;
    SendtoSpeed("j0.val", val_p); // Update progress bar (or speed gauge)

    delay(300); // Delay for readability/processing
  }
}

/* Loop function runs continuously */
void loop() {
  Nexval();
  //When push/pop event occured execute component in touch event list
  nexLoop(nex_listen_list);
  r.execute();
}
