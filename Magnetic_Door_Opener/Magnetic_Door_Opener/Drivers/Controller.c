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
#include "ADC.h"
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
	pmw_delay_flag = 1;
	int sample_touch_flag = 0;
	int first_time_touch_value = 0;
	int second_time_touch_value = 0;


	
	while(1)
	{	
		//UART_transmit_number(Current_FSM_state);
		//UART_transmit_string("\n\r");
		//UART_transmit_string("\n\r");
	
		switch(Current_FSM_state)
		{
			case Initialisation_State:
				
				//UART_transmit_number(55);
				if(!half_Duty_Produced)
				{	
					UART_transmit_number(66);
					 // initialize duty cycle to 50% for sensing
					OCR1A = 50*155/100;
					START_OPENING_CURRENT();
					//Coil_Current_Polarity_State = Opening_Force_Current; // generate a opening force
					//START_8bit_COUNTER2(); // STARTS THE PWM
					half_Duty_Produced = 1;
				}
				
				if (Door_State==Door_Closed)
				{
					// Move to "WaitTouch_State" state
					Current_FSM_state= 	WaitTouch_State;
					// singal the door closed LED
					PORTC &= ~(1<<DDD3);
				}
				
				else if (Door_State==Door_Opened)
				{
					// Move to "Generate_Closing_Force_State" state
					Current_FSM_state = Generate_Closing_Force_State;
					// singal the door closed LED
					PORTC |= (1<<DDD3);
				}
						
				break;
			
			case Generate_Closing_Force_State:
					
					
					if(!closing_force_routine_initialized)
					{	
						
						// configure pwm for closing force
						TCCR1A |= (1<<COM1B1);
						STOP_8bit_COUNTER2(); // stops the coil pwm generator
						Coil_Current_Polarity_State = Closing_Force_Current;
						//initialize duty cycle to 50% for sensing
						duty[0] = 0.5;duty[1] = 0.6;duty[2] = 0.7;duty[3] = 0.8;
						duty[4] = 0.9;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;
						// Start initilize timer to provide 3 sec delay
						PWM_DELAY_init();
						// start 3 sec delay timer
						START_PWM_DELAY();
						// enable 30 sec block
						pmw_delay_flag = 1;
						// do not provide closing force until 30 elapse
						while (pmw_delay_flag);
						//if flag = 1 then wait

						//UART_transmit_number(pwm_delay_flag*100);
						// restart/start the coil pwm generator
						START_8bit_COUNTER2();
							
						//  Initialize "Generate_Closing_Force_State" state once
						closing_force_routine_initialized = 1;
					}
						
						
					if(Sample_Coil_Current)
					{
						Sample_Coil_Current = 0;
						uint16_t adc_closing_current = ADC_convert(_PC1);
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
					// stop coil pwm
					STOP_OPENING_CURRENT();
					// start a 1Khz PWM for capacitive touch
					START_TOUCH_PWM();
					wait_touch_routine_initialized = 1;
				}
				
				if(Sample_touch_circuit) // Sample touch circuit at falling edge of 1Hz pwm
				{	
					Sample_touch_circuit = 0;
					uint16_t touch_adc = ADC_convert(_PC2); // sample the voltage at the touch sensor
					
					if(sample_touch_flag)
					{
						uint16_t value_prev = ADC_convert(_PC2); // sample the voltage at the touch sensor
						if(value_prev >= DOOR_TOUCHED_VOLTAGE)
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
						uint16_t value_current = ADC_convert(_PC2); // sample the voltage at the touch sensor
						if(value_current >= DOOR_TOUCHED_VOLTAGE)
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
					// turn off the fast pwm generator (wait touch pwm)
					STOP_16bit_COUNTER1(); 
					// Configures the 16bit timer to be a 60 sec timer
					//OPENING_CURRENT_TIMER_config();
					// start a 60 sec timer (time frame for the door opening process)
					//START_OPENING_CURRENT_TIMER();
					Coil_Current_Polarity_State = Opening_Force_Current; // change polarity of current
					// provide the duty cycles that will be cycled through (Duty = 0.1 to 0.9)
					duty[0] = 0.1;duty[1] = 0.2;duty[2] = 0.3;duty[3] = 0.4;
					duty[4] = 0.5;duty[5] = 0.6;duty[6] = 0.7;duty[7] = 0.8;duty[8] = 0.9;
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
					
					if (Door_State==Door_Opened)
					{	
						// Move to "Generate_Closing_Force_State" state to provide closing force
						Current_FSM_state = Generate_Closing_Force_State;
						// Enables the "Generate_Closing_Force_State" state to re-initialize
						closing_force_routine_initialized = 0;
					}
					
				}
			
				break;
			default:
				Current_FSM_state = Initialisation_State;
				break;
		}
		
				
	}
	
}

