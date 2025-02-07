/*
  WHAT ALL THINGS ESP8266 IS SENDING TO MEGA
  NOTHING.
  WHAT ALL THINGS ATMEGA IS SENDING TO ESP8266
    1) ABOUT ALERTS AND ALERT LEVEL
    2) ABOUT MODES
    3)ABOUT THE MANUAL STATUS OF RELAYS
    4)
  
  
  void loop
  {
    Check if the MQTT client is not connected. If not connected, attempt to reconnect.
    Run the MQTT client loop to maintain the connection and handle incoming messages.
    Update the time client to keep track of time.
    Check if the specified time interval (1 second) has elapsed since the last execution of sendsetpointmega(). If yes, execute sendsetpointmega().
    Check if the specified time interval (3 seconds) has elapsed since the last execution of getvalue.start(100). If yes, trigger the data retrieval process.
    Check if the data retrieval process (getvalue) is currently running. If yes, execute sendnodedata().
    Check if the data retrieval process (getvalue) has just finished. If yes, perform any necessary actions (this section is currently empty in the provided code).
    Check if the specified time interval (5 seconds) has elapsed since the last execution of alert() and eventstatus(). If yes, trigger these functions.
    Check if the specified time interval (4 seconds) has elapsed since the last execution of manualstatus(). If yes, retrieve and update the status of pump 1 and pump 2.
    Check if the specified time interval (8 seconds) has elapsed since the last execution of tocloud(). If yes, send incoming data from the mega to the MQTT broker.
  }







*/
