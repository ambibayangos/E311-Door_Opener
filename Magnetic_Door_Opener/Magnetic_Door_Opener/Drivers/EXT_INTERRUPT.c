/*
 * EXT_INTERRUPT.c
 *
 * Created: 15/10/2020 2:17:34 PM
 *  Author: abay643
 */ 

#include <avr/io.h>
#include "EXT_INTERRUPT.h"

/*
 * This function initialise the external interupt module
 */ 
void INIT_EXTERNAL_ISR(void)
{	
	// Enable external isr on pin PD2
	EIMSK |= (1<<INT0);
	// trigger ISR on falling edge on PD2
	EICRA |= (1<<ISC01); //EICRA &= ~(1<<ISC00);
	
	// Enable external isr on PD3
	EIMSK |= (1<<INT1);
	
	// trigger ISR on rising edge on PD3
	EICRA |= (1<<ISC10) | (1<<ISC11);
}