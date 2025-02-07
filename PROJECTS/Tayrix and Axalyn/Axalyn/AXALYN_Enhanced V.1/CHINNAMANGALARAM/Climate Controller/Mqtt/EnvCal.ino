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

  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  // Determine signal strength level based on RSSI value
  if (WiFi.RSSI() > -50) {
    u8x8.drawString(0, 0, "WIFI:4");  // Strong signal
  } else if (WiFi.RSSI() > -60) {
    u8x8.drawString(0, 0, "WIFI:3");  // Good signal
  } else if (WiFi.RSSI() > -70) {
    u8x8.drawString(0, 0, "WIFI:2");  // Medium signal
  } else if (WiFi.RSSI() > -80) {
    u8x8.drawString(0, 0, "WIFI:1");  // Weak signal
  } else {
    u8x8.drawString(0, 0, "WIFI:0");  // Very weak signal or disconnected
  }

  if (battery >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battery >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battery >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battery >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("AT:");
  u8x8.setCursor(5, 4);
  u8x8.print(ES_AT, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("RH:");
  u8x8.setCursor(5, 5);
  u8x8.print(ES_RH, 1);
  u8x8.print("%");

  u8x8.setCursor(0, 6);
  u8x8.print("BP:");
  u8x8.setCursor(5, 6);
  u8x8.print(ES_BP, 1);
  u8x8.print(" hPa");

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

}

void BMEData() {
  printBME280Data(&Serial);
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  // Determine signal strength level based on RSSI value
  if (WiFi.RSSI() > -50) {
    u8x8.drawString(0, 0, "WIFI:4");  // Strong signal
  } else if (WiFi.RSSI() > -60) {
    u8x8.drawString(0, 0, "WIFI:3");  // Good signal
  } else if (WiFi.RSSI() > -70) {
    u8x8.drawString(0, 0, "WIFI:2");  // Medium signal
  } else if (WiFi.RSSI() > -80) {
    u8x8.drawString(0, 0, "WIFI:1");  // Weak signal
  } else {
    u8x8.drawString(0, 0, "WIFI:0");  // Very weak signal or disconnected
  }

  if (battery >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battery >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battery >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battery >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("HI:");
  u8x8.setCursor(5, 4);
  u8x8.print(HI, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("Dep:");
  u8x8.setCursor(5, 5);
  u8x8.print(dewpt, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 6);
  u8x8.print("Sl:");
  u8x8.setCursor(5, 6);
  u8x8.print(sl, 1);
  u8x8.print(" hPa");
}

float calculateSolarAltitude(int currentHour, int currentMinute, int dayOfYear) {
  float declination = 23.44 * sin((360.0 / 365.0) * (dayOfYear - 81) * (M_PI / 180.0));
  float solarTime = (currentHour + (currentMinute / 60.0)) - 12.0;
  float hourAngle = (solarTime * 15.0) - (longitude / 15.0);
  float latitudeRad = latitude * (M_PI / 180.0);
  float declinationRad = declination * (M_PI / 180.0);
  float hourAngleRad = hourAngle * (M_PI / 180.0);

  float altitude = asin(sin(declinationRad) * sin(latitudeRad) +
                        cos(declinationRad) * cos(latitudeRad) * cos(hourAngleRad)) * (180.0 / M_PI);

  return altitude;
}

float estimateSolarRadiation(float altitudeAngle) {

  solarRadiation = maxRadiation * pow(sin(altitudeAngle * (M_PI / 180.0)), 2); // Adjust calculation

  return solarRadiation; // Return 0 if the altitude angle is <= 0
}

float calculateDailyAverageSolarRadiation() {
  float totalRadiation = 0.0;
  int minutesPerInterval = 15; // Calculate every 15 minutes
  int intervalsPerDay = (24 * 60) / minutesPerInterval; // Total number of intervals in a day
  int dayOfYear = timeClient.getDay() + 1; // Get the actual day of the year from the NTP client

  for (int hour = 0; hour < 24; hour++) {
    for (int minute = 0; minute < 60; minute += minutesPerInterval) {
      altitudeAngle = calculateSolarAltitude(hour, minute, dayOfYear); // Update global variable
      solarRadiation = estimateSolarRadiation(altitudeAngle); // Update global variable
      totalRadiation += solarRadiation * (minutesPerInterval / 60.0); // Convert to hours
    }
  }

  float averageRadiation = totalRadiation / 24.0; // Average radiation per hour

  return averageRadiation;
}

void ETcalcul()
{
  ETo = (0.4 * ES_AT + 0.3 * ES_RH + 0.2 * VPDcal() + 0.1 * solarRadiation);

  Serial.print("ETo: ");
  Serial.print(ETo);
  Serial.println(" mm");

  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  // Determine signal strength level based on RSSI value
  if (WiFi.RSSI() > -50) {
    u8x8.drawString(0, 0, "WIFI:4");  // Strong signal
  } else if (WiFi.RSSI() > -60) {
    u8x8.drawString(0, 0, "WIFI:3");  // Good signal
  } else if (WiFi.RSSI() > -70) {
    u8x8.drawString(0, 0, "WIFI:2");  // Medium signal
  } else if (WiFi.RSSI() > -80) {
    u8x8.drawString(0, 0, "WIFI:1");  // Weak signal
  } else {
    u8x8.drawString(0, 0, "WIFI:0");  // Very weak signal or disconnected
  }

  if (battery >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battery >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battery >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battery >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("Co2:");
  u8x8.setCursor(5, 4);
  u8x8.print(co2);
  u8x8.print(" ppm");

  u8x8.setCursor(0, 5);
  u8x8.print("ET:");
  u8x8.setCursor(5, 5);
  u8x8.print(ETo, 1);
  u8x8.print(" mm");

  u8x8.setCursor(0, 6);
  u8x8.print("SR:");
  u8x8.setCursor(5, 6);
  u8x8.print(solarRadiation, 0);
  u8x8.print(" Wh/m²");


  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  // Determine signal strength level based on RSSI value
  if (WiFi.RSSI() > -50) {
    u8x8.drawString(0, 0, "WIFI:4");  // Strong signal
  } else if (WiFi.RSSI() > -60) {
    u8x8.drawString(0, 0, "WIFI:3");  // Good signal
  } else if (WiFi.RSSI() > -70) {
    u8x8.drawString(0, 0, "WIFI:2");  // Medium signal
  } else if (WiFi.RSSI() > -80) {
    u8x8.drawString(0, 0, "WIFI:1");  // Weak signal
  } else {
    u8x8.drawString(0, 0, "WIFI:0");  // Very weak signal or disconnected
  }

  if (battery >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battery >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battery >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battery >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("VPD:");
  u8x8.setCursor(5, 4);
  u8x8.print(VPDcal(), 1);
  u8x8.print(" kPa");

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
