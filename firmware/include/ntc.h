/*
 * ntc.h
 *
 *  Created on: 19.11.2011
 *      Author: matthias
 */

#ifndef NTC_H_
#define NTC_H_

void ntc_init(void);
void update_ntc_temperature(void);

extern int16_t temperature;
extern int8_t ntc_offset;
#define get_ntc_temperature(x) ((const int16_t)temperature)

#endif /* NTC_H_ */