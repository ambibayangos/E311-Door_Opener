/*
 * UART.h
 *
 * Created: 13/10/2020 9:52:36 PM
 *  Author: ambib
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

//Proteus setup with 0.8MHz external clock 
//(everything scaled by // 1/10 from a typical implementation)
#define F_CPU 8000000
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

/*
 * This function initializes the UART
 * 
 */
void UART_init(unsigned int ubrr);


/*
 * This function transmit a character serially via UART
 * 
 */
void UART_transmit_char(unsigned char data);


/*
 * This function transmit a string serially via UART
 * 
 */
void UART_transmit_string(char *str);



#endif /* UART_H_ */