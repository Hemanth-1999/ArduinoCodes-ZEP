#include "DFRobot_PH.h"
#include <EEPROM.h>

#define PH_PIN A0
float voltagePH, phValue, temperature = 25;

DFRobot_PH ph;

void setup()
{
  Serial.begin(115200);
  ph.begin();
}

void loop()
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) { // Time interval: 1s
    timepoint = millis();

    // Read temperature
    temperature = readTemperature(); // Implement this function correctly

    // Read raw ADC value and calculate voltage
    int rawValue = analogRead(PH_PIN);
    voltagePH = rawValue / 1024.0 * 5; // Assuming a 5V reference

    // Debugging outputs
    Serial.print("Raw ADC Value: ");
    Serial.println(rawValue);
    Serial.print("Voltage PH: ");
    Serial.println(voltagePH, 4);

    ph.calibration(voltagePH, temperature);

    // Attempt to read pH value

    phValue = ph.readPH(voltagePH, temperature);

    if (isnan(phValue)) {
      Serial.print("Error calculating pH. Voltage: ");
      Serial.println(voltagePH, 4);
      Serial.print("Temperature: ");
      Serial.println(temperature, 2);
    } else {
      Serial.print("pH: ");
      Serial.println(phValue, 2);
    }
  }
}

// Implement your temperature reading function
float readTemperature()
{
  // Replace this placeholder with actual temperature sensor reading logic
  return 25.0; // Placeholder for testing
}
