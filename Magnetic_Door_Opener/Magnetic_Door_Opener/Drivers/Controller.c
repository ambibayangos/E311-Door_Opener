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
	int flag_set = 0;
	int fast_pwm_started = 0;
	int sample_touch_flag = 0;
	int first_time_touch_value = 0;
	int second_time_touch_value = 0;
	
	while(1)
	{	
		
		UART_transmit_number(Current_FSM_state);
		
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
				
				if(Sample_Coil_Current) // some flag
				{	
					Sample_Coil_Current = 0;
					uint16_t adc = ADC_convert(_PC0);
					Door_State = get_doorstate(adc); // decide if the door is open or not
					
					if (Door_State==Door_Closed)
					{	
						//TODO: also stop pwm to stop checking
							
						Current_FSM_state= 	WaitTouch_State;					
					}
					
					else if (Door_State==Door_Opened)
					{	
						Current_FSM_state = Generate_Closing_Force_State;
					}

			  }
				
				break;
			
			case Generate_Closing_Force_State:
					
					if(!flag_set)
					{
					STOP_8bit_COUNTER2(); // stops the pwm generator
					Coil_Current_Polarity_State = Closing_Force_Current;
					// initialize duty cycle to 50% for sensing
					duty[0] = 0.5;duty[1] = 0.6;duty[2] = 0.7;duty[3] = 0.8;
					duty[4] = 0.9;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;	
					// start a timer
					//while(1); // delay , whhile timer dint expire
					START_8bit_COUNTER2(); // change polarity
					
					// cycle thru pwm 0.5 to 0.9
					//at 0.5 duty cycle set flag to get the door state
					flag_set = 1;
					}
					
					if(Sample_Coil_Current)
					{
						Sample_Coil_Current = 0;
						uint16_t adc = ADC_convert(_PC0);
						Door_State = get_doorstate(adc); // decide if the door is open or not
						
						if (Door_State==Door_Closed)
						{
							//TODO: also stop pwm to stop checking
							
							Current_FSM_state= 	WaitTouch_State;
						}
						else if (Door_State==Door_Opened)
						{
							Current_FSM_state = Generate_Closing_Force_State;
						}
						
					}
			
				break;
				
			case WaitTouch_State:
				
				if(!fast_pwm_started)
				{	
					// stop pmw on the coil
					STOP_8bit_COUNTER2();
					// start the 1Khz pwm generation
					START_16bit_COUNTER1();
					fast_pwm_started = 1;
				}
				
				if(Sample_touch_circuit) // if sample edge occurs sample the PD2
				{	
					Sample_touch_circuit = 0;
					uint16_t touch_adc = ADC_convert(_PC1); // sample the voltage at the touch sensor
					
					if(sample_touch_flag)
					{
						uint16_t value_prev = ADC_convert(_PC1);
						if(value_prev >= 977)
						{
							first_time_touch_value = 0; // not touched
						}
						else
						{
							first_time_touch_value = 1; // touched
						}
						
						sample_touch_flag = 0;
					}
					else
					{
						uint16_t value_current = ADC_convert(_PC1);
						if(value_current >= 977)
						{	
							
							second_time_touch_value = 0; // not touched
						}
						else
						{	
							second_time_touch_value = 1; // touched
						}
						
						if(first_time_touch_value*second_time_touch_value == 1) //grabbed
						{	
							Current_FSM_state = Generate_Opening_Force_State;
						}
						
						sample_touch_flag = 1;
					}
	
					// call adc
					//somehow pass some time
					//call adc again
					//compare 2 values
					//decide if touch
				}
				
				break;	
				
			case Generate_Opening_Force_State:
				
				STOP_16bit_COUNTER1(); // turn off the fast pwm generator
				
				UART_transmit_number(666);
				
				break;
		
		}
		
				
	}
	
}


/*
 *  this function gets door state
 *  it returns 1 if door is open and return 0 if door is closed 
 */ 
int get_doorstate(uint16_t adc)
{
	float value = (adc*5.0)/ADC_REF;
	
	value = value/SHUNT; // calculate current
	
	
	if (value <= DOOR_THRESHOLD_CURRENT)//still placeholders for actual values
	{	
		//UART_transmit_number(77);
		return Door_Closed; 
	}
	
	return Door_Opened;
}