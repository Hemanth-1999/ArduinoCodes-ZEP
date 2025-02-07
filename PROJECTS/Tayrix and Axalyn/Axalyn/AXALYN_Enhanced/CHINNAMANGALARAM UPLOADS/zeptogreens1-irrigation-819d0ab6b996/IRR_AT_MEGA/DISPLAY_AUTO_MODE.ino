void Z1PAsavetime() //SAVE ON TIME
{
  Z1PASThr = othr();
  Z1PASTmin = otmin();
  EEPROM.update(EepromZ1PASThr, Z1PASThr);
  EEPROM.update(EepromZ1PASTmin, Z1PASTmin);
}

void Z1PAsavedue() //SAVE OFF TIME
{
  Z1PASPhr = ofthr();
  Z1PASPmin = oftmin();
  EEPROM.update(EepromZ1PASPhr, Z1PASPhr);
  EEPROM.update(EepromZ1PASPmin, Z1PASPmin);
}


void Z1PAsavetimeq()
{

  Z1PASThrq = othr();
  Z1PASTminq = otmin();
  EEPROM.update(Eepromsch8hr, Z1PASThrq);
  EEPROM.update(Eepromsch8min, Z1PASTminq);

}

void Z1PAsavedueq()
{
  Z1PASPhrq = ofthr();
  Z1PASPminq = oftmin();
  EEPROM.update(Eepromsch8hr1, Z1PASPhrq);
  EEPROM.update(Eepromsch8min1, Z1PASPminq);
}

void Z1PAsavetimeqq() 
{

  Z1PASThrqq = othr();
  Z1PASTminqq = otmin();
  EEPROM.update(Eepromsch9hr, Z1PASThrqq);
  EEPROM.update(Eepromsch9min, Z1PASTminqq);
}
void Z1PAsavedueqq() 
{
  Z1PASPhrqq = ofthr();
  Z1PASPminqq = oftmin();

  EEPROM.update(Eepromsch9hr1, Z1PASPhrqq);
  EEPROM.update(Eepromsch9min1, Z1PASPminqq);
}

void Z1PBsavetime() 
{
  Z1PBSThr = othr();
  Z1PBSTmin = otmin();
  EEPROM.update(EepromZ1PBSThr, Z1PBSThr);
  EEPROM.update(EepromZ1PBSTmin, Z1PBSTmin);
}
void Z1PBsavedue() 
{
  Z1PBSPhr = ofthr();
  Z1PBSPmin = oftmin();
  EEPROM.update(EepromZ1PBSPhr, Z1PBSPhr);
  EEPROM.update(EepromZ1PBSPmin, Z1PBSPmin);
}

void Z1APsavetime() 
{
  Z1APSThr = othr();
  Z1APSTmin = otmin();
  EEPROM.update(EepromZ1APSThr, Z1APSThr);
  EEPROM.update(EepromZ1APSTmin, Z1APSTmin);

}
void Z1APsavedue() 
{
  Z1APSPhr = ofthr();
  Z1APSPmin = oftmin();
  EEPROM.update(EepromZ1APSPhr, Z1APSPhr);
  EEPROM.update(EepromZ1APSPmin, Z1APSPmin);
}

void LBPAsavedue() 
{
  LBPASPhr = ofthr();
  LBPASPmin = oftmin();
  EEPROM.update(EepromLBPASPhr, LBPASPhr);
  EEPROM.update(EepromLBPASPmin, LBPASPmin);
}
void LBPAsavetime() 
{
  LBPASThr = othr();
  LBPASTmin = otmin1();
  EEPROM.update(EepromLBPASThr, LBPASThr);
  EEPROM.update(EepromLBPASTmin, LBPASTmin);
}
void sch1savetime() 
{
  sch1PSThr = othr();
  sch1PSTmin = otmin();
  EEPROM.update(Eepromsch1hr, sch1PSThr);
  EEPROM.update(Eepromsch1min, sch1PSTmin);
}
void sch1savedue() 
{
  sch1PSThr1 = ofthr();
  sch1PSTmin1 = oftmin1();
  EEPROM.update(Eepromsch1hr1, sch1PSThr1);
  EEPROM.update(Eepromsch1min1, sch1PSTmin1);

}


void sch2savetime() 
{
  sch2PSThr = othr();
  sch2PSTmin = otmin();
  EEPROM.update(Eepromsch2hr, sch2PSThr);
  EEPROM.update(Eepromsch2min, sch2PSTmin);
}
void sch2savedue() 
{
  sch2PSThr1 = ofthr();
  sch2PSTmin1 = oftmin();
  EEPROM.update(Eepromsch2hr1, sch2PSThr1);
  EEPROM.update(Eepromsch2min1, sch2PSTmin1);

}


void sch3savetime() 
{
  sch3PSThr = othr();
  sch3PSTmin = otmin();
  EEPROM.update(Eepromsch3hr, sch3PSThr);
  EEPROM.update(Eepromsch3min, sch3PSTmin);
}
void sch3savedue() 
{
  sch3PSThr1 = ofthr();
  sch3PSTmin1 = oftmin();
  EEPROM.update(Eepromsch3hr1, sch3PSThr1);
  EEPROM.update(Eepromsch3min1, sch3PSTmin1);

}
void sch4savetime() 
{
  sch4PSThr = othr();
  sch4PSTmin = otmin();
  EEPROM.update(Eepromsch4hr, sch4PSThr);
  EEPROM.update(Eepromsch4min, sch4PSTmin);
}
void sch4savedue() 
{
  sch4PSThr1 = ofthr();
  sch4PSTmin1 = oftmin();
  EEPROM.update(Eepromsch4hr1, sch4PSThr1);
  EEPROM.update(Eepromsch4min1, sch4PSTmin1);

}
void sch5savetime() 
{
  sch5PSThr = othr();
  sch5PSTmin = otmin();
  EEPROM.update(Eepromsch5hr, sch5PSThr);
  EEPROM.update(Eepromsch5min, sch5PSTmin);
}
void sch5savedue() 
{
  sch5PSThr1 = ofthr();
  sch5PSTmin1 = oftmin();
  EEPROM.update(Eepromsch5hr1, sch5PSThr1);
  EEPROM.update(Eepromsch5min1, sch5PSTmin1);

}


void sch6savetime() 
{
  sch6PSThr = othr();
  sch6PSTmin = otmin();
  EEPROM.update(Eepromsch6hr, sch6PSThr);
  EEPROM.update(Eepromsch6min, sch6PSTmin);
}
void sch6savedue() 
{
  sch6PSThr1 = ofthr();
  sch6PSTmin1 = oftmin();
  EEPROM.update(Eepromsch6hr1, sch6PSThr1);
  EEPROM.update(Eepromsch6min1, sch6PSTmin1);

}

void sch7savetime() 
{
  sch7PSThr = othr();
  sch7PSTmin = otmin();
  EEPROM.update(Eepromsch7hr, sch7PSThr);
  EEPROM.update(Eepromsch7min, sch7PSTmin);
}
void sch7savedue() 
{
  sch7PSThr1 = ofthr();
  sch7PSTmin1 = oftmin();
  EEPROM.update(Eepromsch7hr1, sch7PSThr1);
  EEPROM.update(Eepromsch7min1, sch7PSTmin1);

}

void sch8savetime() 
{
  sch8PSThr = othr();
  sch8PSTmin = otmin();
  EEPROM.update(Eepromsch8hr, sch8PSThr);
  EEPROM.update(Eepromsch8min, sch8PSTmin);
}
void sch8savedue() 
{
  sch8PSThr1 = ofthr();
  sch8PSTmin1 = oftmin();
  EEPROM.update(Eepromsch8hr1, sch8PSThr1);
  EEPROM.update(Eepromsch8min1, sch8PSTmin1);

}

void sch9savetime() 
{
  sch9PSThr = othr();
  sch9PSTmin = otmin();
  EEPROM.update(Eepromsch9hr, sch9PSThr);
  EEPROM.update(Eepromsch9min, sch9PSTmin);
}
void sch9savedue() 
{
  sch9PSThr1 = ofthr();
  sch9PSTmin1 = oftmin();
  EEPROM.update(Eepromsch9hr1, sch9PSThr1);
  EEPROM.update(Eepromsch9min1, sch9PSTmin1);

}
//*********************helping functions****************
int othr() 
{
  int val = dfd.indexOf("g") + 1;//
  dfd.remove(0, val);
  int otHr = dfd.toInt();
  return (otHr);
}

int otmin() 
{
  char *nt = strtok(buff, "g");//off time hr val
  String str = nt;
  int val = str.indexOf("f") + 1;
  str.remove(0, val);
  int otMin = str.toInt();
  return (otMin);
}

int ofthr() 
{
  char *fr = strtok(buff, "f");
  String str = fr;
  int val = str.indexOf("e") + 1;
  str.remove(0, val);
  int oftHr = str.toInt();
  return (oftHr);
}

int oftmin() 
{
  char *ui = strtok(buff, "e");
  String str = ui;
  str.remove(0, 5);
  int oftMin = str.toInt();
  return (oftMin);
}

int otmin1() 
{
  char *nt = strtok(buff, "g");
  String str = nt;
  int val = str.indexOf("f") ;
  str.remove(0, val);
  int otMin = str.toInt();
  return (otMin);

}
int oftmin1() 
{
  char *ui = strtok(buff, "e");
  String str = ui;
  str.remove(0, 4);
  int oftMin = str.toInt();
  return (oftMin);
}
