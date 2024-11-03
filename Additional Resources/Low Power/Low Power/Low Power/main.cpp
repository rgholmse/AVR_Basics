/*
 * Low Power.cpp
 *
 * Created: 11/3/2024 5:24:11 PM
 * Author : RalvGSHolmsen
 */ 

/*
	This code is an example of how to make your microcontroller more power efficient. While
	it does not discuss all aspects of how to save power, it will focus on the primary 
	feature which is putting the CPU into different sleep modes. Depending on the sleep
	mode, the CPU and peipherals are shut down to save power. It is important that you
	enable some sort of wake-up function for the CPU such that the CPU doesn't shut down
	immediately and never wakes up again.
	
	The SLEEP instruction puts the CPU into one of three sleeep modes - IDLE, STANDBY,
	POWER DOWN. It is the SLPCTRL peripheral that controls which of these modes the CPU
	enters when the sleep instruction is given. The CPU can be woken by an interrupt.
	
	IDLE - All peripherals run, the CPU stops executing code
	STANDBY - Only peripherals where the RUNSTDBY bit is set is active, CPU is halted
	POWER DOWN - Only RTC (with RUNSTDBY), WDT, BOD, PORT are active, CPU is halted
	
	Only interrupts from a peripheral that is active may wake the CPU. In POWER DOWN only
	fully asynchronous peripherals may wake the processor. For GPIO only pin 2 and 6 for 
	each port is fully asynchronous.
	
	This code will show how the RTC peripheral can be used to wake up the CPU every 
	second to run the code in main.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


int main(void)
{
	// Make PC6 (LED) output
	PORTC.DIRSET	= PIN6_bm;
	
	/*
		Setup Real Time Counter (RTC) with Periodic InTerrupt (PIT) every 1 second
		which is 32768 cycles (f_RTC = 32768 Hz)
	*/
	RTC.CTRLA		= RTC_RUNSTDBY_bm;			// Make RTC peripheral run in standby (and power down)
	RTC.PITINTCTRL	= RTC_PI_bm;				// Enable PIT periodic interrupts
	RTC.PITCTRLA	= RTC_PERIOD_CYC32768_gc	// Periodic interrupts every 32768 cycle
					| RTC_PITEN_bm;				// Enable RTC PIT
	
	// Enable global interrupts
	sei();
	
	/*
		Setup for SLPCTRL
	*/
	uint8_t sleep_setup;
	sleep_setup		= SLPCTRL_SMODE_PDOWN_gc	// Sleep mode set to power down
					| SLPCTRL_SEN_bm;			// Enable sleep mode instruction
	
    while (1) 
    {
		// It is recommended that the SEN bit is set only when you intend to enter sleeep
		SLPCTRL.CTRLA = sleep_setup;
		
		// Sleep instruction. Once waken, the CPU will execute code after this instruction
		sleep_cpu();
		
		// It is recommended that you clear the SEN bit after leaving sleep mode
		SLPCTRL.CTRLA = 0x00;
		
		// Toggle LED to show that the CPU has waken up
		PORTC.OUTTGL = PIN6_bm;
    }
}

ISR(RTC_PIT_vect)
{
	// Clear interrupt flag, no other action needed
	RTC.PITINTFLAGS = RTC_PI_bm;
}
