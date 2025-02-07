
void batterypercent()
{
  sensorvalue = analogRead(34);
  battry = map(sensorvalue, 0, 4096, 0, 100);
}
