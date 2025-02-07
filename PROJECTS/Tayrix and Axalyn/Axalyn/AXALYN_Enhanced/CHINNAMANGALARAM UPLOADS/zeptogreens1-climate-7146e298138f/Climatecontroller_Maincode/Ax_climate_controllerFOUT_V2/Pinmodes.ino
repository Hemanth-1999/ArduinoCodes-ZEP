void Pinmodes()
{
    pinMode(ExFan1, OUTPUT);
    pinMode(ExFan2, OUTPUT);
    pinMode(ExFan3, OUTPUT);
    pinMode(ExFan4, OUTPUT);
    pinMode(ExFan5, OUTPUT);
    pinMode(ExFan6, OUTPUT);
    pinMode(ExFan7, OUTPUT);
    pinMode(ExFan8, OUTPUT);
    pinMode(pad1, OUTPUT);
    pinMode(Fogger, OUTPUT);
    pinMode(FoggerSol, OUTPUT);
    pinMode(Co2Gen, OUTPUT);
}
/*
THE RELAYS WE ARE USING ARE BY DEFAULT THEY ARE ON (high).
IF WE DO digitalWrite(relay_no,HIGH); THEN RELAY WILL OFF.
IF WE DO digitalWrite(relay_no,LOW); THEN RELAY WILL ON.
*/
void alloff()
{
  digitalWrite(ExFan1, HIGH);//relay will off 
  digitalWrite(ExFan2, HIGH);//relay will off 
  digitalWrite(ExFan3, HIGH);//relay will off 
  digitalWrite(ExFan4, HIGH);//relay will off 
  digitalWrite(ExFan5, HIGH);//relay will off 
  digitalWrite(ExFan6, HIGH);//relay will off 
  digitalWrite(ExFan7, HIGH);//relay will off 
  digitalWrite(ExFan8, HIGH);//relay will off 
  digitalWrite(pad1, HIGH);//relay will off 
  digitalWrite(Fogger, HIGH);//relay will off 
  digitalWrite(FoggerSol, HIGH);//relay will off 
  digitalWrite(Co2Gen, HIGH);//relay will off 
}

void allon()
{
  digitalWrite(ExFan1, LOW);//relay will on
  digitalWrite(ExFan2, LOW);//relay will on
  digitalWrite(ExFan3, LOW);//relay will on
  digitalWrite(ExFan4, LOW);//relay will on
  digitalWrite(ExFan5, LOW);//relay will on
  digitalWrite(ExFan6, LOW);//relay will on
  digitalWrite(ExFan7, LOW);//relay will on
  digitalWrite(ExFan8, LOW);//relay will on
  digitalWrite(pad1, LOW);//relay will on
  digitalWrite(Fogger, LOW);//relay will on
  digitalWrite(FoggerSol, LOW);//relay will on
  digitalWrite(Co2Gen, LOW);//relay will on
}
