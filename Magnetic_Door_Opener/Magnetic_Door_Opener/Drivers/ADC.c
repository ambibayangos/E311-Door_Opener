/*
 * ADC.c
 *
 * Created: 13/10/2020 9:05:02 PM
 *  Author: ambib
 */ 

#include <avr/io.h>
#include "ADC.h"

/*  
 * This function initializes the ADC unit
 *  in Atmega 328P
 */
void ADC_init(void)
{
	// Use connect AREF to ACCC (reference voltage select)
	ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);
	
	// set prescale to 128 (125 kHz)
	ADCSRA |= (1<<ADPS1) | (1<< ADPS0); ADCSRA &= ~(1<<ADPS2);
	
	// enable ADC
	//TODO: This could be power hungry since the ADC is enabled when ADC isnt being used   
	ADCSRA |= (1<<ADEN);
}

/*  
 * This function  converts the current voltage
 *  at of pin PC0 = 0 (ADC0) or PC1 = 1 (ADC1) once and then 
 *	returns the ADC value as a 16 bit integer.
 */
uint16_t ADC_convert(uint8_t channel)
{	
	/*
	// select channel to convert
	ADMUX |= (1<<channel);
	
	// start conversion
	ADCSRA |= (1<<ADSC); 
	
	// wait until conversion finishes
	while((ADCSRA &(1<<ADIF)) != 0);
	
	// read the ADC result
	uint16_t result = ADC;
	
	return result;
	*/
	
	// Set the channel we wish to convert
	ADMUX |= channel;
	
	// Start the conversion
	ADCSRA |= (1 << ADSC);
	
	// Wait for the conversion to finsih
	while ((ADCSRA & (1 << ADIF)) == 0);
	
	// Read out the ADC counts
	uint16_t adc_count = ADC;//(ADCL << 0) | (ADCH << 8);
	
	return adc_count;
}
