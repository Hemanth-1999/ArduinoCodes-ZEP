void page1()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RSS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(4, 0, currentrs);
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
  u8x8.print(AT, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("RH:");
  u8x8.setCursor(5, 5);
  u8x8.print(RH, 1);
  u8x8.print("%");

  u8x8.setCursor(0, 6);
  u8x8.print("BP:");
  u8x8.setCursor(5, 6);
  u8x8.print(BP, 1);
  u8x8.print(" hPa");
}

void page2()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RSS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(4, 0, currentrs);
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
  u8x8.print(DP, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 6);
  u8x8.print("Sl:");
  u8x8.setCursor(5, 6);
  u8x8.print(SL, 1);
  u8x8.print(" hPa");
}

void page3()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RSS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(4, 0, currentrs);
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
  u8x8.print(Co2);
  u8x8.print(" ppm");

  u8x8.setCursor(0, 5);
  u8x8.print("ET:");
  u8x8.setCursor(5, 5);
  u8x8.print(ET, 1);
  u8x8.print(" mm");

  u8x8.setCursor(0, 6);
  u8x8.print("SR:");
  u8x8.setCursor(5, 6);
  u8x8.print(SR, 0);
  u8x8.print(" Wh/m²");
}

void page4()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(0, 0, "RSS:");
  receivedRssi = LoRa.packetRssi();
  char currentrs[64];
  receivedRssi.toCharArray(currentrs, 64);
  u8x8.drawString(4, 0, currentrs);
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
  u8x8.print(VPD, 1);
  u8x8.print(" kPa");

}

void LoRafailed()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("LoRa failed!");
}

void displayData() {

  processData();

  if (!LoRa.begin(433E6)) {
    // LoRafailed
    u8x8.clearDisplay();
    LoRafailed();
    delay(5000); // Add delay to view the page
    while (1);
  } else {
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
  }
}
