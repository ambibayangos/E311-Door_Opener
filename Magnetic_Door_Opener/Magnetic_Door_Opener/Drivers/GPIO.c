/*
 * GPIO.c
 *
 * Created: 14/10/2020 1:19:19 AM
 *  Author: ambib
 */ 

#include <avr/io.h>
#include "GPIO.h"

/*
 * This function initializes all pins to Vcc
 */ 
void GPIO_init(void)
{
	// Set all pins to output
	DDRD = 0xFF; DDRC = 0xFF; DDRB = 0xFF;
	
	// Set all pins to High
	PORTD = 0xFF; PORTC = 0xFF; PORTB = 0xFF;
}