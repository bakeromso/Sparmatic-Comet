/*
 * adc.h
 *
 *  Created on: 06.01.2012
 *      Author: matthias
 *
 *  Source: https://github.com/NerdyProjects/sparmatic-zero
 *  LICENSE: GPLv2!
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

static inline uint16_t get_adc(uint8_t channel)
{

	ADMUX = (1 << REFS0) | (channel & 0x1F);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2);
	while (ADCSRA & (1 << ADSC))
		/* Conversion time will be about 400 µs at first conversion, 200 µs any other */
		;
	return ADC;
}

extern uint16_t batter_mv;

#define getBatteryVoltage() (battery_mv)

#endif /* ADC_H_ */