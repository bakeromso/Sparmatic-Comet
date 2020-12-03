/*
 * ntc.c
 *
 *  Created on: 19.11.2011
 *      Author: matthias
 *
 *  Source: https://github.com/NerdyProjects/sparmatic-zero
 *  LICENSE: GPLv2!
 */

#include "const.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "adc.h"


#define ADC_CH_NTC (1)

/* all resistances in 10 Ohms */

#define VOLTAGE_DIVIDER_RES 1200000000UL
#define NTC_START_DEGREE 0
#define NTC_DEGREE_STEPS 5

static const uint16_t ntc_res_table[] PROGMEM = {
	34090		, //  0°C
	26310		, //  5°C
20440		, // 10°C
	16000		, // 15°C
	12610		, // 20°C
	10000		, // 25°C
	 7981		, // 30°C
	 6408		, // 35°C
	 5174		, // 40°C
	 4202		, // 45°C
	 3431		, // 50°C
	 2816		, // 55°C
 2322		, // 60°C
	 1925		, // 65°C
	 1603		, // 70°C
	 1340		, // 75°C
	 1126		, // 80°C
	  949		, // 85°C
	  804		, // 90°C
	  684		, // 95°C
	  	0
};

int16_t temperature;
int16_t ntc_offset = 0;


void ntc_init(void)
{
	NTC_DDR |= (1 << NTC_ACTIVATE);
}

static uint16_t get_ntc_adc(void)
{
	uint16_t ntc;
	NTC_PORT |= (1 << NTC_ACTIVATE);
	ntc = get_adc(ADC_CH_NTC);
	NTC_PORT &= ~(1 << NTC_ACTIVATE);
	return ntc;
}


/** returns temperature * 100 */
void update_ntc_temperature(void)
{
	uint16_t ntc_voltage = get_ntc_adc();
	uint16_t ntc_res = VOLTAGE_DIVIDER_RES / (102300000UL / ntc_voltage - 100000);
	uint16_t ntc_res_tbl;
	uint8_t i = 0;
	int16_t _temperature;
	while((ntc_res_tbl = pgm_read_word(&ntc_res_table[i])) > ntc_res)
		++i;

	_temperature = NTC_START_DEGREE + i * NTC_DEGREE_STEPS * 100UL -
			(((ntc_res - ntc_res_tbl) * NTC_DEGREE_STEPS * 100UL) /
					(pgm_read_word(&ntc_res_table[i-1]) - ntc_res_tbl));

	temperature = _temperature - ntc_offset;
}