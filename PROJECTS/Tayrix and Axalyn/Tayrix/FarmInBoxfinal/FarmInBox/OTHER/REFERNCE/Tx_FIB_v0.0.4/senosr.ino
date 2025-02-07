//===========================================
// Water Pressure Senosr
//===========================================
float WaterPressureSenosr(int SensorPin) {
  int pin = SensorPin;
  //Connect sensor to Analog 0
  V = analogRead(pin) * 5.00 / 1024;     //Sensor output voltage
  Pressure = (V - OffSet) * 250;             //Calculate water pressure

  Serial.print("Voltage:");
  Serial.print(V, 3);
  Serial.println("V");

  Serial.print(" Pressure:");
  Serial.print(Pressure, 1);
  Serial.println(" KPa");
  Serial.println();

  return Pressure;

}
//===========================================
// WaterFlow Sensor
//===========================================

int InFlowRate() {
  Meter1->tick(period);
  Serial.println("Meter 1 currently " + String(Meter1->getCurrentFlowrate()) + " l/min, " + String(Meter1->getTotalVolume())+ " l total.");
   InFlowRateMin = Meter1->getCurrentFlowrate();
   InFlowTotalVolume = Meter1->getTotalVolume();

  return InFlowTotalVolume; 
}



int OutFlowRate() {
   Meter2->tick(period);
   Serial.println("Meter 2 currently " + String(Meter2->getCurrentFlowrate()) + " l/min, " + String(Meter2->getTotalVolume())+ " l total.");
   int OutFlowRateMin = Meter2->getCurrentFlowrate();
   int OutFlowTotalVolume =Meter2->getTotalVolume();
   return OutFlowTotalVolume; 
}

//===========================================
// Reading Sensor Data
//===========================================
void ReadingSensorData() {

  float MainInPressure = WaterPressureSenosr(MainInPressure_Pin);
  Serial.print(" Pressure:");
  Serial.print(MainInPressure, 1);
  Serial.println(" KPa");
  Serial.println();

}
