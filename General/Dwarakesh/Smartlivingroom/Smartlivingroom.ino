#include "ACS712.h"
ACS712 sensor(ACS712_30A, A0);
#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <MQ135.h>
#define PIN_MQ135 A0

MQ135 mq135_sensor(PIN_MQ135);

int sensorPin = 2;
volatile long pulse;
unsigned long lastTime;

float volume;

float temperature = 21.0; // Assume current temperature
float humidity = 25.0; // Assume current humidity

// Assumed environmental values:
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position


BME280I2C::Settings settings(
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::OSR_X1,
  BME280::Mode_Forced,
  BME280::StandbyTime_1000ms,
  BME280::Filter_16,
  BME280::SpiEnable_False,
  BME280I2C::I2CAddr_0x76
);

BME280I2C bme(settings);

//////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);

  while (!Serial) {} // Wait

  Wire.begin();

  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch (bme.chipModel())
  {
    case BME280::ChipModel_BME280:
      Serial.println("Found BME280 sensor! Success.");
      break;
    case BME280::ChipModel_BMP280:
      Serial.println("Found BMP280 sensor! No Humidity available.");
      break;
    default:
      Serial.println("Found UNKNOWN sensor! Error!");
  }

}

//////////////////////////////////////////////////////////////////

void printBME280Data
(
  Stream* client
)
{
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  client->print("Temp: ");
  client->print(temp);
  client->print("°" + String(tempUnit == BME280::TempUnit_Celsius ? "C" : "F"));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->print(String(presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

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

  client->print("\t\tAltitude: ");
  client->print(altitude);
  client->print((envAltUnit == EnvironmentCalculations::AltitudeUnit_Meters ? "m" : "ft"));
  client->print("\t\tDew point: ");
  client->print(dewPoint);
  client->print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));
  client->print("\t\tEquivalent Sea Level Pressure: ");
  client->print(seaLevel);
  client->print(String( presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

  client->print("\t\tHeat Index: ");
  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);
  client->print(heatIndex);
  client->print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));

  client->print("\t\tAbsolute Humidity: ");
  client->println(absHum);

  Serial.print(" Assumed outdoor temperature: "); Serial.print(outdoorTemp);
  Serial.print("°C\n Assumed reduced sea level Pressure: "); Serial.print(referencePressure);
  Serial.print("hPa\n Assumed barometer altitude: "); Serial.print(barometerAltitude);
  Serial.println("m\n***************************************");

  delay(1000);
}

void Airquality()
{
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float AirQuality = mq135_sensor.getPPM();
  float CorrectedAirQuality = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.print(" MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\n AirQuality: ");
  Serial.print(AirQuality);
  Serial.print("\t Corrected AirQuality: ");
  Serial.print(CorrectedAirQuality);
  Serial.println("ppm");
  Serial.println("\n***************************************");
}

void MeasureCurrent()
{
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  int zero = sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);
  float I = sensor.getCurrentDC();
  
  // Send it to serial
  Serial.println(String("I = ") + I + " A");
}

//////////////////////////////////////////////////////////////////

void loop()
{
  printBME280Data(&Serial);
  delay(500);
  Airquality();
  delay(500);
  MeasureCurrent();
  delay(500);
}
