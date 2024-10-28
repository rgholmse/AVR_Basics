/*
 * Part 5 Solution SPI.cpp
 *
 * Created: 21.10.2024 13:46:10
 * Author : RalvGSHolmsen
 */ 

/*
	
	
	Hardware:
	Pin	Name
	PA4	MOSI
	PA5	MISO
	PA6	SCK
	PA7	SS
*/

#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

uint8_t spi_xfer(uint8_t data)
{
	// Start transaction
	SPI0.DATA = data;
	
	// Wait for transaction to complete
	while(!(SPI0.INTFLAGS & SPI_IF_bm));
	
	// Return received data
	return SPI0.DATA;
}


int main(void)
{
	// Setup GPIO
	PORTA.DIRSET	= PIN4_bm	// MOSI - Master Out Slave In
					| PIN6_bm	// SCK - Serial ClocK
					| PIN7_bm;	// SS - Slave Select (active LOW)
	PORTA.DIRCLR	= PIN5_bm;	// MISO - Master In Slave Out
	
	// Setup SPI
	SPI0.CTRLB		= SPI_SSD_bm		// Disable SS as input on SPI0 (Overrides master mode)
					| SPI_MODE_0_gc;	// SPI mode 0. Make sure this is _gc!
	SPI0.CTRLA		= SPI_MASTER_bm		// Make this device SPI master
					| SPI_PRESC_DIV4_gc	// F_CPU/4 = 1 MHz. For our slave max frequency is 5 MHz
					| SPI_ENABLE_bm;
	
	// Deselect salve
	PORTA.OUTSET	= PIN7_bm;
	
	// Make PC6 an output (LED)
	PORTC.DIRSET	= PIN6_bm;
	
	uint8_t rx, tx = 0;
	
    while (1) 
    {
		// Select slave
		PORTA.OUTCLR = PIN7_bm;
		
		// SPI transmit/receive
		rx = spi_xfer(tx++);
		
		// De-select slave
		PORTA.OUTSET = PIN7_bm;
		
		// LED == least significant bit
		if (rx & (1<<0)) {
			// LED ON
			PORTC.OUTCLR = PIN6_bm;
		}
		else {
			// LED OFF
			PORTC.OUTSET = PIN6_bm;
		}
		
		_delay_ms(500);
    }
}

