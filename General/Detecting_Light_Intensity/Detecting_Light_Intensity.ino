#include <Wire.h>
#include "Adafruit_TCS34725.h"
/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

#include "TSL2561.h"
#include <ML8511.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Hardware pin definitions
//int UVOUT = A0; //Output from the sensor
//int REF_3V3 = A1; //3.3V power on the Arduino board
# define UVOUT 15

# define REF_3V3  4


/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

TSL2561 tsl(TSL2561_ADDR_FLOAT);

void setup(void){
  Serial.begin(9600);

  if (tcs.begin())
  {
    Serial.println("Found sensor");
  } 
  
  else
  
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!

  

    if (tsl.begin())
    {
      Serial.println("Found sensor");
    }
     
    else
    
    {
      Serial.println("No sensor?");
      while (1);
    }

    // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    //tsl.setGain(TSL2561_GAIN_0X);         // set no gain (for bright situtations)
    tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)

    // Changing the integration time gives you a longer time over which to sense light
    // longer timelines are slower, but are good in very low light situtations!
    tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
    //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
    //tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)

    // Now we're ready to get readings!

  

   {
  // initialize the LCD
  lcd.begin(21,22);  // sda=21, scl=22
  
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  }
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop(void){
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  delay(100); delay(100);   delay(100);

  // Simple data read example. Just read the infrared, fullspecrtrum diode
  // or 'visible' (difference between the two) channels.
  // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
  //uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);

  Serial.print(x, DEC);
  Serial.print("\t");
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
//  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
//  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");

  
    int uvLevel = analogRead(UVOUT);
    int refLevel = analogRead(REF_3V3);

    //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
    float outputVoltage = 3.3 / refLevel * uvLevel;

    float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
 //Convert the voltage to a UV intensity level

    Serial.print("output: ");
    Serial.print(refLevel);

    Serial.print("ML8511 output: ");
    Serial.print(uvLevel);

    Serial.print(" / ML8511 voltage: ");
    Serial.print(outputVoltage);

    Serial.print(" / UV Intensity (mW/cm^2): ");
    Serial.print(uvIntensity);
    lcd.clear();
    lcd.print("UV Ray Intensity");
    lcd.setCursor(0, 1);
    lcd.print(uvIntensity);
    lcd.print(" mW/cm^2");

    Serial.println();

    delay(200);
  


}
