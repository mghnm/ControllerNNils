
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/common.h>
#include <stdbool.h>
#include <string.h>
#include "uart.h"
#include "lcd.h"
#include "i2c.h"
#include "font.h"

int up;
int right;

void main_print(int right, int up, char msg[])
{
	right = -right;
	uint8_t x = 10 + right;
	uint8_t y = 3 + up;
	
	lcd_gotoxy(x,y);  //line x column y

	lcd_puts(msg);
	lcd_puts(" ");
}

int main(void)
{
	lcd_init(0xAF); 
	initUART0();
	
	char message = 'Y';
	unsigned int xAxis;
	unsigned int yAxis;
	char xString[4];
	char yString[4];
	
	// Set Port A as Output
	DDRD = 0b01111111;
	//Print_pos(0,0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
    /* Replace with your application code */
    while (1) 
    {
		//Toggle the LSB in the ADMUX register to switch betweeen ADC0 and ADC1
		ADMUX ^= (1 << 0);
		
		ADCSRA |= (1<<ADSC); // In Free Running mode no external trigger, write this bit to 1 to start conversion
		
		// wait for conversion to complete...then ADSC becomes ’0? again...till then, run loop continuously
		while(ADCSRA & (1<<ADSC));
		
		xAxis = ADC; //Store ADC value
		
	
		ADMUX ^= (1 << 0);
		ADCSRA |= (1<<ADSC); // In Free Running mode, write this bit to 1 to start conversion
		
		// wait for conversion to complete...then ADSC becomes ’0? again...till then, run loop continuously
		while(ADCSRA & (1<<ADSC));
		
		yAxis = ADC; //Store ADC value
		
		up = (xAxis/146)-3;
		right = (yAxis/50)-10;

		itoa(right,yString,10);
		lcd_gotoxy(0,2);
		lcd_puts("y axis  ");
		lcd_puts(yString);
		
		lcd_puts("   ");
		
		itoa(up,xString,10);
		lcd_gotoxy(0,0);
		
		lcd_puts("x axis  ");
		lcd_puts(xString);
		lcd_puts("     ");
		
		if (right <= 1 && right >= -1)
		{
			if (up > 3)
			{
				message = 'F';
				//strcpy(message, "F");
				
			}
			else if (up > 1)
			{
				message = 'f';
				//strcpy(message, "f");
			}
			else if (up < -3)
			{
				message = 'B';
				//strcpy(message, "B");
			}
			else if (up < -1)
			{
				message = 'b';
				//strcpy(message, "b");
			}
			else
			{
				message = 'S';
				//strcpy(message, "S");
			}
		}
		else if (up > 0)
		{
			if (right > 5)
			{
				message = 'R';
				//strcpy(message, "F");
			}
			else if (right > 1)
			{
				message = 'r';
				//strcpy(message, "r");
			}
			else if (right < -5)
			{
				message = 'L';
				//strcpy(message, "L");
			}
			else if (right < -1)
			{
				message = 'l';
				//strcpy(message, "l");
			}
		}
		else
		{
			message = 'S';
			//strcpy(message, "S");
		}
		
		char printC[1];
		memset(printC, '\0', sizeof(printC));
		printC[0] = message;
		main_print(0, 0, printC);
		uart0_putc(message);
		//uart0_puti16((int16_t) message);

		lcd_display();
    }
}
