void sendsetpointmega()
{
  DynamicJsonDocument relaydoc(1024);
  JsonObject CHANGERELAY = relaydoc.createNestedObject("Allrelays");
  if (valuedata != NULL)
  {
    String compare;
    String readd = valuedata;
    if (compare != readd)
    {
      compare = readd;
      CHANGERELAY["FAN1"] =  readd;
    }

    serializeJson(relaydoc, RELAYSTRING);
    Serial.println("");
    delay(50);
    serializeJson(relaydoc, Serial);
    Serial.print("\n");
    memset(RELAYSTRING, 0, 1024);
  }
}
//i did not understand this.
void sendnodedata()
{
  DynamicJsonDocument metricdoc(1024);
  DynamicJsonDocument Nodedoc(1024);

}
