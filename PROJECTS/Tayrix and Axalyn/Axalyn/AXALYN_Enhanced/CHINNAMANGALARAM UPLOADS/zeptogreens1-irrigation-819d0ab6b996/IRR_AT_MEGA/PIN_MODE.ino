void Pin_mode()
{
  pinMode(Z1PUMPA,OUTPUT);
  pinMode(Z1PUMPB,OUTPUT);
  pinMode(Z1AIRPUMP,OUTPUT);
  pinMode(Z1SOLVALVE,OUTPUT);
  pinMode(DOOA,OUTPUT);
  pinMode(DOOB,OUTPUT);


  digitalWrite(Z1PUMPA,HIGH);
  digitalWrite(Z1PUMPB,HIGH);
  digitalWrite(Z1AIRPUMP,HIGH);
  digitalWrite(Z1SOLVALVE,HIGH);
  digitalWrite(DOOA,HIGH);
  digitalWrite(DOOB,HIGH);
}
