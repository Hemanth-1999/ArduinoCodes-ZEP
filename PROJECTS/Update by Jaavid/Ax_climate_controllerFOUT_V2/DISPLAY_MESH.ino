/*
this is not implemented in display
*/
void meshpass() 
{
  char *pd = strtok(buff, ">");
  str = pd;
  int vale = str.indexOf("<") + 1;
  str.remove(0, vale);//meshpass
  String mstr = str;
  Serial.print("Mesh Pswd: ");
  Serial.println(mstr);
}
/*
I DID NOT FIND ABOUT THIS IN THE DISPLAY.
BUT WHAT EVER IT IS READING FROM DISPLAY IT IS STORING IN str1 WHICH IS A LOCAL VARIABLE NO USE.
this is not implemented in display.
*/
void prefix() 
{
  char *pt = strtok(buff, "<");
  String str1 = pt;
  str1.remove(0, 4);
  str1 = str1;
  Serial.print("Prefix: ");
  Serial.println(str1);
}
/*
I DID NOT FIND ABOUT THIS IN THE DISPLAY.
THE FINAL RESULT IS STORED IN GLOBAL STRING dfd
this is not implemented in display.
*/
void port() 
{
  int vale = dfd.indexOf(">") + 1;
  dfd.remove(0, vale);//port
  Serial.print("Port: ");
  Serial.println(dfd);
}
