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
#include "Drivers/EXT_INTERRUPT.h"


volatile adc = 0;

ISR(TIMER1_COMPB_vect)
{	
	adc = ADC_convert(_PC0);
	get_doorstate();
}




int main(void)
{	
	// Initializes the drivers
	ADC_init();
	//COUNTER_8bit_timer2_init();
	//COUNTER_8bit_timer0_init();
	//COUNTER_16bit_timer1_init();
	GPIO_init();
	init_phase_correct_mode();
	UART_init(MYUBRR);
	//INIT_EXTERNAL_ISR();
	sei(); // enable global interupt 
	
	// strart fsm
	FSM_start();
}

/*
 *  this function gets door state
 *  it returns 1 if door is open and return 0 if door is closed 
 */ 
int get_doorstate(uint16_t adc)
{
	float value = (adc*5.0)/ADC_REF;
	
	value = (value/COIL_SENSOR_CIRCUIT_GAIN)/SHUNT; // calculate current
	
	if (value >= DOOR_THRESHOLD_CURRENT)//still placeholders for actual values
	{	
		Door_State = Door_Opened; 
	}
	else
	{	
		Door_State =  Door_Closed; 
	}

}