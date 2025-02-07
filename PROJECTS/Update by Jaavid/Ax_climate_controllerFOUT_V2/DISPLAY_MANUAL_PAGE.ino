void manual(byte k)
{
  switch(k)
  {
    case 1:
            {
              feedBack = "f1on";
              digitalWrite(ExFan1, LOW);//it will on the fan
              Serial.println("f1 On");
              break;
            }
    case 2:
            {
              feedBack = "f1off";
              digitalWrite(ExFan1, HIGH);//it will off the fan
              Serial.println("f1 off");
              break;
            }
    case 3:
            {
              feedBack = "f2on";
              digitalWrite(ExFan2, LOW);//it will on the fan
              Serial.println("f2 On");
              break;
            }
    case 4:
            {
              feedBack = "f2off";
              digitalWrite(ExFan2, HIGH);//it will off the fan
              Serial.println("f2 off");
              break;
            }
    case 5:
            {
              feedBack = "f3on";
              digitalWrite(ExFan3, LOW);//it will on the fan
              Serial.println("f3 On");
              break;
            }
    case 6:
            {
              feedBack = "f3off";
              digitalWrite(ExFan3, HIGH);//it will off the fan
              Serial.println("f3 off");
              break;
            }
    case 7:
            {
              feedBack = "f4on";
              digitalWrite(ExFan4, LOW);//it will on the fan
              Serial.println("f4 On");
              break;
            }
    case 8:
            {
              feedBack = "f4off";
              digitalWrite(ExFan4, HIGH);//it will off the fan
              Serial.println("f4 off");
              break;
            }
    case 9:
            {
              feedBack = "f5on";
              digitalWrite(ExFan5, LOW);//it will on the fan
              Serial.println("f5 On");
              break;
            }
    case 10:
            {
              feedBack = "f5off";
              digitalWrite(ExFan5, HIGH);//it will off the fan
              Serial.println("f5 off");
              break;
            }
    case 11:
            {
              feedBack = "f6on";
              digitalWrite(ExFan6, LOW);//it will on the fan
              Serial.println("f6 On");
              break;
            }
    case 12:
            {
              feedBack = "f6off";
              digitalWrite(ExFan6, HIGH);//it will off the fan
              Serial.println("f6 off");
              break;
            }
    case 13:
            {
              feedBack = "f7on";
              digitalWrite(ExFan7, LOW);//it will on the fan
              Serial.println("f7 On");
              break;
            }
    case 14:
            {
              feedBack = "f7off";
              digitalWrite(ExFan7, HIGH);//it will off the fan
              Serial.println("f7 off");
              break;
            }
    case 15:
            {
              feedBack = "f8on";
              digitalWrite(ExFan8, LOW);//it will on the fan
              Serial.println("f8 On");
              break;
            }
    case 16:
            {
              feedBack = "f8off";
              digitalWrite(ExFan8, HIGH);//it will off the fan
              Serial.println("f8 off");
              break;
            }
    case 17:
            {
              feedBack = "cpon";
              digitalWrite(pad1, LOW);//it will on the relay
              Serial.println("cp On");
              break;
            }
    case 18:
            {
              feedBack = "cpoff";
              digitalWrite(pad1, HIGH);//it will off the relay
              Serial.println("cp Off");
              break;
            }
    case 19:
            {
              feedBack = "fpon";
              digitalWrite(Fogger, LOW);//it will on the relay
              Serial.println("Fogger On");
              break;
            }
    case 20:
            {
              feedBack = "fpoff";
              digitalWrite(Fogger, HIGH);//it will off the relay
              Serial.println("Fogger Off");
              break;
            }
    case 21:
            {
              feedBack = "fcon";
              digitalWrite(FoggerSol, LOW);//it will on the relay
              Serial.println("FoggerSol On");
              break;
            }
    case 22:
            {
              feedBack = "fcoff";
              digitalWrite(FoggerSol, HIGH);//it will off the relay
              Serial.println("FoggerSol Off");
              break;
            }
    case 23:
            {
              feedBack = "co2on";
              digitalWrite(Co2Gen, LOW);//it will on the relay
              Serial.println("co2 On");
              break;
            }
    case 24:
            {
              feedBack = "co2off";
              digitalWrite(Co2Gen, HIGH);//it will off the relay
              Serial.println("co2 Off");
              break;
            }
    
    default :Serial.println("you entered wrong choise");break;
  }
}
