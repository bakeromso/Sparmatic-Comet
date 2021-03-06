/*
* motor.c
* part of Sparmatic-Comet firmware by olel
*
*/

#include "const.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "motor.h"
#include "lcd.h"


// global variables
volatile uint16_t motor_steps = 0;
uint16_t valve_max = 0;

// local variables
#define RETRACT 0
#define EXTRACT 1
uint8_t motor_direction = RETRACT;


void motor_init() {
    // Initialise motor

    // Set pin directions
    MOTOR_DDR |= (1 << MOTOR_A);     // MOTOR_A output
    MOTOR_DDR |= (1 << MOTOR_B);     // MOTOR_B output
    MOTOR_PULSE_DDR |= (1 << MOTOR_PULSE_LED); // MOTOR_PULSE_LED output
    MOTOR_PULSE_DDR &=~(1 << MOTOR_PULSE_INP); // MOTOR_PULSE_INP input

    // Turn everything on
    motor_after_sleep();

    // Enable interrupts
    EIMSK |= (1 << 4); // External Interrupt Mask (Enable PCIE0)
    PCMSK0 |= (1 << PCINT3); // Pin Change Interrupt 0 Mask (Enable PCINT3, Motor Encoder)
    sei();

    // Turn off motor
    motor_stop();
}

ISR(PCINT0_vect) {
    if (motor_direction == RETRACT) {
        if (motor_steps > 0) {
            motor_steps --;
        }
    }
    else{
        motor_steps ++;
    }
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

void _motor_move() {
    if (motor_direction == RETRACT) {
        _motor_retract();
    }
    else{
        _motor_extract();
    }
}

void motor_stop() {
    // Stop motor
    MOTOR_PORT &= ~(1 << MOTOR_A);
    MOTOR_PORT &= ~(1 << MOTOR_B);
}

uint8_t _read_motor_pulse() {
    return MOTOR_PULSE_PIN & (1 << MOTOR_PULSE_INP);
}

void motor_retract_max() {
    uint16_t cycles = 0;
    uint8_t running = true;
    uint16_t current_val;
    motor_direction = RETRACT;
    _motor_move();
    while (running) {
        cycles = 0;
        current_val = motor_steps;
        while (motor_steps == current_val){
            cycles += 1;
            if (cycles > STUCK_COUNT){
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
    motor_direction = EXTRACT;
    _motor_move();
    while (motor_steps < DEBUG_VALVE_MAX) {
        ;
    }
    motor_stop();
    return;
    #endif
    uint16_t cycles = 0;
    uint8_t running = true;
    uint16_t current_val;
    motor_direction = EXTRACT;
    _motor_move();
    while (running) {
        motor_steps ++;
        cycles = 0;
        current_val = motor_steps;
        while (motor_steps == current_val){
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
    write_valve_max();
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
    
    cli();
    if (absolute_pos == motor_steps){
        sei();
        return; // Cancel if nothing changed
    }
    sei();

    uint8_t dest_reached = 0;
    
    motor_direction = (absolute_pos > motor_steps) ? EXTRACT : RETRACT;
    _motor_move();

    if(absolute_pos < motor_steps) while (absolute_pos < motor_steps);
    if(absolute_pos > motor_steps) while (absolute_pos > motor_steps);

    motor_stop();
    write_motor_steps();
}

uint16_t get_motor_steps() {
    cli();
    uint16_t ret = motor_steps;
    sei();
    return ret;
}

uint16_t get_valve_max() {
    return valve_max;
}

void write_valve_max() {
    // Writes valve max position to eeprom
    uint8_t lo = valve_max & 0xff;
    uint8_t hi = valve_max >> 8;
    eeprom_write_byte((uint8_t *)EEP_VALVE_MAX_LOB, lo);
    eeprom_write_byte((uint8_t *)EEP_VALVE_MAX_HIB, hi);
}
void read_valve_max() {
    // Writes valve max position to eeprom
    uint8_t lo = eeprom_read_byte((uint8_t *)EEP_VALVE_MAX_LOB);
    uint8_t hi = eeprom_read_byte((uint8_t *)EEP_VALVE_MAX_HIB);
    
    valve_max = (hi << 8) | lo;
    
}
void write_motor_steps() {
    // Writes current motor steps to eeprom
    uint8_t lo = motor_steps & 0xff;
    uint8_t hi = motor_steps >> 8;
    eeprom_write_byte((uint8_t *)EEP_MOTOR_STEPS_LOB, lo);
    eeprom_write_byte((uint8_t *)EEP_MOTOR_STEPS_HIB, hi);
}
void read_motor_steps() {
    // Reads current motor steps from eeprom
    // Call this after wakeup/reset !
    uint8_t lo = eeprom_read_byte((uint8_t *)EEP_MOTOR_STEPS_LOB);
    uint8_t hi = eeprom_read_byte((uint8_t *)EEP_MOTOR_STEPS_HIB);
    
    motor_steps = (hi << 8) | lo;
    
}