/*
THIS PROGRAM IS THE CLIMAT CONTROLL MASTER ESP8266.
THINGS TO COMPLETE:
                    IN THIS PROGRAM INTERRUPT FOR RESETTING PORTAL IS PENDING.
                    IF NO ONE IS CONNECTING TO THE PORTAL IN 3 MINUTES WE NEED TO RESTART ESP.
                    
*/
#include "config.h"
void setup() 
{
 Serial.begin(115200);
 //running average related
 clearRunningAverage();
 //INTERRUPT PIN MODE
 
 configure_wifi_portal();//SET HER WIFI CREDENTIALS + MQTT THINGS
 //NTP SERVER
 timeClient.begin();
 timeClient.setTimeOffset(5 * 3600 + 30 * 60);
 //FILE SYSTEM
 FileSystemInitialize();
 //read mqtt credentials from file system.
 if(read_from_file_status) readsavedvalues();
 //mqtt
 client.setServer(mqttcredentials.Broker, mqttcredentials.Port);
 client.setCallback(callback);
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  timeClient.update();
  unsigned long now2 = millis();
  if (now2 - lastMsg01 > 4000) 
  {
    lastMsg01 = now2;
    SendDataToMega();
  }
  unsigned long torecievecurrentMillis = millis();
  if (torecievecurrentMillis - torecievestatus >= 5000)
  {
    torecievestatus = torecievecurrentMillis;
    alert();
    delay(15);
    eventstatus();
  }
   unsigned long torecievecurrentMillism = millis();
  if (torecievecurrentMillism - torecievestatusm >= 4000)
  {
    torecievestatusm = torecievecurrentMillism;
    manualstatus();
  }
  unsigned long cloudcurrentMillis = millis();
  if (cloudcurrentMillis - tocloudstatus >= 8000)
  {
    tocloudstatus = cloudcurrentMillis;
    tocloud();
  }
}
