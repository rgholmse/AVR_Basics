/*
 * ADCSelSampRateEvsys.cpp
 *
 * Created: 11/3/2024 12:05:34 PM
 * Author : RalvGSHolmsen
 */ 

/*
	This code shows how a timer peripheral can be used to control the sample rate of
	the ADC by starting a new conversion on the timer interrupt. Instead of using
	software interrupts to start a new ADC measurement, the event system is used to
	make this happen in hardware.
	
	In this example the ADC is connected to AIN7 (PD7) and the LED (PC6) is controlled
	by the value of the ADC result.
*/

#define F_CPU 24000000

#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC_SAMP_RATE 1000 // Sample rate in Hertz

volatile uint8_t adc_flag = 0;
volatile uint16_t adc_res;

int main(void)
{
	/*
		Set F_CPU to 24 MHz
	*/
	CCP = CCP_IOREG_gc;									// Unlock protected registers
	CLKCTRL.OSCHFCTRLA	= CLKCTRL_FRQSEL_24M_gc;		// Change OSCHF frequency to 24 MHz
	
	/*
		Setup GPIO
	*/
	ADC0.MUXPOS			= ADC_MUXPOS_AIN7_gc;			// Connect ADC to AIN7 (PD7)
	PORTD.DIRCLR		= PIN7_bm;						// Make sure PD7 is an input
	PORTC.DIRSET		= PIN6_bm;						// Make PC6 an output (LED)
	
	/*
		Setup TCB0 with periodic interrupt - Do not enable periodic interrupt as the event system will be
		used to start ADC conversions in hardware instead
	*/
	TCB0.CCMP			= (uint16_t)( ((float)F_CPU) / ((float)ADC_SAMP_RATE) - 0.5 );	// Calculate TOP value
	TCB0.CTRLB			= TCB_CNTMODE_INT_gc;			// Periodic interrupt mode
	TCB0.CTRLA			= TCB_CLKSEL_DIV1_gc			// Prescaler = DIV1
						| TCB_ENABLE_bm;
	
	/*
		Setup ADC0 such that a new conversion is started on TCB0 event (periodic event)
	*/
	VREF.ADC0REF		= VREF_REFSEL_2V048_gc;			// Select ADC reference - Internal 2.048 V reference used
	ADC0.EVCTRL			= ADC_STARTEI_bm;				// Enable conversion start on event
	ADC0.INTCTRL		= ADC_RESRDY_bm;				// Enable result ready interrupt
	ADC0.CTRLC			= ADC_PRESC_DIV12_gc;			// F_CPU / 12 = 2 MHz (Max ADC clock frequency)
	ADC0.CTRLA			= ADC_RESSEL_12BIT_gc			// ADC in 12 bit mode
						| ADC_CONVMODE_SINGLEENDED_gc	// ADC in single ended mode (Only MUXPOS used)
						| ADC_ENABLE_bm;
	
	/*
		Connect TCB0 and ADC0 in the event system
	*/
	EVSYS.CHANNEL0		= EVSYS_CHANNEL0_TCB0_CAPT_gc;	// Event system channel 0 generator is TCB0 CAPT event
	EVSYS.USERADC0START	= EVSYS_USER_CHANNEL0_gc;		// Connect ADC0START input to channel 0 (TCB0 CAPT)
	
	
	// Enable global interrupts
	sei();
	
    while (1) 
    {
		// Check if ADC measurement is complete (non-blocking)
		if (adc_flag)
		{
			adc_flag = 0;
			
			// Do something with ADC result - Example LED control
			if (adc_res < 2048)
			{
				// LED ON
				PORTC.OUTCLR = PIN6_bm;
			}
			else
			{
				// LED OFF
				PORTC.OUTSET = PIN6_bm;
			}
		}
		
		// Do something else..
    }
}


ISR(ADC0_RESRDY_vect)
{
	// Interrupt flag is cleared when reading RES (unique)
	adc_res = ADC0.RES;
	
	adc_flag = 1;
}

