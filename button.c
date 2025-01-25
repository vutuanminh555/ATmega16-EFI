#include "button.h"
#include "lcd.h"

int state = 0 ;

void Speed_up(unsigned int *PWM1 ,unsigned int Temp1)
{
	_delay_ms(5);
	if(state == 1){
	if((PINB & (1 << button_up)) == 0) 
	{
		while((PINB & (1 << button_up)) == 0); // de-bounce 
		*PWM1 += Temp1;
		if(*PWM1>=60000)
		*PWM1 = 60000;
	}
	}
}

void Speed_down(unsigned int *PWM1, unsigned int Temp1)
{
		_delay_ms(5);
		if(state == 1){
		if((PINB & (1 << button_down)) == 0)
		{
			while((PINB & (1 << button_down)) == 0); // de-bounce
			if(*PWM1 > 0)
			*PWM1 -= Temp1;
			if(*PWM1 <= 0) 
			*PWM1 = 0;
		}
		}
}

void On_Off(unsigned int *PWM1 ,unsigned int Temp1) 
{
	_delay_ms(5);
	if((PINB & (1 << button_start)) == 0)
	{
		while((PINB & (1 << button_start)) == 0); // de-bounce
		state ^= 1; 
		LCD_OnOff(state);
		if(state == 1){
			*PWM1 = Temp1;
		}
		if(state == 0)
		{
			*PWM1 = 0;	
		}
	}
}
