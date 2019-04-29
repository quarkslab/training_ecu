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


 This file contains all CAN specific functions: send/receive messages, CAN Database management, periodic messages, init

*/



#include "types.h"

/*
 * CAN DB: for each ECU, associate a UDS service number, a MASK on the CAN ID, and a function pointer
 */

// ECUs
#define NB_ECU 4
ecu_t ecu[NB_ECU];

// ------------------------------------------
// Debug function: pretty print a CAN message
// ------------------------------------------
void hexDump(uint32_t arb_id, int8_t dumpLen, uint8_t *bytePtr, const char *str)
{
  Serial.print(arb_id, HEX);
  Serial.write("  ");
  Serial.print(str);
  Serial.write("  ");
  Serial.print(dumpLen);
  Serial.write("  ");
  while( dumpLen-- ) {
    Serial.print(*bytePtr++, HEX);
    Serial.write(" ");
  }
  Serial.println();
}

// ------------------
// send a CAN message
// ------------------
void CAN_send(CAN_message_t * msg) {
  int res = 0;
  int count = 0;
  msg->flags.remote = 0;
  msg->flags.extended = 0;

  while ((res==0) && (count<10)) {
    res = Can0.write(*msg);
    delay(10);
    if (!res) { // transmission error: no buffer available for sending
      digitalWrite(ledPin, HIGH);   // set the LED on
      count++;
      delay(1);
    }
  }
  digitalWrite(ledPin, LOW);   // set the LED off
}


// ------------------
// CAN DB declaration
// ------------------
can_db_entry_t can_entry_1 = {0,    0x700, 0x7FF, process_message_ECU01};
can_db_entry_t can_entry_2 = {0x27, 0x7E0, 0x7FF, process_message_ECU1_service_27};
can_db_entry_t can_entry_2a= {0x10, 0x7E0, 0x7FF, process_message_ECU1_service_10};
can_db_entry_t can_entry_3 = {0x10, 0x7E1, 0x7FF, process_message_ECU2_service_10_01};
can_db_entry_t can_entry_4 = {0x27, 0x7E1, 0x7FF, process_message_ECU2_service_27};
can_db_entry_t can_entry_4a= {0x0 , 0x13F, 0x7FF, process_message_ECU2_RPM};
can_db_entry_t can_entry_5 = {0,    0x271, 0x7FF, process_message_ECU3_periodic_messages};
can_db_entry_t can_entry_6 = {0x23, 0x7E2, 0x7FF, process_message_ECU3_service_23};
can_db_entry_t can_entry_7 = {0,    0x304, 0x70F, process_message_ECU4_periodic_message_1};
can_db_entry_t can_entry_8 = {0,    0x142, 0x7FF, process_message_ECU4_periodic_message_2};
can_db_entry_t can_entry_9 = {0x23, 0x7E2, 0x7FF, process_message_ECU4_service_23};

can_db_t CAN_DB_1 = { &can_entry_1, &can_entry_2, &can_entry_2a, 0 };
can_db_t CAN_DB_2 = { &can_entry_3, &can_entry_4, &can_entry_4a, 0 };
can_db_t CAN_DB_3 = { &can_entry_5, &can_entry_6, 0 };
can_db_t CAN_DB_4 = { &can_entry_7, &can_entry_8, &can_entry_9, 0 };

can_db_t *can_dbs[NB_ECU] = {&CAN_DB_1, &CAN_DB_2, &CAN_DB_3, &CAN_DB_4};


// -----------------------------
// Process a single CAN DB entry
// -----------------------------
void process_can_entry(can_db_entry_t *can_entry, CAN_message_t *msg) {
  if ((msg->id & can_entry->CAN_ID_mask) == can_entry->CAN_ID_value) {
    // TODO: check also the UDS command of the incoming message against the one of the CAN DB entry
    can_entry->UDS_service(msg);
  }
}


// ------------------------
// Process the whole CAN DB
// ------------------------
void process_can_db(can_db_t *db, CAN_message_t *msg) {
  int i;
  for (i = 0; i<CANDB_SIZE; i++) {
    if (!db->can_entry[i]) return;
    process_can_entry(db->can_entry[i], msg);
  }
}

// ---------------------------------------------
// Pass all received messages through the CAN DB
// ---------------------------------------------
void process_received_messages_for_ECU(int e, CAN_message_t *msg) {

  can_db_t * db = can_dbs[e];

  process_can_db(db, msg);
}

// ---------------------------------------------------
// Check if CAN messages are received and process them
// ---------------------------------------------------
void process_received_CAN_messages(void) {
// management of input CAN messages
  CAN_message_t msg;
  int e;
  if (Can0.available()) {
    if (!Can0.read(msg)) {
      Serial1.println("Message disappeared!");
    } else {
      hexDump(msg.id, msg.len, msg.buf, "RX");
      for (e = 0; e<NB_ECU; e++) {
        process_received_messages_for_ECU(e, &msg);
      }
    }
  }
}

// -------------------------
// Process periodic messages
// -------------------------
void process_periodic_messages(void) {
  int e;
    // send periodic messages
    for (e = 0; e<NB_ECU; e++) {
      if (ecu[e].periodicSendMsg) {
        ecu[e].periodicSendMsg(ecu[e].id);
      }
    }
}

// ---------------
// Init of the ECU
// ---------------
void init_ECUs(void) {
  ecu[0].id = 0x301;
  ecu[0].canDB = CAN_DB_1;
  ecu[0].periodicSendMsg = heartBeat_ECU1;
  
  ecu[1].id = 0x302;
  ecu[1].canDB = CAN_DB_2;
  ecu[1].periodicSendMsg = heartBeat_ECU2;
  
  ecu[2].id = 0x303;
  ecu[2].canDB = CAN_DB_3;
  ecu[2].periodicSendMsg = heartBeat_ECU3;
  
  ecu[3].id = 0x304;
  ecu[3].canDB = CAN_DB_4;
  ecu[3].periodicSendMsg = heartBeat_ECU4;
}
