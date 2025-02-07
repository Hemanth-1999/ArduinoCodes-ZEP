void page1()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(3, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  if (battry >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battry >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battry >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battry >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("AT:");
  u8x8.setCursor(5, 4);
  u8x8.print(ES_AT, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("RH:");
  u8x8.setCursor(5, 5);
  u8x8.print(ES_RH, 1);
  u8x8.print("%");

  u8x8.setCursor(0, 6);
  u8x8.print("BP:");
  u8x8.setCursor(5, 6);
  u8x8.print(ES_BP, 1);
  u8x8.print(" hPa");
}

void page2()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(3, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  if (battry >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battry >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battry >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battry >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("HI:");
  u8x8.setCursor(5, 4);
  u8x8.print(HI, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("Dep:");
  u8x8.setCursor(5, 5);
  u8x8.print(dewpt, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 6);
  u8x8.print("Sl:");
  u8x8.setCursor(5, 6);
  u8x8.print(sl, 1);
  u8x8.print(" hPa");
}

void page3()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(3, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());


  if (battry >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battry >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battry >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battry >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("Co2:");
  u8x8.setCursor(5, 4);
  u8x8.print(co2);
  u8x8.print(" ppm");

  u8x8.setCursor(0, 5);
  u8x8.print("ET:");
  u8x8.setCursor(5, 5);
  u8x8.print(ETo, 1);
  u8x8.print(" mm");

  u8x8.setCursor(0, 6);
  u8x8.print("SR:");
  u8x8.setCursor(5, 6);
  u8x8.print(solarRadiation, 0);
  u8x8.print(" Wh/m²");
}

void page4()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(3, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  if (battry >= 80) {
    u8x8.drawString(10, 0, "[###]");  // Fully charged or nearly full
  } else if (battry >= 60) {
    u8x8.drawString(10, 0, "[## ]");  // 60% to 79% charged
  } else if (battry >= 40) {
    u8x8.drawString(10, 0, "[#  ]");  // 40% to 59% charged
  } else if (battry >= 20) {
    u8x8.drawString(10, 0, "[   ]");  // 20% to 39% charged
  } else {
    u8x8.drawString(10, 0, "[!  ]");  // Less than 20%, needs charging
  }

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("VPD:");
  u8x8.setCursor(5, 4);
  u8x8.print(VPDcal(), 1);
  u8x8.print(" kPa");
  u8x8.setCursor(0, 6);
  u8x8.print("LoRa Sending...");

}

void LoRaData1()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "PacketRS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(9, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(message1);
  u8x8.setCursor(0, 3);
  u8x8.print(message2);
  u8x8.setCursor(0, 4);
  u8x8.print(message3);
  u8x8.setCursor(0, 5);
  u8x8.print(message4);
  u8x8.setCursor(0, 6);
  u8x8.print(message5);
}

void LoRaData2()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "PacketRS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(9, 0, currentrs);
  u8x8.setCursor(0, 2);
  u8x8.print(message6);
  u8x8.setCursor(0, 3);
  u8x8.print(message7);
  u8x8.setCursor(0, 4);
  u8x8.print(message8);
  u8x8.setCursor(0, 5);
  u8x8.print(message9);
  u8x8.setCursor(0, 6);
  u8x8.print(message10);
}

void LoRafailed()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("LoRa failed!");
}

void BMEData() {
  printBME280Data(&Serial);
}

void OLED() {

  NPKData();
  BMEData();    // Ensure sensor data is read before displaying
  ETcalcul();  // Update ET calculation

  // Page 1
  u8x8.clearDisplay();
  page1();
  delay(5000); // Add delay to view the page

  // Page 2
  u8x8.clearDisplay();
  page2();
  delay(5000);

  // Page 3
  u8x8.clearDisplay();
  page3();
  delay(5000);

  // Page 4
  u8x8.clearDisplay();
  page4();
  delay(5000);

  if (!LoRa.begin(433E6)) {
    // LoRafailed
    u8x8.clearDisplay();
    LoRafailed();
    delay(5000); // Add delay to view the page
    while (1);
  } else {
    // LoRaData1
    u8x8.clearDisplay();
    LoRaData1();
    delay(5000); // Add delay to view the page
    // LoRaData1
    u8x8.clearDisplay();
    LoRaData2();
    delay(5000); // Add delay to view the page
  }
}
