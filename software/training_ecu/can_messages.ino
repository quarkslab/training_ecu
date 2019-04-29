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

 
 This file contains functions to process CAN messages 
 

*/



#include "types.h"

CAN_message_t out_secu16;
CAN_message_t out_secu32;
CAN_message_t out_read1;
CAN_message_t out_read2;

extern uint8_t key[2];
extern uint8_t seed[2];
extern uint8_t key2[4];
extern uint8_t seed2[4];

// -------------------------------------------------------------------------------------------------
// Manage the first message of the training: receive 01020304 from 700, reply with 04030201 from 3D0
// -------------------------------------------------------------------------------------------------
void process_message_ECU01(CAN_message_t *msg) {
  int id = ID_ECU1;
  if (msg->id == 0x700) {
    if ( (msg->len == 4) && (msg->buf[0] == 1) && (msg->buf[1] == 2) && (msg->buf[2] == 3) && (msg->buf[3] == 4) ) {
      CAN_message_t out;
      int len = 4;
      out.ext = 0;
      out.id = id;
      out.len = len;
      for (int i = 0; i < len; i++) {
          out.buf[i] = len-i;
      }
      CAN_send(&out);
    }
  }
}

// ------------------------
// Manage UDS 27 01 in ECU1
// ------------------------
void process_message_ECU02(CAN_message_t *msg) {
  int id = ID_ECU1;
  if (msg->id == 0x7E0) {
    out_secu16.ext = 0;
    out_secu16.id = id;
    if ( (msg->buf[0] == 0x2) && (msg->buf[1] == 0x27) && (msg->buf[2] == 0x01) ) {
      if (diag_session1 == 1) {
        compute_seed_16();
        compute_correct_key_16();
  
        out_secu16.len = 5;
        out_secu16.buf[0] = 0x4;
        out_secu16.buf[1] = 0x67;
        out_secu16.buf[2] = 1;
        for (int i = 0; i < 2; ++i) {
            out_secu16.buf[3 + i] = seed[i];
        }
        CAN_send(&out_secu16);
      } else {
        out_secu16.len = 3;
        out_secu16.buf[0] = 0x2;
        out_secu16.buf[1] = 0x7F;
        out_secu16.buf[2] = 0x22;
        CAN_send(&out_secu16);
      }
    }
  }
}
  
// ------------------------
// Manage UDS 27 02 in ECU1
// ------------------------
void process_message_ECU03(CAN_message_t *msg) {
    if ( (msg->buf[0] == 0x4) && (msg->buf[1] == 0x27) && (msg->buf[2] == 0x02) ) {
      if (diag_session1 == 1) {
        int ret = 0x67;
  
        for (int i = 0; i < 2; ++i) {
            if (key[i] != msg->buf[3 + i]) {
                ret = 0x7f;
            }
        }
        if (ret != 0x7f)
          secu_session1 = 1;
          
        out_secu16.len = 3;
        out_secu16.buf[0] = 0x2;
        out_secu16.buf[1] = ret;  // status
        out_secu16.buf[2] = 2;
        CAN_send(&out_secu16);
      } else {
        out_secu16.len = 3;
        out_secu16.buf[0] = 0x2;
        out_secu16.buf[1] = 0x7F;
        out_secu16.buf[2] = 0x22; // Conditions not correct
        CAN_send(&out_secu16);
      }
    }
}

// ---------------------
// Manage UDS 10 in ECU1
// ---------------------
void process_message_ECU1_service_10(CAN_message_t *msg) {
    if ( (msg->buf[0] == 0x2) && (msg->buf[1] == 0x10) && (msg->buf[2] == 0x01) ) {
      diag_session1 = 1;
      time_diag_session1 = timeECU2;
      out_secu16.len = 3;
      out_secu16.buf[0] = 2;
      out_secu16.buf[1] = 0x50;
      out_secu16.buf[2] = 0x01;
      CAN_send(&out_secu16);
    }
}

// ---------------------
// Manage UDS 27 in ECU1
// ---------------------
void process_message_ECU1_service_27(CAN_message_t *msg) {
  process_message_ECU02(msg);  // service 27 01
  process_message_ECU03(msg);  // service 27 02
}

// ------------------------
// Manage UDS 27 01 in ECU2
// ------------------------
void process_message_ECU2_service_27_01(CAN_message_t *msg) {
  int id = ID_ECU2;
  if (msg->id == 0x7E1) {
    out_secu32.ext = 0;
    out_secu32.id = id;
    if ( (msg->buf[0] == 0x2) && (msg->buf[1] == 0x27) && (msg->buf[2] == 0x01) ) {
      if (diag_session2 == 1) {
        compute_seed_32();
        compute_correct_key_32();
  
        out_secu32.len = 7;
        out_secu32.buf[0] = 0x6;
        out_secu32.buf[1] = 0x67;
        out_secu32.buf[2] = 1;
        for (int i = 0; i < 4; ++i) {
            out_secu32.buf[3 + i] = seed2[i];
        }
        CAN_send(&out_secu32);
      } else {
        out_secu32.len = 3;
        out_secu32.buf[0] = 0x2;
        out_secu32.buf[1] = 0x7F;
        out_secu32.buf[2] = 0x22;
        CAN_send(&out_secu32);
      }
    }
  }
}

// ------------------------
// Manage UDS 27 02 in ECU2
// ------------------------
void process_message_ECU2_service_27_02(CAN_message_t *msg) {
    if ( (msg->buf[0] == 0x6) && (msg->buf[1] == 0x27) && (msg->buf[2] == 0x02) ) {
      if (diag_session2 == 1) {
        int ret = 0x67;
  
        for (int i = 0; i < 4; ++i) {
            if (key2[i] != msg->buf[3 + i]) {
                ret = 0x7f;
            }
        }
        if (ret != 0x7f)
          secu_session2 = 1;
          
        out_secu32.len = 3;
        out_secu32.buf[0] = 2;
        out_secu32.buf[1] = ret;
        out_secu32.buf[2] = 2;
        CAN_send(&out_secu32);
      } else {
        out_secu32.len = 3;
        out_secu32.buf[0] = 0x2;
        out_secu32.buf[1] = 0x7F;
        out_secu32.buf[2] = 0x22;
        CAN_send(&out_secu32);
      }
    }
}

// ---------------------
// Manage UDS 27 in ECU2
// ---------------------
void process_message_ECU2_service_27(CAN_message_t *msg) {
  process_message_ECU2_service_27_01(msg);
  process_message_ECU2_service_27_02(msg);
}


// ---------------------
// Manage UDS 10 in ECU2
// ---------------------
void process_message_ECU2_service_10_01(CAN_message_t *msg) {
    if ( (msg->buf[0] == 0x2) && (msg->buf[1] == 0x10) && (msg->buf[2] == 0x01) ) {
      diag_session2 = 1;
      time_diag_session2 = timeECU2;
      out_secu32.len = 3;
      out_secu32.buf[0] = 2;
      out_secu32.buf[1] = 0x50;
      out_secu32.buf[2] = 0x01;
      CAN_send(&out_secu32);
    }
}

// -------------------------------------
// Read the RPM in ECU2 periodic message
// -------------------------------------
void process_message_ECU2_RPM(CAN_message_t *msg) {
  if (msg->buf[0] == 0x0) {
    rpm = (msg->buf[1] << 8) | (msg->buf[2]);
  }
}


// ---------------------
// Manage UDS 23 in ECU3
// ---------------------
void process_message_ECU3_service_23(CAN_message_t *msg) {
  int id = 0x404;
  uint32_t *ptrl;
  uint32_t data;
  uint32_t adress;
  if (msg->id == 0x7E2) {
    out_read1.ext = 0;
    out_read1.id = id;
    if (msg->buf[0] == 0x23) {
      adress = (msg->buf[1]<<24) | (msg->buf[2]<<16) | (msg->buf[3]<<8) | (msg->buf[4]);
      ptrl = (uint32_t*)adress;
      data = *ptrl;
      out_read1.len = 5;
      out_read1.buf[0] = 0x63;
      out_read1.buf[1] = (uint8_t)(data&0xFF);
      out_read1.buf[2] = (uint8_t)((data>>8)&0xFF);
      out_read1.buf[3] = (uint8_t)((data>>16)&0xFF);
      out_read1.buf[4] = (uint8_t)((data>>24)&0xFF);
      CAN_send(&out_read1);
    }
  }
}


// ---------------------------------------------
// Read the door status in ECU3 periodic message
// ---------------------------------------------
void process_message_ECU3_periodic_messages(CAN_message_t *msg) {
  if (msg->id == 0x271) {
    if ( (msg->buf[0] == 0x07) && (msg->buf[1] == 0x30) && (msg->buf[2] == 0x12) && (msg->buf[3] == 0x07) ) {
      door_hatch = 1;
    }
    if ( (msg->buf[0] == 0x07) && (msg->buf[1] == 0x30) && (msg->buf[2] == 0x11) && (msg->buf[3] == 0x07) && (msg->buf[4] == 0x20) && (msg->buf[5] == 0x40) ) {
      door_hatch = 0;
      door_driver = 0;
      door_passenger = 0;
    }
    if ( (msg->buf[0] == 0x07) && (msg->buf[1] == 0x30) && (msg->buf[2] == 0x11) && (msg->buf[3] == 0x07) && (msg->buf[4] == 0x40) && (msg->buf[5] == 0x40) ) {
      door_hatch = 1;
      door_driver = 1;
      door_passenger = 1;
    }
  }
}

// ---------------------
// Manage UDS 23 in ECU4
// ---------------------
void process_message_ECU4_service_23(CAN_message_t *msg) {
  int id = ID_ECU4;
  uint32_t *ptrl;
  uint32_t data;
  uint32_t adress;
  if ( (msg->id == 0x7E3) && (diag_session2 == 1) ) {
    out_read1.ext = 0;
    out_read1.id = id;
    if (msg->buf[0] == 0x23) {
      adress = (msg->buf[1]<<24) | (msg->buf[2]<<16) | (msg->buf[3]<<8) | (msg->buf[4]);
      ptrl = (uint32_t*)adress;
      data = *ptrl;
      out_read1.len = 5;
      out_read1.buf[0] = 0x63;
      out_read1.buf[1] = (uint8_t)(data&0xFF);
      out_read1.buf[2] = (uint8_t)((data>>8)&0xFF);
      out_read1.buf[3] = (uint8_t)((data>>16)&0xFF);
      out_read1.buf[4] = (uint8_t)((data>>24)&0xFF);
      CAN_send(&out_read1);
    }
  }
}


// ------------------------------------------------
// Read the sensors status in ECU4 periodic message
// ------------------------------------------------
void process_message_ECU4_periodic_message_1(CAN_message_t *msg) {
  if ((msg->id & 0xF0F) == 0x304) { // read the message of the ECU
    other_distance = msg->buf[3];
    other_touch = msg->buf[4];
    other_light = msg->buf[5];
    Serial1.print("Decoded other touch: ");
    Serial1.println(other_touch);
  }
}

// -------------------------------------------------
// Read the hidden commands in ECU4 periodic message
// -------------------------------------------------
void process_message_ECU4_periodic_message_2(CAN_message_t *msg) {
  int id = ID_ECU4;
  if (msg->id == 0x142) {
    out_read1.ext = 0;
    out_read1.id = id;
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x10) ) {
      hidden_command1 = 1;
    }
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x01) ) {
      hidden_command1 = 0;
    }
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x20) ) {
      hidden_command2 = 1;
    }
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x02) ) {
      hidden_command2 = 0;
    }
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x40) ) {
      hidden_command3 = 1;
    }
    if ( (msg->buf[0] == 0x1) && (msg->buf[1] == 0x04) ) {
      hidden_command3 = 0;
    }
    if (msg->buf[0] != 1) {
      out_read1.len = 1;
      out_read1.buf[0] = 0x11;
      CAN_send(&out_read1);
    }
  }
}
