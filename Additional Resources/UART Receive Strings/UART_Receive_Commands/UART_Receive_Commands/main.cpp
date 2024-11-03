/*
 * UART_Receive_Commands.cpp
 *
 * Created: 11/3/2024 4:00:02 PM
 * Author : RalvGSHolmsen
 */ 

/*
	This code is supposed to highlight how ASCII strings can be used to transmit commands
	to an AVR via UART. The code looks for two code words, one beeing a string, and the 
	other beeing an integer. The commands sent via UART is used to control the value of
	the built-in LED of the curiosity nano.
*/


#define F_CPU 4000000
#define BAUD(BAUD_RATE) ( (uint16_t)((64.0 * (float)F_CPU) / (16.0 *(float)BAUD_RATE) + 0.5) )

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdarg.h> // XC8 Compiler (MPLAB)


void cdc_send_char(char c)
{
    // Wait for USART1 to be ready for new data
    while (!(USART1.STATUS & USART_DREIF_bm));
    
    // Give new data to USART 1
    USART1.TXDATAL = c;
}
void cdc_send_string(const char* str)
{
    for (uint8_t i=0; i < strlen(str); i++)
    {
        cdc_send_char(str[i]);
    }
}
void cdc_print(const char* text, ...)
{
    va_list argptr;
    char buffer[100];
    
    va_start(argptr, text);
    vsprintf(buffer, text, argptr);
    
    cdc_send_string(buffer);
    va_end(argptr);
}


#define UART_RXBUF_SIZE 100
volatile uint8_t uart_flag = 0, uart_index;
char uart_rxbuf[UART_RXBUF_SIZE];

int main(void) {
    // Enable interrupt of RXDATA received
	USART1.CTRLA	= USART_RXCIE_bm;
	
	// Enable USART1 transmitter and receiver
    USART1.CTRLB    = USART_TXEN_bm
					| USART_RXEN_bm;
    
    // Configure the baud rate of USART1
    USART1.BAUD     = BAUD(9600);
    
    // Make PC0 an output
    PORTC.DIRSET    = PIN0_bm;
    
    // Make PC1 an input (optional as it is an input by default)
    PORTC.DIRCLR    = PIN1_bm;
	
	// Make PC6 output (LED)
	PORTC.DIRSET	= PIN6_bm;
    
    cdc_print("Command interface. Type \"help\" for help\r\n");
	
	// Enable interrupts
	sei();
    
    while (1) 
    {
        // Check if new string is received via UART
		if (uart_flag)
		{
			uart_flag = 0;
			
			char *trash, *command;
			uint8_t val;
			
			// Separate words by <space>
			command = strtok(uart_rxbuf, " ");		// First word is command (ASCII)
			val = (uint8_t)atoi(strtok(NULL, " "));	// Second workd is value (integer)
			
			// Discard all other words
			do 
			{
				trash = strtok(NULL, " ");
			} while (trash != NULL);
			
			// Check commands
			if ((!(strcmp("LED", command))) || (!(strcmp("led", command))))
			{
				// Command "LED" found! Update LED based on value
				if (val) {
					PORTC.OUTSET = PIN6_bm;
					val = 1;
				}
				else {
					PORTC.OUTCLR = PIN6_bm;
				}
				cdc_print("Command %s acknowledged with value %d\r\n", command, val);
			}
			
			else if (!(strcmp("help", command)))
			{
				cdc_print("Welcome to the UART Receive Command demo!\r\n\r\n");
				cdc_print("This demo controls the value of the built-in LED of the Curiosity Nano kit.\r\n");
				cdc_print("Type \"led <x>\" where <x> is an integer value between 0 and 1. The OUT value of\r\n");
				cdc_print("PC6 will be SET or CLEARED accordingly!\r\n\r\n");
			}
			
			else
			{
				cdc_print("Unknown command, type \"help\" for help\r\n");
			}
		}
		
		// Do something else..
    }
}


ISR(USART1_RXC_vect)
{
	// Reading RXDATA clears interrupt flag (unique)
	char c = USART1.RXDATAL;
	
	// Loopback print value (optional). It is recommended that the following code is removed
	while(!(USART1.STATUS & USART_DREIF_bm));
	USART1.TXDATAL = c;
	// Removing this code makes reception more efficient (non-blocking code)
	// TeraTerm can have a local loopback which is more efficient
	
	
	// Check if char is text
	if ((c != '\n') && (c != '\r') && (c != '\b'))
	{
		uart_rxbuf[uart_index++] = c;
		if (uart_index > UART_RXBUF_SIZE)
		{
			uart_index = 0;
		}
	}
	// If carriage return
	else if (c == '\r')
	{
		uart_rxbuf[uart_index] = '\0';
		uart_index = 0;
		uart_flag = 1;
	}
	// If backspace
	else if ((c == '\b') && (uart_index))
	{
		uart_index--;
	}
}

