/*
 * Solution.cpp
 *
 * Created: 10/30/2024 7:31:32 PM
 * Author : RalvGSHolmsen
 */ 

/*
	Phy	Pin	Name
	2	PC0	Motor direction A (not B)
	3	PC1	Motor direction B (not A)
	13	PD7	AIN7 Potentiometer for speed control
	14	AVDD Connected to battery positive
	15	GND Connected to battery negative
	19	UPDI For programming
	20	VDD Connected to battery positive
	21	GND
	22	PA0 PWM (TCA0) for speed control
	23	PA1 BTN to change direction
*/

#define F_CPU 24000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



void motor_direction_set(uint8_t dir)
{
	if (dir)
	{
		// Clear first, then set
		PORTC.OUTCLR = PIN0_bm;
		PORTC.OUTSET = PIN1_bm;
	}
	else
	{
		// Clear first, then set
		PORTC.OUTCLR = PIN1_bm;
		PORTC.OUTSET = PIN0_bm;
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
	
	// Disable IOREG protected registers for a few clock cycles
	CCP = CCP_IOREG_gc;
	
	// Set the clock frequency of the AVR to 24 MHz
	CLKCTRL.OSCHFCTRLA	= CLKCTRL_FRQSEL_24M_gc; // Protected register
	
	
    PORTA.DIRSET		= PIN0_bm;			// PWM for speed control
	PORTA.PIN1CTRL		= PORT_PULLUPEN_bm;	// Button for direction control
	
	// Set output first such that H-bridge is not on in invalid state
	motor_direction_set(motor_dir);
	motor_speed_set(motor_speed);
	PORTC.DIRSET		= PIN0_bm
						| PIN1_bm;
	
	/*
		Setup ADC
	*/
	VREF.ADC0REF		= VREF_REFSEL_VDD_gc;	// VDD as ADC reference
	
	ADC0.MUXPOS			= ADC_MUXPOS_AIN7_gc;	// PD7
	ADC0.CTRLC			= ADC_PRESC_DIV12_gc;	// 24 MHz / 12 = 2 MHz (max freq)
	ADC0.CTRLA			= ADC_ENABLE_bm;
	
	/*
		Setup TCA
	*/
	TCA0.SINGLE.PER		= 4095;					// Period
	TCA0.SINGLE.CTRLB	= TCA_SINGLE_CMP0EN_bm	// Waveform output enable
						| TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	TCA0.SINGLE.CTRLA	= TCA_SINGLE_ENABLE_bm 
						| TCA_SINGLE_CLKSEL_DIV8_gc; // DIV 8, motor dependent
	
    while (1) 
    {
		// Check button to see if we need to change direction
		if (!(PORTA.IN & PIN1_bm))
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

