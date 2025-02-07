#include "Nextion.h"

// Nextion Objects, ada 5 tombol
// (page id, component id, component name)
NexButton b0 = NexButton(0, 1, "b0");
NexButton b1 = NexButton(0, 2, "b1");
NexButton b2 = NexButton(0, 3, "b2");
NexButton b3 = NexButton(0, 4, "b3");
NexButton b5 = NexButton(0, 6, "b5");
NexButton b4 = NexButton(0, 2, "b4");

// Declare variable global
bool statusb0 = false;
bool statusb1 = false;
bool statusb2 = false;
bool statusb3 = false;
bool statusb4 = false;
String qr_code;
String r1;
String r2;
String r3;
String r4;
String r5;

// Register objects to the touch event list
NexTouch *nex_listen_list[] = {
  &b0,
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  NULL
};


//Saat tombol 1 ditekan,relay 1 menyala
void b0PushCallback(void *ptr) {
  if (statusb0 == false) {
    digitalWrite(A2, LOW);
    statusb0 = true;
    //set button color RED
    b0.Set_background_color_bco(5411);

    //set for QR Code
    r1 = "1";
  }
  else {
    digitalWrite(A2, HIGH);
    statusb0 = false;
    //reset button color
    b0.Set_background_color_bco(57408);

    //set for QR Code
    r1 = "0";
  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void b1PushCallback(void *ptr) {
  if (statusb1 == false) {
    digitalWrite(A3, LOW);
    statusb1 = true;
    //set button color RED
    b1.Set_background_color_bco(5411);

    //set for QR Code
    r2 = "1";
  }
  else {
    digitalWrite(A3, HIGH);
    statusb1 = false;
    //reset button color
    b1.Set_background_color_bco(57408);

    //set for QR Code
    r2 = "0";
  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void b2PushCallback(void *ptr) {
  if (statusb2 == false) {
    digitalWrite(28, LOW);
    statusb2 = true;
    //set button color RED
    b2.Set_background_color_bco(5411);

    //set for QR Code
    r3 = "1";
  }
  else {
    digitalWrite(28, HIGH);
    statusb2 = false;
    //reset button color
    b2.Set_background_color_bco(57408);

    //set for QR Code
    r3 = "0";
  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void b3PushCallback(void *ptr) {
  if (statusb3 == false) {
    digitalWrite(30, LOW);
    statusb3 = true;
    //set button color RED
    b3.Set_background_color_bco(5411);

    //set for QR Code
    r4 = "1";
  }
  else {
    digitalWrite(30, HIGH);
    statusb3 = false;
    //reset button color
    b3.Set_background_color_bco(57408);

    //set for QR Code
    r4 = "0";
  }

}

void b5PushCallback(void *ptr) {
  if (statusb4 == false) {
    digitalWrite(8, LOW);
    statusb4 = true;
    //set button color RED
    b5.Set_background_color_bco(5411);

    //set for QR Code
    r5 = "1";
  }
  else {
    digitalWrite(8, HIGH);
    statusb4 = false;
    //reset button color
    b5.Set_background_color_bco(57408);

    //set for QR Code
    r5 = "0";
  }

}

//Saat tombol reset ditekan, semua relay mati
void b4PushCallback(void *ptr) {
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(30, HIGH);
  statusb0 = false;
  statusb1 = false;
  statusb2 = false;
  statusb3 = false;
  statusb4 = false;

  //reset button color
  b0.Set_background_color_bco(48631);
  b1.Set_background_color_bco(48631);
  b2.Set_background_color_bco(48631);
  b3.Set_background_color_bco(48631);
  b5.Set_background_color_bco(48631);

}

void setup() {
  Serial.begin(9600);
  //Initialize Nextion Library
  nexInit();

  // Register relay (output)
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(30, OUTPUT);

  // Register the push/pop event callback function
  b0.attachPush(b0PushCallback, &b0);
  b1.attachPush(b1PushCallback, &b1);
  b2.attachPush(b2PushCallback, &b2);
  b3.attachPush(b3PushCallback, &b3);
  b4.attachPush(b4PushCallback, &b4);
  b5.attachPush(b5PushCallback, &b5);

  delay(2000);
  //Matikan semua relay
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(30, HIGH);
  delay(500);

}

void loop() {
  //When push/pop event occured execute component in touch event list
  nexLoop(nex_listen_list);
}
