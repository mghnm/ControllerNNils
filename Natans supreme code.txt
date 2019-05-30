#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef F_CPU
/* prevent compiler error by supplying a default */
# warning "F_CPU not defined. Define F_CPU with 16 Mhz."
#define F_CPU 16000000UL    // Quarz with 16 Mhz  */
//#define F_CPU 12000000UL    // Quarz with 16 Mhz  */
#endif

#define UART_BAUDRATE0 9600
#define UART_BAUD_SELECT0 ((F_CPU / (UART_BAUDRATE0 * 16L))-1)

/*#define UART_BAUDRATE1 9600
#define UART_BAUD_SELECT1 ((F_CPU / (UART_BAUDRATE0 * 16L))-1) */

void initUART0();
void uart0_putc(unsigned char c);
void uart0_puts (char *s);
void uart0_puti16 (uint16_t value);
void uart0_puti16B (uint16_t value);

#endif /* UART_H_ */
