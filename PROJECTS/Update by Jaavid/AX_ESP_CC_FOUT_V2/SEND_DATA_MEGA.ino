/*
THIS FUNCTION WILL SEND DATA TO ATMEGA VIA SERIAL.
FIRST NODE ESP WILL SEND DATA TO ESP8266 VIA MQTT.
AFTER THAT ESP8266 WILL READ IT AND SEND THAT DATA TO MEGA VIA SERIAL. 
FORMAT IS JSON.
*/
void SendDataToMega() 
{
  Serial.println("Publish mega:");
  StaticJsonDocument <1024>megadoc;
  megadoc["at_i"]=_AT;
  megadoc["rh_i"]=_HUM;
  megadoc["vpd_i"]=_VPD;
  megadoc["hi_i"]=_HI;
  megadoc["co2_i"]=_CO2;
  megadoc["et_i"]=_ET;
  megadoc["bp_i"]=_BP;
  megadoc["uv_i"]=_UV;
  megadoc["dp_i"]=_DP;
  megadoc["sl_i"]=_SL;
  megadoc["at_o"]=O_AT;
  megadoc["rh_o"]=O_HUM;
  megadoc["vpd_o"]=O_VPD;
  megadoc["hi_o"]=O_HI;
  megadoc["co2_o"]=O_CO2;
  megadoc["et_o"]=O_ET;
  megadoc["bp_o"]=O_BP;
  megadoc["uv_o"]=O_UV;
  megadoc["dp_o"]=O_DP;
  megadoc["sl_o"]=O_SL;
  serializeJson(megadoc, Serial);
  Serial.print("\n");
}
