#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init();
void motor_after_sleep();

void motor_stop();

void motor_extract_step();
void motor_retract_step();

void motor_retract_max();

#endif /* MOTOR_H_ */