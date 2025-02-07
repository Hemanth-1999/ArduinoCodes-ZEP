#include <NTPClient.h>
// change next line to use with another board/shield
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Cenaura";
const char *password = "Centech#2024";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(19800);
}

void loop() {
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  //  Serial.print("Epoch Time: ");
  //  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  //  Serial.print("Formatted Time: ");
  //  Serial.println(formattedTime);

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute);

  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);

  String weekDay = weekDays[timeClient.getDay()];
  //  Serial.print("Week Day: ");
  //  Serial.println(weekDay);

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon + 1;
  //  Serial.print("Month: ");
  //  Serial.println(currentMonth);

  String currentMonthName = months[currentMonth - 1];
  //  Serial.print("Month name: ");
  //  Serial.println(currentMonthName);

  int dayOfYear = ptm->tm_yday + 1;
  Serial.print("Day of Year: ");
  Serial.println(dayOfYear);

  int currentYear = ptm->tm_year + 1900;
  //  Serial.print("Year: ");
  //  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");

  delay(2000);
}
