/*
CHECK LIST:
GSM CODE WORKING ? YES WORKING TESTED.
PORTAL CODE WORKING? YES WORKING TESTED

sending data is not still coded.

*/

#include"config.h"
#define VIA_GSM
//#define ENABLE_FILESYSTEM
#define ENABLE_BME
void setup() 
{
  Serial.begin(115200);
  Serial2.begin(115200);//gsm
  mod.begin(4800, SERIAL_8N1,15,18);//npk sensor <RX_pin>, <TX_pin>
  pinMode(RE,OUTPUT);
  pinMode(RE,OUTPUT);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  Serial.println("WELCOME TO SOIL MINDER");
  
  #ifdef ENABLE_BME
  
//    BME_initialize();
    
  #endif
   
  #ifdef ENABLE_FILESYSTEM
  
    FileSystemInitialize();
    
  #endif
  
  #ifdef ENABLE_PORTAL
  
    ConfigPortal();
    delay(100);
    ReadFromFile();
    
  #endif
  
  #ifdef VIA_GSM
  
    InitializeMqtt(MqttCredentials.Broker,MqttCredentials.Port,MqttCredentials.Username,MqttCredentials.Password,MqttCredentials.Topic1);
  
  #else
  
    client.setServer(MqttCredentials.Broker,MqttCredentials.Port);
    client.setCallback(callback);
    reconnect();
    
  #endif
  
//  timeClient.begin();
//  timeClient.setTimeOffset(5*3600+30*60);//FOR INDIA
  
  #ifdef ENABLE_SLEEP
  
    esp_sleep_enable_timer_wakeup(Time_To_Sleep * S_To_uS_Factor);
    
  #endif
  
}

void loop() 
{
  #ifdef VIA_GSM
  
    serial2read();
    
  #else
  
    if(!client.connected()) 
    {
      reconnect();
    }
    
  #endif
  
  #ifdef ENABLE_NTP
  
  timeClient.update();
  
  #endif
  
  #ifdef ENABLE_BME
  
    printBME280Data(&Serial);
    UVA();
    ETcalcul();
  #endif
  
  #ifdef ENABLE_NPK
  
   getNPKdata();
   
  #endif
  
  #ifdef ENABLE_SLEEP
  
    esp_deep_sleep_start();
    
  #endif
  
  sendDataMQTT();
   delay(2000);
  
}
