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



#ifdef DHTPIN
DHT dht(DHTPIN, DHTTYPE);
#endif

void setup() {
 pinMode(SONAR_TRIGGER_PIN,OUTPUT);  // set TP output pin for trigger
 pinMode(SONAR_ECHO_PIN,INPUT);      // set EP input pin for echo


  //pinMode(LED_PIN, OUTPUT);                                                     
#ifdef DEBUG_RESET                                                              
   Serial.println("RESET");                                                      
   Serial.begin(9600);                                                   
#endif                                                                          
   modem_setup();                                                                
   //buzzer_setup();                                                               
  pinMode(SONAR_ECHO_PIN, INPUT);

#ifdef DHTPIN
dht.begin();
#endif

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
    char* A1_a="0", char* A1_b="0", char* A1_c="0",
    char* A2_a="0", char* A2_b="0", char* A2_c="0",
    char* A3_a="0", char* A3_b="0", char* A3_c="0",
    char* A4_a="0", char* A4_b="0", char* A4_c="0",
    char* A5_a="0", char* A5_b="0", char* A5_c="0")
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
  ax25_send_string("EQNS.");   
  
  char buffer[33];
  ax25_send_string(A1_a); ax25_send_byte(',');
  ax25_send_string(A1_b); ax25_send_byte(',');
  ax25_send_string(A1_c);   

  if(number_of_parameters>= 2) { 
      ax25_send_byte(','); ax25_send_string(A2_a);   
      ax25_send_byte(','); ax25_send_string(A2_b);   
      ax25_send_byte(','); ax25_send_string(A2_c);   
  }
  if(number_of_parameters>= 3) { 
      ax25_send_byte(','); ax25_send_string(A3_a);   
      ax25_send_byte(','); ax25_send_string(A3_b);   
      ax25_send_byte(','); ax25_send_string(A3_c);   
  }
  if(number_of_parameters>= 4) { 
      ax25_send_byte(','); ax25_send_string(A4_a);   
      ax25_send_byte(','); ax25_send_string(A4_b);   
      ax25_send_byte(','); ax25_send_string(A4_c);   
  }
  if(number_of_parameters>= 5) { 
      ax25_send_byte(','); ax25_send_string(A5_a);   
      ax25_send_byte(','); ax25_send_string(A5_b);   
      ax25_send_byte(','); ax25_send_string(A5_c);   
  }

  

  ax25_send_footer();
  ax25_flush_frame();
}

void aprs_send_telemetry_bit_sense_project_name(
    byte number_of_parameters, /* does not matter! */
    byte b1=0, byte b2=0, byte b3=0, byte b4=0, byte b5=0, 
    byte b6=0, byte b7=0, byte b8=0, char* project_name="")
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
  ax25_send_string("BITS.");   

  ax25_send_byte(b1+48); /* 1 -> '1' */
  ax25_send_byte(b2+48); /* 1 -> '1' */
  ax25_send_byte(b3+48); /* 1 -> '1' */
  ax25_send_byte(b4+48); /* 1 -> '1' */
  ax25_send_byte(b5+48); /* 1 -> '1' */
  ax25_send_byte(b6+48); /* 1 -> '1' */
  ax25_send_byte(b7+48); /* 1 -> '1' */
  ax25_send_byte(b8+48); /* 1 -> '1' */
  ax25_send_string(project_name);   
  
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

double get_temperature()
{
#ifdef DHTPIN
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
    return 0;
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
    return -30;;
    return t;
  }
#else
  return 0
#endif

}

double pace_of_sound(double temperature)
{
    double c=331.3 + 0.6*temperature;
    c/=10000;
    return 1/c;
}         

double get_distance() {
  int duration, distance;
  digitalWrite(SONAR_TRIGGER_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(SONAR_TRIGGER_PIN, LOW);
  duration = pulseIn(SONAR_ECHO_PIN, HIGH);
  distance = (duration/2) / pace_of_sound(get_temperature());
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

    get_temperature();

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
    aprs_send_telemetry_eqns(2, A1a,A1b,A1c, A2a,A2b,A2c);
    aprs_send_telemetry_bit_sense_project_name(0,0,0,0,0,0,0,0,0, "wodowskaz");
}
