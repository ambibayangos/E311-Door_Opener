/*
 * Magnetic_Door_Opener.c
 *
 * Created: 13/10/2020 7:28:26 PM
 * Author : ambib
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Drivers/ADC.h"
#include "Drivers/COUNTERS.h"
#include "Drivers/GPIO.h"
#include "Drivers/UART.h"
#include "Drivers/Controller.h"


int c = 1;
int count = 0;
int  d_idx = 0;

/*
 * This ISR timer creates a PWM and starts the timer(3.2ms) used to measure coil current
 *
 */ 
ISR(TIMER1_COMPA_vect)
{	
	if(c == 0)
	{			
		PORTD &= ~((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5));
		OCR1A = 38*(1-duty[d_idx]);
		c = 1;
		count++;
	}
	else if(c == 1)
	{	
		
		if(Coil_Current_Polarity_State == Opening_Force_Current)
		{
			PORTD |= (1<<DDD2) | (1<<DDD3);
		}
		else if(Coil_Current_Polarity_State == Closing_Force_Current)
		{
			PORTD |=  (1<<DDD4) | (1<<DDD5);
		}
		
		START_8bit_COUNTER0();
		OCR1A = 38*duty[d_idx];
		c = 0;
	}
	
	if(count == 10)
	{	
		count = 0;
		 d_idx = ++d_idx%9;
	}
	
}

/*
 * This ISR timer counts 3.2 ms to start sampling coil current
 *it sets a flag to sample the current
 *
 */ 
ISR(TIMER0_COMPA_vect)
{	
	Sample_Coil_Current = 1; // set flag to initialize current sampling
	STOP_8bit_COUNTER0(); // stop timer0 to prevent samping at other times
}

int main(void)
{	
	
	ADC_init();
	COUNTER_16bit_init();
	COUNTER_8bit_timer0_init();
	GPIO_init();
	UART_init(MYUBRR);
	sei(); // enable global interupt 
		
	FSM_start();
}

