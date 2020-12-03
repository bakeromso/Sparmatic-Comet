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

#define ROT_A_VAL !((1<<ROT_A) & ROT_PIN)
#define ROT_B_VAL !((1<<ROT_B) & ROT_PIN)

uint8_t rotarystatus = 0;

void enc_check_status(){
    if(ROT_A_VAL & (!ROT_B_VAL)){
        while(ROT_A_VAL); // ! (?)
        if (ROT_B_VAL)
            enc ++;
            //check if rotation is right
    }
    else if(ROT_B_VAL & (!ROT_A_VAL)) {
        while(ROT_B_VAL); // ! (?)
        if (ROT_A_VAL)
            enc --;
    }
    else if (ROT_A_VAL & ROT_B_VAL) {
        while(ROT_A_VAL); // ! (?)
        if (ROT_B_VAL) enc ++;
        else enc --;
    }
 } 

ISR(TIMER0_OVF_vect){
    enc_check_status();
}

void inputs_init() {
    // Set pin directions for rotary encoder
    ROT_DDR &= ~(1 << ROT_A);
    ROT_DDR &= ~(1 << ROT_B);
    BTN_DDR &= ~(1 << BTN_MENU);

    PORTB |= (1 << ROT_A) | (1 << ROT_B) | (1 << BTN_MENU); // Internal pullups

    TCCR0A |=  (1 << CS02) | (1<<CS00); //prescaller 256 ~122 interrupts/s
    TIMSK0 |= (1<<TOIE2);          //Enable Timer0 Overflow interrupts
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