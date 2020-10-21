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
	
	DDRC &= ~(1<<DDC0);  // set PCO input pin
	PORTC &= ~(1<<DDC0); // 
	PORTC &= ~(1<<DDC1); //
	
	DDRC &= ~(1<<DDC0); // set PCO as input pin to sample voltage across coil
	DDRC &= ~(1<<DDC1); // set PC1 as input pin to sample voltage across coil
	DDRC &= ~(1<<DDC2); // set PC2 as input pin to sample voltage at touch sensor
	
	
	DDRD &= ~(1<<DDD2); // set PD2 as input pin to detect falling edge on fast pwm
	DDRD &= ~(1<<DDD3); // set PD3 as input pin to detect rising edge on coil pwm
}