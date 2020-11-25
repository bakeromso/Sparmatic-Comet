
void _motor_extend() {
    PORTE &= ~(1 << MOTOR_A);
    PORTE |= (1 << MOTOR_B);
}

void _motor_retract() {
    PORTE |= (1 << MOTOR_A);
    PORTE &= ~(1 << MOTOR_B);
}

void motor_stop() {
    PORTE &= ~(1 << MOTOR_A);
    PORTE &= ~(1 << MOTOR_B);
}

uint8_t read_motor_pulse() {
    return PINE & (1 << MOTOR_PULSE_INP);
}

void motor_extend_step() {
    uint8_t current_val = read_motor_pulse();
    _motor_extend();
    while (read_motor_pulse() == current_val); // Wait until motor pulse value changed
    motor_stop();
    motor_steps += 1;
}

void motor_retract_step() {
    uint8_t current_val = read_motor_pulse();
    _motor_retract();
    while (read_motor_pulse() == current_val); // Wait until motor pulse value changed
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
        current_val = read_motor_pulse();
        while (read_motor_pulse() == current_val){
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
/*
void motor_extend_max() {
    uint16_t cycles = 0;
    uint8_t running = true;
    uint8_t current_val;
    motor_extend();
    while (running) {
        cycles = 0;
        current_val = read_motor_pulse();
        while (read_motor_pulse() == current_val){
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
*/
