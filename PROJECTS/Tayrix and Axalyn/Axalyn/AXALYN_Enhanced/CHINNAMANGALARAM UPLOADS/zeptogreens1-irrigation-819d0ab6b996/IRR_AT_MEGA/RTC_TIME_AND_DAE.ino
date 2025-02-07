void read_date_time_rtc()
{
  if (!rtc.begin()) 
  {
    Serial.println("UNABLE TO FIND RTC MODULE, SO YOU CAN NOT READ DATE AND TIME FROM RTC MODULE");
    return;
  }
  // Read the current time from RTC
  DateTime now = rtc.now();
  Rtc_hour = now.hour();
  Rtc_min = now.minute();
  Rtc_sec = now.second();
  Rtc_day = now.day();
  Rtc_month = now.month();
  Rtc_year = now.year();
  Serial.println("DATE AND TIME HAS BEEN READ FROM RTC MODULE");
}
void update_rtc_time()
{
  if(!rtc.begin())
  {
    Serial.println("SORRY WE COULD NOT FIND RTC MODULE SO WE ARE UNABLE TO ADD DATE AND TIME MANUALLY.");
    return;
  }
  rtc.adjust(DateTime(Yearrc, Monthrc, Daterc, hourrc, minrc, secsrc));
  Serial.println("WE HAVE UPDATED THE TIME AND DATE");
}
