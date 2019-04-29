/*

 Training ECU v0.6
 
 A simple ECU software based on a Teensy3.2 board
 
 https://github.com/quarkslab/training_ecu
 
 Copyright (c) 2019, Quarkslab
 Author: Guillaume Heilles
 All rights reserved.

  MIT License
  
  Copyright (c) 2019 Quarkslab
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

 
 Dependancies: 
 
 https://github.com/collin80/FlexCAN_Library
 https://github.com/olikraus/u8g2
 PWMServo
 

*/

#include <U8g2lib.h>
#include <U8x8lib.h>
#include <PWMServo.h>
#include <FlexCAN.h>

#define U8X8_HAVE_HW_I2C
#undef U8X8_HAVE_HW_SPI
#include <Wire.h>

#include "types.h"

// mode
#define modePin 14
int mode = 1;

// Ultrasonic sensor
// Credits to Steve Garratt for the code to read the distance using interrupts: http://homediyelectronics.com/projects/arduino/arduinoprogramminghcsr04withinterrupts/?p=4
#define trigPin 12                                    // Pin 12 trigger output
#define echoPin 2                                     // Pin 2 Echo input
#define echo_int 0                                    // Interrupt id for echo pulse
#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 1000                              // 1000 mS worth of timer ticks
volatile long echo_start = 0;                         // Records start of echo pulse 
volatile long echo_end = 0;                           // Records end of echo pulse
volatile long echo_duration = 0;                      // Duration - difference between end and start
volatile int trigger_time_count = TICK_COUNTS;        // Count down counter to trigger pulse time
volatile long range_flasher_counter = 0;              // Count down counter for flashing distance LED
IntervalTimer myTimer; 
int distance_US_sensor=1;
int previous_distance_US_sensor=1;

// Servo
PWMServo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

// Screen
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C

// Light sensor
int light_pin = 17;
int light = 0;

// Touch sensor
int touch_pin = 16;
int touch = 0;

// other ECUs
int other_touch = 0;
int other_light = 0;
int other_distance = 0;

unsigned long time_diag_session1;
unsigned long time_diag_session2;

int ID_ECU1 = 0x3D0;
int ID_ECU2 = 0x13F;
int ID_ECU3 = 0x303;
int ID_ECU4 = 0x304;

const int ledPin = 13;

int turn_right = 0;
int turn_left = 0;
int speed = 0;
int rpm = 0;
int door_driver = 0;
int door_passenger = 0;
int door_hatch = 0;
int hidden_command1 = 0;
int hidden_command2 = 0;
int hidden_command3 = 0;

int diag_session1 = 0;
int secu_session1 = 0;
int diag_session2 = 0;
int secu_session2 = 0;

int push_button_state = 0;
int light_sensor_threshold_state = 0;

elapsedMillis timeECU;
elapsedMillis timeECU2;

// --------------------------------------------------------
// Setup hardware modules: CAN, debug UARTs, sensors, servo
// --------------------------------------------------------
void setup() {

  // debug serial port
  Serial1.begin(9600);
  Serial.begin(115200);
  delay(100);

  // CAN bus
  Can0.begin(500000);
  delay(100);

  // Ultrasonic Distance sensor
  pinMode(trigPin, OUTPUT);                           // Trigger pin set to output
  pinMode(echoPin, INPUT);                            // Echo pin set to input
  myTimer.begin(timerIsr, TIMER_US); 
  attachInterrupt(digitalPinToInterrupt(echoPin), echo_interrupt, CHANGE);  // Attach interrupt to the sensor echo input

  // Mode pin (jumper)
  pinMode(modePin, INPUT_PULLUP);
  digitalWrite(modePin, HIGH);

  // Servo
  myservo.attach(9);

  // Oled screen
  u8g2.begin();

  // Light sensor
  pinMode(light_pin, INPUT);

  // Touch sensor
  pinMode(touch_pin, INPUT);

  delay(100);
  Serial1.println("Init done");

  // mode jumper
  mode = digitalRead(modePin);

  pinMode(ledPin, OUTPUT);

  init_ECUs();  
  myservo.write(pos);

  Serial.println("Init OK");

}

// ---------------------------------------------------------
// main loop: process CAN messages, update sensors and servo
// ---------------------------------------------------------
void loop() {

  delay(1); // don't block the CPU all the time !
//  Serial.print(".");

  process_received_CAN_messages();

// display text on the screen, and manage periodic CAN messages at the same interval
  if (timeECU >= 200) {
    timeECU -= 200;

    // mode
    mode = digitalRead(modePin);

    if (digitalRead(light_pin)) {
      light = 1;
    } else {
      light = 0;
    }
    if (digitalRead(touch_pin)) {
      touch = 1;
    } else  {
      touch = 0;
    }

    if (other_light != 0) light = other_light;
    if (other_touch != 0) touch = other_touch;
        
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font

    u8g2.drawStr(50, 10,"RPM:");  // write something to the internal memory
    u8g2.setCursor(90, 10);
    u8g2.print(rpm, HEX);
    
    u8g2.drawStr(0,10,"Dist:");  // write something to the internal memory
    u8g2.setCursor(30, 10);
    u8g2.print(previous_distance_US_sensor);


    u8g2.drawStr(0,20,"Light:");  // write something to the internal memory
    u8g2.setCursor(40, 20);
    u8g2.print(light);

    u8g2.drawStr(0,30,"Touch:");  // write something to the internal memory
    u8g2.setCursor(40, 30);
    u8g2.print(touch);

    u8g2.sendBuffer();          // transfer internal memory to the display

    process_periodic_messages();
    
    if ((door_driver == 1) && (door_hatch == 1)) {
      if (pos != 170) {
        pos = 170;
        myservo.write(pos);
      }
    }
    if ((door_driver == 0) && (door_hatch == 1)) {
      if (pos != 100) {
        pos = 100;
        myservo.write(pos);
      }
    }
    if ((door_driver == 0) && (door_hatch == 0)) {
      if (pos != 0) {
        pos = 0;
        myservo.write(pos);
      }
    }
  }

// manage the timeout of diagnostic sessions
  if (diag_session1 && (timeECU2 - time_diag_session1 >= 60000)) {
    diag_session1 = 0;
    time_diag_session1 = timeECU2;
    Serial1.println("Reset diag session1 timer");
  }
  if (diag_session2 && (timeECU2 - time_diag_session2 >= 60000)) {
    diag_session2 = 0;
    time_diag_session2 = timeECU2;
    Serial1.println("Reset diag session2 timer");
  }
}

// --------------------------
// timerIsr() 50uS second interrupt ISR()
// Called every time the hardware timer 1 times out.
// --------------------------
void timerIsr()
{
    trigger_pulse();                                 // Schedule the trigger pulses
}

// --------------------------
// trigger_pulse() called every 50 uS to schedule trigger pulses.
// Generates a pulse one timer tick long.
// Minimum trigger pulse width for the HC-SR04 is 10 us. This system
// delivers a 50 uS pulse.
// --------------------------
void trigger_pulse()
{
      static volatile int state = 0;                 // State machine variable

      if (!(--trigger_time_count))                   // Count to 200mS
      {                                              // Time out - Initiate trigger pulse
         trigger_time_count = TICK_COUNTS;           // Reload
         state = 1;                                  // Changing to state 1 initiates a pulse
      }
    
      switch(state)                                  // State machine handles delivery of trigger pulse
      {
        case 0:                                      // Normal state does nothing
            break;
        
        case 1:                                      // Initiate pulse
           digitalWrite(trigPin, HIGH);              // Set the trigger output high
           state = 2;                                // and set state to 2
           break;
        
        case 2:                                      // Complete the pulse
        default:      
           digitalWrite(trigPin, LOW);               // Set the trigger output low
           state = 0;                                // and return state to normal 0
           break;
     }
}

// --------------------------
// echo_interrupt() External interrupt from HC-SR04 echo signal. 
// Called every time the echo signal changes state.
//
// Note: this routine does not handle the case where the timer
//       counter overflows which will result in the occassional error.
// --------------------------
void echo_interrupt()
{
  switch (digitalRead(echoPin))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      echo_end = 0;                                 // Clear the end time
      echo_start = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      echo_end = micros();                          // Save the end time
      echo_duration = echo_end - echo_start;        // Calculate the pulse duration
      distance_US_sensor = echo_duration / 58;

      // manage sensors
      if (previous_distance_US_sensor != distance_US_sensor) {
        //Serial1.println(distance_US_sensor);               // Print the distance in centimeters
        if (other_distance != 0) {
//          myservo.write(other_distance);
        } else {
          if ((distance_US_sensor<180) && (abs(previous_distance_US_sensor - distance_US_sensor) < 10)) {
//            myservo.write(distance_US_sensor);
            previous_distance_US_sensor = distance_US_sensor;
          }
        }
      }
      break;
  }
}
