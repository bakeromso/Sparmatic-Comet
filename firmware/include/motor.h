/*
* motor.h
* part of Sparmatic-Comet firmware by olel
*
*/

#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init();
void motor_after_sleep();

void motor_stop();

void motor_extract_step();
void motor_retract_step();

void motor_retract_max();
void motor_extract_max();

void motor_adap();
void set_valve_rel(uint8_t rel);

uint16_t get_motor_steps();
uint16_t get_valve_max();

void write_valve_max();
void read_valve_max();
void write_motor_steps();
void read_motor_steps();

#endif /* MOTOR_H_ */