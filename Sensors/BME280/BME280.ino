/*
  Environment_Calculations.ino

  This code shows how to record data from the BME280 environmental sensor
  and perform various calculations.

  GNU General Public License

  Written: Dec 30 2015.
  Last Updated: Oct 07 2017.

  Connecting the BME280 Sensor:
  Sensor              ->  Board
  -----------------------------
  Vin (Voltage In)    ->  3.3V
  Gnd (Ground)        ->  Gnd
  SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
  SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

*/

#include <EnvironmentCalculations.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <U8x8lib.h>

#define SERIAL_BAUD 115200

// OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// Assumed environmental values:
float referencePressure = 1018.6;  // hPa local QFF (official meteor-station reading)
float outdoorTemp = 4.7;           // °C  measured local outdoor temp.
float barometerAltitude = 1650.3;  // meters ... map readings + barometer position

float temp(NAN), hum(NAN), pres(NAN);

void OLED();
void page1();

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
  Serial.begin(SERIAL_BAUD);

  while (!Serial) {} // Wait

  // Initialize default I2C bus for BME280
  Wire.begin(); // SDA, SCL

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
  Serial.print("Assumed outdoor temperature: "); Serial.print(outdoorTemp);
  Serial.print("°C\nAssumed reduced sea level Pressure: "); Serial.print(referencePressure);
  Serial.print("hPa\nAssumed barometer altitude: "); Serial.print(barometerAltitude);
  Serial.println("m\n***************************************");

  u8x8.begin();

}

//////////////////////////////////////////////////////////////////
void loop()
{
  OLED();
  delay(500);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
  Stream* client
)
{

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°" + String(tempUnit == BME280::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\t\tHumidity: ");
  Serial.print(hum);
  Serial.print("% RH");
  Serial.print("\t\tPressure: ");
  Serial.print(pres);
  Serial.print(String(presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

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

  Serial.print("\t\tAltitude: ");
  Serial.print(altitude);
  Serial.print((envAltUnit == EnvironmentCalculations::AltitudeUnit_Meters ? "m" : "ft"));
  Serial.print("\t\tDew point: ");
  Serial.print(dewPoint);
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));
  Serial.print("\t\tEquivalent Sea Level Pressure: ");
  Serial.print(seaLevel);
  Serial.print(String( presUnit == BME280::PresUnit_hPa ? "hPa" : "Pa")); // expected hPa and Pa only

  Serial.print("\t\tHeat Index: ");
  float heatIndex = EnvironmentCalculations::HeatIndex(temp, hum, envTempUnit);
  Serial.print(heatIndex);
  Serial.print("°" + String(envTempUnit == EnvironmentCalculations::TempUnit_Celsius ? "C" : "F"));

  Serial.print("\t\tAbsolute Humidity: ");
  Serial.println(absHum);

  delay(1000);
}
