void sendDataMQTT() 
{
   String str="";
   int E_AT = (int)(ES_AT * 10);//temp from bme sensor. by multiplying by 10 the point moves to rightside[25.789 to 257.89]
   int E_RH = (int)(ES_RH * 10);//hum from bme sensor
   int E_VPD = (int)(VPDcal() * 10);//this will return float value of vpd
   int E_HI = (int)(HI * 10);//heat index from bme sensor
   int E_ETo = (int)(ETo * 10);//ENVIRONMENTAL TEMPERATURE CALCULATION from ETcalcul()function.
   int E_BP = (int)(ES_BP * 10);//pressure from bme sensor.
   int E_DP = (int)(dewpt * 10);//DewPoint from bme sensor
   int UV_intensity=(int)(uvIntensity*10);
   int slp = (int)(sl*10);//sea level pressure from bme.
   int cco2=(int)(co2*10);
   time_t epochTime = timeClient.getEpochTime();
   StaticJsonDocument<500> doc;
   doc["dn"] = "es_node";
   doc["d-id"] = chipId;
   doc["zone_id"] = 1;
   doc["ts"]=epochTime;
   JsonObject metric = doc.createNestedObject("metrics");
   metric["at_in"]=(float)(E_AT) / 10;
   metric["rh_in"]=(float)(E_RH) / 10;
   metric["vpd_in"]=(float)(E_VPD) / 10;
   metric["hi_in"]=(float)(E_HI) / 10;
   metric["co2_in"]=(float)(cco2)/10;
   metric["et_in"]=(float)(E_ETo) / 10;
   metric["bp_in"]=(float)(E_BP) / 10;
   metric["sr_in"]=(float)(UV_intensity)/10;
   metric["dp_in"]=(float)(E_DP) / 10;
   metric["sl_in"]=(float)(slp)/10;

    serializeJson(doc, str);
    client.publish(sendtopic_ESMesh, str.c_str()); //nodes inside farm must publish to sendtopic_ESMesh only.
    client.publish("axalyn", str.c_str());
    serializeJson(doc, Serial);
}
