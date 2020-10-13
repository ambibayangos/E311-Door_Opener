/*
 * COUNTER.c
 *
 * Created: 14/10/2020 12:36:04 AM
 *  Author: ambib
 */ 

#include <avr/io.h>
#include "COUNTERS.h"

/*
 * This function initializes the 16 bit timer in Atmega328P
 *
 */ 
void COUNTER_16bit_init(void)
{
	// Set to CTC mode
	TCCR1B |= (1<<WGM12);
	
	//Enable ISR when count matches OCR1 value
	TIMSK1 |= (1<<OCIE1A);
	
	// Initial Value to count to
	OCR1A = 20;
	
	// Set prescaller to 1024 and start counting
	TCCR1B |= (1<<CS12) | (1<<CS10);
	 
}
