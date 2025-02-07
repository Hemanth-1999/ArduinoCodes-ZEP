/**************************************************************
   Function Name: CallScheduling
   Description: [This function has 2 slots for turning pumps and solenoids automaticaly based on time]
   Output: [Scheduled Slots]
 **************************************************************/
void CallScheduling()
{
  /*==============================================
    First Schedule Section
    ==============================================*/
  if (Schedulesstate == HIGH)
  {

    if ((Slot1hr == RtcHour) && (Slot1min == RtcMin))
    {
      digitalWrite(MainPumpA, HIGH);
      digitalWrite(FertSolenoid, HIGH);
      Serial.println("First slot is running ");

      if (Slot1z1 == HIGH)
      {
        Serial.println("First slot ZONE1 is running ");
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
      }
      if (Slot1z2 == HIGH)
      {
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
        Serial.println("First slot ZONE2 is running ");
      }
      if (Slot1z3 == HIGH)
      {
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
        Serial.println("First slot ZONE3 is running ");
      }
    }
    if ((Slot1hroff == RtcHour) && (Slot1minoff == RtcMin))
    {
      Serial.println("First slot is Completed ");
      digitalWrite(MainPumpA, LOW);
      digitalWrite(FertSolenoid, LOW);
      digitalWrite(ZonE1, LOW);
      digitalWrite(ZonE1, LOW);
    }
  }
  /*==============================================
    Second Schedule Section
    ==============================================*/
  if (Schedulesstate2 == HIGH)
  {

    if ((Ontimehr == RtcHour) && (Ontimemin == RtcMin))
    {
      digitalWrite(MainPumpA, HIGH);
      digitalWrite(FertSolenoid, HIGH);
      Serial.println("Second slot is running ");

      if (Slot2z1 == HIGH)
      {
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
        Serial.println("Second slot ZONE1 is running ");
      }
      if (Slot2z2 == HIGH)
      {
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
        Serial.println("Second slot ZONE2 is running ");
      }
      if (Slot2z3 == HIGH)
      {
        digitalWrite(ZonE1, HIGH);
        digitalWrite(ZonE1, HIGH);
        Serial.println("Second slot ZONE3 is running ");
      }
    }
    if ((offtimehr == RtcHour) && (offtimemin == RtcMin))
    {
      Serial.println("Second slot is Completed ");
      digitalWrite(MainPumpA, LOW);
      digitalWrite(FertSolenoid, LOW);
      digitalWrite(ZonE1, LOW);
      digitalWrite(ZonE1, LOW);
    }
  }
  esp_task_wdt_reset();
}
