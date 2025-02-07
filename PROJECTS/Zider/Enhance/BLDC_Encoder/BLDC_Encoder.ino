#include <SOLOMotorController.h>


//pointer of SOLO object:
SOLOMotorController *SOLO_Obj1; 

//the device address of SOLO:
unsigned char SOLO_address1=0; 

//Desired Switching or PWM Frequency at Output
long pwmFrequency=70; 

//Motor's Number of Poles
long numberOfPoles = 8; 

//Motor's Number of Encoder Lines (PPR pre-quad)
long numberOfEncoderLines = 1000; 

//Select the Normal BLDC_PMSM motor type
long motorType= 1; 

//Speed controller Kp
float speedControllerKp = 0.15; 

//Speed controller Ki
float speedControllerKi = 0.03; 

//Position controller Kp
float positionControllerKp = 0.12;

//Position controller Ki
float positionControllerKi = 0.02; 

// Current Limit of the Motor
float currentLimit= 15.0; 

// Battery or Bus Voltage
float busVoltage = 0; 

// Desired Speed Limit[RPM]
long desiredSpeedLimit =3000; 

// Desired Position Reference 
long desiredPositionReference =0; 

// Motor speed feedback
long actualMotorSpeed = 0; 

// Motor position feedback
long actualMotorPosition = 0; 

void setup() {
  Serial.begin(115200); 

  //Initialize the SOLO object
  SOLO_Obj1 = new SOLOMotorController(SOLO_address1); 
  delay(2000);
  
  busVoltage = SOLO_Obj1->GetBusVoltage();
  while(busVoltage <=0){
      busVoltage = SOLO_Obj1->GetBusVoltage();
      //wait here till communication is established
      Serial.println("\n Trying to Connect To SOLO");
      delay(1000);
      }
      
  Serial.println("\n Communication Established succuessfully!");
  
  //dummy read after Serial.println to open the UART line
  SOLO_Obj1->GetBusVoltage();
  // Initial Configurations
  SOLO_Obj1->SetPWMFrequency(pwmFrequency);
  SOLO_Obj1->SetCurrentLimit(currentLimit);
  SOLO_Obj1->SetEncoderLines(numberOfEncoderLines);

  //select Digital Mode
  SOLO_Obj1->SetCommandMode(1);

  SOLO_Obj1->SetMotorType(motorType);

  //run the motor identification
  //run ID. always after selecting the Motor Type!
  SOLO_Obj1->SetIdentification(1);

  Serial.println("\n Identifying the Motor");

  //wait at least for 2sec till ID. is done
  delay(2000); 
  
  //dummy read after Serial.println to open the UART line
  SOLO_Obj1->GetBusVoltage();
  
  //Operate while using Quadrature Encoder
  SOLO_Obj1->SetSpeedControlMode(1);

  //Control The Position
  SOLO_Obj1->SetControlMode(2);

  //Speed Controller Tunings
  SOLO_Obj1->SetSpeedControllerKp(speedControllerKp);
  SOLO_Obj1->SetSpeedControllerKi(speedControllerKi);

  //Position Controller Tunings
  SOLO_Obj1->SetPositionControllerKp(positionControllerKp);
  SOLO_Obj1->SetPositionControllerKi(positionControllerKi);
  
}

void loop() {

  //set a desired Speed Limit for trajectory in RPM
  desiredSpeedLimit = 5000;
  SOLO_Obj1->SetSpeedLimit(desiredSpeedLimit);
  
  // set a positive desired Position Reference 
  desiredPositionReference =+500000;
  SOLO_Obj1->SetDesiredPosition(desiredPositionReference);

  // wait till motor reaches to the reference 
  delay(3000); 

  actualMotorPosition = SOLO_Obj1->GetEncoderPosition();
  Serial.println("\n Number of Pulses passed: ");
  Serial.println(actualMotorPosition);

  //dummy read after Serial.println to open the UART line
  SOLO_Obj1->GetBusVoltage();

    //set a desired Speed Limit for trajectory in RPM
  desiredSpeedLimit = 1500;
  SOLO_Obj1->SetSpeedLimit(desiredSpeedLimit);
  
  // set a negative desired Position Reference 
  desiredPositionReference =-32559;
  SOLO_Obj1->SetDesiredPosition(desiredPositionReference);

  // wait till motor reaches to the reference 
  delay(6000); 

  actualMotorPosition = SOLO_Obj1->GetEncoderPosition();
  Serial.println("\n Number of Pulses passed: ");
  Serial.println(actualMotorPosition);

  //dummy read after Serial.println to open the UART line
  SOLO_Obj1->GetBusVoltage();
  
}
