
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
    
  float refanalog = analogRead(pinToRead);
  float DCvalue = mapFloat( refanalog, 0, 1023, 0, 5);

  runningValue /= numberOfReadings;
  return (runningValue);
}
void UVA() 
{

  uvLevel = averageAnalogRead(UVOUT);
  refLevel = averageAnalogRead(REF_3V3);


  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  outputvoltage = 3.3 / refLevel * uvLevel;
  //float dcvoltage=mapFloat(refLevel,0,1023,0,5);

  uvIntensity = mapFloat( outputvoltage, 0.0, 31.0, 0.0, 99.0) * 10; //Convert the voltage to a UV intensity level
  //float uvIntensity = mapFloat(outputVoltage,0.99, 2.8, 0.0, 15.0)* 10;
  //uvIntensitynm =uvIntensity*10;


}
