#include <ArduinoJson.h>
#include <RTClib.h>

void Pinmodes();
void alloff();
void alloon();

//**********************************RTC MODULE RELATE *******************************************
RTC_DS3231 rtc;
int Rtc_hour;
int Rtc_min;
int Rtc_sec;
int Rtc_day;
int Rtc_month;
int Rtc_year;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
void update_rtc_time();//THIS FUNCTION IS USED TO SET TIME AND DATE TO RTC MODULE MANUALLY.
void read_date_time_rtc();//THIS FUNCTION WILL READ CURRENT DATE AND TIME FROM RTC MODULE.

//*******************************MILLIS GLOBAL VARIABLES *********************************

unsigned long SCpreviousMillis = 0;
unsigned long ESPpreviousMillis = 0;
unsigned long ATpreviousMillis = 0;
unsigned long VPpreviousMillis = 0;
unsigned long nextionmills = 0;
unsigned long manualsend = 0;

//*******************************DISPLAY VARIABLES *********************************

void callSerialCom();
void SerialCom(String frommob);
String dfd="";//THIS IS USED TO STORE INCOMING DATA FROM DISPLAY.
char buff[200];//THIS IS USED TO STORE INCOMING DATA FROM DISPLAY.
String feedBack;

  //*************************TEMPERATURE PAGE CREDENTIALS********************************
  
  void readtempvalues();//THIS FUNCTION READE VALUES FROM TEMPERATURE PAGE AND STORE IN GLOBAL VARIABLES.
  float vpdtemp_I();//THIS FUNCTION EXTRACT ONLY INDOOR TEMP FROM MAIN STRING AND RETURN IT.
  float vpdtemp_O();//THIS FUNCTION EXTRACT ONLY OUTDOOR TEMP FROM MAIN STRING AND RETURN IT.
  
  float SPtemp;//THIS WILL STORE INDOOR TEMP VALUE FROM TEPERATURE PAGE OF DISPLAY.
  float ODtemp;////THIS WILL STORE OUTDOOR TEMP VALUE FROM TEPERATURE PAGE OF DISPLAY.
  
     //**************************AUTO TEMPERATURE MODE*****************************************
     
     void autotenpmode(bool stat);//THIS FUNCTION WILL READ THE BUTTON(AUTO TEMPERATURE) STATUS FROM DISPLAY AND UPDATE THE STATUS OF GLOBAL VARIABLES.
     bool TemperatureAutoModeOn;//THIS WILL BE TRUE IF AUTO TEMP BUTTON IN TEMPERATURE PAGE IS ON. THIS WILL BE FALSE IT AUTO TEMP BUTTON IN TEMPERATURE PAGE IS FALSE.
     bool temp_func;//THIS WILL BE TRUE IF AUTO TEMP BUTTON IN TEMPERATURE PAGE IS ON. THIS WILL BE FALSE IT AUTO TEMP BUTTON IN TEMPERATURE PAGE IS FALSE.
   
  //************************WIFI PAGE CREDENTIALS***************************************
  
  void ssid_password();//THIS WILL READ SSID AND PASSWORD FROM DISPLAY AND STORE IN SOME GLOBAL VARIABLES.
  String  pwdname();//THIS FUN USES char buff[200] VARIABLE TO SSID NAME AND RETURN IT.
  String pwd();// THIS FUNCTION EXTRACT PASSWORD AND RETURN IT.
  
  String str="";//USED TO STORE DISPLAY WIFI PAGE SSID.
  String ssid="";//USED TO STORE DISPLAY WIFI PAGE SSID.
  String password="";//USED TO STORE DISPLAY WIFI PAGE PASSWORD.
  
  //************************VPD PAGE CREDENTIALS****************************************
  
  void vpdpage(bool stat);//THIS FUNCTION WILL READ THE BUTTON(VPD) STATUS FROM DISPLAY AND UPDATE THE STATUS OF GLOBAL VARIABLES.
  bool VpdautoModeOn;//THIS WILL BE TRUE IF VPD PAGE BUTTON IS ON AND IT WILL BE FALSE IF VPD PAGE BUTTON IS OFF.THIS IS ON touch EVENT
  bool vpd_func;//THIS WILL BE FALSE IF VPD PAGE BUTTON IS ON.THIS IS ON touch EVENT
  //********************************MANUAL MODE **********************************************************

  void manual(byte k);//THIS ON AND PARTICULAT RELAY BASED ON INPUT.
  
  //***************************************DATE AND TIME PAGE***************************************************

  void read_date_time_display();//THIS FUNCTION IS USED TO READ DATE AND TIME FROM THE DISPLY WHICH ARE ENTERED BY USER IN DIPLAY, AFTER ENTERING USER WILL PRESS SAVE OPTION IN DISPLAY THEN DISPLAY WILL SEND THIS DATA TO AT MEGA VIA SERIAL.
  
  void timeHr();//this function will read hours from the display and store it in below variable.
  uint16_t hourrc;
  
  void timeMin();//this function will read minutes from the display and store it in below variable.
  uint16_t minrc;
  
  void timeSec();//this function will read SECONDS from the display and store it in below variable.
  uint16_t secsrc;
  
  void timeMnt();//this function will read MONTH from the display and store it in below variable.
  uint16_t Monthrc;
  
  void timeDt();//this function will read DATE from the display and store it in below variable.
  uint16_t Daterc;

  void timeYr();//this function will read year from the display and store it in below variable.
  int Yearrc;

  
  void update_display_time();//THIS FUNCTION WILL WRITE DATE AND TIME TO DISPLAY.

  
 //******************************MESH(THIS IS NOT IMPLEMENTED IN DISPLAY)***************************
 
// void meshpass();//THIS FUNCTION
// void prefix();
// void port();
//******************************************************ATMEGA  TEMPETRATURE AUTO *************************

int count3 = 10;
extern volatile unsigned long timer0_millis;
void AutoTemp_control(float SPtemp);

//******************************************************ATMEGA  VPD AUTO *************************

int count1 = 5;
void vpd_control(float MESP_ID_Time_AvgVPD_v);

//******************************************************ATMEGA  MANUAL AUTO *************************

int f1,f2,f3,f4,f5,f6,f7,f8,cp,fp,fs,co,ir,doo,dr,pb;
int counterm;
void manualread();
void counterun();

//**********************************************CREDENTIALS FROM ESP8266 or NODE*******************************
//inside node values
float _AT;
float _HUM;
float _VPD;
float _HI;
float _CO2;
float _ET;
float _BP;
float _UV;
float _DP;
float _SL;

//out side node values
float O_AT;
float O_HUM;
float O_VPD;
float O_HI;
float O_CO2;
float O_ET;
float O_BP;
float O_UV;
float O_DP;
float O_SL;


//***********************************RELAYS************************************

#define ExFan1  A3
#define ExFan2  A2
#define ExFan3  30
#define ExFan4  28
#define ExFan5    8
#define ExFan6    9
#define ExFan7   10
#define ExFan8   11
#define pad1      A4
#define Fogger    A6
#define FoggerSol A5
#define Co2Gen    A7

//*************************************************MODUAL ON/OFF*********************************

#define TEST DISPLAY
    #define TEST_DISPLAY_WIFI_PAGE
    #define TEST_DISPLAY_TEMP_PAGE
    #define TEST_DISPLAY_AUTO_TEMP
    #define TEST_DISPLAY_VPD_PAGE
    #define TEST_DISPLAT_TIME
    #define TEST_DISPLAY_MANUAL_MODE
#define TEST_NODE_DATA
