/*
 * ADC_With_Interrupt.cpp
 *
 * Created: 11/2/2024 7:22:32 PM
 * Author : RalvGSHolmsen
 */ 

/*
	The code shows how interrupts can be used to sample many different ADC channels in a sweeping
	fashion using interrupts. The result is a non-blocking code that has an adjustable sample rate.
	
	There are many other ways of solving this issue, and for simplicity the time between each
	channel is static. That means the timing is static and short, which can be a benefit. If it is 
	required that the channel sweep measures all channels as fast as possible (little time between
	channels), the code must be modified such that the sweep happens in the RESRDY ISR.
	
	As an example two channels are used - AIN6 and AIN7, which are connected to PD6 and PD7
*/

#define F_CPU 24000000

#include <avr/io.h>
#include <avr/interrupt.h>


#define NUMBER_OF_ADC_CHANNELS 2		// Number of ADC channels used (Number of analog inputs)
#define ADC_CHANNEL_SAMPLE_RATE 1000	// Sample rate per ADC channel

// Channels to sweep
volatile uint8_t adc_channel[NUMBER_OF_ADC_CHANNELS] = {
	ADC_MUXPOS_AIN6_gc,
	ADC_MUXPOS_AIN7_gc
};
// ADC conversion results for each channel
volatile uint16_t adc_results[NUMBER_OF_ADC_CHANNELS];
// Software result ready flag, and channel index
volatile uint8_t adc_flag = 0, adc_index = 0;


int main(void)
{
	// Set F_CPU to 24 MHz
	CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA	= CLKCTRL_FRQSEL_24M_gc;
	
	/*
		Setup TCB0 for periodic interrupt every 1 / (ADC_CHANNEL_SAMPLE_RATE * NUMBER_OF_ADC_CHANNELS) 
		such that every channel is sampled with sample rate ADC_CHANNEL_SAMPLE_RATE
	*/
	TCB0.CCMP			= (uint16_t)( ((float)F_CPU) / ((float)ADC_CHANNEL_SAMPLE_RATE * (float)NUMBER_OF_ADC_CHANNELS) - 0.5);
	TCB0.INTCTRL		= TCB_CAPT_bm;			// Enable interrupt
	TCB0.CTRLB			= TCB_CNTMODE_INT_gc;	// Periodic interrupt mode
	TCB0.CTRLA			= TCB_CLKSEL_DIV1_gc	// Prescaler = DIV1
						| TCB_ENABLE_bm;
	
	/*
		Setup ADC0 for use with interrupts
	*/
	VREF.ADC0REF		= VREF_REFSEL_2V048_gc;			// Internal reference of 2.048 V selected. Optionally: VREF_REFSEL_VDD_gc
	ADC0.MUXPOS			= adc_channel[adc_index];		// Select first channel
	ADC0.INTCTRL		= ADC_RESRDY_bm;				// Enable interrupt on result ready
	ADC0.CTRLC			= ADC_PRESC_DIV12_gc;			// F_CPU / 12 = 2 MHz (Max ADC clock frequency)
	ADC0.CTRLA			= ADC_RESSEL_12BIT_gc			// Ensure ADC is in 12 bit mode
						| ADC_CONVMODE_SINGLEENDED_gc	// Ensure ADC is in single-ended measurement mode
						| ADC_ENABLE_bm;
	
	/*
		Disable input buffer on analog inputs and ensure that they are inputs. The input buffer can
		add switching noise to the ADC measurement
	*/
	PORTD.DIRCLR		= PIN6_bm						// AIN6
						| PIN7_bm;						// AIN7
	PORTD.PIN6CTRL		= PORT_ISC_INPUT_DISABLE_gc;	// Disable input buffers to avoid switching noise
	PORTD.PIN7CTRL		= PORT_ISC_INPUT_DISABLE_gc;
	
	// Built-in LED as output (PC6 of curiosity nano)
	PORTC.DIRSET		= PIN6_bm;
	
	
	// Enable global interrupts
	sei();
	
    while (1) 
    {
		// Check if ADC result is ready (non-blocking)
		if (adc_flag)
		{
			adc_flag--; // Remove 1 inserted in interrupt
			
			// Dependent on which results is read
			switch(adc_flag)
			{
				case ADC_MUXPOS_AIN6_gc:
				// Dummy function, if below half of reference turn on LED
				if (adc_results[0] < 2048)
				{
					PORTC.OUTCLR = PIN6_bm;
				}
				else
				{
					PORTC.OUTSET = PIN6_bm;
				}
				break;
				
				case ADC_MUXPOS_AIN7_gc:
				// Do something with measurement
				break;
				
				
				// Illegal state, reset channel index
				default:
					adc_index = 0;
					break;
			}
			
			// Clear software flag
			adc_flag = 0;
		}
		
		// Do something else
		
    }
}


ISR(TCB0_INT_vect)
{
	// Clear interrupt flag
	TCB0.INTFLAGS = TCB_CAPT_bm;
	
	// Start conversion
	ADC0.COMMAND = ADC_STCONV_bm;
}

ISR(ADC0_RESRDY_vect)
{
	// Interrupt flag is cleared by hardware once RES is read (unique)
	adc_results[adc_index] = ADC0.RES;		// Store ADC result in global buffer
	adc_flag = adc_channel[adc_index];		// Doesn't protect unread flags!
	adc_flag++;								// Add 1 in case AIN0 was used (0x00)
	
	// Next channel logic
	if (adc_index < NUMBER_OF_ADC_CHANNELS-1)
	{
		adc_index++;
	}
	else
	{
		adc_index = 0;
	}
	
	// Set ADC MUXPOS (ADC channel) for next measurement
	ADC0.MUXPOS = adc_channel[adc_index];
}
