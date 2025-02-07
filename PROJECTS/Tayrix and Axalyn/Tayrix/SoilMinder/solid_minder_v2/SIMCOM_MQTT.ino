void InitializeMqtt(String broker, int port, String username, String password, String topic)
{
  Serial2.println("AT+CMQTTSTART");//start mqtt service, activate PDP context
  delay(1000);
  serial2read();
  Serial2.println("AT+CMQTTACCQ=0,\"ESPClient1\",1");
  delay(1000);
  serial2read();
  Serial2.println("AT+CMQTTWILLTOPIC=0,5");
  delay(100);
  serial2read();
  Serial2.print("TAYRIX");
  delay(100);
  serial2read();
  Serial2.println("AT+CMQTTWILLMSG=0,15,1");
  delay(100);
  serial2read();
  Serial2.print("TAYRIX CONNECTION");
  delay(100);
  serial2read();
  String connectCmd = "AT+CMQTTCONNECT=0,\"tcp://" + broker + ":" + String(port) + "\",60,1,\"" + username + "\",\"" + password + "\"";
  //  Serial.println(connectCmd);
statement1:
  Serial2.println(connectCmd);
  String str12 =  serial2read();
  while (str12.indexOf("+CMQTTCONNECT:") < 0)
  {
    Serial.println("waiting for gsm reply...");
    str12 =  serial2read();
    delay(1000);

  }
  if (str12.indexOf("+CMQTTCONNECT: 0,0\r\n") >= 0)
  {

    Serial.println("GSM MQTT DONE");
  }
  else
  {
    Serial.println("Reconnecting to Mqtt");
    goto statement1;
  }
  //  delay(10000);

}

String serial2read(void)
{

  String str = "";
  while (Serial2.available())
  {
    str = Serial2.readString(); //readString() is a blocking function. it will read continously if there is no gap of 1second with new data.
  }
  if (str != NULL)Serial.println(str);

  return str;
}

void Gsm_Publish(String topic, String payload)
{
  String connectCmd = "AT+CMQTTTOPIC=0," + String(topic.length());
  Serial2.println(connectCmd);
  delay(1000);
  serial2read();
  Serial2.println(topic);
  delay(1000);
  connectCmd = "AT+CMQTTPAYLOAD=0," + String(payload.length());
  Serial2.println(connectCmd);
  delay(500);
  serial2read();
  Serial2.println(payload);
  delay(1000);
statement2:
  connectCmd = "AT+CMQTTPUB=0,1," + String(payload.length());
  Serial2.println(connectCmd);

  String str12 = serial2read();
  while (str12.indexOf("+CMQTTPUB:") < 0)
  {
    Serial.println("waiting for gsm reply...");
    str12 =  serial2read();
    delay(1000);

  }
  if (str12.indexOf("+CMQTTPUB: 0,0\r\n") >= 0)
  {

    Serial.println("PUBLISHED DATA SAFELY");

  }
  else
  {
    Serial.println("FAILED TO PUBLISH DATA");
    goto statement2;
  }

}
