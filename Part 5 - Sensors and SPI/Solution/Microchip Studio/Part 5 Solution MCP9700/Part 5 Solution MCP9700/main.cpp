/*
 * Part 5 Solution MCP9700.cpp
 *
 * Created: 21.10.2024 16:00:59
 * Author : RalvGSHolmsen
 */ 

/*
	
*/

#define F_CPU 4000000
#define BAUD(BAUD_RATE) ( (uint16_t)((64.0 * (float)F_CPU) / (16.0 *(float)BAUD_RATE) + 0.5) )

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


/*
	UART functions
*/
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


int main(void)
{
	// Enable USART1 transmitter
	USART1.CTRLB    = USART_TXEN_bm;
	
	// Configure the baud rate of USART1
	USART1.BAUD     = BAUD(9600);
	
	// Set the ADC reference to VDD
	VREF.ADC0REF	= VREF_REFSEL_VDD_gc;
	
	// Make PC0 an output
	PORTC.DIRSET    = PIN0_bm;
	
	// Make PC1 an input (optional as it is an input by default)
	PORTC.DIRCLR    = PIN1_bm;
	
	// Make PD7 input (optional as it is an input by default)
	PORTD.DIRCLR    = PIN7_bm;
	
	// Connect ADC to PD7 (AIN7)
	ADC0.MUXPOS     = ADC_MUXPOS_AIN7_gc;
	
	// Set the ADC clock divider (Max frequency for ADC is 2 MHz [F_CPU / DIV]) - (optional as DIV2 is default setting)
	ADC0.CTRLC      = ADC_PRESC_DIV2_gc;
	
	// Enable ADC, default config = single ended, 12b mode
	ADC0.CTRLA      = ADC_ENABLE_bm;
	
    while (1) 
    {
		// Start conversion
		ADC0.COMMAND = ADC_STCONV_bm;
		
		// Wait for conversion to complete (blocking)
		while(!(ADC0.INTFLAGS & ADC_RESRDY_bm));
		
		// Get result
		uint16_t res = ADC0.RES;
		
		// Measure time for float function
		float t_float = (3.3*res - 2047.5) / 40.95;
		
		// Floating point printf library included in toolchain (libprintf_flt.a)
		cdc_print("Temperature: %3.1fC\r\n", t_float);
		_delay_ms(500);
    }
}

	