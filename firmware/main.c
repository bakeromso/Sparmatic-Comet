/* 
 * File: main.c
 */

#define NOT_MOUNTED // Define this if the devices is not mounted to heater

#include "include/const.h" // Contains F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "include/const.h"
#include "include/motor.h"
#include "include/lcd.h"
#include "include/inputs.h"


int main (void) {
    // Init stuff
    motor_init();
    motor_stop();
    display_init();
    inputs_init();

    display_string("ADAP");
    motor_adap();

    char* disp = "    ";
    sprintf(disp, "%04d", get_valve_max());
    display_string(disp);

    set_valve_rel(120);
    sprintf(disp, "%04d", get_motor_steps());
    display_string(disp);

    while (1){
        _delay_ms(100);
    }

    return 0;
}
