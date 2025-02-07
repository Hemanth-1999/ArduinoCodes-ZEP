//**************************DISPLAY WIFI PAGE RELATED CREDENTIALS***************************
/*
THIS FUNCTION IS USED TO READ THE WIFI PAGE CREDENTIALS FROM DISPLAY AND STORE IN GLOBAL VARIABLES.
ssid, str WILL STORE THE SSID FROM THE DISPLAY.
password, dfd WILL STORE PASSWORD FROM THE DISPLAY.

*/
void ssid_password()
{
    ssid = pwdname();
    Serial.print("User Name: ");
    Serial.println(str);
    str = ssid;
    password = pwd();
    Serial.print("Password: ");
    Serial.println(dfd);
    dfd = password;
    feedBack = "sdpd";
}
/*
THIS FUNCTION WILL FIND THE SSID IN THE INCOMING DATA FROM DISPLAY AND RETURN ONLY SSID.
*/
String  pwdname() 
{
  char *pd = strtok(buff, "<");
  str = pd;
  str.remove(0, 4);
  str = str;
  return (str);
}
/*
THIS FUNCTION WILL FIND THE PASSWORD IN THE INCOMING DATA FROM DISPLAY AND RETURN ONLY PASSWORD.
*/
String pwd() 
{
  int vale = dfd.indexOf("<") + 1;
  dfd.remove(0, vale);
  return (dfd);
}
