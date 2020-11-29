/*
* motor.c
* part of Sparmatic-Comet firmware by olel
*
*/

#include <avr/io.h>
#include "motor.h"
#include "const.h"
#include "lcd.h"
#include <util/delay.h>


// global variables
uint16_t motor_steps = 0;
uint16_t valve_max = 0;

void motor_init() {
    // Initialise motor

    // Set pin directions
    MOTOR_DDR |= (1 << MOTOR_A);     // MOTOR_A output
    MOTOR_DDR |= (1 << MOTOR_B);     // MOTOR_B output
    MOTOR_PULSE_DDR |= (1 << MOTOR_PULSE_LED); // MOTOR_PULSE_LED output
    MOTOR_PULSE_DDR &=~(1 << MOTOR_PULSE_INP); // MOTOR_PULSE_INP input

    // Turn everything on
    motor_after_sleep();

    // Turn off motor
    motor_stop();
}

void motor_after_sleep(){
    PIN_HIGH(PORTE, MOTOR_PULSE_LED);
}

void _motor_extract() {
    // Start extraction of motor
    MOTOR_PORT &= ~(1 << MOTOR_A);
    MOTOR_PORT |= (1 << MOTOR_B);
}

void _motor_retract() {
    // Start retraction of motor
    MOTOR_PORT |= (1 << MOTOR_A);
    MOTOR_PORT &= ~(1 << MOTOR_B);

}

void motor_stop() {
    // Stop motor
    MOTOR_PORT &= ~(1 << MOTOR_A);
    MOTOR_PORT &= ~(1 << MOTOR_B);
}

uint8_t _read_motor_pulse() {
    return MOTOR_PULSE_PIN & (1 << MOTOR_PULSE_INP);
}

void motor_extend_step() {
    uint8_t current_val = _read_motor_pulse();
    _motor_extract();
    while (_read_motor_pulse() == current_val); // Wait until motor pulse value changed
    motor_stop();
    motor_steps += 1;
}

void motor_retract_step() {
    uint8_t current_val = _read_motor_pulse();
    _motor_retract();
    while (_read_motor_pulse() == current_val); // Wait until motor pulse value changed
    motor_stop();
    motor_steps -= 1;
}

void motor_retract_max() {
    uint16_t cycles = 0;
    uint8_t running = true;
    uint8_t current_val;
    _motor_retract();
    while (running) {
        cycles = 0;
        current_val = _read_motor_pulse();
        while (_read_motor_pulse() == current_val){
            cycles += 1;
            if (cycles > 100){
                running = false;
                break;
            }
            _delay_ms(1);
        }
    }
    motor_stop();
    motor_steps = 0;
}
void motor_extract_max() {
    #ifdef NOT_MOUNTED
    while (motor_steps < DEBUG_VALVE_MAX) {
        motor_extend_step();
    }
    return;
    #endif
    uint16_t cycles = 0;
    uint8_t running = true;
    uint8_t current_val;
    _motor_extract();
    while (running) {
        motor_steps ++;
        cycles = 0;
        current_val = _read_motor_pulse();
        while (_read_motor_pulse() == current_val){
            cycles += 1;
            if (cycles > 100){
                running = false;
                break;
            }
            _delay_ms(1);
        }
    }
    motor_stop();
}

void motor_adap() {
    /*
    Detect maximum valve position
    */
    motor_retract_max();
    motor_extract_max();
    valve_max = motor_steps;
    motor_retract_max();
}

void set_valve_rel(uint8_t rel) {
    /*
    Sets valve to relative position:
    Run motor_adap() to calibrate before!!
    0x00 -> Valve closed
    0xff -> Valve opened
    */

    // Calc with float to make math easier
    float percent = 1.0 - (rel / (float)0xff);
    uint16_t absolute_pos = (uint16_t) (percent * valve_max);
    
    while (absolute_pos > motor_steps) {
        motor_extend_step();
    }
    while (absolute_pos < motor_steps) {
        motor_retract_step();
    }

}

uint16_t get_motor_steps() {
    return motor_steps;
}

uint16_t get_valve_max() {
    return valve_max;
}