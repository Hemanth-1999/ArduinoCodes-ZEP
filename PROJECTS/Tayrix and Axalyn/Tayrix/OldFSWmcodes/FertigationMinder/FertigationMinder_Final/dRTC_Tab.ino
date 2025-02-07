/**************************************************************
 * Function Name: Rtctab
 * Description: [This function is used for maintaing hardware rtc time up to date by syncing it NTP client]
 * Output: [Updated time]
 **************************************************************/
void Rtctab()
{
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  DateTime now = rtc.now(); 
  RtcHour = now.hour(); 
  RtcMin = now.minute();
 String formattedTime = String(now.hour()) + ":" + formatDigits(now.minute()) + ":" + formatDigits(now.second());
  if(formattedTime!=timeClient.getFormattedTime())
  {
    Serial.println("RTC synced with NTP Client");
  rtc.adjust(DateTime(timeClient.getEpochTime()));
}
}
String formatDigits(int digits) {
  if (digits < 10) {
    return "0" + String(digits);
  }
  return String(digits);
}
