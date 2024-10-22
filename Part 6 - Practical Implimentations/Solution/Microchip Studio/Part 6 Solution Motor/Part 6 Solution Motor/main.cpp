/*
 * Part 6 Solution Motor.cpp
 *
 * Created: 19.10.2024 17:10:03
 * Author : RalvGSHolmsen
 */ 
/*
	
	
	Hardware:
	Pin	Name	Description
	PA0	PWM		PWM to regulate motor speed
	PA1	DIR1	Motor direction control
	PC2	DIR2	Motor direction control (Must be opposite of DIR1)
	PD7	AIN7	ADC connected to potentiometer for speed control
	PC7	BTN		Button to switch motor direction
*/

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>

void motor_direction_set(uint8_t dir)
{
	if (dir)
	{
		// Clear first, then set
		PORTC.OUTCLR = PIN2_bm;
		PORTA.OUTSET = PIN1_bm;
	}
	else
	{
		// Clear first, then set
		PORTA.OUTCLR = PIN1_bm;
		PORTC.OUTSET = PIN2_bm;
	}
}

void motor_speed_set(uint16_t speed)
{
	// Guard for invalid speed setting
	if (speed > 4095)
	{
		speed = 4095;
	}
	
	// Update duty cycle
	TCA0.SINGLE.CMP0BUF = speed;
}

int main(void)
{
	uint8_t motor_dir = 0;
	uint16_t motor_speed = 2048;
	motor_direction_set(motor_dir);
	motor_speed_set(motor_speed);
	
	/*
		Setup GPIO
	*/
	PORTA.DIRSET		= PIN0_bm
						| PIN1_bm;
	PORTD.DIRCLR		= PIN7_bm;
	PORTC.DIRSET		= PIN2_bm;
	PORTC.DIRCLR		= PIN7_bm;
	PORTC.PIN7CTRL		= PORT_PULLUPEN_bm;
	
	/*
		Setup ADC
	*/
	VREF.ADC0REF		= VREF_REFSEL_VDD_gc;
	
	ADC0.MUXPOS			= ADC_MUXPOS_AIN7_gc;
	ADC0.CTRLC			= ADC_PRESC_DIV4_gc;
	ADC0.CTRLA			= ADC_ENABLE_bm;
	
	/*
		Setup TCA
	*/
	TCA0.SINGLE.CMP0	= 2048;
	TCA0.SINGLE.PER		= 4095;
	TCA0.SINGLE.CTRLB	= TCA_SINGLE_CMP0EN_bm
						| TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	TCA0.SINGLE.CTRLA	= TCA_SINGLE_ENABLE_bm;
	
    while (1) 
    {
		// Check button to see if we need to change direction
		if (!(PORTC.IN & PIN7_bm))
		{
			// Invert motor direction
			motor_dir = !motor_dir;
			motor_direction_set(motor_dir);
			
			// Simple button debounce
			_delay_ms(50);
		}
		
		// Measure potentiometer value and update motor speed
		ADC0.COMMAND = ADC_STCONV_bm;
		while(!(ADC0.INTFLAGS & ADC_RESRDY_bm));
		motor_speed = ADC0.RES;
		
		motor_speed_set(motor_speed);
    }
}

