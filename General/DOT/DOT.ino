//Texas Instruments TLC 5940 with Arduino
//www.kevindarrah.com

//V7 Update - I did not test this yet, but the bitwise operation
//to set pins LOW was incorrect.  This code fixes that.

#include <SPI.h>//Serial Peripheral Interface Library

  byte ch=0, chbit=0, spibit=0, spibyte=0;// variables used by tlc sub routine
  int SINData;//variable used to shift data to the TLC
  byte transferbyte[48];// bytes that are sent out to the tlc5940 via SPI
  // 48 because 16 channels @ 12bits gives 384bits, 384/8 = 48 bytes, 12 bit to 8 bit conversion
  byte DCvalue[32];//0-63, 6 bit DOT Correction Bytes
  int i, j, k, l,m,n; //misc variables
  int green, oldgreen= 2048;//for random animation
  int red, oldred = 2048;//for random animation
  int blue, oldblue = 2048;//for random animation
  int comeback=0, pick_color=0;//for random animation
  //*******************************************************************************************
  //*******************************************************************************************
void setup(){//   MAIN SETUP   MAIN SETUP   MAIN SETUP   MAIN SETUP   MAIN SETUP
  
  pinMode(2, OUTPUT);//XLAT
  pinMode(3, OUTPUT);//OSC2B GSCLK
  pinMode(4, OUTPUT);//VPRG
  pinMode(11, OUTPUT);//MOSI DATA
  pinMode(13, OUTPUT);//SPI Clock
  
  //Set up the SPI
  SPI.setBitOrder(MSBFIRST);//Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV4);//Run the data in at 16MHz/4 - 4MHz

  for(i=0; i<48; i++)//clear out Gray Scale Data
  transferbyte[i]=0; 
  
  for(i=0; i<32; i++)//set Dot Correction data to max (63 decimal for 6 bit)
  DCvalue[i]=63;  
  
  Serial.begin(115200);//debugging?
  
  //set up DOT Correction
  DotCorrection();// sub routine helps 
  
  noInterrupts();// set up the counters, so don't go into interrupts
  TCCR2A=B00010010;//Timer 2 set to Compare Mode Toggling pin 5 @ 8MHz, Arduino Digital 3
  // TIMER 2 IS GSCLCK
  //Timer 2 prescaler set to 1, 16/1=16 MHz, but toggles pin 5 every other cycle, 8MHz
  TCCR2B=B00000001;
  
  TCCR1A=B00000000;//Timer 1 doesn't toggle anything, used for counting
  //Timer 1 prescaler set to Fclk/256
  //Why? We need to count 4096 pulses out of Timer 2 - pin 5
  //8 MHz = 1 pulse every 125ns - - 4096 pulses would need 512us
  //Timer 1 runs at 16MHz/256=62.5kHz, we need a match at every 512us
  //Basically, I can get an interrupt to get called every 512us, so...
  // I need to run Timer 2 @ 8MHz for 512us to get 4096 pulses
  // I can't count those pulses directy (too fast) , so
  // I'll count using Timer 1, which makes a count every 16us
  // The counter starts at 0, so we'll set it to 31 to get an interrupt after 512us
  TCCR1B=B00001100;//Mode=CTC with OSCR1A = TOP and 256 as the prescaler
  // Mask set up, will call ISR (Inerrupt Service Routine) for Compare match on A
  TIMSK1=B00000010;
  //These are the match values for the counters
  // 0 here means it will match on one cycle of the clock/prescaler
  OCR1A= 31;//to get our 512us Interrupt
  interrupts();// kick off the timers!
  for(i=0; i<32; i++)//wipe out the data in tlc
  tlc(i, 0);// This is how you update the LEDs, tlc is a subroutine with two inputs
  // tlc(Channel, Value)  Channel in this case is 0-32 and value is 0-4095 duty cycle
  //4095 is 100% ON
  pinMode(5, OUTPUT);//BLANK  We set this pin up here, so it remains in a high impedance
  // state throughout the setup, otherwise the LEDs go crazy!  even if you write this HIGH
  
}// END OF SETUP END OF SETUP END OF SETUP END OF SETUP END OF SETUP END OF SETUP END OF SETUP
  //*******************************************************************************************
void loop(){//   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP   MAIN LOOP

 //all_RED(4095);
 //all_GREEN(4095);
 //all_BLUE(4095);
 //lamp_test();
random_animation();
   
}//      loop close      loop close      loop close      loop close
  //*******************************************************************************************
// INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS   INTERRUPTS  
ISR(TIMER1_OVF_vect){}// Over Limit Flag Interrupt  you need this even if you don't use it
ISR(TIMER1_COMPB_vect){ }// Compare B - Not Used

ISR(TIMER1_COMPA_vect){ // Interrupt to count 4096 Pulses on GSLCK
  PORTD |= 1<<5;// write blank HIGH to reset the 4096 counter in the TLC
  PORTD |= 1<<2;// write XLAT HIGH to latch in data from the last data stream
  PORTD &= ~(1<<2);  //XLAT can go low now
  PORTD &= ~(1<<5);//Blank goes LOW to start the next cycle
  SPI.end();//end the SPI so we can write to the clock pin
  PORTB |= 1<<5;// SPI Clock pin to give it the extra count
  PORTB &= ~(1<<5);// The data sheet says you need this for some reason?
  SPI.begin();// start the SPI back up
  for(SINData=47; SINData>=0; SINData--)// send the data out!
  SPI.transfer(transferbyte[SINData]);// The SPI port only understands bytes-8 bits wide
  // The TLC needs 12 bits for each channel, so 12bits times 32 channels gives 384 bits
  // 384/8=48 bytes, 0-47
}//INTERRUPTS END  INTERRUPTS END  INTERRUPTS END  INTERRUPTS END  INTERRUPTS END  INTERRUPTS END  
  //*******************************************************************************************
void lamp_test(){//           lamp_test
   for(l=1; l<32; l++){
   for(k=0; k<4096; k++)
    tlc(l, k);
   for(k=4095; k>=0; k--)
   tlc(l,k);
 }//l
}//lamp_test

void all_RED(int duty){//      all red
    for(k=0; k<duty; k=k+1)
   for(l=1; l<=30; l=l+3)
  tlc(l, k);
   
}//allred
void all_BLUE(int duty){//      all blue
   for(k=0; k<duty; k=k+1)
   for(l=2; l<=30; l=l+3)
   tlc(l, k);

}//allred
void all_GREEN(int duty){//      all GREEN
      for(k=0; k<duty; k=k+1)
   for(l=3; l<=30; l=l+3)
   tlc(l, k);
}//allred
  //*******************************************************************************************
void random_animation(){//  random animation  random animation  random animation

   pick_color++;
   int pick_max = random(100, 4096);
   int speed_swipe = 1;//random(1, 200);
   if(pick_color>3)
   pick_color=1;
   
   
   
   if(pick_color==1){
    while(abs(oldred-red)<100)
   red = random(0, pick_max); 
   } 
   if(pick_color==2){
   while(abs(oldblue-blue)<100)
   blue = random(0, pick_max);
   } 
   if(pick_color==3){
   while(abs(oldgreen-green)<100)
   green = random(0, pick_max);
   } 
   
   delay(1000);
   
   if(comeback==0)
   for(l=1; l<=28; l=l+3){
     if(pick_color==1){
       if(red>oldred){
   for(k=oldred; k<=red; k=k+speed_swipe){
       tlc(l,k);//R
       //delayMicroseconds(20);
     }//for
       }//>oldred
       else 
       if(red<oldred){
   for(k=oldred; k>=red; k=k-speed_swipe){
       tlc(l,k);//R
       //delayMicroseconds(20);
     }//for
       }//>oldred  
      }//pick
      
      if(pick_color==2){
       if(blue>oldblue){
   for(k=oldblue; k<=blue; k=k+speed_swipe){
       tlc(l+1,k);//B
       //delayMicroseconds(20);
     }//for
       }//>oldblue
       else 
       if(blue<oldblue){
   for(k=oldblue; k>=blue; k=k-speed_swipe){
       tlc(l+1,k);//B
       //delayMicroseconds(20);
     }//for
       }//>oldblue    
      }//pick 
      
       if(pick_color==3){
       if(green>oldgreen){
   for(k=oldgreen; k<=green; k=k+speed_swipe){
       tlc(l+2,k);//G
       //delayMicroseconds(20);
     }//for
       }//>oldgreen
       else 
       if(green<oldgreen){
   for(k=oldgreen; k>=green; k=k-speed_swipe){
       tlc(l+2,k);//G
       //delayMicroseconds(20);
     }//for
       }//>oldgreen   
      }//pick 
      comeback=1;
}//l channel             
else
   for(l=28; l>=1; l=l-3){//         2nd sweep
     if(pick_color==1){
       if(red>oldred){
   for(k=oldred; k<=red; k=k+speed_swipe){
       tlc(l,k);//R
       //delayMicroseconds(20);
     }//for
       }//>oldred
       else 
       if(red<oldred){
   for(k=oldred; k>=red; k=k-speed_swipe){
       tlc(l,k);//R
       //delayMicroseconds(20);
     }//for
       }//>oldred  
      }//pick
      
      if(pick_color==2){
       if(blue>oldblue){
   for(k=oldblue; k<=blue; k=k+speed_swipe){
       tlc(l+1,k);//B
       //delayMicroseconds(20);
     }//for
       }//>oldblue
       else 
       if(blue<oldblue){
   for(k=oldblue; k>=blue; k=k-speed_swipe){
       tlc(l+1,k);//B
       //delayMicroseconds(20);
     }//for
       }//>oldblue    
      }//pick 
      
       if(pick_color==3){
       if(green>oldgreen){
   for(k=oldgreen; k<=green; k=k+speed_swipe){
       tlc(l+2,k);//G
       //delayMicroseconds(20);
     }//for
       }//>oldgreen
       else 
       if(green<oldgreen){
   for(k=oldgreen; k>=green; k=k-speed_swipe){
       tlc(l+2,k);//G
       //delayMicroseconds(20);
     }//for
       }//>oldgreen   
      }//pick 
      comeback=0;
}//l channel 2nd


  oldgreen=green;
  oldblue=blue;
  oldred=red;
  
  
}//  random animation end  random animation end  random animation end  random animation end
  //*******************************************************************************************
void tlc(int channel, int value){// TLC to UPDATE TLC to UPDATE TLC to UPDATE TLC to UPDATE
// This routine needs to happen as fast as possible!!!
delayMicroseconds(500);//to control speed if necessary
//Limit check
  if(value>4095)
  value=4095;
  if(value<0)
  value=0;
  
   // We need to convert the 12 bit value into an 8 bit BYTE, the SPI can't write 12bits
   
   //We figure out where in all of the bytes to write to, so we don't have to waste time
   // updating everything
   
   //12 bits into bytes, a start of 12 bits will either at 0 or 4 in a byte
    spibit=0;
    if(bitRead(channel, 0))//if the read of the value is ODD, the start is at a 4
    spibit=4;
    
    //This is a simplification of channel * 12 bits / 8 bits
    spibyte = int(channel*3/2);//this assignes which byte the 12 bit value starts in
  
    for(chbit=0; chbit<12; chbit++, spibit++){// start right at where the update will go
      if(spibit==8){//during the 12 bit cycle, the limit of byte will be reached
      spibyte++;//roll into the next byte
      spibit=0;//reset the bit count in the byte
      }
    if(bitRead(value, chbit))//check the value for 1's and 0's
    bitSet(transferbyte[spibyte], spibit);//transferbyte is what is written to the TLC
    else
    bitClear(transferbyte[spibyte], spibit);
    }//0-12 bit loop
  }//  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE  END OF TLC WRITE
  //*******************************************************************************************
void DotCorrection(){//  DOT Correction  DOT Correction  DOT Correction  DOT Correction
 PORTD |= 1<<4;//VPRG to DC Mode HIGH
spibyte=0;//reset our variables
spibit=0;
  for(ch=0; ch<32; ch++){// 6 bit a piece x 32 Outputs
    for(chbit=0; chbit<6; chbit++){
      if(spibit==8){
      spibyte++;
      spibit=0;
      }
    if(bitRead(DCvalue[ch], chbit))//all 6 bits
    bitSet(transferbyte[spibyte], spibit);//setting bit 7 of transfer byte
    else
    bitClear(transferbyte[spibyte], spibit);
    spibit++;}//i 
     }//j
  SPI.begin();
  for(j=spibyte; j>=0; j--){
  SPI.transfer(transferbyte[j]);
  }
  PORTD |= 1<<2;//XLAT the data in
  PORTD &= ~(1<<2);//XLAT data is in now
  PORTD &= ~(1<<4);//VPRG is good to go into normal mode LOW
}//  end of DOT Correction  end of DOT Correction  end of DOT Correction  end of DOT Correction
  //*******************************************************************************************
