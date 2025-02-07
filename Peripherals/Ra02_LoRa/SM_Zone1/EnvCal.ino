/*
  Environment_Calculations.ino

  This code shows how to record data from the BME280 environmental sensor
  and perform various calculations.

  Connecting the BME280 Sensor:
  Sensor              ->  Board
  -----------------------------
  Vin (Voltage In)    ->  3.3V
  Gnd (Ground)        ->  Gnd
  SDA (Serial Data)   ->  21 on Esp32
  SCK (Serial Clock)  ->  22 on Esp32

*/

void printBME280Data(Stream* client)
{

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  ES_AT = temp - 3.2;

  ES_RH = hum;

  ES_BP = pres;

  Serial.print("Temp: ");
  Serial.print(ES_AT);
  Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\tHumidity: ");
  Serial.print(ES_RH);
  Serial.print("% RH");
  Serial.print("\tPressure: ");
  Serial.print(ES_BP);
  Serial.print(String(presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

  float dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);

  /// To get correct seaLevel pressure (QNH, QFF)
  ///    the altitude value should be independent on measured pressure.
  /// It is necessary to use fixed altitude point e.g. the altitude of barometer read in a map
  float seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(barometerAltitude, temp, pres, envAltUnit, envTempUnit);

  float absHum = EnvironmentCalculations::AbsoluteHumidity(temp, hum, envTempUnit);
  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);

  HI = heatIndex;

  dewpt = dewPoint;

  sl = seaLevel;

  Serial.print((envAltUnit == EnvironmentCalculations::AltitudeUnit_Meters ? "m" : "ft"));
  Serial.print("\tDew point: ");
  Serial.print(dewPoint);
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\tEquivalent Sea Level Pressure: ");
  Serial.print(seaLevel);
  Serial.print(String( presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

  Serial.print("\tHeat Index: ");
  Serial.print(heatIndex);
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));

  Serial.print("\tAbsolute Humidity: ");
  Serial.println(absHum);

  delay(1000);
}

void ETcalcul()
{
  ETo = (0.4 * ES_AT + 0.3 * ES_RH + 0.2 * VPDcal() + 0.1 * solarRadiation);

  Serial.print("ETo: ");
  Serial.print(ETo);
  Serial.println(" mm");
  Serial.print("CO2: ");
  Serial.println(co2);    // Reset CO2 data for this display cycle
}

float VPDcal()
{

  AirTemp = ES_AT;
  LeafTemp = ES_AT;
  RH = ES_RH;
  p1 = pow ( 10 , ((float)(7.5 * AirTemp) / (float)(237.3 + AirTemp)));
  p2 = pow ( 10 , ((float)(7.5 * LeafTemp) / (float)(237.3 + LeafTemp)));
  VPD = (((float)610.7 * (float) p1) / 1000) - (((float)610.7 * (float)p2) / 1000) * (((float)RH) / 100);
  return VPD;

}
