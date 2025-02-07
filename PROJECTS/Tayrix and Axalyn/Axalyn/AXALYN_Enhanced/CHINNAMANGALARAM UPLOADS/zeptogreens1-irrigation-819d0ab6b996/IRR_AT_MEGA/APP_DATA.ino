void Appdata(String str)
{
  StaticJsonDocument<1024> ESP;
  deserializeJson(ESP, str);
  DeserializationError error = deserializeJson(ESP, str);
  if (error) 
  {
    Serial.println("DESERIALIZING  APP-ESP32 DATA FAILED");
    return;
  }
  int mode_check=0;
  if(JsonObject manual= ESP["man"])
  {
    if(manual.containsKey("pa"))//pump A
    {
      
    }
    else if(manual.containsKey("pb"))//pump B
    {
      
    }
    else if(manual.containsKey("dop"))//DO PUMP
    {
      
    }
    else if(manual.containsKey("rov"))//RO valve
    {
      
    }
    else 
    {
      
    }
  }
  else if(JsonObject Auto= ESP["auto"])
  {
    if(JsonObject pumpA= Auto["pa"])
    {
      mode_check=pumpA["mode"];
      if(mode_check)
      {
        pumpA["on"];
        pumpA["off"];
      }
      else
      {
        
      }
    }
    else if(JsonObject pumpB= Auto["pb"])
    {
      mode_check=pumpB["mode"];
      if(mode_check)
      {
        pumpB["on"];
        pumpB["off"];
      }
      else
      {
        
      }
    }
    else if(JsonObject DOp= Auto["dop"])
    {
      mode_check=DOp["mode"];
      if(mode_check)
      {
        DOp["on"];
        DOp["off"];
      }
      else
      {
        
      }
    }
  }
  else if(JsonObject schedule = ESP["schedule"])
  {
     if(JsonObject pumpA = schedule["pa"])
     {
       mode_check=pumpA["mode"];
       if(mode_check)
       {
          if(JsonObject slot1 = pumpA["s1"])
          {
            slot1["on"];
            slot1["off"];
          }
          else if(JsonObject slot2 = pumpA["s2"])
          {
            slot2["on"];
            slot2["off"];
          }
          else if(JsonObject slot3 = pumpA["s3"])
          {
            slot3["on"];
            slot3["off"];
          }
          else if(JsonObject slot4 = pumpA["s4"])
          {
            slot4["on"];
            slot4["off"];
          }
          else if(JsonObject slot5 = pumpA["s5"])
          {
            slot5["on"];
            slot5["off"];
          }
          else if(JsonObject slot6 = pumpA["s6"])
          {
            slot6["on"];
            slot6["off"];
          }
       }
       else
       {
        
       }
     }
     else if(JsonObject pumpB = schedule["pb"])
     {
        mode_check=pumpB["mode"];
       if(mode_check)
       {
          if(JsonObject slot1 = pumpB["s1"])
          {
            slot1["on"];
            slot1["off"];
          }
          else if(JsonObject slot2 = pumpB["s2"])
          {
            slot2["on"];
            slot2["off"];
          }
          else if(JsonObject slot3 = pumpB["s3"])
          {
            slot3["on"];
            slot3["off"];
          }
          else if(JsonObject slot4 = pumpB["s4"])
          {
            slot4["on"];
            slot4["off"];
          }
          else if(JsonObject slot5 = pumpB["s5"])
          {
            slot5["on"];
            slot5["off"];
          }
          else if(JsonObject slot6 = pumpB["s6"])
          {
            slot6["on"];
            slot6["off"];
          }
       }
       else
       {
        
       }
     }
     else if(JsonObject DOp = schedule["dop"])
     {
        mode_check=DOp["mode"];
       if(mode_check)
       {
          if(JsonObject slot1 = DOp["s1"])
          {
            slot1["on"];
            slot1["off"];
          }
          else if(JsonObject slot2 = DOp["s2"])
          {
            slot2["on"];
            slot2["off"];
          }
          else if(JsonObject slot3 = DOp["s3"])
          {
            slot3["on"];
            slot3["off"];
          }
          else if(JsonObject slot4 = DOp["s4"])
          {
            slot4["on"];
            slot4["off"];
          }
          else if(JsonObject slot5 = DOp["s5"])
          {
            slot5["on"];
            slot5["off"];
          }
          else if(JsonObject slot6 = DOp["s6"])
          {
            slot6["on"];
            slot6["off"];
          }
       }
     }
     else
       {
        
       }
  }
  else
  {
    
  }
}
