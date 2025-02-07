#include "config.h"

void setup()
{
  Serial.begin(115200);
  
  configure_wifi_portal();//SET HER WIFI CREDENTIALS + MQTT THINGS

  FileSystemInitialize();
  
  //NTP SERVER
 timeClient.begin();
 timeClient.setTimeOffset(5 * 3600 + 30 * 60);
 
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
  unsigned long currentMillis = millis();
  if (currentMillis - SCpreviousMillis >= 1000)
  {
    SCpreviousMillis = currentMillis;
    //sendsetpointmega();//actually at mega only receives app data from esp8266
  }
  unsigned long megacurrentMillis = millis();
  if (megacurrentMillis - SendtomegapreviousMillis >= 3000)
  {
    SendtomegapreviousMillis = megacurrentMillis;
    getvalue.start(100);
  }
  if (getvalue.isRunning())
  {
   // sendnodedata();//i did not understand about this. because at mega never receives any thing.
  }
  if (getvalue.justFinished())
  {
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
    manualstatus();//status of pump 1 and pump2
  }
  unsigned long cloudcurrentMillis = millis();
  if (cloudcurrentMillis - tocloudstatus >= 8000)
  {
    tocloudstatus = cloudcurrentMillis;
    tocloud();//this function will send incoming data from mega to mqtt broker
  }
}
