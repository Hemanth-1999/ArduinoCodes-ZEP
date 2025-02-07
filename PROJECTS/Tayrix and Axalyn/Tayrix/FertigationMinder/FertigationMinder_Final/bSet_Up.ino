void resetWatchdogTask(void* pvParameters) {
  while (true) {
    // Reset the watchdog timer at regular intervals
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
  }
}
void startWatchdogTask() {
  xTaskCreatePinnedToCore(resetWatchdogTask, "Watchdog Task", 2048, NULL, 1, &watchdogTask, 0);
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);
  delay(2000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS file system");
    return;
  }
  startup();
  readsavedvalues();
  while (!Serial);
  Serial.flush();
  EEPROM.begin(150);
  SPI.begin();
  u8g2.begin();
  pinMode(encBtn, INPUT_PULLUP);
  x = x + scroll_direction;
  if (x > 40) scroll_direction = -1;
  if (x < 1) scroll_direction = 1;
  u8g2.setFont(fontName);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  nav.idleTask = idle;
  Serial.println("setup done.");
  Serial.flush();
  pinMode(MainPumpA, OUTPUT);
  digitalWrite( MainPumpA, LOW);
  pinMode(FertSolenoid, OUTPUT);
  digitalWrite( FertSolenoid, LOW);
  pinMode(ZonE1, OUTPUT);
  digitalWrite( ZonE1, HIGH);
  pinMode(ZonE2, OUTPUT);
  digitalWrite( ZonE2, HIGH);
  Serial.println("FertigationMinder LORA");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  if (!rtc.begin()) {
    Serial.println("RTC initialization failed!");
    while (1);
  }
  timeClient.begin();
  timeClient.update();
  esp_task_wdt_init(watchdogTimeout, true);
  startWatchdogTask();
}
unsigned long Autoandmanual = 0;
unsigned long SSSchedule = 0;
unsigned long Loracall = 0;
void loop() {
  /*==============================================
    For Continues loops
    ==============================================*/
  onReceive(LoRa.parsePacket());
  if (wm_nonblocking) wm.process();
  checkButton();
  Encoderrun();
  /*==============================================
    For Manual and Encoder
    ==============================================*/

  unsigned long currentMillis = millis();
  if (currentMillis - Autoandmanual >= 1000)
  {
    Manualrun();
    Autoandmanual = currentMillis;
  }
  /*==============================================
    For Scheduling
    ==============================================*/
  unsigned long CurrentMillis = millis();
  if (CurrentMillis - SSSchedule >= 5000)
  {
    if ((Schedulesstate == HIGH) || (Schedulesstate2 == HIGH))
    {
      Rtctab();
      CallScheduling();
    }
    SSSchedule = CurrentMillis;

  }
  /*==============================================
    For Lora Communication
    ==============================================*/
  unsigned long loracurrentmillis = millis();
  if (loracurrentmillis - Loracall >= 2000)
  {
    String message = "10";
    sendMessage(message, SoilMinder, MonolineMaster);
    Serial.println(message);
    Loracall = loracurrentmillis;
  }
}
