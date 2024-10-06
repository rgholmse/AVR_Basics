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
  
 4. Select GCC C++ Executable Project
 5. Name the project
 6. Select a location to save the project
 7. Press OK
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject3.png)  
  
 9. Use the search bar in the top right to search for "AVR128DA48"
 10. Select "AVR128DA48"
 11. Press OK
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject4.png)  
  
 13. Create your code
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject5.png)  
  
 15. Press the hammer icon
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MicrochipNewProject6.png)  
  
 17. First select the embedded debugger for the AVR128DA48 Curiosity Nano development board (1)
 18. Then build the project and program the device without debugging by pressing the green play button (2)
 19. The built-in LED on the development board should now blink!
 20. Since the Program is uploaded to the internal flash memory, the microcontroller will start blinking every time power is applied. Try removing the USB cable from the development board and reconnecting it
  
  
#### MPLAB X
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject1.png)  
  
 1. Start MPLAB X
 2. Select File/New Project
 3. Select Standalone Project
 4. Press Next
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject2.png)  
  
 5. Start writing "AVR128DA48" in the Device field
 6. "AVR128DA48" should appear, select it
 7. Press Next
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject3.png)  
  
 8. Select compiler (Either XC8 or AVR-GCC). AVR-GCC is preferred if available
 9. Press Next
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject4.png)  
  
 10. Name the project
 11. Select the project location
 12. Press Finish
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject5.png)  
  
 13. Right click on the new project
 14. Select New/avr-main.c (Alternatively "C Main File")
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject6.png)  
  
 16. Name the file "main"
 17. Press Finish
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject7.png)  
  
 18. Write your code
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject8.png)  
  
 19. Right click on your project
 20. Select Properties
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject9.png)  
  
 21. Select the AVR128DA48 Curiosity Nano board
 22. Press Apply
 23. Press OK
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%201%20-%20Introduction%20to%20Microcontrollers/Pictures/MPLAB%20NewProject10.png)  
  
 24. Program the device
 25. The built-in LED on the development board should now blink!
 26. Since the Program is uploaded to the internal flash memory, the microcontroller will start blinking every time power is applied. Try removing the USB cable from the development board and reconnecting it

