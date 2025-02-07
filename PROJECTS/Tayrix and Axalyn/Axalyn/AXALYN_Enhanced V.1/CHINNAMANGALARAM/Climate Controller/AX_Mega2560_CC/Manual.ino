//Saat tombol 1 ditekan,relay 1 menyala
void bt0PushCallback(void *ptr) {
  if (statusbt0 == false) {
    digitalWrite(A3, LOW);
    Serial.println("FAN1 ON");
    statusbt0 = true;
  }
  else {
    digitalWrite(A3, HIGH);
    Serial.println("FAN1 OFF");
    statusbt0 = false;
  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt1PushCallback(void *ptr) {
  if (statusbt1 == false) {
    digitalWrite(A2, LOW);
    Serial.println("FAN2 ON");
    statusbt1 = true;
  }
  else {
    digitalWrite(A2, HIGH);
    Serial.println("FAN2 OFF");
    statusbt1 = false;
  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt2PushCallback(void *ptr) {
  if (statusbt2 == false) {
    digitalWrite(30, LOW);
    Serial.println("FAN3 ON");
    statusbt2 = true;
  }
  else {
    digitalWrite(30, HIGH);
    Serial.println("FAN3 OFF");
    statusbt2 = false;
  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void bt3PushCallback(void *ptr) {
  if (statusbt3 == false) {
    digitalWrite(28, LOW);
    Serial.println("FAN4 ON");
    statusbt3 = true;
  }
  else {
    digitalWrite(28, HIGH);
    Serial.println("FAN4 OFF");
    statusbt3 = false;
  }

}

void bt4PushCallback(void *ptr) {
  if (statusbt4 == false) {
    digitalWrite(8, LOW);
    Serial.println("FAN5 ON");
    statusbt4 = true;
  }
  else {
    digitalWrite(8, HIGH);
    Serial.println("FAN5 OFF");
    statusbt4 = false;
  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt5PushCallback(void *ptr) {
  if (statusbt5 == false) {
    digitalWrite(9, LOW);
    Serial.println("FAN6 ON");
    statusbt5 = true;
  }
  else {
    digitalWrite(9, HIGH);
    Serial.println("FAN6 OFF");
    statusbt5 = false;
  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt6PushCallback(void *ptr) {
  if (statusbt6 == false) {
    digitalWrite(10, LOW);
    Serial.println("FAN7 ON");
    statusbt6 = true;
  }
  else {
    digitalWrite(10, HIGH);
    Serial.println("FAN7 OFF");
    statusbt6 = false;
  }

}

//Saat tombol 4 ditekan,relay 4 menyala
void bt7PushCallback(void *ptr) {
  if (statusbt7 == false) {
    digitalWrite(11, LOW);
    Serial.println("FAN8 ON");
    statusbt7 = true;
  }
  else {
    digitalWrite(11, HIGH);
    Serial.println("FAN8 OFF");
    statusbt7 = false;
  }

}

void bt8PushCallback(void *ptr) {
  if (statusbt8 == false) {
    digitalWrite(A4, LOW);
    Serial.println("COOLING PAD ON");
    statusbt8 = true;
  }
  else {
    digitalWrite(A4, HIGH);
    Serial.println("COOLING PAD OFF");
    statusbt8 = false;
  }

}

//Saat tombol 2 ditekan,relay 2 menyala
void bt9PushCallback(void *ptr) {
  if (statusbt9 == false) {
    digitalWrite(9, LOW);
    Serial.println("FOGGERPUMP ON");
    statusbt1 = true;
  }
  else {
    digitalWrite(10, HIGH);
    Serial.println("FOGGERPUMP OFF");
    statusbt9 = false;
  }

}

//Saat tombol 3 ditekan,relay 3 menyala
void bt10PushCallback(void *ptr) {
  if (statusbt10 == false) {
    digitalWrite(A5, LOW);
    Serial.println("FOGGERZONE ON");
    statusbt10 = true;
  }
  else {
    digitalWrite(A5, HIGH);
    Serial.println("FOGGERZONE OFF");
    statusbt10 = false;
  }

}

void bt11PushCallback(void *ptr) {
  if (statusbt11 == false) {
    digitalWrite(A7, LOW);
    Serial.println("CO2 GENERATOR ON");
    statusbt11 = true;
  }
  else {
    digitalWrite(A7, HIGH);
    Serial.println("CO2 GENERATOR OFF");
    statusbt11 = false;
  }

}
