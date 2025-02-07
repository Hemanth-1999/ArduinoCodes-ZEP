#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <cmath> // Include cmath for mathematical functions

const char* ssid = "Cenaura";
const char* password = "Centech#2024";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // 19800 seconds for Indian Standard Time

float altitudeAngle;
float solarRadiation;

const float latitude = 17.388333;  // Your latitude
const float longitude = 78.230630; // Your longitude
const float maxRadiation = 1000.0; // Max solar radiation at zenith (clear sky conditions)

// Define M_PI if it's not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function prototypes
float calculateSolarAltitude(int currentHour, int currentMinute, int dayOfYear);
float estimateSolarRadiation(float altitudeAngle);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  timeClient.begin();
}

void loop() {
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime(&epochTime);

  int currentHour = ptm->tm_hour;
  int currentMinute = ptm->tm_min;
  int dayOfYear = ptm->tm_yday + 1;

  Serial.print("Hour: ");
  Serial.println(currentHour);
  Serial.print("Minutes: ");
  Serial.println(currentMinute);
  Serial.print("Day of Year: ");
  Serial.println(dayOfYear);

  altitudeAngle = calculateSolarAltitude(currentHour, currentMinute, dayOfYear);
  Serial.print("Calculated Altitude Angle: ");
  Serial.println(altitudeAngle);

  solarRadiation = estimateSolarRadiation(altitudeAngle);

  Serial.print("Estimated Solar Radiation: ");
  Serial.print(solarRadiation);
  Serial.println(" W/m²");

  float averageDailyRadiation = calculateDailyAverageSolarRadiation();
  Serial.print("Average Daily Solar Radiation: ");
  Serial.print(averageDailyRadiation);
  Serial.println(" Wh/m²"); // Assuming you want the result in Wh/m²

  delay(2000); // Adjust delay if necessary
}

float calculateSolarAltitude(int currentHour, int currentMinute, int dayOfYear) {
  float declination = 23.44 * sin((360.0 / 365.0) * (dayOfYear - 81) * (M_PI / 180.0));
  float solarTime = (currentHour + (currentMinute / 60.0)) - 12.0;
  float hourAngle = (solarTime * 15.0) - (longitude / 15.0);
  float latitudeRad = latitude * (M_PI / 180.0);
  float declinationRad = declination * (M_PI / 180.0);
  float hourAngleRad = hourAngle * (M_PI / 180.0);

  float altitude = asin(sin(declinationRad) * sin(latitudeRad) +
                        cos(declinationRad) * cos(latitudeRad) * cos(hourAngleRad)) * (180.0 / M_PI);

  return altitude;
}

float estimateSolarRadiation(float altitudeAngle) {
  if (altitudeAngle > 0) {
    float radiation = maxRadiation * pow(sin(altitudeAngle * (M_PI / 180.0)), 2); // Adjust calculation
    return fmax(0.0f, radiation); // Ensure non-negative using fmax
  }
  return 0.0f; // Return 0 if the altitude angle is <= 0
}

float calculateDailyAverageSolarRadiation() {
  float totalRadiation = 0.0;
  int minutesPerInterval = 15; // Calculate every 15 minutes
  int intervalsPerDay = (24 * 60) / minutesPerInterval; // Total number of intervals in a day
  int dayOfYear = timeClient.getDay() + 1; // Get the actual day of the year from the NTP client

  for (int hour = 0; hour < 24; hour++) {
    for (int minute = 0; minute < 60; minute += minutesPerInterval) {
      altitudeAngle = calculateSolarAltitude(hour, minute, dayOfYear); // Update global variable
      solarRadiation = estimateSolarRadiation(altitudeAngle); // Update global variable
      totalRadiation += solarRadiation * (minutesPerInterval / 60.0); // Convert to hours
    }
  }

  float averageRadiation = totalRadiation / 24.0; // Average radiation per hour
  
  return averageRadiation;
}
