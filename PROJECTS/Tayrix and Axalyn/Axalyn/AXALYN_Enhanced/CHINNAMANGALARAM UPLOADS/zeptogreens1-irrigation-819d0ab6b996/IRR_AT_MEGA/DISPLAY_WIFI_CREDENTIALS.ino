String  pwdname() 
{
  char *pd = strtok(buff, "<");
  String str = pd;
  str.remove(0, 4);
  str = str;
  return (str);
}
String pwd() 
{
  int vale = dfd.indexOf("<") + 1;
  dfd.remove(0, vale);
  return (dfd);
}
