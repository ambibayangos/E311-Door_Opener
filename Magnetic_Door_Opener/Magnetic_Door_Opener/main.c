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


int pmw_arbiter = 1; // decides if the pwm should create rising or falling edge
int period_count = 0; // counts "x" period before swithing into new duty cycle
int duty_index = 0; // index of the pwm array(allow changing pwm duty cycle)
int opening_current_timer_count = 0; // counts the number of period the opening current is generating pwm
int current_duty_cycle_is_50 = 0;
int fast_pwm_period_count = 0;
volatile int adc = 0;

/*
 * This ISR timer creates a PWM and starts the timer(3.2ms) used to measure coil current
 *
 */ 
ISR(TIMER2_COMPA_vect)
{	
	
	if(pmw_arbiter == 0) // create a falling edge on pwm
	{			
		PORTD &= ~((1<<DDD6) | (1<<DDD7) | (1<<DDD4) | (1<<DDD5));
		OCR2A = PERIOD_50ms*(1-duty[duty_index]); // changes duty cycle
		pmw_arbiter = 1; // create rising edge on pwm on next 16 bit timer match
		period_count++;
	}
	else if(pmw_arbiter == 1) // create a rising edge on pwm
	{	
		
		if(Coil_Current_Polarity_State == Opening_Force_Current) 
		{	
			opening_current_timer_count++;
			PORTD |= (1<<DDD6) | (1<<DDD7); // set pwm pins for opening current gate drivers
		}
		else if(Coil_Current_Polarity_State == Closing_Force_Current) 
		{	
			PORTD |=  (1<<DDD4) | (1<<DDD5); // set pwm pins for closing current gate drivers
		}
		//start a 3.2ms timer on rising edge of 50% duty cycle to initialize coil current sampling procedure
		if(current_duty_cycle_is_50) 
		{
			START_8bit_COUNTER0();
		}  
		OCR2A = PERIOD_50ms*duty[duty_index]; // changes duty cycle
		pmw_arbiter = 0; // create a falling edge on next 16bit timer match 
	}
	
	if(period_count == 10) // wait 10 period count before changing into new duty cycle
	{	
		period_count = 0; //reset period count
		duty_index = ++duty_index%9; // cycles through pwm duty cycles repeatedly
		
		
		if(duty[duty_index] == 0.5)
		{
			current_duty_cycle_is_50 = 1;
		}
		else
		{			
			current_duty_cycle_is_50 = 0;
		}
	
	}
	
	if(opening_current_timer_count == 100)
	{
		Current_FSM_state = WaitTouch_State;
		wait_touch_routine_initialized = 0;
		opening_current_timer_count = 0;
	}
	
}

/*
 * This ISR timer counts 3.2 ms to start sampling coil current
 * it sets a flag to sample the current
 */ 
ISR(TIMER0_COMPA_vect)
{	
		Sample_Coil_Current = 1; // set flag to initialize current sampling
		STOP_8bit_COUNTER0(); // stop timer0 to prevent sampling coil current at other times	
}


ISR(INT0_vect)
{	
	if(fast_pwm_period_count == 1000) // wait 1 sec for before sampling the touch circuit again
	{
		Sample_touch_circuit = 1; // set flag to initialize the sampling process
		fast_pwm_period_count  = 0;
	}
		
	fast_pwm_period_count++;	
}



ISR(TIMER1_COMPA_vect)
{	
	// signal generate closing force
	pmw_delay_flag = 0; 
	// stop the delay timer
	STOP_PWM_DELAY();
}



ISR(TIMER1_COMPB_vect)
{
	adc = ADC_convert(_PC0);
	UART_transmit_number(adc);
	UART_transmit_string("\n\r");
	
	Door_State = Door_Opened;
}

/*
 *  this function gets door state
 *  it returns 1 if door is open and return 0 if door is closed 
 */ 
int get_doorstate(uint16_t adc)
{
	float coil_current = (adc*5.0)/ADC_REF;
	
	coil_current = (coil_current/COIL_SENSOR_CIRCUIT_GAIN)/SHUNT; // calculate current
	
	if (coil_current >= DOOR_THRESHOLD_CURRENT)//still placeholders for actual values
	{	
		Door_State = Door_Opened; 
	}
	else
	{	
		Door_State = Door_Closed; 
	}

}


int main(void)
{	
	// Initializes the drivers
	ADC_init();
	PWM_COIL_GENERATOR_INIT();
	TOUCH_PWM_INIT();
	//COUNTER_8bit_timer2_init();
	//COUNTER_8bit_timer0_init();
	//COUNTER_16bit_timer1_init();
	//INIT_EXTERNAL_ISR();
	GPIO_init();
	UART_init(MYUBRR);
	sei(); // enable global interupt 
	

	// Start and loop the programme
	FSM_start();
}

