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
		
		// turn off all coil pwm
		PORTD &= ~((1<<DDD6) | (1<<DDD7) | (1<<DDD4) | (1<<DDD5));
		
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

// Disable Intrrupt
TIMSK1 &= ~(1<<OCIE1A);

//Set inverinting
TCCR1A |= (1<<COM1A0);  TCCR1A &= ~(1<<COM1A1);

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


/*
 * This function initializes the 16 bit timer (Timer1/Counter1) to timeout after 60 seconds (this timer is the 16bit timer)
 */ 
void PWM_DELAY_init(void)
{	
	
	// Set to CTC mode
	TCCR1A &= ~((1<<WGM10) | (1<<WGM11)); TCCR1B &= ~(1<<WGM13); TCCR1B |= (1<<WGM12);
	
	// Trigger ISR when timer is up (Timer matches OCR1A value)
	TIMSK1 |= (1<<OCIE1A);
	
	// Set timeout to be 30 sec
	OCR1A = 10000;//23436;
}

/*
 * This function this function starts a 60 second timer (this timer is the 16bit timer)
 */ 
void START_PWM_DELAY(void)
{	
	// reset timer value
	TCNT1 = 0;
	// Set prescaller to 1024
	TCCR1B |= (1<<CS12) | (1<<CS10); TCCR1B &= ~(1<<CS11);
}

/*
 * This function stops the 60 second timer "OPENING_CURRENT_TIMER" (this timer is the 16bit timer)
 */ 
void STOP_PWM_DELAY(void)
{
	// Disconnect the timer clock (stop FAST PWM generation)
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}


void PWM_COIL_GENERATOR_INIT(void)
{
	// Set to fast pwm (mode 14 - ICR1 is top)
	TCCR1B |= (1<<WGM13) | (1<<WGM12); TCCR1A |= (1<<WGM11); TCCR1A &= ~(1<<WGM10);
	
	// Set frequency to 20 Hz
	ICR1 = 155;
	
	// initialize duty cycle to 50%
	OCR1A = 50*155/100;
	
	// enable isr for compare match B
	TIMSK1 |= (1<<OCIE1B);
	
	// trigger isr after 3.2ms
	OCR1B = 10;
	
	// set mode - toggle pin on match compare
	TCCR1A |= (1<<COM1A1); TCCR1A &= ~(1<<COM1A0);
}

void START_OPENING_CURRENT(void)
{
	//Resets the timer count to zero
	TCNT1 = 0;
	// Set prescaller to 256 and start the timer
	TCCR1B |= (1<<CS12); TCCR1B &= ~((1<<CS11)| (1<<CS10));
}



void STOP_OPENING_CURRENT(void)
{
	// Set prescaller to 256 and start the timer
	TCCR1B &= ~((1<<CS12) | (1<<CS11)| (1<<CS10));
}


void TOUCH_PWM_INIT()
{	
	// set ctc mode
	TCCR0B &= ~(1<<WGM02); TCCR0A |= (1<<WGM01); TCCR0A &= ~(1<<WGM00);
	
	//set 1kHz as frequency
	OCR0A = 6;
	
	//set toggle mode to get 50% duty cycle
	TCCR0A |= (1<<COM0A0); TCCR0A &= ~(1<<COM0A1);
}

void START_TOUCH_PWM(void)
{	
	TCNT0 = 0;
	// set precaller to 64
	TCCR0B |= (1<<CS00) | (1<<CS01); TCCR0B &= ~(1<<CS02);
}