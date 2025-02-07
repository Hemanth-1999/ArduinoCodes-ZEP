// code use for KinCony KC868-A8S board
// make sure set your Serial2 pins in your "HardwareSerial.cpp" for ESP32 as follows:
//#define RX2  15
//#define TX2  13

#include "PCF8574.h"

String Publish = "relay8/12345/state"; //Publish Topic
String Subscribe = "relay8/12345/set"; //Subscribe Topic

PCF8574 pcf8574_o(0x24,4,5);
PCF8574 pcf8574_i(0x22,4,5);

void setup() 
{
  pinMode(2, OUTPUT);   //beep
 
  Serial.begin(115200); 
  Serial2.begin(115200);
  
  pcf8574_i.pinMode(P0, INPUT);

  pcf8574_o.pinMode(P0, OUTPUT);
  pcf8574_o.pinMode(P1, OUTPUT);
  pcf8574_o.pinMode(P2, OUTPUT);
  pcf8574_o.pinMode(P3, OUTPUT);
  pcf8574_o.pinMode(P4, OUTPUT);
  pcf8574_o.pinMode(P5, OUTPUT);
  pcf8574_o.pinMode(P6, OUTPUT);
  pcf8574_o.pinMode(P7, OUTPUT);
  

    Serial.print("Init pcf8574_i...");
    if (pcf8574_i.begin()){
        Serial.println("PCF8674_input_OK");
    }else{
        Serial.println("PCF8674_input_KO");
    }

    Serial.print("Init pcf8574_o...");
    if (pcf8574_o.begin()){
        Serial.println("PCF8674_output_OK");
    }else{
        Serial.println("PCF8674_output_KO");
    }

  pcf8574_o.digitalWrite(P0, HIGH); 
  pcf8574_o.digitalWrite(P1, HIGH); 
  pcf8574_o.digitalWrite(P2, HIGH); 
  pcf8574_o.digitalWrite(P3, HIGH); 
  pcf8574_o.digitalWrite(P4, HIGH); 
  pcf8574_o.digitalWrite(P5, HIGH); 
  pcf8574_o.digitalWrite(P6, HIGH); 
  pcf8574_o.digitalWrite(P7, HIGH); 

  //AT Commands for setting up the client id and Server
  //Need to be executed once -- Open serial terminal doe seeing the debug messages
  Serial.println("Connecting To Server........");
  Serial2.println("ATE1");
  delay(2000);
  Serial2.println("AT+CMQTTSTART"); //Establishing MQTT Connection
  delay(2000); 
  Serial2.println("AT+CMQTTACCQ=0,\"kincony\""); //Client ID - change this for each client as this need to be unique
  delay(2000);
  Serial2.println("AT+CMQTTCONNECT=0,\"tcp://iot.kincony.com:1883\",120,1,\"mqtt\",\"123\""); //MQTT Server Name for connecting this client  user name: mqtt   password: 123
  delay(2000);

  Serial2.println("AT+CMQTTSUB=0,16,1,1"); //Length of message
  delay(1000);
  Serial2.println("relay8/12345/set"); //message
  delay(1000);
  Serial.println("Done");
}

void loop() 
{
  String incoming;

 //Receiving MODEM Response
  while(Serial2.available()>0)
  {
    delay(10);
    incoming = Serial2.readString();
    Serial.println(incoming);

    //Serial.println(b);
    if(incoming.indexOf("PAYLOAD") != -1)
    {
       int new1 = incoming.indexOf("PAYLOAD");
       String neww = incoming.substring(new1);
       int new2 = neww.indexOf('\n');
       String new3 = neww.substring(new2+1);
       int new4 = new3.indexOf('\n');
       String new5 = new3.substring(0,new4);
       
       Serial.println("Topic: relay/subscribe");
       Serial.print("Message is: ");
       Serial.println(new5);
       new5.remove(new5.length()-1);
       if(new5 == "{\"relay1\":{\"on\":1}}")
       {
        Serial.println("relay1 ON");
        pcf8574_o.digitalWrite(P0, LOW); 


      Serial.println("Publishing Message: relay1 ON");
      Serial2.println("AT+CMQTTTOPIC=0,18"); //AT Command for Setting up the Publish Topic Name
      delay(1000);
      Serial2.println(Publish); //Topic Name
      delay(1000);
      Serial2.println("AT+CMQTTPAYLOAD=0,19"); //Payload length
      delay(1000);
      Serial2.println("{\"relay1\":{\"on\":1}}"); //Payload message
      delay(1000);
      Serial2.println("AT+CMQTTPUB=0,1,60"); //Acknowledgment
      delay(1000);
        
       }
       else if(new5 == "{\"relay1\":{\"on\":0}}")
       {
        Serial.println("relay1 OFF");
        pcf8574_o.digitalWrite(P0, HIGH); 

       
      Serial.println("Publishing Message: relay1 OFF");
      Serial2.println("AT+CMQTTTOPIC=0,18"); //AT Command for Setting up the Publish Topic Name
      delay(1000);
      Serial2.println(Publish); //Topic Name
      delay(1000);
      Serial2.println("AT+CMQTTPAYLOAD=0,19"); //Payload length
      delay(1000);
      Serial2.println("{\"relay1\":{\"on\":0}}"); //Payload message
      delay(1000);
      Serial2.println("AT+CMQTTPUB=0,1,60"); //Acknowledgment
      delay(1000);       
              
       }
    }      
  }
}
