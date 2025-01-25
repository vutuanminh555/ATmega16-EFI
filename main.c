#define F_CPU 8000000UL
#include "main.h"
#include "stdio.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdbool.h>


volatile unsigned int encoderE = 0; // so vong quay cua dong co trong 10ms
volatile unsigned int encoderP = 0; // so vong quay cua may bom trong 10ms
volatile int encoder = 0;
unsigned int PWM1 = 0;
unsigned int PWM2 = 0;
volatile unsigned long CountT2 = 0, CountT0 = 0; 
unsigned int Temp1 = 5000; // Toc do tang/giam Duty Cycle cua dong co, tuong ung 8.3%
char lcd[16]; // luu tru ky tu de ghi vao LCD

// gia tri khoi diem
float V_old = 10.0;
float V_new = 10.0; 

volatile unsigned int Rpm = 0; // toc do quay cua dong co tren giay
volatile unsigned int Prpm = 0; // toc do quay cua may bom tren phut 
float Gpm = 0; // the tich xang duoc tieu thu tren phut
float Pump = 0.0; // the tich xang duoc bom tren phut
float P = 4.0; // ap suat trong ong

void fuelPump(unsigned int step){ // dieu chinh toc do may bom de cung cap xang cho dong co 
	float rightValue = Rpm / 5.0; 
		if(rightValue == 0)
			PWM2 = 0;
		else if(Prpm < rightValue - 5 ){
			PWM2 += step;
		}
		else if(Prpm > rightValue + 5 ){
			PWM2 -= step;
		}
		OCR1AH = PWM2 >> 8 ;
		OCR1AL = PWM2 & 0x00FF ;
}

void pressure_balance() // tinh ap suat theo the tich xang duoc them vao va tieu thu trong 1s
{
	V_old = V_new;

	Gpm = (float) Rpm / (100.0 * 60.0);
	Pump = (float) Prpm / (20.0 * 60.0);
	V_new += Pump;  // cong them the tich xang duoc bom vao
	V_new -= Gpm;  // tru di the tich xang duoc tieu thu
	P =  4.0 * V_new  / V_old;   

}

void display(){ 

	sprintf(lcd,"%4.0d ", Rpm);
	LCD_String_xy(1, 0, lcd);
	
	sprintf(lcd,"%3.0d ",Prpm);
	LCD_String_xy(1, 6, lcd);
	
	sprintf(lcd,"%1.1f ", P);
	LCD_String_xy(1, 13, lcd); 

}

int main(void)
{
	// Port LCD
	DDRA  |= (1<<PA7) | (1<<PA6) | (1<<PA5) | (1<<PA4) | (1<<PA3) | (1<<PA2) | (0<<PA1) | (0<<PA0);
	PORTA |= (0<<PA7) | (0<<PA6) | (0<<PA5) | (0<<PA4) | (0<<PA3) | (0<<PA2) | (0<<PA1) | (0<<PA0);

	// Port Button
	DDRB = 0x00; 
	PORTB = 0x00; 

	// Port Motor DC
	DDRD |= (0<<PD7) | (0<<PD6) | (1<<PD5) | (1<<PD4) | (0<<PD3) | (0<<PD2) | (1<<PD1) | (0<<PD0); 
	PORTD |= (1<<PD1); 

	// Khai báo timer 0 mode normal chia 64 đếm 1ms
	TCCR0 = (0<<CS02) | (1<<CS01) | (1<<CS00); // Prescaler = 1/64 
	TCNT0 = 130; // gia tri bat dau dem cua timer0; tcnt = 255 - deltaT * fMCU * prescaler, X phai nho hon 255 va phai la so nguyen 

	// Khai báo timer2 mode normal chia 64 đếm 1ms
	TCCR2 = (1<<CS22) | (0<<CS21) | (0<<CS20); // Prescaler = 1/64
	TCNT2 = 130; // gia tri bat dau dem cua timer2

	// Khai báo fast PWM của timer 1 có chu kỳ mới sử dụng ICR1, chu kỳ 2ms
	TCCR1A |= (1<<COM1A1) | (1<<WGM11) | (0<<WGM10) | (1<<COM1B1); // Clear OC1A/B on compare match, set both at BOTTOM (non-invert mode)
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10); // Prescaler = 1, Fast PWM, top = ICR1, update OCR1X = BOTTOM, TOV1 flag set on TOP 
	TCNT1H = 0x00; // bat dau dem tu 0
	TCNT1L = 0x00;
	ICR1H = 0xEA; // dem den 60000, dung de cai dat chu ki PWM
	ICR1L = 0x60;
	OCR1AH = 0x00;  // thanh ghi so sanh bat dau tu gia tri 0 
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;

	TIMSK = (1 << TOIE2) | (1 << TOIE0); // enable overflow interrupt timer0 va timer 2
	MCUCR = (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00); // ngat ngoai int0, int1 kich hoat o suon len 

	sei();

	GICR &= ~(1 << INT1) & ~(1 << INT0); // tat int0 va int1 luc dau	

	while (1)
	{	
		_delay_ms(100);
		fuelPump(1000);
		pressure_balance();
		display(); 
	}

	return 0;
}


ISR(INT0_vect) // ngat ngoai int0;  
{
		encoderE++; // Pulse per revolution = 200
}

ISR(INT1_vect)
{
		encoderP++; // Pulse per revolution = 200
}

ISR (TIMER0_OVF_vect ){ // timer0 overflow sau 1ms
		if(CountT0 >= 100) // sau 100ms
		{
			if(encoder == 0){
				GICR &= ~(1 << INT0); // tat int0 de lay du lieu
				Rpm = encoderE * 3;
				encoderE = 0;
				GICR |= (1 << INT1); // bat int1 de tiep tuc dem
			}
			if(encoder == 1){
				GICR &= ~(1 << INT1); // tat int1 de lay du lieu
				Prpm = encoderP * 3;
				encoderP = 0;
				GICR |= (1 << INT0); // bat int0 de tiep tuc dem
			}
			encoder ^= 1;
			CountT0 = 0;
		}
		CountT0++;
		TCNT0 = 130;
}

ISR (TIMER2_OVF_vect) 
{
		if(CountT2 >= 101) // sau 101ms
		{
			CountT2=0;
			
			if((PINB & (1 << button_up)) == 0) // button_up duoc nhan 
			{
				Speed_up(&PWM1, Temp1);
			}
			
			if((PINB & (1 << button_down)) == 0) // button_down duoc nhan 
			{
				Speed_down(&PWM1, Temp1);
			}
			
			if((PINB & (1 << button_start)) == 0) // On/Off duoc nhan 
			{
				On_Off(&PWM1, Temp1);
			}
			_delay_ms(1);

			OCR1BH = PWM1 >>8 ;	
			OCR1BL = PWM1 & 0x00FF ; 	
			
		}
		CountT2++;
		TCNT2 = 130;	
}
