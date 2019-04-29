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

 
 This file contains common functions (PRNG, challenge/response algorithms)
 

*/


uint8_t key[2];
uint8_t seed[2] = {0};
uint8_t key2[4];
uint8_t seed2[4] = {0};


// -----------------------------------------------------------------------------
// PRNG on 16 bits: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
// -----------------------------------------------------------------------------
uint16_t compute_lfsr_16(uint16_t lfsr) {
    unsigned lsb = lfsr & 1;   /* Get LSB (i.e., the output bit). */
    lfsr >>= 1;                /* Shift register */
    lfsr ^= (-lsb) & 0xB400u;  /* If the output bit is 1, apply toggle mask. */
    return lfsr;
}

// ------------------------------
// Commute a challenge on 16 bits
// ------------------------------
void compute_seed_16(void) {
    for (int i = 0; i < 2; ++i) {
        seed[i] = random(0xff);
    }
}

// -----------------------------
// Commute a response on 16 bits
// -----------------------------
void compute_correct_key_16(void) {
    uint16_t lseed, lkey;

    lseed = (seed[0]<<8) + seed[1];
    lkey = lseed+0x12B2;
    key[0] = (lkey>>8)&255;
    key[1] = lkey&255;
}

// -----------------------------------------------------------------------------
// PRNG on 32 bits: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
// -----------------------------------------------------------------------------
uint32_t compute_lfsr32(uint32_t lfsr) {
    unsigned lsb = lfsr & 1;   /* Get LSB (i.e., the output bit). */
    lfsr >>= 1;                /* Shift register */
    lfsr ^= (-lsb) & 0xC7000017u;  /* If the output bit is 1, apply toggle mask. */
    return lfsr;
}

// ------------------------------
// Commute a challenge on 32 bits
// ------------------------------
void compute_seed_32(void) {
    int k, l;
    uint32_t rnd = 0xADF48ECB;
    l = random(0xff);
    for (k= 0; k<l; k++) rnd = compute_lfsr32(rnd);
    seed2[0] = (rnd>>24)&255;
    seed2[1] = (rnd>>16)&255;
    seed2[2] = (rnd>>8)&255;
    seed2[3] = rnd&255;
}

// -----------------------------
// Commute a response on 32 bits
// -----------------------------
void compute_correct_key_32(void) {
    uint32_t lseed, lkey;
    uint32_t rnd = 0x2CD32781;
    lseed = (seed2[0]<<24) + (seed2[1]<<16) + (seed2[2]<<8) + seed2[3];
    lkey = ((lseed)+7) ^ rnd;
    key2[0] = (lkey>>24)&255;
    key2[1] = (lkey>>16)&255;
    key2[2] = (lkey>>8)&255;
    key2[3] = lkey&255;
}
