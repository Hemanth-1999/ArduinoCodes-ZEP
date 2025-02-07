void page1()
{
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 14);
  u8g2.print(ESP.getChipModel());
  if (a > 0)
  {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();
  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);
    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 35);
  u8g2.print("AT");
  u8g2.setCursor(40, 35);
  u8g2.print(":");
  u8g2.setCursor(50, 35);
  u8g2.print(ES_AT, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 49);
  u8g2.print("RH");
  u8g2.setCursor(40, 49);
  u8g2.print(":");
  u8g2.setCursor(50, 49);
  u8g2.print(ES_RH, 1);
  u8g2.print("%");
  u8g2.setCursor(0, 63);
  u8g2.print("P");
  u8g2.setCursor(40, 63);
  u8g2.print(":");
  u8g2.setCursor(50, 63);
  u8g2.print(ES_BP, 1);
  u8g2.print("hPa");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();

}

void page2()
{
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 14);
  u8g2.print(ESP.getChipModel());
  if (a > 0)
  {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();
  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);
    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 35);
  u8g2.print("HI");
  u8g2.setCursor(45, 35);
  u8g2.print(":");
  u8g2.setCursor(55, 35);
  u8g2.print(HI, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 49);
  u8g2.print("Dep");
  u8g2.setCursor(45, 49);
  u8g2.print(":");
  u8g2.setCursor(55, 49);
  u8g2.print(dewpt, 1);
  u8g2.print("°C");
  u8g2.setCursor(0, 63);
  u8g2.print("Sl");
  u8g2.setCursor(45, 63);
  u8g2.print(":");
  u8g2.setCursor(55, 63);
  u8g2.print(sl, 1);
  u8g2.print("hPa");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
}

void page3()
{
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 14);
  u8g2.print(ESP.getChipModel());
  if (a > 0)
  {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();
  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);
    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 35);
  u8g2.print("Co2");
  u8g2.setCursor(45, 35);
  u8g2.print(":");
  u8g2.setCursor(55, 35);
  u8g2.print(co2);
  u8g2.print("ppm");
  u8g2.setCursor(0, 49);
  u8g2.print("ET");
  u8g2.setCursor(45, 49);
  u8g2.print(":");
  u8g2.setCursor(55, 49);
  u8g2.print(ETo, 1);
  u8g2.print("mm");
  u8g2.setCursor(0, 63);
  u8g2.print("SR");
  u8g2.setCursor(45, 63);
  u8g2.print(":");
  u8g2.setCursor(55, 63);
  u8g2.print(uvIntensity * 10, 1);
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.print("nm");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
}

void page4()
{
  // delay(1000);
  //u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0, 14);
  u8g2.print(ESP.getChipModel());
  if (a > 0)
  {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57882);
  }
  else {
    u8g2.setCursor(66, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(66, 14, 57879);
  }
  u8g2.sendBuffer();
  u8g2.setCursor(86, 14);
  u8g2.print(battry);
  u8g2.print("%");

  if ((battry >= 0) && (battry < 33))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57932);
    u8g2.sendBuffer();
  }
  else if ((battry >= 33) && (battry < 66))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57937);
    u8g2.sendBuffer();
  }
  else if ((battry >= 66) && (battry <= 100))
  {
    u8g2.setCursor(115, 14);
    u8g2.setFont(u8g2_font_siji_t_6x10);
    u8g2.drawGlyph(115, 14, 57940);
    u8g2.sendBuffer();
  }

  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.setCursor(0, 35);
  u8g2.print("VPD");
  u8g2.setCursor(40, 35);
  u8g2.print(":");
  u8g2.setCursor(50, 35);
  u8g2.print(VPDcal(), 1);
  u8g2.print("");
  u8g2.print("kPa");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();

}

void BMEData() {
  printBME280Data(&Serial);
}

void OLED()
{
  BMEData();
  UVA();
  ETcalcul();
  co2 = 00;
  page1();
  page2();
  page3();
  page4();

}
