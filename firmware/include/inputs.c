/*
* inputs.c
* part of Sparmatic-Comet firmware by olel
*
*/
#include "const.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "inputs.h"

volatile int16_t enc = 0;
int16_t prev_enc = 0;

ISR(PCINT1_vect){
    bool rot_a_val = (ROT_PIN & (1 << ROT_A)) > 0;
    bool rot_b_val = (ROT_PIN & (1 << ROT_B)) > 0;
    if (rot_a_val & !rot_b_val){
        enc --;
    }
    if (rot_a_val & rot_b_val){
        enc ++;
    }
}

void inputs_init() {
    // Set pin directions for rotary encoder
    ROT_DDR &= ~(1 << ROT_A);
    ROT_DDR &= ~(1 << ROT_B);
    BTN_DDR &= ~(1 << BTN_MENU);

    PORTB |= (1 << ROT_A) | (1 << ROT_B) | (1 << BTN_MENU); // Internal pullups
    
    EIMSK |= (1 << 5); // External Interrupt Mask (Enable PCIE1)
    PCMSK1 |= (1 << PCINT8); // Pin Change Interrupt 1 Mask (Enable PCINT8 & 15 (ROT_A & ROT_B))
    sei();
}


int16_t get_enc() {
    cli();
    int16_t tmp_enc = enc;
    sei();
    return tmp_enc;
}

int8_t get_rel_enc() {
    cli();
    int16_t tmp_enc = enc;
    int8_t rel = (tmp_enc - prev_enc);
    if (rel < 0) rel = -1;
    if (rel > 0) rel = 1;
    prev_enc = enc;
    sei();
    return rel;
}