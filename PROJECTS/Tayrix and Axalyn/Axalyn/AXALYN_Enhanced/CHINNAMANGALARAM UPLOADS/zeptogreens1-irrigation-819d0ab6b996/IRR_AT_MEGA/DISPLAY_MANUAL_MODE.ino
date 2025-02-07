void irroff()
{
  Serial.println("nothing to do");

  pinMode(Z1PUMPA, OUTPUT);
  digitalWrite(Z1PUMPA, HIGH);
}
void dooff()
{
  pinMode(Z1PUMPB, OUTPUT);
  digitalWrite(Z1PUMPB, HIGH);
}
void dripoff()
{
  pinMode(Z1AIRPUMP, OUTPUT);
  digitalWrite(Z1AIRPUMP, HIGH);
}
void irrboff()
{
  pinMode(Z1SOLVALVE, OUTPUT);
  digitalWrite(Z1SOLVALVE, HIGH);
}
