/*
	SPI Sensor.cpp
	Created: 10/21/2024 11:50:09 AM
	Author : RalvGSHolmsen
	
	Description:
	This program is meant to accompany part 5 of the course. The code makes an ATtiny404
	a slave peripheral to a SPI master. The "sensor" doesn't actually do any measurements,
	but returns the same value that was sent to it in the next transaction
	
	Hardware:
	Phy	Pin	Name	Description
	1	VDD
	14	GND
	11	PA1	MOSI	SPI Master Out Slave In
	12	PA2	MISO	SPI Master In Slave Out
	13	PA3	SCK		SPI Serial ClocK
	2	PA4	SS		SPI Slave Select
*/

#define F_CPU 10000000

#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
	// Setup F_CPU to run at 10 MHz -- SPI max CLK frequency is F_CPU / 2 = 5 MHz!
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB	= CLKCTRL_PDIV_2X_gc	// F_CPU prescaler DIV 2
						| CLKCTRL_PEN_bm;		// F_CPU prescaler enable
	
	// Make MISO an output
	PORTA.DIRSET		= PIN2_bm;
	
	// Disable input buffer so it doesn't affect ADC measurements (optional)
	PORTB.PIN1CTRL		= PORT_ISC_INPUT_DISABLE_gc;
	
	PORTA.PIN4CTRL		= PORT_PULLUPEN_bm;		// Enable internal pull-up on SS pin in case no master is connected
	
	// Setup SPI
	SPI0.INTCTRL		= SPI_IE_bm;				// SPI Interrupt Enable (not in buffer mode)
	SPI0.CTRLB			= SPI_MODE_0_gc;			// Use SPI mode 0
	SPI0.CTRLA			= SPI_ENABLE_bm;
	
	// Enable interrupts
	sei();
	
    while (1) 
    {
		// Do nothing
    }
}



ISR(SPI0_INT_vect)
{
	// Interrupt flag is cleared on ISR completion (unique feature)
	
	// Store received data
	uint8_t spi_data = SPI0.DATA;
	
	// Update transmit data to be received data
	SPI0.DATA = spi_data;
}
