// Function to turn on specific fans
void LowState(int fan1, int fan2, int fan3 = -1, int fan4 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  if (fan3 != -1) digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
}

void MidState(int fan1, int fan2, int fan3, int fan4 = -1, int fan5 = -1, int fan6 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  if (fan4 != -1) digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
}

void HighState(int fan1, int fan2, int fan3, int fan4, int fan5 = -1, int fan6 = -1, int fan7 = -1, int fan8 = -1) {
  turnOffAllFans(); // Ensure all fans are off first
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(fan3, LOW);
  digitalWrite(fan4, LOW);
  if (fan5 != -1) digitalWrite(fan5, LOW);
  if (fan6 != -1) digitalWrite(fan6, LOW);
  if (fan7 != -1) digitalWrite(fan7, LOW);
  if (fan8 != -1) digitalWrite(fan8, LOW);
}

// Function to handle fans based on VPD
void VPDMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgVPD_v > VPD_LOW && MESP_ID_Time_AvgVPD_v < VPD_MID) {
      LowState(ExFan5, ExFan7);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_MID && MESP_ID_Time_AvgVPD_v < VPD_HIGH) {
      MidState(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_HIGH) {
      HighState(ExFan2, ExFan4, ExFan6, ExFan8);
    } else if (MESP_ID_Time_AvgVPD_v < VPD_LOW) {
      turnOffAllFans();
    }
  } else {
    // Alternate fans when toggleState is false
    if (MESP_ID_Time_AvgVPD_v > VPD_LOW && MESP_ID_Time_AvgVPD_v < VPD_MID) {
      LowState(ExFan1, ExFan3);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_MID && MESP_ID_Time_AvgVPD_v < VPD_HIGH) {
      MidState(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgVPD_v > VPD_HIGH) {
      HighState(ExFan1, ExFan3, ExFan5, ExFan7);
    } else if (MESP_ID_Time_AvgVPD_v < VPD_LOW) {
      turnOffAllFans();
    }
  }
}

// Function to handle fans based on Temperature
void TempMode() {
  if (toggleState) {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v < TEMP_MID) {
      LowState(ExFan5, ExFan7); // Second state: Fan5, Fan7
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v < TEMP_HIGH) {
      MidState(ExFan1, ExFan3, ExFan5);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      HighState(ExFan1, ExFan3, ExFan5, ExFan7);
    } else if (MESP_ID_Time_AvgAT_v < TEMP_LOW) {
      turnOffAllFans();
    }
  } else {
    if (MESP_ID_Time_AvgAT_v > TEMP_LOW && MESP_ID_Time_AvgAT_v < TEMP_MID) {
      LowState(ExFan1, ExFan3); // First state: Fan1, Fan3
    } else if (MESP_ID_Time_AvgAT_v > TEMP_MID && MESP_ID_Time_AvgAT_v < TEMP_HIGH) {
      MidState(ExFan2, ExFan4, ExFan6);
    } else if (MESP_ID_Time_AvgAT_v > TEMP_HIGH) {
      HighState(ExFan2, ExFan4, ExFan6, ExFan8);
    } else if (MESP_ID_Time_AvgAT_v < TEMP_LOW) {
      turnOffAllFans();
    }
  }
}
