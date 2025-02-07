int PIRpin = 15;    // The pin of Arduino connected to the PIR output  
int PIRvalue = 0;  // It specifies the status of PIR sensor  

void setup() {     
 pinMode(PIRpin, INPUT);  
  // the output from the sensor is considered as input for Arduino  
 Serial.begin(9600);  
}  

void Detect()
{
   PIRvalue = digitalRead(PIRpin);   
 if (PIRvalue == HIGH)   
 { 
   Serial.println("Motion Detected");
   delay(1000); 
 }   
 else   
 { 
   Serial.println("Not Detected");  
   delay(1000);  
 }
}

void loop()  
{  
Detect(); 
}  
