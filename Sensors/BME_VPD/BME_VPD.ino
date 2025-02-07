#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // Create an instance of the BME280 sensor

void setup() {
  Serial.begin(115200);

  // Initialize default I2C bus for BME280
  Wire.begin(4, 5); // SDA, SCL

  if (!bme.begin(0x76)) {  // Change the address if necessary (0x76 or 0x77)
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  float temperature = bme.readTemperature();   // Read temperature in °C
  float humidity = bme.readHumidity();         // Read humidity in %

  float saturationVaporPressure = calculateSaturationVaporPressure(temperature);
  float actualVaporPressure = calculateActualVaporPressure(humidity, saturationVaporPressure);
  float vpd = saturationVaporPressure - actualVaporPressure;  // Calculate VPD in kPa

  float eto = calculateEvapotranspiration(temperature, saturationVaporPressure, actualVaporPressure, vpd);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, VPD: ");
  Serial.print(vpd);
  Serial.print(" kPa, ET: ");
  Serial.print(eto);
  Serial.println(" mm/day");

  delay(2000);  // Wait for 2 seconds before the next reading
}

float calculateSaturationVaporPressure(float temperature) {
  // Magnus formula for Saturation Vapor Pressure in kPa
  return 0.6108 * exp((17.27 * temperature) / (temperature + 237.3));
}

float calculateActualVaporPressure(float humidity, float saturationVaporPressure) {
  // Actual Vapor Pressure in kPa
  return (humidity / 100.0) * saturationVaporPressure;
}

float calculateEvapotranspiration(float temperature, float saturationVaporPressure,
                                  float actualVaporPressure, float vpd) {
  // Simplified Penman-Monteith equation for reference ET (mm/day)
  float gamma = 0.066; // Psychrometric constant (kPa/°C), approximate value at sea level
  float delta = (4098 * saturationVaporPressure) / pow((temperature + 237.3), 2);  // Slope of the saturation vapor pressure curve (kPa/°C)
  float netRadiation = 0.8;  // Net radiation (MJ/m²/day), assume a typical value for simplicity
  float windSpeed = 2.0; // Assume a typical wind speed (m/s)

  // Simplified ET calculation:
  float eto = (0.408 * delta * (netRadiation) + (gamma * (900 / (temperature + 273)) * windSpeed * (saturationVaporPressure - actualVaporPressure))) /
              (delta + gamma * (1 + 0.34 * windSpeed));

  return eto; // Return ET in mm/day
}
