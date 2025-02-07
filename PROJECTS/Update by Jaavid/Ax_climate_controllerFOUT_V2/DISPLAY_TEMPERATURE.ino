/*
THIS FUNCTION READS TEMPERATURE PAGE CREDENTIAL.
THE CREDENTIALS OF TEMPERATURE PAGE ARE 1)INDOOR TEMPERATURE 2)OUTDOOR TEMPERATUE.
AFTER READING IT WILL STORE IN LOCAL VARIABLES.
IT WILL STORE INDOOR TEMPERATURE IN GLOBAL VARIABLE SPtemp.
IT WILL STORE INDOOR TEMPERATURE IN GLOBAL VARIABLE ODtemp.
*/
void readtempvalues()
{
  float In_TempAutoSetpoint = vpdtemp_I();
  SPtemp = In_TempAutoSetpoint ;
  
  float Out_TempAutoSetpoint = vpdtemp_O();
  ODtemp = Out_TempAutoSetpoint;
  
  feedBack = "tempat";
}
//THIS FUN WILL EXTRACT ONLY INDOOR TEMPERATURE FROM DISPLAY AND RETURN IT
float vpdtemp_I() 
{
  char *ui = strtok(buff, "o");
  str = ui;
  int vale = str.indexOf("i") + 3;
  str.remove(0, vale);
  float i_temp = float(str.toInt()) / 10;
  return (i_temp);

}
//THIS FUN WILL EXTRACT ONLY OUTDOOR TEMPERATURE FROM DISPLAY AND RETURN IT
float vpdtemp_O() 
{
  //tempit10ot10
  Serial.println(dfd);
  int vale = dfd.indexOf("o") + 3;
  dfd.remove(0, vale);
  Serial.println(dfd);
  float o_temp = float(dfd.toInt()) / 10;
  return (o_temp);

}

//******************************************************AUTO TEMPERATURE MODE**********************

/*
THIS FUNCTION IS CALLED WHEN USER PRESS ON BUTTON IN TEMPERATURE.
THIS FUNCTION WILL SET THE TemperatureAutoModeOn , temp_func TO TRUE IF BUTTON IS ON.
THIS FUNCTION WILL SET THE TemperatureAutoModeOn , temp_func TO FALSE IF BUTTON IS OFF.
*/
void autotenpmode(bool stat)
{
  if(stat)//auto temperatyre mode is on
  {
    feedBack = "tmpon";
    TemperatureAutoModeOn = true;
    Serial.println("TEMP  ON");
    temp_func = true;
  }
  else//auto temperature mode is off
  {
    feedBack = "tmpoff";
    TemperatureAutoModeOn = false;
    alloff();//this will off all relays
    Serial.println("TEMP  OFF");
    temp_func = false;
  }
}
