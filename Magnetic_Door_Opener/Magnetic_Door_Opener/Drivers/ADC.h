/*
 * ADC.h
 *
 * Created: 13/10/2020 9:04:24 PM
 *  Author: ambib
 */ 


#ifndef ADC_H_
#define ADC_H_

/*  
 * This function initializes the ADC unit
 *  in Atmega 328P
 */
void ADC_init(void);

/*  
 * This function initializes converts the current voltage
 *  at of pin PC0 = 0 (ADC0) or PC1 = 1 (ADC1) once and returns the ADC value
 *	at a 16 bit integer.
 */
uint16_t ADC_convert(uint8_t channel);


enum CHANNEL{
	_PC0 = 0,  // switch adc channel to sample the coil voltage when current is opening polarity
	_PC1 = 1,  // switch adc channel to sample the coil voltage when current is closing polarity
	_PC2 = 2   // switch adc channel to sample the touch sensor output
};


#endif /* ADC_H_ */