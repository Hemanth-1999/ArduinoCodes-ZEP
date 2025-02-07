void message_from_outside_node(byte * payload, unsigned int length)
{
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  if(JsonObject metrics= doc["metrics"])
  {
    float AT = metrics["at_in"];//temp from bme sensor.
    float RH = metrics["rh_in"];//humidity from bme sensor
    float VPD = metrics["vpd_in"];//this will return float value of vpd
    float HI = metrics["hi_in"];//heat index from bme sensor
    float CO2 = metrics["co2_in"];//co2 value
    float ET = metrics["et_in"];//ENVIRONMENTAL TEMPERATURE CALCULATION from ETcalcul()function.
    float BP = metrics["bp_in"];//pressure
    float UV = metrics["sr_in"];//uv intensity
    float DP = metrics["dp_in"];//drew point
    float SL = metrics["sl_in"];//sea level pressure from bme.
    //enter the values to ring buffer of running average
    RAT.addValue(AT);
    RAH.addValue(RH);
    RAHI.addValue(HI);
    RAVPD.addValue(VPD);
    RACO2.addValue(CO2);
    RAET.addValue(ET);
    RABP.addValue(BP);
    RASOL.addValue(SL);
    RADEW.addValue(DP);
    RAUV.addValue(UV);
    //output the average of values inside ring buffer
     O_AT=RAT.getAverage();
     O_HUM=RAH.getAverage();
     O_HI= RAHI.getAverage();
     O_VPD=RAVPD.getAverage();
     O_CO2=RACO2.getAverage();
     O_ET= RAET.getAverage();
     O_BP= RABP.getAverage();
     O_SL= RASOL.getAverage();
     O_DP=RADEW.getAverage();
     O_UV=RAUV.getAverage();
  }
}
