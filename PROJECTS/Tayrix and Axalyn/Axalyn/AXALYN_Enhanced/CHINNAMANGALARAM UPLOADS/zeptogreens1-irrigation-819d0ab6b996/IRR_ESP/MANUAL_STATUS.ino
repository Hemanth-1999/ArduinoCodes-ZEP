void manualstatus()
{
  DynamicJsonDocument relaystatus(500);

  DeserializationError error = deserializeJson(relaystatus, Serial);

  if (error) {

    return;
  }
  JsonObject Manualstatus = relaystatus["Manualstatus"];
  Manualstatus_irz1a = Manualstatus["irz1a"];//this is the status of pump 1 relay
  Manualstatus_irz1b = Manualstatus["irz1b"];
  Manualstatus_irz2a = Manualstatus["irz2a"];//this is the status of pump 2 relay
  Manualstatus_irz2b = Manualstatus["irz2b"];
  Manualstatus_doa = Manualstatus["doa"];
  Manualstatus_dob = Manualstatus["dob"];
}
