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

 
 This file contains common types (CAN database, ECU)
 

*/

#ifndef TYPES_H
#define TYPES_H

#define CANDB_SIZE 8

typedef struct {
  uint16_t UDS;
  uint16_t CAN_ID_value;
  uint16_t CAN_ID_mask;
  void (*UDS_service)(CAN_message_t *msg);
} can_db_entry_t;

typedef struct {
  can_db_entry_t *can_entry[CANDB_SIZE];
} can_db_t;

typedef struct {
  int id;
  can_db_t canDB;
  void (*periodicSendMsg)(int);
} ecu_t;

#endif
