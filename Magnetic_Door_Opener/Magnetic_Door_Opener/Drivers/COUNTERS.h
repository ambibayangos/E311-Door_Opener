/*
 * COUNTERS.h
 *
 * Created: 14/10/2020 12:34:12 AM
 *  Author: ambib
 */ 


#ifndef COUNTERS_H_
#define COUNTERS_H_

#include <avr/io.h>

/*
 * This function initializes the 16 bit timer in Atmega328P
 *
 */ 
void COUNTER_16bit_init(void);

/*
 * This function starts the 16 bit counter with a prescale of 1024
 */ 
void START_16bit_COUNTER(void);

/*
 * This function stops the 16 bit counter 
 */ 
void STOP_16bit_COUNTER(void);


/*
 * This function initializes the 8 bit timer (Timer0/Counter0)
 */ 
void COUNTER_8bit_timer0_init(void);


/*
 * This function starts the 16 bit counter with a prescale of 1024
 */ 

void START_8bit_COUNTER0(void);

/*
 * This function stops the 16 bit counter 
 */ 
void STOP_8bit_COUNTER0(void);


#endif /* COUNTERS_H_ */