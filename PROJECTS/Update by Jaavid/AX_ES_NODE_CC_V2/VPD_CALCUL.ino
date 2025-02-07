float VPDcal()
{

  float AirTemp = ES_AT;
  float LeafTemp = ES_AT;
  float RH = ES_RH;
  float p1 = pow ( 10 , ((float)(7.5 * AirTemp) / (float)(237.3 + AirTemp)));
  float p2 = pow ( 10 , ((float)(7.5 * LeafTemp) / (float)(237.3 + LeafTemp)));
  VPD = (((float)610.7 * (float) p1) / 1000) - (((float)610.7 * (float)p2) / 1000) * (((float)RH) / 100);
  return VPD;
}
