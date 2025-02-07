void page1()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 2);
  u8x8.print(ESP.getChipModel());

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(0, 4);
  u8x8.print("AT:");
  u8x8.setCursor(5, 4);
  u8x8.print(temp, 1);
  u8x8.print(" Deg C");

  u8x8.setCursor(0, 5);
  u8x8.print("RH:");
  u8x8.setCursor(5, 5);
  u8x8.print(hum, 1);
  u8x8.print("%");

  u8x8.setCursor(0, 6);
  u8x8.print("BP:");
  u8x8.setCursor(5, 6);
  u8x8.print(pres, 1);
  u8x8.print(" hPa");
}

void BMEData() {
  printBME280Data(&Serial);
}

void OLED() {
  //  SendDatatoLoRa();
  BMEData();    // Ensure sensor data is read before displaying

  // Page 1
  u8x8.clearDisplay();
  page1();
  delay(3000); // Add delay to view the page
}
