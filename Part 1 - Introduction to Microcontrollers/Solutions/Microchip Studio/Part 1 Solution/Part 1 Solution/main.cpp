/*
	Solution part 1
	
	Description:
	Blink a LED every 0.5 second
	
	Hardware:
	Pin	Name	Description
	PC6	LED0	LED0 is activated on logic 0 and deactivated on logic 1
*/

// Define F_CPU for util/delay.h
#define F_CPU 4000000 // Default clock frequency for AVR DA series is 4 MHz

// Include avr/io.h for register definitions
#include <avr/io.h>

// Include util/delay.h for delay functions
#include <util/delay.h>


int main(void)
{
    // Set PC6 direction as output
	PORTC.DIRSET = PIN6_bm;
	
    while (1) 
    {
		// Use the toggle register to toggle the output logic value of PC6
		PORTC.OUTTGL = PIN6_bm;
		
		// Wait for 500 ms
		_delay_ms(500);
    }
}

