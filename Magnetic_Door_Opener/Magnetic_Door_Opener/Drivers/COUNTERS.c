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
void COUNTER_8bit_timer2_init(void)
{
	// Set to CTC mode
	TCCR2A |= (1<<WGM21);TCCR2A &= ~(1<<TCCR2A); TCCR2B &= (1<<WGM22);
	
	//Enable ISR when count matches OCR1 value
	TIMSK2 |= (1<<OCIE2A);
	
	// Initial Value to count to
	OCR2A = 1;
		 
}

/*
 * This function starts the 16 bit counter with a prescale of 1024
 */ 

void START_8bit_COUNTER2(void)
{		
		// Resets counter value to zero
		TCNT2 = 0;
		// Set prescaller to 1024 and start counting
		TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
}


/*
 * This function stops the 16 bit counter 
 */ 
void STOP_8bit_COUNTER2(void)
{		
		// Disconnect clk from timer (Stop the timer)
		TCCR2B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
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
void COUNTER_16bit_timer1_init(void)
{
// Set FAST-PWM mode
TCCR1A |= (1<<WGM11) | (1<<WGM10); TCCR1B |= (1<<WGM13) | (1<<WGM12);

// Set PWM period to 1ms
OCR1A = 5;

//Set inverinting
TCCR1A |= (1<<COM1A0);

}

/*
 * This function starts the 8 bit counter2 with a prescale of 256
 */ 
void START_16bit_COUNTER1(void)
{	
	// Reset timer value to zero
	TCNT1 = 0;
	// Set prescaller to 64 and start timer
	TCCR1B |= (1<<CS11) | (1<<CS10); TCCR1B &= ~(1<<CS12);
}

/*
 * This function stops the 8 bit counter2 
 */ 
void STOP_16bit_COUNTER1(void)
{
	// Disconnect the timer clock (stop FAST PWM generation)
	TCCR1B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
}
