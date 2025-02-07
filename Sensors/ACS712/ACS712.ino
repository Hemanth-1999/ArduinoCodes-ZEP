 
//Measuring Current Using ACS712
 
const int analogchannel = 0; //Connect current sensor with A0 of Arduino
int sensitivity = 185; // use 100 for 20A Module and 66 for 30A Module
int adcvalue= 0;
int offsetvoltage = 2500; 
double Voltage = 0; //voltage measuring
double ecurrent = 0;// Current measuring
 
void setup() {
 //baud rate
 Serial.begin(9600);//baud rate at which arduino communicates with Laptop/PC

}
 
void loop() //method to run the source code repeatedly
{
 
 adcvalue = analogRead(analogchannel);//reading the value from the analog pin
 Voltage = (adcvalue / 1024.0) * 5000; // Gets you mV
 ecurrent = ((Voltage - offsetvoltage) / sensitivity);
 
//Prints on the serial port
 Serial.print("Raw Value = " ); // prints on the serial monitor
 Serial.print(adcvalue); //prints the results on the serial monitor
 delay(1000);//delay of 1 sec
 
 Serial.print("\t mV = "); // shows the voltage measured 
 Serial.print(Voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
 
 Serial.print("\t ecurrent = "); // shows the voltage measured 
 Serial.println(ecurrent,3);// the '3' after voltage allows you to display 3 digits after decimal point
 delay(2500); //delay of 2.5 sec
}
