// vim: syn=cpp tabw=2

#define trigPin 9
#define echoPin 10



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
 pinMode(trigPin,OUTPUT);       // set TP output pin for trigger
 pinMode(echoPin,INPUT);        // set EP input pin for echo


  //pinMode(LED_PIN, OUTPUT);                                                     
  Serial.begin(9600);                                                   
#ifdef DEBUG_RESET                                                              
   Serial.println("RESET");                                                      
#endif                                                                          
   modem_setup();                                                                
   //buzzer_setup();                                                               
  pinMode(echoPin, INPUT);
           
}



//#include <stdio.h>
//#include <stdlib.h>

// Exported functions // taken from aprs.cpp and modified by SQ6JNX
void aprs_send()
{
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  // according to http://www.aprs.pl/teoria.htm#ident we should use 'T' as
  // telemetry or '!' as 'Position without timestamp (no APRS messaging), 
  // or Ultimeter 2000 WX Station'

  // example frame is:
  // !5225.85NS01654.50E#
  // where:
  // ! - identifier
  // 5225.85N - szerokość geograficzna
  // 01654.50E - długość geograficzna
  // S# - symbol
  ax25_send_byte('T');
  ax25_send_string("5107.76N");     // Lat: 38deg and 22.20 min (.20 are NOT seconds, but 1/100th of minutes)
  ax25_send_byte('S');              // Symbol table
  ax25_send_string("1654.26");      // Lon: 000deg and 25.80 min
  ax25_send_byte('#');              // Symbol: O=balloon, -=QTH
  ax25_send_string("Vin=8.8V");     // input voltage
  ax25_send_string("H=80/Hw=120/Ha=150/");     // sensor values
  ax25_send_string(APRS_COMMENT);     // Comment
  ax25_send_footer();
  ax25_flush_frame();                 // Tell the modem to go
}

void telemetry_report_format() {
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte('T');            // Telemetry symbol
  // TODO:
  ax25_send_string('#MIC');       // MIC-E format (or, maybe, serial number??)

  ax25_send_string('123');         // analog value 1
  ax25_send_byte(',');
  ax25_send_string('456');         // analog value 2
  ax25_send_byte(',');
  ax25_send_string('456');         // analog value 3
  ax25_send_byte(',');
  ax25_send_string('456');         // analog value 4
  ax25_send_byte(',');
  ax25_send_string('456');         // analog value 5
  ax25_send_byte(',');
  ax_send_string('00000000');      // digital value
  ax25_send_byte(',');
  ax_send_string('comment');       // comment

  ax25_send_string(LATITUDE);     // Lat: 38deg and 22.20 min (.20 are NOT seconds, but 1/100th of minutes)
  ax25_send_byte(SYMBOL_1);              // Symbol table
  ax25_send_string(LONGITUDE);      // Lon: 000deg and 25.80 min
  ax25_send_byte('#');              // Symbol: O=balloon, -=QTH
  ax25_send_string("Vin=8.8V");     // input voltage
  ax25_send_string("H=80/Hw=120/Ha=150/");     // sensor values
  ax25_send_string(APRS_COMMENT);     // Comment
  ax25_send_footer();
  ax25_flush_frame();                 // Tell the modem to go
}

void loop() {
  
  /* 
  
  We must implement two -- overlapping -- "loops":

  First one takes, say 10 measurements in 1 minute intervals, then takes an
  average. This average is sensor value we're going to push via APRS.

  The second loop has to send APRS frames with:
  * Telemetry Report Format
  * Parameter Name Message
  * Unit/Label Message
  * Equation Coefficients Message
  * Bit Sense/Project Name Message

  Thus, we can measure water level after every frame.

  */




    
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  //if (distance >= 500 || distance <= 0){
  //  Serial.println("Out of range");
  //}
  //else {
    Serial.print(distance);
    Serial.println(" cm");
  //}
  delay(500);
  aprs_send();
  delay(5000);
}
