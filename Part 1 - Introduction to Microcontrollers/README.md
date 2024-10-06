# Introduction to Microcontrollers
### Goals
 - Understand what a microcontroller is
 - Know where microcontrollers are used
 - Know why microcontrollers are used
 - Know what an AVR is, and why it is used in this course
 - Understand project design flows
 - Blink a LED

### Solution
By cloning the github repository or by manually downloading/copying the contents of main.c the solution can be explored. To program the AVR connect the AVR128DA48 Curiosity Nano kit to your computer and use Microchip Studio or MPLAB X to upload the code. Alternatively the entire workflow can be seen for both Microchip Studio and MPLAB X in the following sections

#### Microchip Studio
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject1.png)
 1. Start Micrichip Studio
 2. Select File/New/Project
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject2.png)
 3. Select GCC C++ Executable Project
 4. Name the project
 5. Select a location to save the project
 6. Press OK
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject3.png)
 7. Use the search bar in the top right to search for "AVR128DA48"
 8. Select "AVR128DA48"
 9. Press OK
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject4.png)
 10. Create your code
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject5.png)
 11. Press the hammer icon
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject6.png)
 12. First select the embedded debugger for the AVR128DA48 Curiosity Nano development board (1)
 13. Then build the project and program the device without debugging by pressing the green play button (2)
 14. The built-in LED on the development board should now blink!
 15. Since the Program is uploaded to the internal flash memory, the microcontroller will start blinking every time power is applied. Try removing the USB cable from the development board and reconnecting it

#### MPLAB X
