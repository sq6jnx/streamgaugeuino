#define trigPin 9
#define echoPin 10

/*
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance >= 500 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}
*/

#include "aprs.h"
#include "ax25.h"
#include "buzzer.h"
#include "config.h"
#include "debug.h"
#include "modem.h"
#include "radio.h"
#include "sensors.h"

// Arduino/AVR libs
#include <Wire.h>
#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>


void setup() {
                                                                               
  pinMode(LED_PIN, OUTPUT);                                                     
  Serial.begin(GPS_BAUDRATE);                                                   
#ifdef DEBUG_RESET                                                              
   Serial.println("RESET");                                                      
 #endif                                                                          
   modem_setup();                                                                
   buzzer_setup();                                                               
           
}

void loop() {
aprs_send();
delay(5000);
}
