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
float duty[9] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
int count = 0;
int  d_idx = 0;
ISR(TIMER1_COMPA_vect)
{
	if(c == 0)
	{
		PORTD &= ~((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5));
		OCR1A = 38*(1-duty[d_idx]);
		c = 1;
		count++;
		UART_transmit_number(duty[d_idx]*100);
		UART_transmit_string("\n\r");
	}
	else if(c == 1)
	{
		PORTD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
		OCR1A = 38*duty[d_idx];
		c = 0;
	}
	
	if(count == 5)
	{	
		count = 0;
		d_idx = (++d_idx)%9;
		
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
	UART_transmit_string("\n\r");
	adc =  ADC_convert(_PC1);
	UART_transmit_number(adc);
	
    while (1) 
    {	
    }
}

