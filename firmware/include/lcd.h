/*
 * lcd.h
 *
 *  Created on: 18.11.2011
 *      Author: matthias
 *
 *  Source: https://github.com/NerdyProjects/sparmatic-zero
 *  LICENSE: GPLv2!
 */

#ifndef LCD_H_
#define LCD_H_

typedef enum {LCD_AUTO = 1, LCD_MANU = 2, LCD_BAG = 4, LCD_TOWER = 8, LCD_INHOUSE = 16, LCD_OUTHOUSE = 32, LCD_MOON = 64, LCD_STAR = 128, LCD_BATTERY = 256, LCD_LOCK = 512, LCD_DP = 1024, LCD_HOURS = 2048} LCD_SYMBOLS;
#define LCD_SYM_ALL (2 * LCD_HOURS - 1)

void display_init(void);
void display_ascii_digit(char c, uint8_t pos);
void display_string(char *str);
void display_number(int16_t num, int8_t width);
void display_bargraph(uint32_t bargraphOn);
void display_weekday(uint8_t dayOn);
void display_symbols(LCD_SYMBOLS on, LCD_SYMBOLS mask);
void display_off(void);


#endif /* LCD_H_ */
