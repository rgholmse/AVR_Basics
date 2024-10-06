/*
 * Solution for part 2
 * 
 * Description:
 * The AVR is connected to a push button and a light emitting diode. The AVR
 * shall turn the LED on when the button is pressed and turn it off when the
 * button is not pressed. The button is connected between PC7 and GND, while the 
 * LED is connected between PC6 and VDD. The internal pull-up resistor shall be
 * enabled on PC7 to give a default value when the button is not pressed
 * 
 * Hardware
 * Pin  Name    Description
 * PC6  LED0    LED0 is active when logic 0
 * PC7  SW0     Button connects to logic 0, must have internal pull-up enabled
 */

#include <avr/io.h>

int main(void) {
    // Enable internal pull-up on PC7
    PORTC.PIN7CTRL  = PORT_PULLUPEN_bm;
    
    // Make PC7 input (optional as GPIO are input by default)
    PORTC.DIRCLR    = PIN7_bm;
    
    // Make PC6 output
    PORTC.DIRSET    = PIN6_bm;
    
    
    while (1) 
    {
        // Check if !(PC7 is logic 1))
        if (!(PORTC.IN & PIN7_bm))
        {
            // Button is logic 0 and is therefore pressed - LED ON
            PORTC.OUTCLR = PIN6_bm; // Remember LED is active LOW
        }
        else
        {
            // Button is logic 1 and is therefore not pressed - LED OFF
            PORTC.OUTSET = PIN6_bm; // Remember LED is active LOW
        }
    }
}
