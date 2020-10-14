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
	OCR1A = 1;
	
	// Set prescaller to 1024 and start counting
	//TCCR1B |= (1<<CS12) | (1<<CS10); TCCR1B &= ~(1<<CS11);
		 
}

/*
 * This function starts the 16 bit counter with a prescale of 1024
 */ 

void START_16bit_COUNTER(void)
{		
		// Resets counter value to zero
		TCNT1 = 0;
		// Set prescaller to 1024 and start counting
		TCCR1B |= (1<<CS12) | (1<<CS10); TCCR1B &= ~(1<<CS11);
}


/*
 * This function stops the 16 bit counter 
 */ 
void STOP_16bit_COUNTER(void)
{
		// Disconnect clk from timer (Stop the timer)
		TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}


/*
 * This function initializes the 8 bit timer (Timer0/Counter0)
 */ 
void COUNTER_8bit_timer0_init(void)
{
	// Set to CTC mode
	TCCR0A |= (1<<WGM01); TCCR0A &= ~(1<<WGM00);  TCCR0B &= (1<<WGM02);

	
	//Enable ISR when count matches OCR1 value
	TIMSK0 |= (1<<OCIE0A);
	
	// Initial Value to count to
	OCR0A = 10;
	
	
}

/*
 * This function starts the 16 bit counter with a prescale of 1024
 */ 

void START_8bit_COUNTER0(void)
{		
		// Resets counter value to zero
		TCNT0 = 0;
		// Set prescaller to 1024 and start counting
		TCCR0B |= (1<<CS12); TCCR0B &= ~((1<<CS10) |(1<<CS11));
}

/*
 * This function stops the 16 bit counter 
 */ 
void STOP_8bit_COUNTER0(void)
{
		// Disconnect clk from timer (Stop the timer)
		TCCR0B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}

/*
 * This function initializes the 8 bit timer (Timer2/Counter2)
 */ 
void COUNTER_8bit_timer2_init(void)
{
	// Set to FAST-PWM mode where OCR1A is TOP value
	TCCR1B |= (1<<WGM13) | (1<<WGM12); TCCR1A |= (1<<WGM11) | (1<<WGM10);
	
	//Set PWM frequency to 1ms
	OCR1A = 11; 
	
	//Create a rising edge on compare match
	TCCR1A |= (1<<COM1A1); TCCR1A &= ~(1<<COM1A0);
}

/*
 * This function starts the 8 bit counter2 with a prescale of 256
 */ 
void START_8bit_COUNTER2(void)
{	
	// reset timer value
	//TCNT1 = 0;
	// Set prescaller to 256 and start the FAST PWM
	TCCR1B |= (1<<CS12) | (1<<CS10); TCCR1B &= ~(1<<CS11);
}

/*
 * This function stops the 8 bit counter2 
 */ 
void STOP_8bit_COUNTER2(void)
{
	// Disconnect the timer clock (stop FAST PWM generation)
	TCCR2B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
}
