/*
 * UART.c
 *
 * Created: 14/10/2020 1:30:55 AM
 *  Author: ambib
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "UART.h"

/*
 * This function initializes the UART
 * 
 */
void UART_init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}


/*
 * This function transmit a character serially via UART
 * 
 */
void UART_transmit_char(unsigned char data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}


/*
 * This function transmit a string serially via UART
 * 
 */
void UART_transmit_string(char *str)
{
	int i;
	for(i = 0; i < strlen(str); i++) {
		UART_transmit_char(str[i]);
	}
}