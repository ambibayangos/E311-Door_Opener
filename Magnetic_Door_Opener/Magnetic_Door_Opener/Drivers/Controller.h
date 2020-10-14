/*
 * Controller.h
 *
 * Created: 14/10/2020 2:36:05 PM
 *  Author: ambib
 */ 


#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <avr/io.h>


#define ADC_REF 1024 // ADC resolution 10 bits
#define SHUNT 0.5 // The resistor value used to measure current in coil in ohms
#define DOOR_THRESHOLD_CURRENT 0.8 // The current (amps) used to determine if the open/closed door state

/*
 * This enum represents the state of the FSM
 */
enum FSM_States {
	Open_Door_State,
	Close_Door_State,
	WaitTouch_State,
	Initialisation_State
	};
	
/*
 * This enum represents the state of the door (opened or closed)
 */
enum Door_States {
	Door_Closed,
	Door_Opened,
	Uknown
};


enum Current_Polarity{
	Opening_Force_Current,
	Closing_Force_Current
};
	
uint8_t  Current_FSM_state;
uint8_t  Door_State;
uint8_t Duty_Cycle;
uint8_t Sample_Coil_Current;
uint8_t Coil_Current_Polarity_State;
float duty[9];

/*
 *  This function represents the FSM that controls the current driver.
 *  When this function is called , the controller begins working.
 */ 
void FSM_start(void);



#endif /* CONTROLLER_H_ */