/*
 * Part 4 Solution INT.cpp
 *
 * Created: 13.10.2024 18:09:49
 * Author : RalvGSHolmsen
 */ 

/*
	
*/

#define F_CPU 4000000

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t millis = 0;

int main(void)
{
	// Set PC6 as output
	PORTC.DIRSET	= PIN6_bm;
	
	// Set CNTMODE for TCB
	TCB0.CTRLB		= TCB_CNTMODE_INT_gc;
	
	// Enable TCB0 overflow interrupt
	TCB0.INTCTRL	= TCB_CAPT_bm;
	
	// Calculate the TCB TOP Value
	TCB0.CCMP		=  (uint16_t)( ((float)F_CPU) / (1000.0) - 0.5 );
	
	// Enable TCB and set clock source to F_CPU DIV1
	TCB0.CTRLA		= TCB_CLKSEL_DIV1_gc
					| TCB_ENABLE_bm;
					
	// Set Enable (Global) Interrupts
	sei();
	
    uint32_t led_millis_last = 0, led_millis_delay = 500;
    while (1) 
    {
		// Check if 500 ms has passed
		if ((millis - led_millis_last) >= led_millis_delay)
		{
			// Update last millisecond value
			led_millis_last = millis;
			
			PORTC.OUTTGL = PIN6_bm;
		}
    }
}	

ISR(TCB0_INT_vect)
{
	// Clear interrupt flag
	TCB0.INTFLAGS = TCB_CAPT_bm;
	
	millis++;
}

