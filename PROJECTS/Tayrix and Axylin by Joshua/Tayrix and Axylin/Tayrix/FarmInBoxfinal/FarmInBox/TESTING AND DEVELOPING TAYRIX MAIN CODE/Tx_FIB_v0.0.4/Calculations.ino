


//Calculating the maximal irrigation area based on water supply

/*https://www.netafim.com/499749/globalassets/products/drippers-and-dripperlines/drip-irrigation-system-handbook.pdf#page=35&zoom=100,0,0
   Pump discharge per hour * Daily hours/shift * numbers of shifts
  Daily return




*/
//===========================================
// Includes
//===========================================
#include <math.h>
////dosing time for each nutrient/////
float N_Dosing_time;
float P_Dosing_time;
float K_Dosing_time;

int Dripperline_length_Ha = 1000; //m3
float Nitrogen_fertilizer_irrigation;
//===========================================
// DOSER MANAGEMENT
//===========================================

float NUTRIENTNEX()
{
  float DoseTime;

  DoseTime =  Nitrogen_fertilizer_irrigation;
  Serial.println(DoseTime);

  float DosingTimePerHa = P_rate;
  Serial.println("DosingTimePerHa : " + String(DosingTimePerHa) + " /Ha ");
  float fertilizer_injector_capable = 100; //liters per hour
  float Dosing_time = DoseTime / fertilizer_injector_capable;
  Serial.println("  Dosing_time : " + String(Dosing_time) + " Hr ");

  if ( nutreintstats == true)
  {
    
    N_Dosing_time = Dosing_time;
    nutreintstats = false;
    Serial.print("nitrogen dosing time:");
    Serial.println(N_Dosing_time);
    int nitrhr =N_Dosing_time;
   int hrmilis = nitrhr*ms_per_hour;
   hours_minutes = ExtractDecimalPart(N_Dosing_time, 2);
    float nitromins = hours_minutes*ms_per_min;
    totalnitromills = hrmilis+nitromins;
  Serial.print("totalnitromills");
  Serial.println(totalnitromills);
  }
  if ( pstats == true)
  {
    P_Dosing_time = Dosing_time;
    pstats = false;
    Serial.print("phosphorus  dosing time :");
    Serial.println(P_Dosing_time);
    int phshr =P_Dosing_time;
   int hrmilis = phshr*ms_per_hour;
   hours_minutes = ExtractDecimalPart(P_Dosing_time, 2);
    float phsmins = hours_minutes*ms_per_min;
    totalphsmills = hrmilis+phsmins;
  Serial.print("totalphsmills");
  Serial.println(totalphsmills);
  }
  if ( kstats == true)
  {
    K_Dosing_time = Dosing_time;
    kstats = false;
    Serial.print("potassium  dosing time :");
    Serial.println(K_Dosing_time);
     int pota =K_Dosing_time;
   int hrmilis = pota*ms_per_hour;
   hours_minutes = ExtractDecimalPart(K_Dosing_time, 2);
    float potamins = hours_minutes*ms_per_min;
    totalpotamills = hrmilis+potamins;
  Serial.print("totalpotamills");
  Serial.println(totalpotamills);
  
  }

}
float Nutrient_Cal( float fertilizer_Kg , float percentage, float area_Acre, float MixWater_Ltr, float Req_NutrientKg )
{

  int zoneinput = area_Acre ;
  float acreToHectare = 0.405 * zoneinput;
  int WaterMixing = MixWater_Ltr;
  float INpercentage = percentage; 
  float TOTALVal = fertilizer_Kg * INpercentage;  
  float Pure_nutrient_Kg = WaterMixing / TOTALVal;
  Serial.println("Pure_nitrogen_Kg : " + String(Pure_nutrient_Kg) + " KG ");
  Nitrogen_fertilizer_irrigation = acreToHectare * TOTALVal * Pure_nutrient_Kg;
  Serial.println("Fertilizer_irrigation : " + String(Nitrogen_fertilizer_irrigation) + " liters ");
  return Nitrogen_fertilizer_irrigation;

}





//===========================================
// IRRIGATION MANAGEMENT
//===========================================

int Flow_Variation_FV(int Q_max, int Q_min) {
  int Q1 = Q_max;
  int Q2 = Q_min;
  int FV1 = Q1 - Q2 / Q1;
  int FV = FV1 * 100;

  Serial.println("FV :" + String(FV) + " % ");
  return FV;
}

float Precipitation_Rate(float Q, float E, float D) {

  float  Q_Pr = Q;   // l/hr //Dripper_flowrate
  float  E_Pr = E / 10; //m Distance_between_drippers
  float  D_Pr = D / 10; //m Distance_between_dripperlines

  Serial.println("Q_Pr :" + String(Q_Pr) + " m ");
  Serial.println("E_Pr :" + String(E_Pr) + " m ");
  P_rate = Q_Pr / (E_Pr * D_Pr);
  Serial.println("Precipitation Rate : " + String(P_rate) + "  mm/hr ");

  return P_rate;
}


float Hourly_Irrigation_Rate() {
  //Dripperline_length_Ha


  Serial.println("P_rate : " + String(P_rate) + "  mm/hr ");
  float HIR1 =  P_rate / Dripperline_length_Ha ;
  float HIR =  HIR1 * 10 * 1000 ; //Meter conv *1000

  Serial.println("Hourly_Irrigation_Rate : " + String(HIR) + "  m3/Ha/hr ");
  return HIR;


}

//===========================================
// DWR -Calculating the Daily Water Requiremen
//===========================================
//



float DWR(float ET, float Kc) {

  float DWR_Val = ET ;

  DWR_Val_ha = DWR_Val * 10; // Conv
  Serial.println("DWR_Val_ha : " + String(DWR_Val_ha) + " m3/Ha/d ");

  return DWR_Val_ha;
}
float ExtractDecimalPart(float Value, int numberOfDecimals)
{
  float temp = Value - (long)(Value);
  long p = 1;
  for (int i = 0; i < numberOfDecimals; i++) p *= 10;
  long DecimalPart = p * temp;
  Serial.println ("DecimalPart");
  Serial.println (DecimalPart);
  return DecimalPart;
}
//==================================================================
// Calculate the required irrigation duration (hours/day):
//==================================================================


float Irrigation_Time() {

  float HIR = Hourly_Irrigation_Rate();
  Serial.println("DWR : " + String(DWR_Val_ha) + "   m3/Ha/d ");
  Serial.println("HIR : " + String(HIR) + "  m3/Ha/hr ");
  Irrigation_time =  DWR_Val_ha  / HIR;

  Serial.println("Irrigation_time : " + String(Irrigation_time) + "  hours ");
  hours_minutes = ExtractDecimalPart(Irrigation_time, 2) / 100 * 60;
  Serial.println("hours_minutes : " + String(hours_minutes) + "  minutes ");

  return Irrigation_time;


}

//==================================================================
// Calculating the maximal irrigation area based on water supply
//==================================================================



//FRUIT CROPS
int mango[5] = {70, 15, 65, 0, 0}; //N,P,K,sulfur,acid
int sapota[5] = {95, 45, 45, 0, 0}; //N,P,K,sulfur,acid
int citrus[5] = {100, 30, 50, 0, 0}; //N,P,K,sulfur,acid
int Pomegranate[5] = {400, 400, 900, 0, 0}; //N,P,K,sulfur,acid
int CustardApple[5] = {300, 200, 700, 0, 0}; //N,P,K,sulfur,acid
int Geranium[5] = {75, 20, 70, 0, 0}; //N,P,K,sulfur,acid

//VEGETABLE CROPS
int Tomato[5] = {150, 100, 100, 0, 0}; //N,P,K,sulfur,acid
int brinjal[5] = {150, 100, 120, 0, 0}; //N,P,K,sulfur,acid
int potato[5] = {55, 95, 100, 0, 0}; //N,P,K,sulfur,acid
int bhendi[5] = {55, 95, 100, 0, 0}; //N,P,K,sulfur,acid

void NPKcal()
{
  Serial.println(" ");
  Serial.print("N val  ");
  Serial.print(mango[0]);
  Serial.println(" ");
  Serial.print("P val  ");
  Serial.print(mango[1]);
  Serial.println(" ");
  Serial.print("K val  ");
  Serial.print(mango[2]);
  Serial.println(" ");
  Serial.print("ph val  ");
  Serial.print(mango[3]);
  Serial.println(" ");
  Serial.print("S val  ");
  Serial.print(mango[4]);
}
