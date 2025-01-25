#include "lcd.h"

void LCD_Command( unsigned char cmnd ) // chuyen du lieu toi lcd 4 bit mode 
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); // Ket hop 4 bit cuoi cua lcd_port voi 4 bit dau cua cmnd
	cbi(LCD_Port, RS); 	
	sbi(LCD_Port, EN); 		
	_delay_us(1); 
	cbi(LCD_Port, EN);

	_delay_us(100);  

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  // Ket hop 4 bit cuoi cua lcd_port voi 4 bit dau cuoi cua cmnd, do cmnd bi dich trai 4 bit 
	sbi(LCD_Port, EN); 
	_delay_us(1); 
	cbi(LCD_Port, EN);  
	_delay_ms(2); 
}


void LCD_Char( unsigned char data ) // command: RS = 0; data: RS = 1 
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); // Ket hop 4 bit cuoi port_lcd va 4 bit dau data  
	sbi(LCD_Port, RS);  	
	sbi(LCD_Port, EN);  
	_delay_us(1); 
	cbi(LCD_Port, EN);  

	_delay_us(100);  

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); //Ket hop 4 bit cuoi port_lcd va 4 bit cuoi data ( da duoc dich trai) 
	sbi(LCD_Port, EN);  
	_delay_us(1); 
	cbi(LCD_Port, EN); 
	_delay_ms(2);  
}

void LCD_String (char *str) // hien thi string, su dung voi char() vi la data		
{
	int i;
	for(i=0;str[i]!=0;i++)		
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	
{
	if (row == 0 && pos<16)
	LCD_Command( (pos & 0x0F) | 0x80);	// 80: con tro dau dong 1
	if (row == 1 && pos<16)
	LCD_Command( (pos & 0x0F) | 0xC0);	// C0: con tro dau dong 2 
	LCD_String(str);		
}

void LCD_OnOff(int state){
	LCD_Command(0x02);
	_delay_ms(2);
	if(state == 1){
		LCD_Command(0x0C); // Display on, cursor off 
		LCD_Command(0x28); // che do 4-bit voi 2 dong va font 5x7
		_delay_ms(2);
		LCD_String_xy(0, 0, "Erpm |");
		LCD_String_xy(0, 6, "Prpm  |");
		LCD_String_xy(0, 13, "P ");
	}
	if(state == 0){
		LCD_Command(0x01); // Clear screen
		LCD_Command(0x08); // Display off, cursor off
	}
}

