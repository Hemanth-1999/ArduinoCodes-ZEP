/*
ESP 32 NODE WILL SEND SOME DATA TO ESP8266 OF CLIMATE CONTROLL.
CLIMATE CONTROLer ESP8266 MUST RECEIVE AND PROCESS THIS INCOMING DATA.
THIS INCOMING DATA IS FROM MQTT, IT IS OF JSOIN FORMAT.
NOW THIS JSON DATA IS SENT TO ATMEGA VIA SERIAL3.
THEREFORE NODE ESP32 WILL SEND VIA MQTT TO ESP8266. ESP8266 WILL SEND DATA TO ATMEGA VIA SERIAL3.
 
WHAT I DO NOT UNDERSTAND IS THIS RETERIVED JSON VALUES ARE USED TO DISPLAY ON THE DISPLAY.
 */
void FromEspData()
{
  //temperature data from app
  float indoor_temp=0;
  float outdoor_temp=0;
  //vpd data from aapp
  byte vpd_mode=0;
  float vpd_temp=0;
  float vpd_hum=0;
  float vpd_app=0;
  //manual fan on/off data from app
  byte app_f1=0;
//  byte app_f2=0;
//  byte app_f3=0;
//  byte app_f4=0;
//  byte app_f5=0;
//  byte app_f6=0;
//  byte app_f7=0;
//  byte app_f8=0;
//  byte app_cp=0;
//  byte app_fp=0;
//  byte app_co=0;
//  byte app_ir=0;
//  byte app_do=0;
//  byte app_dr=0;
//  byte app_pb=0;
  StaticJsonDocument<1024> ESP;
  deserializeJson(ESP, Serial3);
  DeserializationError error = deserializeJson(ESP, Serial3);

  if (error) 
  {
    Serial.println("DESERIALIZING NODE ESP32 DATA FAILED");
    return;
  }

  if(JsonObject metrics= ESP["temp"])
  {
    if(metrics.containsKey("indoor"))
    {
      indoor_temp=metrics["indoor"];
    }
    if(metrics.containsKey("outdoor"))
    {
      outdoor_temp=metrics["outdoor"];
    }
  }
  else if(JsonObject metrics= ESP["vpd"])
  {
    if(metrics.containsKey("mode"))
    {
      vpd_mode=metrics["mode"];
      if(vpd_mode)
      {
        if(metrics.containsKey("mode"))
        {
           VpdautoModeOn=true;
           _VPD==metrics["vpd"];
           
        }
      }
      else
      {
        VpdautoModeOn=false;
      }
    }
//    
//    vpd_temp=metrics["temp"];
//    vpd_hum=metrics["hum"];
//    vpd_app=metrics["vpd"];
  }
  else if(JsonObject metrics= ESP["man"])
  {
    if(metrics.containsKey("fan1"))
    {
      app_f1 = metrics["fan1"];
      digitalWrite(ExFan1,app_f1);
    }
    if(metrics.containsKey("fan2"))
    {
      app_f1 = metrics["fan2"];
      digitalWrite(ExFan2,app_f1);
    }
    if(metrics.containsKey("fan3"))
    {
      app_f1 = metrics["fan3"];
      digitalWrite(ExFan3,app_f1);
    }
    if(metrics.containsKey("fan4"))
    {
      app_f1 = metrics["fan4"];
      digitalWrite(ExFan4,app_f1);
    }
    if(metrics.containsKey("fan5"))
    {
      app_f1 = metrics["fan5"];
      digitalWrite(ExFan5,app_f1);
    }
    if(metrics.containsKey("fan6"))
    {
      app_f1 = metrics["fan6"];
      digitalWrite(ExFan6,app_f1);
    }
    if(metrics.containsKey("fan7"))
    {
      app_f1 = metrics["fan7"];
      digitalWrite(ExFan7,app_f1);
    }
    if(metrics.containsKey("fan8"))
    {
      app_f1 = metrics["fan8"];
      digitalWrite(ExFan8,app_f1);
    }
    if(metrics.containsKey("cp"))
    {
      app_f1 = metrics["cp"];
      digitalWrite(pad1,app_f1);
    }
    if(metrics.containsKey("fp"))
    {
      app_f1 = metrics["fp"];
      digitalWrite(Fogger,app_f1);
    }
    if(metrics.containsKey("fs"))
    {
      app_f1 = metrics["fs"];
      digitalWrite(FoggerSol,app_f1);
    }
    if(metrics.containsKey("co"))
    {
      app_f1 = metrics["co"];
      digitalWrite(Co2Gen,app_f1);
    }
    
  }
  else
  {
    _AT =ESP["at_i"];
    _HUM = ESP["rh_i"];
    _VPD = ESP["vpd_i"];
    _HI =ESP["hi_i"];
    _CO2 =ESP["co2_i"];
    _ET =ESP["et_i"];
    _BP =ESP["bp_i"];
    _UV =ESP["uv_i"];
    _DP =ESP["dp_i"];
    _SL =ESP["sl_i"];
    O_AT =ESP["at_o"];
    O_HUM = ESP["rh_o"];
    O_VPD = ESP["vpd_o"];
    O_HI =ESP["hi_o"];
    O_CO2 =ESP["co2_o"];
    O_ET =ESP["et_o"];
    O_BP =ESP["bp_o"];
    O_UV =ESP["uv_o"];
    O_DP =ESP["dp_o"];
    O_SL =ESP["sl_o"];
  }
  
}
