/* 
 * File: main.c
 */

#include "include/const.h" // Contains F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "include/const.h"
#include "include/motor.h"
#include "include/lcd.h"


int main (void) {
    // Init stuff
    motor_init();
    display_init();

    char* disp = "    ";
    uint16_t counter = 0;
    while (1){
        sprintf(disp, "%04d", counter);
        display_string(disp);
        _delay_ms(50);
        counter += 1;
    }

    return 0;
}
