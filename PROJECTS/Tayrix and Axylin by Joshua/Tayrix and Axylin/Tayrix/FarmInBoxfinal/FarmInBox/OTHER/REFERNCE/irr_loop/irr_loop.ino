//////*** reference***///////

//  if (RTC_Hr == Slot01_DIModeON_SetTime_Hr && RTC_Min == Slot01_DIModeON_SetTime_min )
//     {
//    
//      pinMode (irrigation,OUTPUT);
//      digitalWrite(irrigation,LOW);
//      for(int i=0;i<=5;i++)
//      {
//       sensorfun();
//       Serial.println("function");
//       Serial.println(i);
//      }
//      
//     }
//          if (RTC_Hr == Slot02_DIModeON_SetTime_Hr && RTC_Min == Slot02_DIModeON_SetTime_min )
//     {
//      pinMode (irrigation,OUTPUT);
//      digitalWrite(irrigation,LOW); 
//      for(int i=0;i<=5;i++)
//      {
//       sensorfun();
//       Serial.println("function");
//       Serial.println(i);
//      }
//         
//     }
//    
// if((MainInPressure>=3)&&(MainInPressure<=6)&&(pressurecheck==true))
// {
//  Serial.println("pressure is ok");
//  
//    irr();
//  }
//  else
//  {
//    Serial.println("pressure is not good we cant run irrigation pump");
//    pinMode(irrigation,OUTPUT);
//    digitalWrite(irrigation,HIGH);
//
//  }
