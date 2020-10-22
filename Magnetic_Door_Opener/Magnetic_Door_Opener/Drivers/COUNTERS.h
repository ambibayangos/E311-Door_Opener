/*
 * COUNTERS.h
 *
 * Created: 14/10/2020 12:34:12 AM
 *  Author: ambib
 */ 


#ifndef COUNTERS_H_
#define COUNTERS_H_

#include <avr/io.h>

#include "Controller.h"

/*
 * This function initializes the 8 bit timer2 in Atmega328P
 *
 */ 
void COUNTER_8bit_timer2_init(void);

/*
 * This function starts the 8 bit counter with a prescale of 1024
 */ 
void START_8bit_COUNTER2(void);

/*
 * This function stops the 16 bit counter 
 */ 
void STOP_8bit_COUNTER2(void);


/*
 * This function initializes the 8 bit timer (Timer0/Counter0)
 */ 
void COUNTER_8bit_timer0_init(void);


/*
 * This function starts the 8 bit counter with a prescale of 256
 */ 

void START_8bit_COUNTER0(void);

/*
 * This function stops the 16 bit counter 
 */ 
void STOP_8bit_COUNTER0(void);


/*
 * This function initializes the 8 bit timer (Timer2/Counter2)
 */ 
void COUNTER_16bit_timer1_init(void);

/*
 * This function starts the 8 bit counter2 with a prescale of 256
 */ 
void START_16bit_COUNTER1(void);

/*
 * This function stops the 8 bit counter2 
 */ 
void STOP_16bit_COUNTER1(void);

/*
 * This function initializes the 16 bit timer (Timer1/Counter1) to timeout after 60 seconds
 */ 
void OPENING_CURRENT_TIMER_config(void);

/*
 * This function this function starts a 60 second timer
 */ 
void START_OPENING_CURRENT_TIMER(void);

/*
 * This function stops the 60 second timer "OPENING_CURRENT_TIMER"
 */ 
void STOP_OPENING_CURRENT_TIMER(void);


void start_opening_current(void);



#endif /* COUNTERS_H_ */