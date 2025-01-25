#ifndef LCD_H_

#define LCD_H_
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "stdio.h"
#define LCD_Dir DDRA 
#define LCD_Port PORTA 
#define RS PA2 
#define EN PA3
#define tbi(port,bit) port ^= (1 << bit);	// toggle
#define cbi(port,bit) port &= ~(1 << bit);  // clear
#define sbi(port,bit) port |= (1 << bit);	// set 

void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_OnOff();
void LCD_Command(unsigned char cmnd);

#endif 