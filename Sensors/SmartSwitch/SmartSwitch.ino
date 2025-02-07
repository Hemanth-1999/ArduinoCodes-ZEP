# define Light1 12
# define Light2 13
# define Light3 14

#define ms_per_hour  3600000
#define ms_per_min    60000
#define ms_per_sec    1000

long OnHrDelay  = 18;
long OnMinDelay = 1;

long OnHrDelayconvOn  = OnHrDelay * ms_per_hour;
long OnMinDelayconvOn = OnMinDelay * ms_per_min;
long Zone1TotalOnTime = OnHrDelayconvOn + OnMinDelayconvOn;

long OffHrDelay  = 18;
long OffMinDelay = 1;

long OffHrDelayconvOff  = OffHrDelay * ms_per_hour;
long OffMinDelayconvOff = OffMinDelay * ms_per_min;
long Zone1TotalOffTime = OffHrDelayconvOff + OffMinDelayconvOff;

void setup() {
  Serial.begin(9600);
  pinMode(Light1, OUTPUT);
  digitalWrite(Light1, HIGH);

  pinMode(Light2, OUTPUT);
  digitalWrite(Light2, HIGH);

  pinMode(Light3, OUTPUT);
  digitalWrite(Light3, HIGH);

}
void onfun()
{
  Serial.println("Light1 On");
  pinMode(Light1, OUTPUT);
  digitalWrite(Light1, LOW);

  Serial.println("Light2 On");
  pinMode(Light2, OUTPUT);
  digitalWrite(Light2, LOW);

  Serial.println("Light3 On");
  pinMode(Light3, OUTPUT);
  digitalWrite(Light3, LOW);
}

void offfun()
{
  Serial.println("Light1 Off");
  pinMode(Light1, OUTPUT);
  digitalWrite(Light1, HIGH);

  Serial.println("Light2 Off");
  pinMode(Light2, OUTPUT);
  digitalWrite(Light2, HIGH);

  Serial.println("Light3 Off");
  pinMode(Light3, OUTPUT);
  digitalWrite(Light3, HIGH);
}
void loop() 
{
  onfun();
  delay(OnMinDelayconvOn);
  offfun();
  delay(OffMinDelayconvOff);

}
