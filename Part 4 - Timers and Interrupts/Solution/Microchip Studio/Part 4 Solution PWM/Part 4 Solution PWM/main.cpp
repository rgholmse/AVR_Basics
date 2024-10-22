/*
 * Part 4 Solution PWM.cpp
 *
 * Created: 13.10.2024 19:14:48
 * Author : RalvGSHolmsen
 */ 

/*

*/

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>

void servo_set(uint8_t duty_cycle)
{
	// Optional guard
	if (duty_cycle > 100)
	{
		duty_cycle = 100;
	}
	
	// Calculate compare value
	uint16_t cmp = duty_cycle * 40 + 1000;
	
	// Update CMPxBUF register. CMPx register us updated by buffer on overflow
	// This ensures that the pulse on time is always valid
	TCA0.SINGLE.CMP0BUF = cmp;
}

void servo_setup()
{
	#define TCA_CLKDIV 2
	#define TCA_FREQ 50.0
	// Calculate TOP value for TCA
	TCA0.SINGLE.PER		= (uint16_t)(  (((float)F_CPU) / ((float)TCA_CLKDIV))  / TCA_FREQ - 0.5  );
	
	TCA0.SINGLE.CTRLB	= TCA_SINGLE_CMP0EN_bm				// Enable pin override on CMP0 - WO0 - PD0
						| TCA_SINGLE_WGMODE_SINGLESLOPE_gc;	// TCA0 in PWM mode (Single slope)
	
	TCA0.SINGLE.CTRLA	= TCA_SINGLE_CLKSEL_DIV2_gc			// Set F_TCA = F_CPU/2
						| TCA_SINGLE_ENABLE_bm;
}

int main(void)
{
	PORTA.DIRSET	= PIN0_bm;
	
	servo_setup();
	   
    while (1) 
    {
		servo_set(0);	// Servo 0%
		_delay_ms(500);
		
		servo_set(50);	// Servo 50%
		_delay_ms(500);
		
		servo_set(100);	// Servo 100%
		_delay_ms(500);
    }
}

