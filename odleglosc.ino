// vim: syn=cpp tabw=2




#include "aprs.h"
#include "ax25.h"
//#include "buzzer.h"
#include "config.h"
#include "debug.h"
#include "modem.h"
//#include "radio.h"
//#include "sensors.h"

// Arduino/AVR libs
#include <Wire.h>
#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>

void setup() {
 pinMode(SONAR_TRIGGER_PIN,OUTPUT);  // set TP output pin for trigger
 pinMode(SONAR_ECHO_PIN,INPUT);      // set EP input pin for echo


  //pinMode(LED_PIN, OUTPUT);                                                     
  Serial.begin(9600);                                                   
#ifdef DEBUG_RESET                                                              
   Serial.println("RESET");                                                      
#endif                                                                          
   modem_setup();                                                                
   //buzzer_setup();                                                               
  pinMode(SONAR_ECHO_PIN, INPUT);
           
}

//#include <stdio.h>
//#include <stdlib.h>


void telemetry_report(int n, 
        double a1=0,double a2=0,double a3=0,double a4=0,double a5=0) {
  const struct s_address addresses[] = { 
    {D_CALLSIGN, D_CALLSIGN_ID},  // Destination callsign
    {S_CALLSIGN, S_CALLSIGN_ID},  // Source callsign (-11 = balloon, -9 = car)
#ifdef DIGI_PATH1
    {DIGI_PATH1, DIGI_PATH1_TTL}, // Digi1 (first digi in the chain)
#endif
#ifdef DIGI_PATH2
    {DIGI_PATH2, DIGI_PATH2_TTL}, // Digi2 (second digi in the chain)
#endif
  };

  char buffer[33];

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte('T');            // Telemetry symbol
  // TODO: 

  ax25_send_string("#MIC");       // MIC-E format (or, maybe, serial number??)

  
  itoa( (int)(a1-A1c)/A1b,buffer,10);
  ax25_send_string(buffer);         // analog value 1 (3 digits)
  
  if(n>1) {
      itoa( (int)(a2-A2c)/A2b,buffer,10);
      ax25_send_byte(',');
      ax25_send_string("255");         // analog value 2 (3 digits)
  }

  if(n>2) {
      itoa( (int)(a3-A3c)/A3b,buffer,10);
      ax25_send_byte(',');
      ax25_send_string("255");         // analog value 3 (3 digits)
  }

  if(n>3) {
      itoa( (int)(a4-A4c)/A4b,buffer,10);
      ax25_send_byte(',');
      ax25_send_string("255");         // analog value 4 (3 digits)
  }

  if(n>4) {
      itoa( (int)(a5-A5c)/A5b,buffer,10);
      ax25_send_byte(',');
      ax25_send_string("255");         // analog value 5 (3 digits)
  }

  // I'll leave it here for future generations.
  //ax25_send_byte(',');
  //ax25_send_string("00000000");      // digital value (8 0/1 digits)
  //ax25_send_byte(',');

  ax25_send_string(OBJECT_COMMENT);       // comment
  ax25_send_footer();
  ax25_flush_frame();                 // Tell the modem to go
}


 //Exported functions // taken from aprs.cpp and modified by SQ6JNX
void aprs_send(int poziom_wody)
{
  const struct s_address addresses[] = { 
    {D_CALLSIGN, D_CALLSIGN_ID},  // Destination callsign
    {S_CALLSIGN, S_CALLSIGN_ID},  // Source callsign (-11 = balloon, -9 = car)
/*#ifdef DIGI_PATH1
    {DIGI_PATH1, DIGI_PATH1_TTL}, // Digi1 (first digi in the chain)
#endif
#ifdef DIGI_PATH2
    {DIGI_PATH2, DIGI_PATH2_TTL}, // Digi2 (second digi in the chain)
#endif*/
  };

  char symbol[3]="/w";
  char buffer[33];
  if(poziom_wody>WATER_WARN_STATE) {
      strcpy(symbol,"\\w");
  } else if (poziom_wody<0) {
        strcpy(buffer, "ERR");
  } else {
      itoa(poziom_wody,buffer,10);
  }

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte(')');                 // Item report format
  ax25_send_string(OBJECT_NAME);       // Item name
  ax25_send_byte('!');                 // Live Item
  ax25_send_string(OBJECT_LATITUDE);   // Lat: 38deg and 22.20 min (.20 are 
                                       // NOT seconds, but 1/100th of minutes)
  ax25_send_byte(symbol[0]);                 // Symbol table ID
  ax25_send_string(OBJECT_LONGITUDE);  // Lon: 000deg and 25.80 min
  ax25_send_byte(symbol[1]);                 // Symbol code (w==water station)
  // comment:  36-43 chars
  ax25_send_string(OBJECT_COMMENT);
  ax25_send_string(buffer);
  ax25_send_string("/140/180cm 5.7V");   

  ax25_send_footer();
  ax25_flush_frame();              // Tell the modem to go
}

double get_distance() {
  int duration, distance;
  digitalWrite(SONAR_TRIGGER_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(SONAR_TRIGGER_PIN, LOW);
  duration = pulseIn(SONAR_ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  if (distance >= 500 || distance <= 0){
      return -1;
  }
  else {
      Serial.print(distance);
      Serial.println(" cm");
      return distance;
  }
}  


void loop() {

    double sensor = get_distance();
    if (sensor<0) {
        aprs_send(-1);
    } else {
        double current_state = WATER_MOUNT_HEIGHT-sensor;
        aprs_send((int)current_state);
    }

    delay(6000);
}
