void message_from_app(byte * payload, unsigned int length)
{
  byte index=0;
  byte can_i_send_data_to_mega=0;
  //temperature data from app
  
  //manual fan on/off data from app
  
  
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  if(JsonObject metrics= doc["temp"])
  {
    can_i_send_data_to_mega++;
  }
  else if(JsonObject metrics= doc["vpd"])
  {
    can_i_send_data_to_mega++;
  }
  
  for (int i = 0; i < length; i++) 
    {
      APP_DATA[index++]= ((char)payload[i]);
    }
  if(can_i_send_data_to_mega>0)
       Serial.println(APP_DATA);
}
