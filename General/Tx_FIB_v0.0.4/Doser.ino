/*
  if irrigation pump on doser start
  Dosing ph up
  doser A B C and Acid




*/


void Doser_auto(){
  
  
  
  
  }

void DoserFun() {

  if (Doser_pH_Enable == true) {
    if (DoserPh_Delay.justFinished()) {
      digitalWrite(Dose_pHUp_sol_pin, HIGH); // turn led off
      Serial.println("Dose_pHUp_sol_pin Off");
    }

  }
  if (Doser_A_Enable == true) {
    if (DoserA_Delay.justFinished()) {
      digitalWrite(Dose_A_sol_pin, HIGH); // turn led off
      Serial.println("Doser_A_Enable Off");
    }

  }
  if (Doser_B_Enable == true) {
    if (DoserB_Delay.justFinished()) {
      digitalWrite(Dose_B_sol_pin, HIGH); // turn led off
      Serial.println("Doser_B_Enable Off");
    }

  }
  if (Doser_C_Enable == true) {
    if (DoserC_Delay.justFinished()) {
      digitalWrite(Dose_C_sol_pin, HIGH); // turn led off
      Serial.println("Doser_C_Enable Off");
    }

  }
  if (Doser_Acid_Enable == true) {
        if (DoserAcid_Delay.justFinished()) {
        digitalWrite(Dose_Acid_sol_pin, HIGH); // turn led off
        Serial.println("Doser_Acid_Enable Off");
      }

  }

}
void Dose_pHUp_sol_ON() {

  if (digitalRead(BooosPump_pin) == Relay_ON)
  {
    digitalWrite(Dose_pHUp_sol_pin, Relay_ON);

  }
  else if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_pHUp_sol_pin, Relay_OFF);

  }
}

void Dose_pHUp_sol_OFF() {

  if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_pHUp_sol_pin, Relay_OFF);

  }
}
void Dose_A_sol_ON() {

  if (digitalRead(BooosPump_pin) == Relay_ON)
  {
    digitalWrite(Dose_A_sol_pin, Relay_ON);

  }
  else if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_A_sol_pin, Relay_OFF);

  }
}

void Dose_A_sol_OFF() {

  if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_A_sol_pin, Relay_OFF);

  }
}

void Dose_B_sol_ON() {

  if (digitalRead(BooosPump_pin) == Relay_ON)
  {
    digitalWrite(Dose_B_sol_pin, Relay_ON);

  }
  else if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_B_sol_pin, Relay_OFF);

  }
}

void Dose_B_sol_OFF() {

  if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_B_sol_pin, Relay_OFF);

  }
}

void Dose_C_sol_ON() {

  if (digitalRead(BooosPump_pin) == Relay_ON)
  {
    digitalWrite(Dose_C_sol_pin, Relay_ON);

  }
  else if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_C_sol_pin, Relay_OFF);

  }
}

void Dose_C_sol_OFF() {

  if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_C_sol_pin, Relay_OFF);

  }
}

void Dose_Acid_sol_ON() {

  if (digitalRead(BooosPump_pin) == Relay_ON)
  {
    digitalWrite(Dose_Acid_sol_pin, Relay_ON);

  }
  else if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_Acid_sol_pin, Relay_OFF);

  }
}

void Dose_Acid_sol_OFF() {

  if (digitalRead(BooosPump_pin) == Relay_OFF)
  {
    digitalWrite(Dose_Acid_sol_pin, Relay_OFF);

  }
}

void Dosing() {



}
