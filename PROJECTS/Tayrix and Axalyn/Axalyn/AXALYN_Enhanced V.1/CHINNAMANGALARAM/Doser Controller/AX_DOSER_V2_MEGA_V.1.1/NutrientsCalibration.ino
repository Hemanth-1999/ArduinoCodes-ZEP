/************************************  calibration    **********************************/
void Calibration() {
  char cmd[10];
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {                      //time interval: 1s
    timepoint = millis();
    //temperature = readTemperature();                   // read your temperature sensor to execute temperature compensation
    voltagePH = analogRead(PH_PIN) / 1024.0 * 5000;      // read the ph voltage
    phValue    = ph.readPH(voltagePH, temperature);      // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.print(phValue, 2);
    voltageEC = analogRead(EC_PIN) / 1024.0 * 5000;
    ecValue    = ec.readEC(voltageEC, temperature);      // convert voltage to EC with temperature compensation
    Serial.print(", EC:");
    Serial.print(ecValue, 2);
    Serial.println("ms/cm");
    Serial.println();
  }
  if (readSerial(cmd)) {
    toUpperCase(cmd);
    if (strstr(cmd, "PH")) {
      Serial.println("Entering pH calibration");
      ph.calibration(voltagePH, temperature, cmd);     //PH calibration process by Serail CMD
    }
    if (strstr(cmd, "EC")) {
      ec.calibration(voltageEC, temperature, cmd);     //EC calibration process by Serail CMD
    }
  }
}

float getNutrientTDS() {
  float tdsValue = ecValue * 500;
  tdsValue = roundDecimalPoint(tdsValue, 2);

  nutrientTDS = tdsValue ;
  unitConduct = nutrientTDS;
  return tdsValue;
}

float getNutrientCF() {
  float cfValue = ecValue * 10;
  nutrientCF = cfValue;
  return cfValue;
}

float getNutrientDO() {
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
  float DO = (readDO(ADC_Voltage, Temperaturet));
  float DOvalue = DO;
  return DOvalue;
}

void getNutrientTL()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds (5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceincm = duration * 0.034 / 2;
  distanceinInch = distanceincm / 2.54;
  volume = pi * 16 * distanceincm; //change based on the volume for the tank
  waterHeight = tankHeight - distanceinInch;

  //  waterQuantity = totalVolume - volume - offset;
  waterconlevel = tankHeight - waterHeight;

  int Wtemp;
  int tankSetHysteris = 10; //10Inches
  int TankHysteris = (tanksetpoint - tankSetHysteris);
  if (distanceinInch < tanksetpoint && Wtemp == 0)
  {
    digitalWrite(RORELAYR, HIGH);
    Serial.println("Water PUMP OFF");

    Tankfull == false;

    Wtemp = 1;
  }

  else if (distanceinInch < tanksetpoint && Wtemp == 1)
  {
    digitalWrite(RORELAYR, HIGH);
    Serial.println("Water PUMP OFF");
    Tankfull == false;
  }
  else if (distanceinInch < TankHysteris)
  {
    digitalWrite(RORELAYR, LOW);
    Serial.println("Water PUMP ON");
    Tankfull == false;
    Wtemp = 0;
  }
  else if (distanceinInch < tankHeight)
  {

    Serial.println("Water PUMP ON");
    Tankfull == true;
    Wtemp = 0;
  }
  return distance;

}
