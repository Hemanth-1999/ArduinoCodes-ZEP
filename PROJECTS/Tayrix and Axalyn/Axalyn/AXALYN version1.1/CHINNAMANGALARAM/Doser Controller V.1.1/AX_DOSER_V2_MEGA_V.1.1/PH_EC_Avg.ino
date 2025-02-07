// Read water temperature
float getNutrientTemperature() {
  // Read DALLAS
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  float ntValue = (sensors.getTempCByIndex(0)); // Why "byIndex"?
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire
  ntValue = roundDecimalPoint(ntValue, 1);
  Fahrenheit = sensors.toFahrenheit(ntValue);

  nutrientTemperature = ntValue;

  return ntValue;
}

//!******************************************************************************
//! Name: getNutrientPHavg()                               *
//! Description: For Doser Shedule                     *
//! Param : pH                                  *
//! Returns: void                                     *
//! Example:          *
//!******************************************************************************

float getNutrientPHavg() {

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static unsigned long timepoint = millis();
  static float voltagePHavrg, phtest;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(PH_PIN);
    if (pHArrayIndex == ArrayLenth)
      pHArrayIndex = 0;

    voltagePH = analogRead(PH_PIN) / 1024.0 * 5000;      // read the ph voltage
    phValue   = (ph.readPH(voltagePH, nutrientTemperature)) + Offset;

    voltagePHavrg = avergearray(pHArray, ArrayLenth) / 1024.0 * 5000;
    phValueavrg   = (ph.readPH(voltagePHavrg, nutrientTemperature)) + Offset;

    phtest = roundDecimalPoint(phValue, 2);

    int phRv = phtest;
    int phValueavrgO = roundDecimalPoint(phValueavrg, 2);

    samplingTime = millis();

    if (phRv == phValueavrgO) {
      Serial.println();
      Serial.println("PH Stable");
      pHAvg = true;
    }
    else {
      Serial.println();
      Serial.println("PH Not Stable");
      pHAvg = false;
    }
  }

  ph.calibration(voltagePHavrg, nutrientTemperature);

  pH = roundDecimalPoint(phValueavrg, 2);
  Serial.print("PH read SENSOR : ");
  Serial.println(phValueavrg);
  return phValueavrg;

}

float getNutrientECavg() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static unsigned long timepoint = millis();
  static float voltageECavrg, ECtest;

  if (millis() - samplingTime > samplingInterval) {
    ECArray[ECArrayIndex++] = analogRead(EC_PIN);
    if (ECArrayIndex == ArrayLenth)
      ECArrayIndex = 0;

    voltageEC = analogRead(EC_PIN) / 1024.0 * 5000;      // read the ph voltage
    ecValue   = ec.readEC(voltageEC, nutrientTemperature) + Offsetec;

    voltageECavrg = avergearrayec(ECArray, ArrayLenth) / 1024.0 * 5000;

    float EC_Value_avrg = ((float)(ec.readEC(voltageECavrg, nutrientTemperature) + Offsetec));

    String EC_Value_str = String(EC_Value_avrg, 2);

    ECValueavrg = EC_Value_str.toFloat();

    ECtest = roundDecimalPoint(ecValue, 2);
    int ECRv = ECtest;

    getNutrientTemperature();

    int ECValueavrgO = roundDecimalPoint(ECValueavrg, 2);
    samplingTime = millis();

    float cfValue = ecValue * 10;
    nutrientCF = cfValue;

    if (ECRv == ECValueavrgO) {
      Serial.println("EC Stable");
      ECAvg = true;
    }
    else {
      Serial.println("EC Not Stable");
      ECAvg = false;
    }
  }

  ec.calibration(voltageECavrg, nutrientTemperature);

  EC = roundDecimalPoint(ECValueavrg, 2);
  Serial.print("EC read SENSOR : ");
  Serial.println(ECValueavrg);
  Serial.println();
  return ECValueavrg;

}
