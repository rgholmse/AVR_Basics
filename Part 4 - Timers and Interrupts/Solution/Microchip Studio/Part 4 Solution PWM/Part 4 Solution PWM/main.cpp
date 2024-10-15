/*
 * Part 4 Solution PWM.cpp
 *
 * Created: 13.10.2024 19:14:48
 * Author : RalvGSHolmsen
 */ 

/*

*/

#include <avr/io.h>

void servo_set(uint8_t duty_cycle)
{
	// Optional guard
	if (duty_cycle > 100)
	{
		duty_cycle = 100;
	}
	
	// Calculate compare value
	uint16_t cmp = duty_cycle * 20 + 2000;
	
	// Update CMPxBUF register. CMPx register us updated by buffer on overflow
	// This ensures that the pulse on time is always valid
	TCA0.SINGLE.CMP0BUF = cmp;
}

void servo_setup()
{
	// Solve TOP value for TCA @50 Hz
	TCA0.SINGLE.PER	= per;
}

int main(void)
{
    
    while (1) 
    {
    }
}

