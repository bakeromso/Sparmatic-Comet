/*
* const.h
* part of Sparmatic-Comet firmware by olel
*
*/

// Stuffs
#define false 0
#define true  1

// INPUTS
#define ROT_A           PB7
#define ROT_B           PB0

#define BTN_MENU        PB4
#define BTN_TIME        PB5
#define BTN_OKAY        PB6

#define MOTOR_PULSE_INP PINE3
#define NTC_READ        PF1

// OUTPUT
#define MOTOR_A         PE7
#define MOTOR_B         PE6
#define MOTOR_PULSE_LED PE2
#define NTC_ACTIVATE    PF3

// DDR's
#define MOTOR_DDR       DDRE
#define MOTOR_PULSE_DDR DDRE
#define NTC_DDR         DDRF
#define BTN_DDR         DDRB
#define ROT_DDR         DDRB

// Ports/Pins
#define MOTOR_PORT      PORTE
#define MOTOR_PULSE_PIN PINE
#define ROT_PIN         PINB
#define BTN_PIN         PINB

// MACROS
#define PIN_HIGH(PORT, PIN) PORT |= (1 << PIN)
#define PIN_LOW(PORT, PIN) PORT &= ~(1 << PIN)

// HARDWARE VALUES
#define MAX_STEPS  850
#define F_CPU 1000000UL 

// DEBUG VALUES
#define NOT_MOUNTED // Define this if the devices is not mounted to heater
#define DEBUG_VALVE_MAX  638 // Some random number between 600 -> 700