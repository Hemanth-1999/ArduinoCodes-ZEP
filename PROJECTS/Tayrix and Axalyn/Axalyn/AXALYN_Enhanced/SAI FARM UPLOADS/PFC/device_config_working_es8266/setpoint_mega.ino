void sendsetpointmega()
{
  Serial.println(senddatato);
  DynamicJsonDocument Externalcommand(50);
  JsonObject Fromapp = Externalcommand.createNestedObject("Fromapp"); 
  Fromapp["Setpoint"] =  senddatato ;
  Serial.println("");
  serializeJson(Externalcommand, Serial);
  Serial.print("\n");
 
 }
