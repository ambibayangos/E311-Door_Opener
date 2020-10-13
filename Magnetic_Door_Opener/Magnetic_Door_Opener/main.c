/*
 * Magnetic_Door_Opener.c
 *
 * Created: 13/10/2020 7:28:26 PM
 * Author : ambib
 */ 

#include <avr/io.h>

#include "Drivers/ADC.h"


int main(void)
{	
	
	ADC_init();
    while (1) 
    {
		uint16_t adc =  ADC_convert(_PC0);
    }
}

