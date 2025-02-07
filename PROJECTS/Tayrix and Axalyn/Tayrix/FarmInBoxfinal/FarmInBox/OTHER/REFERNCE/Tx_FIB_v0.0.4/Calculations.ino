


//Calculating the maximal irrigation area based on water supply

/*https://www.netafim.com/499749/globalassets/products/drippers-and-dripperlines/drip-irrigation-system-handbook.pdf#page=35&zoom=100,0,0
 * Pump discharge per hour * Daily hours/shift * numbers of shifts
Daily return
 *
 *


 */
//===========================================
// Includes
//===========================================
#include <math.h>
//Area
#define Acre 2.5 //0.405
float Hectare ; //2.47105 acre

//Data
int Dripperflowrate = 0.5; //l/hr
int Distance_between_drippers = 0.5; //m
int Distance_between_dripperlines = 1.8; //m
int Hourly_irrigation_rate = 0.55; // mm/hr
int Daily_return = 6.3; //mm/d (equal_to_HIR)
int Daily_hour_Shift = 11.45; //hr
int Number_of_shifts_in_24_hours = 2;
int Pump_discharge_pHr = 36;

float Quantity_pure_nitrogen_perday = 1.66; //Kg/ha/day

//User Input
float  Q_Pr ;   // l/hr //Dripper_flowrate
float  E_Pr ; //m Distance_between_drippers
float  D_Pr ;  //Distance between dripperlines
float Pr; //Precipitation rate (Pr)
int Dripperline_length_Ha = 1000; //m3
//===========================================
// DOSER MANAGEMENT
//===========================================


int  QP_Nitrogen_per_month = 50;  //kg Quantity of pure nitrogen per month:
int Days = 30; //Total days in the month:
int  PureNitrogen_perDay = 1.66;  //=50/30; Kg/ha/Day
int Total_hectares = 1;
int Mixting_Water = 100;
int water_ltr = 100;
float onekg_mixture = 100 / 9; //11.1
int urea = 20; //kg

int liquidfertilizer() {
  int water_ltr = 100;
  //  int liquid_to_kg =
  //    int  pure_nitrogen =

  int  fertilizer_everyday = Total_hectares * PureNitrogen_perDay * onekg_mixture;

}

int Nitrogen_WaterMix = 100; // liters
float ureaKg = 20; //Kg
int Zonearea_acre = 10;
float N_val = 1.66;
int percentage = 0.45;
// fertilizer

float Nitrogen() {

  int Urea_fertilizer = 20;
  float per_nitrogen = 0.45;
  float Zone_sizein_acre = 5;
  int Nutrient_tank = 100;
  float Req_NutrientKg = 1.66;

  Nutrient_Cal(Urea_fertilizer, per_nitrogen, Zone_sizein_acre, Nutrient_tank, Req_NutrientKg); //(float ureaKg,  int area, int mixTank, int N_val)

  float dripper_flow = 0.8;
  float drip_dis = 0.50;
  float drip_gap = 1.60;

  float DosingTimePerHa = dripper_flow / (drip_dis * drip_gap); 

  
}


float Nutrient_Cal(float fertilizer_Kg , float percentage, int area_Acre, int MixWater_Ltr, float Req_NutrientKg ) {
  //User Input

  int zoneinput = area_Acre ;
  float acreToHectare = 0.405 * zoneinput; //20; //0.405 * area;

  int WaterMixing = MixWater_Ltr;//mixTank; //100ltr

  float Urea_in_percentage = percentage; //45% 0.45 urea

  float UreaVal = ureaKg * Urea_in_percentage;//9

  float Pure_nitrogen_Kg = WaterMixing / UreaVal;
  Serial.println("Pure_nitrogen_Kg : " + String(Pure_nitrogen_Kg) + " KG ");
  float Nitrogen_fertilizer_irrigation = acreToHectare * N_val * Pure_nitrogen_Kg;
  Serial.println("Nitrogen_fertilizer_irrigation : " + String(Nitrogen_fertilizer_irrigation) + " liters ");
  return Nitrogen_fertilizer_irrigation;


}



//===========================================
// Required duration of irrigation
//===========================================


float Req_duration_irrigation() {

  float dripper_flow = 0.8;
  float drip_dis = 0.50;
  float drip_gap = 1.60;

  float irrigation_hr = dripper_flow / (drip_dis * drip_gap); //1.0 mm per hour or 10.0 m3 per hectare per hour

  float ET0 = DWR(8, 0.8);
  float RUN_Time = ET0 / irrigation_hr; //Total water to irrigate in mm / irrigation Hourly capacity in mm / hour

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
  float  E_Pr = E; //m Distance_between_drippers
  float  D_Pr = D; //m Distance_between_dripperlines
  Serial.println("Q_Pr :" + String(Q_Pr) + " m ");
  Serial.println("E_Pr :" + String(E_Pr) + " m ");
  float Pr = Q_Pr / (E_Pr * D_Pr);
  Serial.println("Precipitation Rate : " + String(Pr) + "  mm/hr ");
  return Pr;
}


float Hourly_Irrigation_Rate() {
  //Dripperline_length_Ha

  float P_rate = Precipitation_Rate(0.5, 0.5, 1.8);
  Serial.println("P_rate : " + String(P_rate) + "  mm/hr ");
  float HIR1 =  P_rate / Dripperline_length_Ha ;
  float HIR =  HIR1 * 10 * 1000 ; //Meter conv *1000

  //  // Calculate the total length of the dripperlines
  //  float Dripperline_length_Ha = Dripperline_length_Ha / D_Pr;
  //
  //  // Calculate the total number of drippers per Hectare:
  //  float Drippers_Ha = Dripperline_length_Ha/E_Pr;
  //
  //  //Calculate the hourly irrigation rate
  //
  //  float HIR2 = (Drippers_Ha*Q_Pr)/1000;
  Serial.println("Hourly_Irrigation_Rate : " + String(HIR) + "  m3/Ha/hr ");
  return HIR;


}
//===========================================
// DWR -Calculating the Daily Water Requiremen
//===========================================
//
float Pan_evaporation_EVTo = 8.4;  //mm/d
float Crop_coefficient_Kc = 0.75;


float DWR(float ET, float Kc) {

  float Pan_evaporation_EVTo = ET ;
  float Crop_coefficient_Kc = Kc ;
  float DWR_Val = Pan_evaporation_EVTo * Crop_coefficient_Kc;
  Serial.println("DWR_Val : " + String(DWR_Val) + " mm/d ");
  float DWR_Val_ha = DWR_Val * 10; // Conv
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
  float DWRVal =  DWR(8.4, 0.75); //ET kc
  float HIR = Hourly_Irrigation_Rate();
  Serial.println("DWR : " + String(DWRVal) + "   m3/Ha/d ");
  Serial.println("HIR : " + String(HIR) + "  m3/Ha/hr ");
  float Irrigation_time = DWRVal / HIR;

  Serial.println("Irrigation_time : " + String(Irrigation_time) + "  hours ");
  int hours_minutes = ExtractDecimalPart(Irrigation_time, 2) / 100 * 60;
  Serial.println("hours_minutes : " + String(hours_minutes) + "  minutes ");

  return Irrigation_time;
}
//==================================================================
// Calculating the maximal irrigation area based on water supply
//==================================================================


int MaxIrrAreaWaterSupply() {
  /*
     Pump discharge per hour * Daily hours/shift * numbers of shiftsDaily return
  */

  float Irr_time = Irrigation_Time();
  float MaxIrrAreaWaterSupply_ha = Pump_discharge_pHr * Irr_time * Number_of_shifts_in_24_hours / DWR(8.4, 0.75);
  Serial.println("MaxIrrAreaWaterSupply_ha : " + String(MaxIrrAreaWaterSupply_ha) + "  Ha ");

  int MaxIrrAreaWaterSupply_haRound = round(MaxIrrAreaWaterSupply_ha);
  Serial.println("round : " + String(MaxIrrAreaWaterSupply_haRound) + "  Ha ");
  return MaxIrrAreaWaterSupply_ha;
}


void irrigation_area_based_watersupply(){
  
  int irrigation_area_based_watersupply_val =Pump_discharge_pHr*Daily_hour_Shift*Number_of_shifts_in_24_hours/(Daily_return);
  Serial.print("irrigation_area_based_watersupply   = ");
  Serial.print(irrigation_area_based_watersupply_val);
  }

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

void NPKcal(){
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
