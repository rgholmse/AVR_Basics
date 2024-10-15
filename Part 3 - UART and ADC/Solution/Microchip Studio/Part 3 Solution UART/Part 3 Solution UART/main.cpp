/*
 * Part 3 Solution for UART
 * 
 * Description:
 * The AVR is connected to a personal computer via USB. The embedded debugger of
 * the AVR128DA48 Curiosity Nano is used to transmit data to the computer via
 * UART. On the computer TeraTerm is used to monitor the data using a baud rate
 * of 9600. The AVR counts and prints the count value every 500 ms
 * 
 * Hardware:
 * Pin  Name    Description
 * PC0  TXD     TXD pin for USART1 - Physically connected to embedded debugger
 * PC1  RXD     RXD pin for USART1 - Physically connected to embedded debugger
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
    
    uint8_t counter = 0;
    
    while (1) 
    {
        // Print and increment counter value
        cdc_print("Counting %d\r\n", counter++);
        _delay_ms(500);
    }
}