/*
THIS IS THE NODE WHICH IS KEPT INSIDE THE FARM.
*/
#include "config.h"
void setup() 
{
  Serial.begin(115200);
  //these pin are used to read analog reading of UV
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  pinMode(34, INPUT);//BATTERY VOLTAGE
  
  configure_wifi_portal();//SET HER WIFI CREDENTIALS + MQTT THINGS
  
  EEPROM.begin(512);//eeprome begin
  u8g2.begin();//display begin
  co2Sensor.calibrate();
  while (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
   
  }
  timeClient.begin();
  timeClient.setTimeOffset(5*3600+30*60);//FOR INDIA
  //mqtt
 client.setServer(mqttcredentials.Broker, mqttcredentials.Port);
 client.setCallback(callback);
}

void loop() 
{
  wm.process();
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  timeClient.update();
  unsigned long nowon = millis();
  if (nowon - lastMsg01 > 1000) 
  {
      lastMsg01 = nowon;
      Display();
   }
   unsigned long now1 = millis();
    if (now1 - lastMsg > 5000) 
    {
      lastMsg = now1;
      if (client.connected()) 
      {
        sendDataMQTT();
      }
     }
}
