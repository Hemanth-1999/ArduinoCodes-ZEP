void Sensorrun() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert Pa to hPa
  float seaLevelPressure = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float vpd = calculateVPD(temperature, humidity);
  float dewPoint = calculateDewPoint(temperature, humidity);
  float evapotranspiration = calculateEvapotranspiration(temperature, humidity, vpd);
  AT = temperature;
  RH = humidity;
  BP = pressure;
  SL = seaLevelPressure;
  VPD = vpd;
  DP = dewPoint;
  ET = evapotranspiration;
  StaticJsonDocument<500> doc;
  JsonObject TXCC = doc.createNestedObject("txcc_es");
  TXCC["AT"] = AT;
  TXCC["RH"] = RH;
  TXCC["BP"] = BP;
  TXCC["SL"] = SL;
  TXCC["VPD"] = VPD;
  TXCC["DP"] = DP;
  TXCC["ET"] = ET;
   char buffer[1024];
  serializeJson(doc, buffer);
  client.publish("tayrix", buffer);
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("-----------------------");

}

// Function to calculate Vapor Pressure Deficit (VPD) in hPa
float calculateVPD(float temperature, float humidity) {
  float es = 6.112 * exp((17.67 * temperature) / (temperature + 243.5));
  float e = (humidity / 100.0) * es;
  float vpd = es - e;
  return vpd;
}

// Function to calculate Dew Point in °C
float calculateDewPoint(float temperature, float humidity) {
  float a = 17.271;
  float b = 237.7;
  float temp = (a * temperature) / (b + temperature) + log(humidity / 100.0);
  float dewPoint = (b * temp) / (a - temp);
  return dewPoint;
}

// Function to calculate Evapotranspiration in mm/day
float calculateEvapotranspiration(float temperature, float humidity, float vpd) {
  // Add your custom evapotranspiration calculation here (e.g., using Penman-Monteith equation)
  // This calculation depends on various factors like wind speed, solar radiation, crop type, etc.
  // For a simple example, you can use a constant value for demonstration purposes.
  // This example assumes that the evapotranspiration is directly proportional to VPD.
  const float evapotranspirationConstant = 0.1; // Adjust this constant for your application
  float evapotranspiration = evapotranspirationConstant * vpd;
  return evapotranspiration;
}
