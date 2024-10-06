/*
 * Part 3 Solution for ADC
 * 
 * Description:
 * The AVR is connected to a potentiometer. The potentiometer is connected to
 * VDD/VTG, GND, and PD7 (AIN7), and is connected to the embedded debugger via
 * PC0 and PC1 (UART). The AVR prints the result of a blocking ADC measurement
 * every 500 ms and prints the result via UART to TeraTerm
 * 
 * Hardware:
 * Pin  Name    Description
 * PC0  TXD     TXD pin for USART1 - Physically connected to embedded debugger
 * PC1  RXD     RXD pin for USART1 - Physically connected to embedded debugger
 * PD7  AIN7    Connected to the potentiometer to measure analog voltage
 */

#define F_CPU 4000000
#define BAUD(BAUD_RATE) ( (uint16_t)((64.0 * (float)F_CPU) / (16.0 *(float)BAUD_RATE) + 0.5) )

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


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

int main(void) {
    // Enable USART1 transmitter
    USART1.CTRLB    = USART_TXEN_bm;
    
    // Configure the baud rate of USART1
    USART1.BAUD     = BAUD(9600);
    
    // Make PC0 an output
    PORTC.DIRSET    = PIN0_bm;
    
    // Make PC1 an input (optional as it is an input by default)
    PORTC.DIRCLR    = PIN1_bm;
    
    // Make PD7 input (optional as it is an input by default)
    PORTD.DIRCLR    = PIN7_bm;
    
    // Connect ADC to PD7 (AIN7)
    ADC0.MUXPOS     = ADC_MUXPOS_AIN7_gc;
    
    // Set the ADC clock divider (Max frequency for ADC is 2 MHz [F_CPU / DIV])
    ADC0.CTRLC      = ADC_PRESC_DIV2_gc;
    
    // Enable ADC, default config = single ended, 12b mode
    ADC0.CTRLA      = ADC_ENABLE_bm;
    
    uint16_t adc_result;
    
    while (1) 
    {
        // Start ADC measurement
        ADC0.COMMAND = ADC_STCONV_bm;
        
        // Wait for ADC measurement to be complete (blocking)
        while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
        
        // Store ADC result in a variable
        adc_result = ADC0.RES;
        
        // Print the ADC result
        cdc_print("ADC Result = %d\r\n", adc_result);
        _delay_ms(500);
    }
}
