void ETcalcul()
{

  float Tmax; //this will store temperature from bme sensor
  float Tmin;//this reduce the temperature by 0.3
  float Tme;//this store sum of Tmax+Tmin
  float Tmean;//mean value of Tme
  
  float h;//which will store humidity of bme sensor.//h=8.2;//height of the measurement above the ground surface
  float dn1;//which will store h*67.8
  float dn;//which will store dn-5.42
  float num1,num2,num3,num5,num6;
  float numcalc;
  float U2;
  float Un = 2.4; ////measured wind speed z m above the ground surface
  float enum1;
  float enum2;
  float expon;
  float pdenom;
  float powercal;
  float top1;
  float top2;
  float delta;
  float P;
  float powercalc2;
  float denom2;
  float gama;
  float expnum1,expnum2,expnum3,expnum4;
  float expcal,expcal2;
  float ETmax,ETmin,ETcal;
  float RHmax, RHmin,RHcal,RHcal2;
  float Ea,Es;
  float T;
  float Rn;
  float G;
  float fdenom1,fdenom2,fdenom3,fdenom4;
  float fnum1,fnum2,fnum3,fnum4,fnum5,fnum6,fnum7,fnum8,fnum9,fnum10;
  float z = 840; //3.6;//elevation above sealevel//
  
  Tmax = ES_AT;//ASSIGINING TEMPERATURE FROM BME SENSOR TO Tmax.
  Tmin = ES_AT - 0.3;
  Tme = Tmax + Tmin;

  Tmean = Tme / 2;
  h = ES_RH;//which will store humidity of bme sensor
  dn1 = 67.8 * h;
  dn = dn1 - 5.42;
  num1 = 4.87 / dn;
  U2 = Un * num1;
  //U2=(Un*(4.87/(67.8*h-5.42)));

  enum1 = 17.27 * Tmean;
  enum2 = Tmean + 237.3;
  expon = exp(enum1 / enum2);
  pdenom = Tmean + 237.3;
  powercal = pow(pdenom, 2);
  top1 = 0.6108 * expon;
  top2 = top1 * 4098;
  delta = top2 / powercal;


  //delta=4098*0.6108*exp(17.27*Tmean/Tmean+237.3)/pow(Tmean+237.3,2);

  P = 101.3 * pow(((293 - 0.0065 * z) / 293), 5.26);
  num2 = 0.0065 * z;
  num3 = 293 - num2;
  denom2 = num3 / 293;
  powercalc2 = pow(denom2, 5.26);
  P = 101.3 * powercalc2;
  gama = (0.000665 * P);
  expnum1 = 17.27 * Tmax;
  expnum2 = Tmax + 237.3;
  expcal = exp(expnum1 / expnum2);
  ETmax = 0.6108 * expcal;
  //ETmax=0.6108*exp(17.27*Tmax/Tmax+237.3);

  expnum3 = 17.27 * Tmin;
  expnum4 = Tmin + 237.3;
  expcal2 = exp(expnum3 / expnum4);
  ETmin = 0.6108 * expcal2;
  //ETmin=0.6108*exp(17.27*Tmin/Tmin+237.3);

  ETcal = ETmax + ETmin;
  Es = ETcal / 2;
  //Es=(ETmax+ETmin)/2;


  RHmax = ES_RH;
  RHmin = ES_RH - 0.3;
  RHcal = RHmax / 100;
  RHcal2 = RHmin / 100;
  num5 = ETmin * RHcal;
  num6 = ETmax * RHcal2;
  numcalc = num5 + num6;
  Ea = numcalc / 2;
  //Ea=ETmin*(RHmax/100)+ETmax*(RHmin/100)/2;

  T = ES_AT;
  Rn = uvIntensity;
  G = HI;
  fdenom1 = 0.34 * U2;
  fdenom2 = 1 + fdenom1;
  fdenom3 = gama * fdenom2;
  fdenom4 = delta + fdenom3;
  fnum1 = 0.408 * delta;
  fnum2 = Rn - G;
  fnum3 = fnum1 * fnum2;
  fnum4 = Es - Ea;
  fnum5 = U2 * fnum4;
  fnum6 = T + 273;
  fnum7 = 900 / fnum6;
  fnum8 = fnum7 * fnum5;
  fnum9 = gama * fnum8;
  fnum10 = fnum3 + fnum9;
  ETo = fnum10 / fdenom4;
  //ETo=0.408*delta*(Rn-G)+gama*(900/T+273)*U2*(Es-Ea)/delta+gama*(1+0.34*U2);
}
