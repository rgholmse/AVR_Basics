/*
	Solution project for part 1
	
	Description:
	Blink a LED with 0.5 second interval
	
	Hardware:
	Pin	Name	Description
	PC6	LED0	A LED is connected to this pin. The LED is activated by driving this GPIO to GND (logic 0)
*/


// Define the clock frequency for util/delay.h
#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	// Set PC6 as an output
	PORTC.DIRSET = PIN6_bm;
	   
    while (1) 
    {
		// Use the toggle register to toggle the output value of PC6
		PORTC.OUTTGL = PIN6_bm;
		
		// Wait for 500 ms using util/delay.h
		_delay_ms(500);
    }
}

