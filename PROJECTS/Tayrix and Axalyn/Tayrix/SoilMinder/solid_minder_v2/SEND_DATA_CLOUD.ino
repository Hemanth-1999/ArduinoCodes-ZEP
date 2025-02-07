void sendDataMQTT() 
{
   ES_AT = 30;
   String str="";
   int E_AT = (int)(ES_AT * 10);//temp from bme sensor. by multiplying by 10 the point moves to rightside[25.789 to 257.89]
   ES_RH = 30;
   int E_RH = (int)(ES_RH * 10);//hum from bme sensor
   HI = 30;
   int E_HI = (int)(HI * 10);//heat index from bme sensor
   ETo = 30;
   int E_ETo = (int)(ETo * 10);//ENVIRONMENTAL TEMPERATURE CALCULATION from ETcalcul()function.
   ES_BP = 30;
   int E_BP = (int)(ES_BP * 10);//pressure from bme sensor.
   dewpt = 30;
   int E_DP = (int)(dewpt * 10);//DewPoint from bme sensor
   uvIntensity = 30;
   int UV_intensity = (int)(uvIntensity*10);
   sl = 30;
   int slp = (int)(sl*10);//sea level pressure from bme.
   
   time_t epochTime = timeClient.getEpochTime();
   StaticJsonDocument<1024> doc;
   doc["dn"] = "sm/b";
   doc["d-id"] = chipId;
   doc["zone_id"] = 1;
   doc["ts"] = epochTime;
   JsonObject metric = doc.createNestedObject("metrics");
   metric["at"]=(float)(E_AT)/10;
   metric["rh"]=(float)(E_RH)/10;
   metric["hi"]=(float)(E_HI)/10;
   metric["et"]=(float)(E_ETo)/10;
   metric["bp"]=(float)(E_BP)/10;
   metric["sr"]=(float)(UV_intensity)/10;
   metric["dp"]=(float)(E_DP)/10;
   metric["sl"]=(float)(slp)/10;
   metric["ec"]=(float)(NPK_ec)/10;
   metric["ph"]=(float)(NPK_ph)/10;
   metric["st"]=(float)(NPK_temp)/10;
   metric["sm"]=(float)(NPK_moist)/10;
   metric["n"]=(float)(NPK_nitr)/10;
   metric["p"]=(float)(NPK_phosp)/10;
   metric["k"]=(float)(NPK_potass)/10;
   
   serializeJson(doc, str);
   #ifdef VIA_GSM
    Gsm_Publish("telemetry",str);
           
   #else
      client.publish("telemetry",str.c_str());
   #endif
   serializeJson(doc, Serial);
   Serial.println();
   Serial.println("-----------------------");   
}
