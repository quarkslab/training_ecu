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

 
 This file contains functions to process periodic CAN messages 
 

*/


#include "types.h"

// ---------------------------------
// Send the periodic message of ECU1
// ---------------------------------
void heartBeat_ECU1(int id) {
    CAN_message_t out;
    out.id = id;
    out.len = 8;
    out.buf[0] = 02;
    out.buf[1] = 40;
    out.buf[2] = 0;
    out.buf[3] = 0;
    out.buf[4] = (turn_right<<7) | (turn_left<<6);
    out.buf[5] = 8;
    out.buf[6] = 0x10;
    out.buf[7] = diag_session1 | (secu_session1<<4);
    
    CAN_send(&out);
}

// ---------------------------------
// Send the periodic message of ECU2
// ---------------------------------
void heartBeat_ECU2(int id) {
    CAN_message_t out;
    out.id = id;
    out.len = 5;
    out.buf[0] = 0;
    out.buf[1] = 0x60;
    out.buf[2] = (rpm>>8)&255;
    out.buf[3] = rpm&255;
    out.buf[4] = diag_session2 | (secu_session2<<4);
    CAN_send(&out);
    rpm = 0; // to simulate that it is reset by the legit ECU.
}

// ---------------------------------
// Send the periodic message of ECU3
// ---------------------------------
void heartBeat_ECU3(int id) {
    CAN_message_t out;
    out.id = id;
    out.len = 3;
    out.buf[0] = door_driver;
    out.buf[1] = door_passenger;
    out.buf[2] = door_hatch;
    CAN_send(&out);
}

// ---------------------------------
// Send the periodic message of ECU4
// ---------------------------------
void heartBeat_ECU4(int id) {
    CAN_message_t out;
    out.id = 0x304;
    out.len = 6;
    out.buf[0] = hidden_command1;
    out.buf[1] = hidden_command2;
    out.buf[2] = hidden_command3;
    out.buf[3] = distance_US_sensor;
    out.buf[4] = touch;
    out.buf[5] = light;
    CAN_send(&out);
}
