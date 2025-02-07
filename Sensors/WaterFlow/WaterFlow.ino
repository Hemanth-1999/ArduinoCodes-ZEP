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

int sensorPin = 2;
volatile long pulse;
unsigned long lastTime;

float volume;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);

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

ICACHE_RAM_ATTR void increase() {
  pulse++;
  volume = 2.663 * pulse / 1000 * 30;
  if (millis() - lastTime > 2000) {
    pulse = 0;
    lastTime = millis();
  }
  Serial.print(volume);
  Serial.println(" L/m");
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long SCpreviousMillis = 0;
  if (currentMillis - SCpreviousMillis >= 500) {
    SCpreviousMillis = currentMillis;
    increase();
  }
  unsigned long ATcurrentMillis = millis();
  unsigned long ATpreviousMillis = 0;
  if (ATcurrentMillis - ATpreviousMillis >= 1000) {
    ATpreviousMillis = ATcurrentMillis;
    onfun();
    delay(OnMinDelayconvOn);
    offfun();
    delay(OffMinDelayconvOff);
  }
}
