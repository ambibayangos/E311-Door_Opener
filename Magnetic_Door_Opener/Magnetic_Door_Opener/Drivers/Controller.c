/*
 * Controller.c
 *
 * Created: 14/10/2020 2:35:47 PM
 *  Author: ambib
 */ 

#include <avr/io.h>

#include "Controller.h"
#include "COUNTERS.h"
#include "ADC.h"
#include "UART.h"

/*
 *  This function represents the FSM that controls the current driver 
 *  as
 */ 
void FSM_start(void)
{	
	uint8_t half_Duty_Produced = 0;
	Current_FSM_state = Initialisation_State; // Initialize current state to Initialization state
	Door_State = Uknown; // Door state is unknown when programme just started
	Sample_Coil_Current = 0;
	Sample_touch_circuit = 0;
	closing_force_routine_initialized = 0;
	wait_touch_routine_initialized = 0;
	opening_force_routine_initialized = 0;
	int sample_touch_flag = 0;
	int first_time_touch_value = 0;
	int second_time_touch_value = 0;
	
	int ccc = 0;
	
	while(1)
	{	
		UART_transmit_number(88);
	
		
		switch(Current_FSM_state)
		{
			case Initialisation_State:
					
				
				if(!half_Duty_Produced)
				{	
					 // initialize duty cycle to 50% for sensing
					duty[0] = 0.5;duty[1] = 0.5;duty[2] = 0.5;duty[3] = 0.5;
					duty[4] = 0.5;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;
					Coil_Current_Polarity_State = Opening_Force_Current; // generate a opening force
					START_8bit_COUNTER2();
					half_Duty_Produced = 1;
				}
				
				if(Sample_Coil_Current)
				{	
					Sample_Coil_Current = 0;
					uint16_t adc = ADC_convert(_PC0);
					Door_State = get_doorstate(adc); // decide if the door is open or not
					
					if (Door_State==Door_Closed)
					{	
						// Move to "WaitTouch_State" state 							
						Current_FSM_state= 	WaitTouch_State;					
					}
					
					else if (Door_State==Door_Opened)
					{	
						// Move to "Generate_Closing_Force_State" state
						Current_FSM_state = Generate_Closing_Force_State;
					}

			  }
				
				break;
			
			case Generate_Closing_Force_State:
					
					//UART_transmit_number(99);
					
					if(!closing_force_routine_initialized)
					{
					STOP_8bit_COUNTER2(); // stops the coil pwm generator
					Coil_Current_Polarity_State = Closing_Force_Current;
					// initialize duty cycle to 50% for sensing
					duty[0] = 0.5;duty[1] = 0.6;duty[2] = 0.7;duty[3] = 0.8;
					duty[4] = 0.9;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;	
					//TODO -------------------------------------------> start a timer
					// ----------------------------------------------->delay , whhile timer dint expire
					START_8bit_COUNTER2(); // restart the coil pwm generator
					
					//  Initialize "Generate_Closing_Force_State" state once
					closing_force_routine_initialized = 1;
					}
					
					if(Sample_Coil_Current)
					{
						Sample_Coil_Current = 0;
						uint16_t adc_closing_current = ADC_convert(_PC0);
						Door_State = get_doorstate(adc_closing_current); // decide if the door is open or not
						
						if (Door_State==Door_Closed)
						{
							// Move to "WaitTouch_State" state				
							Current_FSM_state= 	WaitTouch_State;
							// Enables the "WaitTouch_State" to re-initialize
							wait_touch_routine_initialized = 0;
						}
						
					}
			
				break;
				
			case WaitTouch_State:
				
				
				if(!wait_touch_routine_initialized)
				{	
					// stop pmw on the coil
					STOP_8bit_COUNTER2();
					// Initialize 16bit timer to fast pwm mode
					COUNTER_16bit_timer1_init();
					// start the 1KHz pwm generation
					START_16bit_COUNTER1();
					// Intialize "WaitTouch_State" only once
					wait_touch_routine_initialized = 1;
				}
				
				if(Sample_touch_circuit) // Sample touch circuit at falling edge of 1Hz pwm
				{	
					Sample_touch_circuit = 0;
					uint16_t touch_adc = ADC_convert(_PC1); // sample the voltage at the touch sensor
					
					if(sample_touch_flag)
					{
						uint16_t value_prev = ADC_convert(_PC1);
						if(value_prev >= 977)
						{
							first_time_touch_value = 0; // door not touched
						}
						else
						{
							first_time_touch_value = 1; //door touched
						}
						
						sample_touch_flag = 0;
					}
					else
					{
						uint16_t value_current = ADC_convert(_PC1);
						if(value_current >= 977)
						{	
							
							second_time_touch_value = 0; // door not touched
						}
						else
						{	
							second_time_touch_value = 1; //door touched
						}
						
						if(first_time_touch_value*second_time_touch_value == 1) //door grabbed for at least 1 second
						{	
							// Move to "Generate_Opening_Force_State" state
							Current_FSM_state = Generate_Opening_Force_State;
							// Enables the "Generate_Opening_Force_State" state to re-initialize
							opening_force_routine_initialized = 0;
						}
						
						sample_touch_flag = 1;
					}
	
				}
				
				break;	
				
			case Generate_Opening_Force_State:
								
				if(!opening_force_routine_initialized)
				{	
					
					// turn off the fast pwm generator
					STOP_16bit_COUNTER1(); 
					// Configures the 16bit timer to be a 60 sec timer
					OPENING_CURRENT_TIMER_config();
					// start a 60 sec timer (time frame for the door opening process)
					START_OPENING_CURRENT_TIMER();
					Coil_Current_Polarity_State = Opening_Force_Current; // change polarity of current
					// provide the duty cycles that will be cycled through (Duty = 0.1 to 0.9)
					duty[0] = 0.1;duty[1] = 0.2;duty[2] = 0.3;duty[3] = 0.4;
					duty[4] = 0.5;duty[5] = 0.6;duty[6] = 0.7;duty[7] = 0.8;duty[8] = 0.9;
					COUNTER_8bit_timer2_init(); // remove later
					// restart pwm generation on the coil
					START_8bit_COUNTER2(); 
					// initialize "Generate_Opening_Force_State" state once
					opening_force_routine_initialized = 1;
				}
				
				
				if(Sample_Coil_Current)
				{
					
					Sample_Coil_Current = 0;
					uint16_t adc_opening_current = ADC_convert(_PC0);
					Door_State = get_doorstate(adc_opening_current); // decide if the door is open or not
					
					UART_transmit_number(adc_opening_current);
							
				
					if (Door_State==Door_Opened)
					{	
						
						//Move to "Generate_Closing_Force_State" state to provide closing force
						//Current_FSM_state = Generate_Closing_Force_State;
						// Enables the "Generate_Closing_Force_State" state to re-initialize
						closing_force_routine_initialized = 0;
					}
					
				}
				
							
				break;
		
		}
		
				
	}
	
}

int hhh = 0;

/*
 *  this function gets door state
 *  it returns 1 if door is open and return 0 if door is closed 
 */ 
int get_doorstate(uint16_t adc)
{
	float value = (adc*5.0)/ADC_REF;
	
	value = value/SHUNT; // calculate current

	
	if (value >= DOOR_THRESHOLD_CURRENT)//still placeholders for actual values
	{	
		return Door_Opened; 
	}

	return Door_Closed; 
}