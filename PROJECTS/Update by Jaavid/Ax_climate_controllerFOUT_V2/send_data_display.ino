/*
THIS FUNCTION IS USED TO SEND DATA TO DISPLAY PAGES.
IN DISPLAY HOME1, HOME2, HOME3, HOME4, HOME5 PAGES WILL RECEIVE THIS.

*/

void nexval()
{
   //*****************HOME 1 PAGE CREDENTIALS**********************
    Serial2.print("at.txt=\"");
    Serial2.print(_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.print("rh.txt=\"");
    Serial2.print(_HUM);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("vpd.txt=\"");
    Serial2.print(_VPD);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("ev.txt=\"");
    Serial2.print(_ET);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);


    Serial2.print("bp.txt=\"");
    Serial2.print(_BP);
    Serial2.print("\"");
    Serial2.write(0x3ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    //*************************HOME 2 PAGE**************
    Serial2.print("sl.txt=\"");
    Serial2.print(_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("co.txt=\"");
    Serial2.print(_CO2);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("dpp.txt=\"");
    Serial2.print(_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sr.txt=\"");
    Serial2.print(_UV);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("hi.txt=\"");
    Serial2.print(_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
  
  /***************** HWM ****************/

  
    Serial2.print("att.txt=\"");
    Serial2.print(O_AT);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rhh.txt=\"");
    Serial2.print(O_HUM);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("bpp.txt=\"");
    Serial2.print(O_BP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("sll.txt=\"");
    Serial2.print(O_SL);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);



    Serial2.print("srr.txt=\"");
    Serial2.print(O_UV);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("hii.txt=\"");
    Serial2.print(O_HI);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("rp.txt=\"");
    Serial2.print(O_CO2);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);



    Serial2.print("evv.txt=\"");
    Serial2.print(O_UV);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

    Serial2.print("dppp.txt=\"");
    Serial2.print(O_DP);
    Serial2.print("\"");
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);
    Serial2.write(0x03ff);

  /**************** DATE & TIME ***********************/

  Serial2.print("hourr.txt=\"");
  Serial2.print(Rtc_hour);//hourupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

  Serial2.print("min.txt=\"");
  Serial2.print(Rtc_min);//minupg
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("sec.txt=\"");
  Serial2.print(Rtc_sec);//secslive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  

  Serial2.print("dayy.txt=\"");
  Serial2.print(Rtc_day);//Datelive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("month.txt=\"");
  Serial2.print(Rtc_month);//Monthlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);


  Serial2.print("yearr.txt=\"");
  Serial2.print(Rtc_year);//Yearlive
  Serial2.print("\"");
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);
  Serial2.write(0x03ff);

}
