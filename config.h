/* trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* 
   NOTE by SQ6JNX: according to APRS101, p. 93 there is no ID we can use for
   water gauges, but, as stated below, symbol in the information field takes
   precedence. Anyhow, we should not care about SSID.

   On the other hand, APRSdroid (google it) states, that SSIDs 1 to 4 and 15 are
   free ("unused"). I suggest, that one should use 1 to 4 or 15, as stated in
   APRSdroid.
*/

#define S_CALLSIGN      "SQ6JNX"
#define S_CALLSIGN_ID   15

// Destination callsign: APRS (with SSID=0) is usually okay.
#define D_CALLSIGN      "APRS"
#define D_CALLSIGN_ID   0
#define DIGI_PATH1      "WIDE2"
#define DIGI_PATH1_TTL  1

// Object information
#define OBJECT_LATITUDE    "5107.76N"
#define OBJECT_LONGITUDE   "1654.26E"
#define OBJECT_NAME        "Byst/Jarn"
#define OBJECT_COMMENT     "wodowskaz Bystrzyca/Jarnoltow " // please append space!

#define WATER_MOUNT_HEIGHT   300
#define WATER_WARN_STATE     120
#define WATER_ALERT_STATE    150

// Telemetry coefficients -- Ax_a will always be 0
// caution! coefficient MUST be defined as char*!
#define A1_name "Stan"
#define A1_unit "cm"
#define A1a "0"
#define A1b "1"   // water level
#define A1c "0"

#define A2_name "Vs"
#define A2_unit "V"
#define A2a "0"
#define A2b ".1" // source voltage
#define A2c "0"

/* unused  */
#define A3_name ""
#define A3_unit ""
#define A3a "0"
#define A3b "1"
#define A3c "0"

#define A3_name ""
#define A3_unit ""
#define A4a "0"
#define A4b "1"
#define A4c "0"

#define A3_name ""
#define A3_unit ""
#define A5a "0"
#define A5b "1"
#define A5c "0"

// Sensor configuration
#define SONAR_TRIGGER_PIN  9
#define SONAR_ECHO_PIN    10


#include "DHT.h"
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

//// --------------------------------------------------------------------------
//// THIS IS THE TRACKUINO FIRMWARE CONFIGURATION FILE. YOUR CALLSIGN AND
//// OTHER SETTINGS GO HERE.
////
//// NOTE: all pins are Arduino based, not the Atmega chip. Mapping:
//// http://www.arduino.cc/en/Hacking/PinMapping
//// --------------------------------------------------------------------------
//
//
//// --------------------------------------------------------------------------
//// APRS config (aprs.c)
//// --------------------------------------------------------------------------
//
//// Set your callsign and SSID here. Common values for the SSID are
//// (from http://zlhams.wikidot.com/aprs-ssidguide):
////
//// - Balloons:  11
//// - Cars:       9
//// - Home:       0
//// - IGate:      5
//#define S_CALLSIGN      "SQ6JNX"
//#define S_CALLSIGN_ID   0
//
//// Destination callsign: APRS (with SSID=0) is usually okay.
//#define D_CALLSIGN      "APRS"
//#define D_CALLSIGN_ID   0
//
//// Digipeating paths:
//// (read more about digipeating paths here: http://wa8lmf.net/DigiPaths/ )
//// The recommended digi path for a balloon is WIDE2-1 or pathless. The default
//// is to use WIDE2-1. Comment out the following two lines for pathless:
//#define DIGI_PATH1      "WIDE2"
//#define DIGI_PATH1_TTL  1
//
//// APRS comment: this goes in the comment portion of the APRS message. You
//// might want to keep this short. The longer the packet, the more vulnerable
//// it is to noise. 
//#define APRS_COMMENT    "Bystrzyca/Jerzmanow"

// --------------------------------------------------------------------------
// AX.25 config (ax25.cpp)
// --------------------------------------------------------------------------
//
// TX delay in milliseconds
#define TX_DELAY      300

//// --------------------------------------------------------------------------
//// Tracker config (trackuino.cpp)
//// --------------------------------------------------------------------------
//
//// APRS_PERIOD is the period between transmissions. Since we're not listening
//// before transmitting, it may be wise to choose a "random" value here JUST
//// in case someone else is transmitting at fixed intervals like us. 61000 ms
//// is the default (1 minute and 1 second).
////
//// Low-power transmissions on occasional events (such as a balloon launch)
//// might be okay at lower-than-standard APRS periods (< 10m). Check with/ask
//// permision to local digipeaters beforehand.
//#define APRS_PERIOD   61000UL
//
//// Set any value here (in ms) if you want to delay the first transmission
//// after resetting the device.
//#define APRS_DELAY    0UL
//
//// GPS baud rate (in bits per second)
////#define GPS_BAUDRATE  9600


// --------------------------------------------------------------------------
// Modem config (modem.cpp)
// --------------------------------------------------------------------------

// AUDIO_PIN is the audio-out pin. The audio is generated by timer 2 using
// PWM, so the only two options are pins 3 and 11.
// Pin 11 doubles as MOSI, so I suggest using pin 3 for PWM and leave 11 free
// in case you ever want to interface with an SPI device.

#define AUDIO_PIN       3

// Radio: I've tested trackuino with two different radios:
// Radiometrix HX1 and SRB MX146. The interface with these devices
// is implemented in their respective radio_*.cpp files, and here
// you can choose which one will be hooked up to the tracker.
// The tracker will behave differently depending on the radio used:
//
// RadioHx1 (Radiometrix HX1):
// - Time from PTT-on to transmit: 5ms (per datasheet)
// - PTT is TTL-level driven (on=high) and audio input is 5v pkpk
//   filtered and internally DC-coupled by the HX1, so both PTT
//   and audio can be wired directly. Very few external components
//   are needed for this radio, indeed.
//
// RadioMx146 (SRB MX146):
// - Time from PTT-on to transmit: signaled by MX146 (pin RDY)
// - Uses I2C to set freq (144.8 MHz) on start
// - I2C requires wiring analog pins 4/5 (SDA/SCL) via two level
//   converters (one for each, SDA and SCL). DO NOT WIRE A 5V ARDUINO
//   DIRECTLY TO THE 3.3V MX146, YOU WILL DESTROY IT!!!
//
//   I2C 5-3.3v LEVEL TRANSLATOR:
//
//    +3.3v o--------+-----+      +---------o +5v
//                   /     |      /
//                R  \     |      \ R
//                   /    G|      /
//              3K3  \   _ L _    \ 4K7
//                   |   T T T    |
//   3.3v device o---+--+|_| |+---+---o 5v device
//                     S|     |D
//                      +-|>|-+
//                             N-channel MOSFET
//                           (BS170, for instance)
//
//   (Explanation of the lever translator:
//   http://www.neoteo.com/adaptador-de-niveles-para-bus-i2c-3-3v-5v.neo)
//
// - Audio needs a low-pass filter (R=8k2 C=0.1u) plus DC coupling
//   (Cc=10u). This also lowers audio to 500mV peak-peak required by
//   the MX146.
//
//                   8k2        10uF
//   Arduino out o--/\/\/\---+---||---o
//                     R     |     Cc
//                          ===
//                     0.1uF | C
//                           v
//
// - PTT is pulled internally to 3.3v (off) or shorted (on). Use
//   an open-collector BJT to drive it:
//        
//                             o MX146 PTT
//                             |
//                    4k7    b |c
//   Arduino PTT o--/\/\/\----K  (Any NPN will do)
//                     R       |e
//                             |
//                             v GND
// 
// - Beware of keying the MX146 for too long, you will BURN it.
//
// So, summing up. Options are:
//
// - RadioMx146
// - RadioHx1
#define RADIO_CLASS   RadioHx1

// --------------------------------------------------------------------------
// Radio config (radio_*.cpp)
// --------------------------------------------------------------------------

// This is the PTT pin
#define PTT_PIN           4

//// This is the pin used by the MX146 radio to signal full RF
//#define MX146_READY_PIN   2
//
//// --------------------------------------------------------------------------
//// Sensors config (sensors.cpp)
//// --------------------------------------------------------------------------
//
//// Most of the sensors.cpp functions use internal reference voltages (either
//// AVCC or 1.1V). If you want to use an external reference, you should
//// uncomment the following line:
////
//// #define USE_AREF
////
//// BEWARE! If you hook up an external voltage to the AREF pin and 
//// accidentally set the ADC to any of the internal references, YOU WILL
//// FRY YOUR AVR.
////
//// It is always advised to connect the AREF pin through a pull-up resistor,
//// whose value is defined here in ohms (set to 0 if no pull-up):
////
//#define AREF_PULLUP           4700
////
//// Since there is already a 32K resistor at the ADC pin, the actual
//// voltage read will be VREF * 32 / (32 + AREF_PULLUP)
////
//// Read more in the Arduino reference docs:
//// http://arduino.cc/en/Reference/AnalogReference?from=Reference.AREF
//
//// Pin mappings for the internal / external temperature sensors. VS refers
//// to (arduino) digital pins, whereas VOUT refers to (arduino) analog pins.
//#define INTERNAL_LM60_VS_PIN     6
//#define INTERNAL_LM60_VOUT_PIN   0
//#define EXTERNAL_LM60_VS_PIN     7
//#define EXTERNAL_LM60_VOUT_PIN   1
//
//// Units for temperature sensors (Added by: Kyle Crockett)
//// 1 = Celsius, 2 = Kelvin, 3 = Fahrenheit
//#define TEMP_UNIT 1
//
//// Calibration value in the units selected. Use integer only.
//#define CALIBRATION_VAL 0
//
//// --------------------------------------------------------------------------
//// Buzzer config (buzzer.cpp)
//// --------------------------------------------------------------------------
//
//// This is the buzzer frequency. Choose one that maximizes the output volume
//// according to your buzzer's datasheet. It must be between L and 65535,
//// where L = F_CPU / 65535 and F_CPU is the clock frequency in hertzs. For
//// Arduinos (16 MHz), that gives a low limit of 245 Hz.
//#define BUZZER_FREQ             895     // Hz
//
//// These are the number of seconds the buzzer will stay on/off alternately
//#define BUZZER_ON_TIME          2       // secs
//#define BUZZER_OFF_TIME         2       // secs
//
//// This option disables the buzzer above BUZZER_ALTITUDE meters. This is a
//// float value, so make it really high (eg. 1000000.0 = 1 million meters)
//// if you want it to never stop buzzing.
////#define BUZZER_ALTITUDE         3000.0  // meters (divide by 0.3048 for ft.)
//
//// The buzzer is driven by timer 1, so the options here are pin 9 or 10
//#define BUZZER_PIN              9
//
//// --------------------------------------------------------------------------
//// Debug
//// --------------------------------------------------------------------------
//
//// This is the LED pin (13 on Arduinos). The LED will be on while the AVR is
//// running and off while it's sleeping, so its brightness gives an indication
//// of the activity.
//#define LED_PIN                 13
//
//// Debug info includes printouts from different modules to aid in testing and
//// debugging.
//// 
//// 1. To properly receive debug information, only connect the Arduino RX pin 
////    to the GPS TX pin, and leave the Arduino TX pin disconnected. 
////
//// 2. On the serial monitor, set the baudrate to GPS_BAUDRATE (above),
////    usually 9600.
////
//// 3. When flashing the firmware, disconnect the GPS from the RX pin or you
////    will get errors.
//
////#define DEBUG_GPS    // GPS sentence dump and checksum validation
#define DEBUG_AX25   // AX.25 frame dump
//#define DEBUG_MODEM  // Modem ISR overrun and profiling
#define DEBUG_RESET  // AVR reset


#endif
