/* 
 * File: main.c
 */

#define F_CPU 2000000UL // 2 MHz

#include <avr/io.h>
#include <util/delay.h>

#define ROT_A PB7
#define ROT_B PB0
#define BTN_MENU PB4
#define BTN_TIME PB5
#define BTN_OKAY PB6

#define MOTOR_A PE7
#define MOTOR_B PE6

#define ENDSTOP_LED PE2
#define ENDSTOP_INP PINE3

#define LED     PB2 // Must be removed when smth should communicate via SPI

#define PIN_HIGH(PORT, PIN) PORT |= (1 << PIN)
#define PIN_LOW(PORT, PIN) PORT &= ~(1 << PIN)

//PORTB |= (1 << PB3);     // set pin 3 of Port B high
//PORTB &= ~(1 << PB3);    // set pin 3 of Port B low

void motor_extend() {
    PORTE &= ~(1 << MOTOR_A);
    PORTE |= (1 << MOTOR_B);
}

void motor_retract() {
    PORTE |= (1 << MOTOR_A);
    PORTE &= ~(1 << MOTOR_B);
}

void motor_stop() {
    PORTE &= ~(1 << MOTOR_A);
    PORTE &= ~(1 << MOTOR_B);
}

uint8_t read_endstop() {
    return PINE & (1 << ENDSTOP_INP);
}

void motor_extend_step() {
    uint8_t current_val = read_endstop();
}

void motor_retract_step() {

}

void motor_zero() {
    // retracts motor to zero level
    uint8_t endstop_value = 0;
    endstop_value = read_endstop();
    motor_retract();
    _delay_ms(100);
    while (1){
        
    }
}


int main (void) {

    DDRE |= (1 << MOTOR_A);     // MOTOR_A output
    DDRE |= (1 << MOTOR_B);     // MOTOR_B output
    DDRE |= (1 << ENDSTOP_LED); // ENDSTOP_LED output
    DDRE &=~(1 << ENDSTOP_INP); // ENDSTOP_INP input

    DDRB |= (1 << LED);     // output

    // Turn off motor
    motor_stop();
    motor_retract();
    _delay_ms(5000);
    motor_stop();

    // Turn on endstop led TODO: While powersaving, turn it off :)
    PIN_HIGH(PORTE, ENDSTOP_LED);

    uint8_t endstop_value = 0;
    while (1){
        
    }

    return 0;
}
