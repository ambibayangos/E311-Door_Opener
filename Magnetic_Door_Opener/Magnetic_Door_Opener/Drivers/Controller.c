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
	int flag_set = 0;
	while(1)
	{	
		
		//UART_transmit_number(Coil_Current_Polarity_State);
		
		switch(Current_FSM_state)
		{
			case Initialisation_State:
				
				
				if(!half_Duty_Produced)
				{	
					 // initialize duty cycle to 50% for sensing
					duty[0] = 0.5;duty[1] = 0.5;duty[2] = 0.5;duty[3] = 0.5;
					duty[4] = 0.5;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;
					Coil_Current_Polarity_State = Opening_Force_Current; // generate a opening force
					START_16bit_COUNTER();
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
						Current_FSM_state = Open_Door_State;
					}

			  }
				
				break;
			
			case Open_Door_State:
					
					if(!flag_set)
					{
					STOP_16bit_COUNTER(); // stops the pwm generator
					Coil_Current_Polarity_State = Closing_Force_Current;
					// initialize duty cycle to 50% for sensing
					duty[0] = 0.5;duty[1] = 0.6;duty[2] = 0.7;duty[3] = 0.8;
					duty[4] = 0.9;duty[5] = 0.5;duty[6] = 0.5;duty[7] = 0.5;duty[8] = 0.5;	
					// start a timer
					//while(1); // delay , whhile timer dint expire
					START_16bit_COUNTER(); // change polarity
					
					// cycle thru pwm 0.5 to 0.9
					//at 0.5 duty cycle set flag to get the door state
					flag_set = 1;
					}
					
					if(Sample_Coil_Current)
					{
						
					}
			
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
	float value = adc*5/ADC_REF;
	
	value = value/SHUNT; //0.5 is placeholder for reverse math
	
	if (value <= DOOR_THRESHOLD_CURRENT)//still placeholders for actual values
	{	
		return Door_Closed; 
	}
	
	return Door_Opened;
}