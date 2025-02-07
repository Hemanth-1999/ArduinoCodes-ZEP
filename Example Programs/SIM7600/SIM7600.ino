/*
Project: SIM7600 4G Lesson
Programmer: Shahrulnizam Mat Rejab
Board: ESP32
Last Modified: 13 June 2019
Website: http://shahrulnizam.com
*/

//#define SIM5320
#define SIM7600

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

const char apn[] = "celcom3g";
const char url[] = "http://shahrulnizam.com/web/datetime.php";
const char phone[] = "+7702826670";

char data,temp;
String message;

WiFiManager wifiManager;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(5000);

  wifiManager.setTimeout(60);
  if(!wifiManager.autoConnect("ESP32"))
  {
    Serial.println("failed to connect and hit timeout");
  }
  else
  {
    Serial.println("Successfully connected");
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  data=0;
  while(1)
  {
    if(data==0)
    {
      Serial.println("Baudrate 115200");
      Serial2.begin(115200);
    }
    else if(data==1)
    {
      Serial.println("Baudrate 9600");
      Serial2.begin(9600);
    }
    data=!data;
    delay(1000);
    Serial2.print("AT+IPR=9600\r"); //Change baudrate
    delay(500);
    message="";
    while(Serial2.available())
    {
      message+=(char)Serial2.read();
      if(message.indexOf("OK")>-1) break;
    } 
    if(message.indexOf("OK")>-1) break;
  }
  Serial2.begin(9600);
  
  Serial.println("Enter");
  Serial.println("A - Wifi manager");
  Serial.println("B - Change baudrate");
  Serial.println("C - Signal quality test");
  Serial.println("D - Manufacturer identification");
  Serial.println("E - Send message");
  Serial.println("F - Read message");
  Serial.println("G - List message");
  Serial.println("H - Delete message");
  Serial.println("I - Initialize http");
  Serial.println("J - Request http");
}

void loop()
{
  ArduinoOTA.handle();
  if(Serial.available())
  {
    data=Serial.read();
    
    if(data=='A')
    {
      WiFi.disconnect(false,true);
      Serial.println("Reset wifi setting and restart");
      wifiManager.resetSettings();
      ESP.restart();      
    }

    if(data=='B')
    {
      Serial2.print("AT+IPR=9600\r"); //Change baudrate
    }

    if(data=='C')
    {
      Serial2.print("ATE1\r"); //Echo enable
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }
      
      Serial2.write("AT+CSQ\r\n"); //Signal quality test
    }
    
    if(data=='D')
    {
      Serial2.write("AT+CGMI\r\n"); //Manufacturer identification
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }

      Serial2.write("AT+CGMM\r\n"); //Model identification
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }

      Serial2.print("AT+CGMR\r\n"); //Software version
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }

      Serial2.print("AT+CGSN\r\n"); //IMEI number
    }
    
    if(data=='E')
    {
      Serial2.print("AT+CNMI=2,1,0,0,0\r"); //New message indication
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }
      
      Serial2.print("AT+CMGF=1\r");  //Set text mode
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }
      
      Serial2.print("AT+CMGS=\""+String(phone)+"\"\r"); //Send message
      delay(500);
      Serial2.print("Hai, I'm sim7600");//Text message
      Serial2.println((char)0x1A); //Ctrl+Z
    }
    
    if(data=='F')
    {
      Serial2.print("AT+CMGR=0\r"); //Read message
    }

    if(data=='G')
    {
      Serial2.print("AT+CMGL=\"REC UNREAD\"\r"); //List Message
    }
    
    if(data=='H')
    {
      Serial2.print("AT+CMGD=1,4\r"); //Delete all message
    }

    #if defined (SIM5320)
      if(data=='I')
      {      
        Serial2.print("AT+CGSOCKCONT=1,\"IP\",\""+String(apn)+"\"\r");
        delay(500);
        while(Serial2.available())
        {
          data=(char)Serial2.read();
          Serial.write((char)data);
        }
  
        Serial2.print("AT+CSOCKSETPN=1\r");
        delay(500);
        while(Serial2.available())
        {
          data=(char)Serial2.read();
          Serial.write((char)data);
        }
  
        Serial2.print("AT+NETOPEN\r");    
      }
        
      if(data=='J')
      {
        Serial2.print("AT+CHTTPACT=\"shahrulnizam.com\",80\r");
      }
    #endif

    #if defined (SIM7600)
      if(data=='I')
      {      
        Serial2.print("AT+CGSOCKCONT=1,\"IP\",\""+String(apn)+"\"\r");
        delay(500);
        while(Serial2.available())
        {
          data=(char)Serial2.read();
          Serial.write((char)data);
        }
  
        Serial2.print("AT+HTTPINIT\r");
        delay(500);
        while(Serial2.available())
        {
          data=(char)Serial2.read();
          Serial.write((char)data);
        }
  
        Serial2.print("AT+NETOPEN\r");    
      }

      if(data=='J')
      {
        Serial2.print("AT+HTTPPARA=\"URL\",\""+String(url)+"\"\r");
      }      
    #endif

    if(data=='K')
    {
      Serial2.print("AT+CGPSNMEA=511\r"); //configure NMEA
      delay(100);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }

      Serial2.print("AT+CGPSHOT\r"); //hot start GPS session
      delay(100);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }

      Serial2.print("AT+CGPSINFOCFG=2,31\r"); //Report GPS NMEA-0183 sentence
    }

    if(data=='L')
    {
      Serial2.print("AT+CGPS=1,1\r"); //Start GPS Session
    }

    if(data=='M')
    {
      Serial2.print("AT+CGPSINFO\r"); //Get GPS fixed position information
    }

    if(data=='N')
    {
      Serial2.print("AT+CGPS=0\r"); //Stop GPS Session
    }
  }

  if(Serial2.available()>0)
  {
    delay(60);
    message="";
    while(Serial2.available())
    {
      message+=(char)Serial2.read();
    }
    Serial.print(message);
    
    if(message.indexOf("CMTI:")>-1)
    {
      message=message.substring(message.indexOf(",")+1);
      message.replace("\r","");
      message.replace("\n","");
      if(message=="10") temp=1;
      Serial2.print("AT+CMGR="+message+"\r"); //Read message
    }
    else if(message.indexOf("CMGR:")>-1)
    {
      if(temp==1)
      {
        Serial2.print("AT+CMGD=1,4\r");         //Delete all message
        temp=0;
      }
    }
    else if(message.indexOf("NETOPEN:")>-1)
    {
      Serial2.print("AT+IPADDR\r");
      delay(500);
      while(Serial2.available())
      {
        data=(char)Serial2.read();
        Serial.write((char)data);
      }
    }
    else if(message.indexOf("IPADDR:")>-1)
    {
      #if defined (SIM5320)
        Serial2.print("AT+CHTTPSSTART\r");
      #endif      
    }
    #if defined (SIM5320)
      else if(message.indexOf("CHTTPACT: REQUEST")>-1)
      {
        message=String(url);
        message.replace("http://","");
        message=message.substring(0,message.indexOf("/"));
        Serial2.print("GET "+String(url)+" HTTP/1.1\r\n");
        Serial2.print("Host: "+message+"\r\n");
        Serial2.print("Content-Length: 0\r\n\r\n");
        Serial2.println((char)0x1A); //Ctrl+Z     
      }
    #endif
    #if defined (SIM7600)
      else if(message.indexOf("HTTPPARA=\"URL\"")>-1)
      {
        Serial2.print("AT+HTTPACTION=0\r");
      }
      else if(message.indexOf("HTTPACTION:")>-1)
      {
        message=message.substring(message.indexOf(",")+1);
        message=message.substring(message.indexOf(",")+1);
        message.replace("+HTTP_PEER_CLOSED","");
        message.replace("\r","");
        message.replace("\n","");
        Serial2.print("AT+HTTPREAD=0,"+message+"\r");
      }
    #endif
  }
}
