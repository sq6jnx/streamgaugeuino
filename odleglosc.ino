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







void aprs_send_position_report(char* symbol)
{
    /*
    As stated in APRS101.pdf (p. 32) frame with no timestamp and no APRS
    messaging BUT with comment should look like:

    !4903.50N/07201.75W-Test 001234

    and can not be longer than 43 characters (what we don't check here).
    */

  // TODO: move this to a separate fucntion/macro
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


  //char symbol[3]="/w";
  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte('!');                 // Live Item
  ax25_send_string(OBJECT_LATITUDE);   // Lat: 38deg and 22.20 min (.20 are 
                                       // NOT seconds, but 1/100th of minutes)
  ax25_send_byte(symbol[0]);                 // Symbol table ID
  ax25_send_string(OBJECT_LONGITUDE);  // Lon: 000deg and 25.80 min
  ax25_send_byte(symbol[1]);                 // Symbol code (w==water station)
  // comment:  36-43 chars
  ax25_send_string(OBJECT_COMMENT);

  ax25_send_footer();
  ax25_flush_frame();              // Tell the modem to go
}











void aprs_send_telemetry_report(
    byte number_of_parameters, /* analog+binary! */
    byte a1, byte a2=0, byte a3=0, byte a4=0, byte a5=0,
    byte b1=0, byte b2=0, byte b3=0, byte b4=0, byte b5=0, 
    byte b6=0, byte b7=0, byte b8=0)
{
  // TODO: move this to a separate fucntion/macro
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_string("PARM.");   
  
  char buffer[33];
  ax25_send_string(itoa(a1, buffer, 10));   
  if(number_of_parameters>= 2) { 
      ax25_send_byte(','); ax25_send_string(itoa(a2, buffer, 10)); }
  if(number_of_parameters>= 3) { 
      ax25_send_byte(','); ax25_send_string(itoa(a3, buffer, 10)); }
  if(number_of_parameters>= 4) { 
      ax25_send_byte(','); ax25_send_string(itoa(a4, buffer, 10)); }
  if(number_of_parameters>= 5) { 
      ax25_send_byte(','); ax25_send_string(itoa(a5, buffer, 10)); }
  if(number_of_parameters>= 6) { ax25_send_byte(b1+48); } /* 1 -> '1' */
  if(number_of_parameters>= 7) { ax25_send_byte(b2+48); } /* 1 -> '1' */
  if(number_of_parameters>= 8) { ax25_send_byte(b3+48); } /* 1 -> '1' */
  if(number_of_parameters>= 9) { ax25_send_byte(b4+48); } /* 1 -> '1' */
  if(number_of_parameters>=10) { ax25_send_byte(b5+48); } /* 1 -> '1' */
  if(number_of_parameters>=11) { ax25_send_byte(b6+48); } /* 1 -> '1' */
  if(number_of_parameters>=12) { ax25_send_byte(b7+48); } /* 1 -> '1' */
  if(number_of_parameters>=13) { ax25_send_byte(b8+48); } /* 1 -> '1' */

  ax25_send_footer();
  ax25_flush_frame();
}


void aprs_send_telemetry_eqns(
    byte number_of_parameters, /* only analog! */
    byte a1, byte a2=0, byte a3=0, byte a4=0, byte a5=0,
    byte b1=0, byte b2=0, byte b3=0, byte b4=0, byte b5=0, 
    byte b6=0, byte b7=0, byte b8=0)
{
  // TODO: move this to a separate fucntion/macro
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_string("PARM.");   
  
  char buffer[33];
  
  sprintf(cVal,"%f",fVal); // string result '65' is stored in char array cVal

  ax25_send_string(itoa(a1, buffer, 10));   
  if(number_of_parameters>= 2) { 
      ax25_send_byte(','); ax25_send_string(itoa(a2, buffer, 10)); }
  if(number_of_parameters>= 3) { 
      ax25_send_byte(','); ax25_send_string(itoa(a3, buffer, 10)); }
  if(number_of_parameters>= 4) { 
      ax25_send_byte(','); ax25_send_string(itoa(a4, buffer, 10)); }
  if(number_of_parameters>= 5) { 
      ax25_send_byte(','); ax25_send_string(itoa(a5, buffer, 10)); }
  if(number_of_parameters>= 6) { ax25_send_byte(b1+48); } /* 1 -> '1' */
  if(number_of_parameters>= 7) { ax25_send_byte(b2+48); } /* 1 -> '1' */
  if(number_of_parameters>= 8) { ax25_send_byte(b3+48); } /* 1 -> '1' */
  if(number_of_parameters>= 9) { ax25_send_byte(b4+48); } /* 1 -> '1' */
  if(number_of_parameters>=10) { ax25_send_byte(b5+48); } /* 1 -> '1' */
  if(number_of_parameters>=11) { ax25_send_byte(b6+48); } /* 1 -> '1' */
  if(number_of_parameters>=12) { ax25_send_byte(b7+48); } /* 1 -> '1' */
  if(number_of_parameters>=13) { ax25_send_byte(b8+48); } /* 1 -> '1' */

  ax25_send_footer();
  ax25_flush_frame();
}


void aprs_send_telemetry_parameter_name(
    byte number_of_parameters, /* analog+binary! */
    char* a1, char* a2="", char* a3="", char* a4="", char* a5="",
    char* b1="", char* b2="", char* b3="", char* b4="", char* b5="", 
    char* b6="", char* b7="", char* b8="")
{
  // TODO: move this to a separate fucntion/macro
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_string("PARM.");   
  
  ax25_send_string(a1);   
  if(number_of_parameters>= 2) { ax25_send_byte(','); ax25_send_string(a2); }
  if(number_of_parameters>= 3) { ax25_send_byte(','); ax25_send_string(a3); }
  if(number_of_parameters>= 4) { ax25_send_byte(','); ax25_send_string(a4); }
  if(number_of_parameters>= 5) { ax25_send_byte(','); ax25_send_string(a5); }
  if(number_of_parameters>= 6) { ax25_send_byte(','); ax25_send_string(b1); }
  if(number_of_parameters>= 7) { ax25_send_byte(','); ax25_send_string(b2); }
  if(number_of_parameters>= 8) { ax25_send_byte(','); ax25_send_string(b3); }
  if(number_of_parameters>= 9) { ax25_send_byte(','); ax25_send_string(b4); }
  if(number_of_parameters>=10) { ax25_send_byte(','); ax25_send_string(b5); }
  if(number_of_parameters>=11) { ax25_send_byte(','); ax25_send_string(b6); }
  if(number_of_parameters>=12) { ax25_send_byte(','); ax25_send_string(b7); }
  if(number_of_parameters>=13) { ax25_send_byte(','); ax25_send_string(b8); }

  ax25_send_footer();
  ax25_flush_frame();
}

void aprs_send_telemetry_unit_label(
    byte number_of_units, /* analog+binary! */
    char* a1, char* a2="", char* a3="", char* a4="", char* a5="",
    char* b1="", char* b2="", char* b3="", char* b4="", char* b5="", 
    char* b6="", char* b7="", char* b8="")
{
  // TODO: move this to a separate fucntion/macro
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

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_string("UNIT.");   
  
  ax25_send_string(a1);   
  if(number_of_units>= 2) { ax25_send_byte(','); ax25_send_string(a2); }
  if(number_of_units>= 3) { ax25_send_byte(','); ax25_send_string(a3); }
  if(number_of_units>= 4) { ax25_send_byte(','); ax25_send_string(a4); }
  if(number_of_units>= 5) { ax25_send_byte(','); ax25_send_string(a5); }
  if(number_of_units>= 6) { ax25_send_byte(','); ax25_send_string(b1); }
  if(number_of_units>= 7) { ax25_send_byte(','); ax25_send_string(b2); }
  if(number_of_units>= 8) { ax25_send_byte(','); ax25_send_string(b3); }
  if(number_of_units>= 9) { ax25_send_byte(','); ax25_send_string(b4); }
  if(number_of_units>=10) { ax25_send_byte(','); ax25_send_string(b5); }
  if(number_of_units>=11) { ax25_send_byte(','); ax25_send_string(b6); }
  if(number_of_units>=12) { ax25_send_byte(','); ax25_send_string(b7); }
  if(number_of_units>=13) { ax25_send_byte(','); ax25_send_string(b8); }

  ax25_send_footer();
  ax25_flush_frame();
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
    char* symbol="/w";
    double sensor = get_distance();
    double current_state=0;

    

    if (sensor<0) {
        //aprs_send(-1);
    } else {
        current_state = WATER_MOUNT_HEIGHT-sensor;
        if(current_state>=WATER_WARN_STATE) {
            symbol="\\w";
        }
    }

    aprs_send_position_report(symbol); delay(3000);
    aprs_send_telemetry_parameter_name(2, A1_name, A2_name); delay(3000);
    aprs_send_telemetry_unit_label(2, A1_unit, A2_unit); delay(3000);
    aprs_send_telemetry_report(2, current_state, 50); delay(3000);
    

        //aprs_send((int)current_state);
}
