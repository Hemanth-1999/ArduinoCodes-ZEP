//THIS FUNCTION IS USED IN MANUAL MODE
void EepromReadZ1AP()
{
  Z1PASThr = EEPROM.read(EepromZ1PASThr);
  Z1PASTmin = EEPROM.read(EepromZ1PASTmin);
  Z1PASPhr = EEPROM.read(EepromZ1PASPhr);
  Z1PASPmin = EEPROM.read(EepromZ1PASPmin);
}

//THIS FUNCTION IS USED IN MANUAL MODE
void EepromReadZ1APq()
{
  Z1PASThrq = EEPROM.read(Eepromsch8hr);
  Z1PASTminq = EEPROM.read(Eepromsch8min);
  Z1PASPhrq = EEPROM.read(Eepromsch8hr1);
  Z1PASPminq = EEPROM.read(Eepromsch8min1);
}
//THIS FUNCTION IS USED IN MANUAL MODE
void EepromReadZ1APqq()
{
  Z1PASThrqq = EEPROM.read(Eepromsch9hr1);
  Z1PASTminqq = EEPROM.read(Eepromsch9min);
  Z1PASPhrqq = EEPROM.read(Eepromsch9hr1);
  Z1PASPminqq = EEPROM.read(Eepromsch9min1);
}
//THIS FUNCTION IS USED IN MANUAL MODE
void EepromReadZ1PB()
{
  Z1PBSThr = EEPROM.read(EepromZ1PBSThr);
  Z1PBSTmin = EEPROM.read(EepromZ1PBSTmin);
  Z1PBSPhr = EEPROM.read(EepromZ1PBSPhr);
  Z1PBSPmin = EEPROM.read(EepromZ1PBSPmin);

}
//THIS FUNCTION IS USED IN MANUAL MODE
void EepromReadZ1Air()
{
  Z1APSThr = EEPROM.read(EepromZ1APSThr);
  Z1APSTmin = EEPROM.read(EepromZ1APSTmin);
  Z1APSPhr = EEPROM.read(EepromZ1APSPhr);
  Z1APSPmin = EEPROM.read(EepromZ1APSPmin);

}
