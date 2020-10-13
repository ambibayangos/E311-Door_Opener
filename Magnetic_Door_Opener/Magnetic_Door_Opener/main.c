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

int c = 1;
ISR(TIMER1_COMPA_vect)
{
	if(c == 0)
	{
		PORTD &= ~((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5));
		OCR1A = 38*(1-0.5);
		c = 1;
	}
	else if(c == 1)
	{
		PORTD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
		OCR1A = 38*0.5;
		c = 0;
	}
}


int main(void)
{	
	
	ADC_init();
	COUNTER_16bit_init();
	GPIO_init();
	UART_init(MYUBRR);
	sei(); // enable global interupt 
	
	uint16_t adc =  ADC_convert(_PC0);
	UART_transmit_number(adc);
	
    while (1) 
    {	
		//UART_transmit_string("Hi Welcome to the Proteus Demo\n");
    }
}

