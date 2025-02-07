void nexval() {
  if (Serial3.available()) {
    
    if (MESP_ID_Time_AvgAT_v > 0)
    {
      Serial2.print("at.txt=\"");
      Serial2.print(MESP_ID_Time_AvgAT_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("rh.txt=\"");
      Serial2.print(MESP_ID_Time_AvgRH_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("vpd.txt=\"");
      Serial2.print(MESP_ID_Time_AvgVPD_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      if (MESP_ID_Time_EvTr_v > 0)
      {
        Serial2.print("ev.txt=\"");
        Serial2.print(MESP_ID_Time_EvTr_v);
        Serial2.print("\"");
        Serial2.write(0xFF);
        Serial2.write(0xFF);
        Serial2.write(0xFF);
      }

      Serial2.print("bp.txt=\"");
      Serial2.print(MESP_ID_Time_BaPr_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("sl.txt=\"");
      Serial2.print(MESP_ID_Time_SeL_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("co.txt=\"");
      Serial2.print(MESP_ID_Time_co2_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("dpp.txt=\"");
      Serial2.print(MESP_ID_Time_DePt_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("sr.txt=\"");
      Serial2.print(MESP_ID_Time_SoRa_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("hi.txt=\"");
      Serial2.print(MESP_ID_Time_AvgHI_v);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);
    }

    if (WAT > 0)
    {
      Serial2.print("att.txt=\"");
      Serial2.print(WAT);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("rhh.txt=\"");
      Serial2.print(WRH);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("bpp.txt=\"");
      Serial2.print(WBaPa);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("sll.txt=\"");
      Serial2.print(WSeaL);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("srr.txt=\"");
      Serial2.print(WSolar);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("hii.txt=\"");
      Serial2.print(WHI);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("rp.txt=\"");
      Serial2.print(WRH);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("evv.txt=\"");
      Serial2.print(WEv);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);

      Serial2.print("dppp.txt=\"");
      Serial2.print(WDew);
      Serial2.print("\"");
      Serial2.write(0xFF);
      Serial2.write(0xFF);
      Serial2.write(0xFF);
    }

    /**************** DATE & TIME ***********************/
    Serial2.print("hour.txt=\"");
    Serial2.print(hourupg);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("hourr.txt=\"");
    Serial2.print(hourupg);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("min.txt=\"");
    Serial2.print(minupg);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("sec.txt=\"");
    Serial2.print(secslive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("day.txt=\"");
    Serial2.print(Datelive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("dayy.txt=\"");
    Serial2.print(Datelive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("month.txt=\"");
    Serial2.print(Monthlive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("monthh.txt=\"");
    Serial2.print(Monthlive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("year.txt=\"");
    Serial2.print(Yearlive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);

    Serial2.print("yearr.txt=\"");
    Serial2.print(Yearlive);
    Serial2.print("\"");
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);
  }
}
