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
#define PERIOD_50ms 38 // represents a 50ms pwm period with system clock 0.8Mhz prescaled by 1024
#define DOOR_TOUCHED_VOLTAGE 977 //	represents the digital value of the touch sensor voltage(4.86V) when "touch" is detected
#define COIL_SENSOR_CIRCUIT_GAIN 3.1 //represents the gain of current sensor conditioner circuit								   
											

/*
 * This enum represents the state of the FSM
 */
enum FSM_States {
	Generate_Closing_Force_State = 1,
	Generate_Opening_Force_State = 2,
	WaitTouch_State = 3,
	Initialisation_State = 4
	};
	
/*
 * This enum represents the state of the door (opened or closed)
 */
enum Door_States {
	Door_Closed = 1,
	Door_Opened = 2,
	Uknown = 3
};


enum Current_Polarity{
	Opening_Force_Current,
	Closing_Force_Current
};
	
enum FSM_States Current_FSM_state;
enum Door_States  Door_State;
enum Current_Polarity Coil_Current_Polarity_State;
uint8_t Duty_Cycle;
uint8_t Sample_Coil_Current;
uint8_t Sample_touch_circuit;
int closing_force_routine_initialized;
int wait_touch_routine_initialized;
int opening_force_routine_initialized;
volatile int pmw_delay_flag;
float duty[9];

/*
 *  This function represents the FSM that controls the current driver.
 *  When this function is called , the controller begins working.
 */ 
void FSM_start(void);

#endif /* CONTROLLER_H_ */