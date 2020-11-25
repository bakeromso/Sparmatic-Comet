/* 
 * File: main.c
 */

#define F_CPU 2000000UL // 2 MHz

#include <avr/io.h>
#include <util/delay.h>

// Gloabal variables
uint16_t motor_steps = 0;

#include "defines.h"
#include "motor.h"

int main (void) {
   
    DDRE |= (1 << MOTOR_A);     // MOTOR_A output
    DDRE |= (1 << MOTOR_B);     // MOTOR_B output
    DDRE |= (1 << MOTOR_PULSE_LED); // MOTOR_PULSE_LED output
    DDRE &=~(1 << MOTOR_PULSE_INP); // MOTOR_PULSE_INP input

    // Turn off motor
    motor_stop();

    // Turn on MOTOR_PULSE led TODO: While powersaving, turn it off :)
    PIN_HIGH(PORTE, MOTOR_PULSE_LED);
    motor_retract_max();

    int i = 0;
    while (i < 850){
        motor_extend_step();
        i++;
    }

    while (1){
        _delay_ms(10);
    }

    return 0;
}
