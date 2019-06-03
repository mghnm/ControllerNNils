/*
 * RemotControler.c
 *
 * Created: 2019-05-30 12:12:57
 * Author : walda
 */ 


#define F_CPU 16000000UL
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include "lcd.h"
#include "i2c.h"
#include "font.h"

volatile char commandChar;

/******************************************************************
* Name: InitADC()                                                 *
* Aim: Initialize the ADC registers                               *
* Input: none                                                     *
* Output: void                                                    *
*******************************************************************/
void InitADC(void)
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}

uint16_t readadc(uint8_t ch)
{
	ch&=0b00000111;               // ANDing to limit input to 7
	ADMUX = (ADMUX & 0xf8)|ch;    // Clear last 3 bits of ADMUX, OR with ch
	
	ADCSRA|=(1<<ADSC);            // START CONVERSION
	while((ADCSRA)&(1<<ADSC));    // WAIT UNTIL CONVERSION IS COMPLETE
	return(ADC);                  // RETURN ADC VALUE
}


ISR(USART_RX_vect){
	commandChar = UDR0;
}

/*
void sendCommand(uint16_t x,uint16_t y)
{
	if ((x>=140)&&(x >=150)&&(y==255))
	{
	}

}
*/

void printCommand(char command){
	lcd_gotoxy(0,7);
	lcd_putc(command);
	lcd_puts(" ");
}



int main(void)
{   
	char message = 'Y';
	unsigned int xAxis;
	unsigned int yAxis;
	char xVal;
	char yVal;
	char xString[4];
	char yString[4];
	
	DDRD = 0xff;
	DDRC = 0x00;
	DDRB |= (1<<0);
	
	InitADC();
	//blutooth();
	initUART0(); 
	
	i2c_init();	        
	lcd_init(LCD_DISP_ON);  // INITIALIZE LCD
	lcd_clrscr();           // Clear LCD screen
	lcd_gotoxy(0,0);
	lcd_puts("Hello!");
	
	sei();
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
		
	/*	
		itoa(yAxis,yString,10);
		lcd_gotoxy(0,2);
		lcd_puts("y axis  ");
		lcd_puts(yString);
		
		lcd_puts("   ");
		
		itoa(xAxis,xString,10);
		lcd_gotoxy(0,0);
		
		lcd_puts("x axis  ");
		lcd_puts(xString);
		lcd_puts("     ");
		
	*/	
	
		if(commandChar == 'O'){
			lcd_gotoxy(0,0);
			lcd_puts("Obstacle Detected");
			lcd_puts("                 ");
		} else if(commandChar == 'H'){
				lcd_gotoxy(0,0);
				lcd_puts("Abused!");
				lcd_puts("                 ");
		} else if(commandChar == 'D'){
				lcd_gotoxy(0,0);
				lcd_puts("Driving...");
				lcd_puts("                 ");
		} else {
				lcd_gotoxy(0,0);
				lcd_puts("Unknown State");
				lcd_puts("                 ");
		}
		
		if(xAxis<=640 && xAxis >= 615){
			xVal = 'N'; 
		} else if(xAxis>640 && xAxis <= 840){
			xVal = 'l'; 
		}else if (xAxis>840){
			xVal = 'L'; 
		} else if(xAxis < 615 && xAxis >= 415){
			xVal = 'r';
		} else if(xAxis<415){
			xVal = 'R';
		} else{}
			
		if(yAxis<=680 && yAxis >= 670){
			yVal = 'N';
		} else if(yAxis>680 && yAxis <= 880){
			yVal = 'f';
		}else if (yAxis>880){
			yVal = 'F';
		} else if(yAxis < 670 && yAxis >= 470){
			yVal = 'b';
		} else if(yAxis<470){
			yVal = 'B';
		} else{}
			
		
		if(xVal == 'N' && yVal == 'N'){
			message = 'S';
		} else if (xVal != 'N'){
			message = xVal;
		} else{
			message = yVal;
		}
		
		char printC[1];
		memset(printC, '\0', sizeof(printC));
		printC[0] = message;
		//main_print(0, 0, printC);
		printCommand(message);
		uart0_putc(message);
		//uart0_puti16((int16_t) message);

//		lcd_display();
	}
}