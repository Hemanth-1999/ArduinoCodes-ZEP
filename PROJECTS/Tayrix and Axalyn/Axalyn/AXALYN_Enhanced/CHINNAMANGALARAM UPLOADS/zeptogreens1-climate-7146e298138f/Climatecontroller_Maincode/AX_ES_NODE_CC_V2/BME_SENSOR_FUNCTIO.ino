void printBME280Data( Stream* client)
{
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);


  ES_AT = temp;
  tempdec = (temp, 2);

  ES_RH = hum;

  ES_BP = pres;

  float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
  float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
  float barometerAltitude = 1650.3;  // meters ... map readings + barometer position
  
  EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
  EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;

  /// To get correct local altitude/height (QNE) the reference Pressure
  ///    should be taken from meteorologic messages (QNH or QFF)
  float altitude = EnvironmentCalculations::Altitude(pres, envAltUnit, referencePressure, outdoorTemp, envTempUnit);

  float dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);

  /// To get correct seaLevel pressure (QNH, QFF)
  ///    the altitude value should be independent on measured pressure.
  /// It is necessary to use fixed altitude point e.g. the altitude of barometer read in a map
  float seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(barometerAltitude, temp, pres, envAltUnit, envTempUnit);

  float absHum = EnvironmentCalculations::AbsoluteHumidity(temp, hum, envTempUnit);


  alti = altitude;

  dewpt = dewPoint;

  sl = (int)seaLevel;

  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);

  HI = heatIndex;

}
