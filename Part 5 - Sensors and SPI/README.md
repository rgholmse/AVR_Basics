# Sensors and SPI
### Goals
 - Learn more about sensors
 - Do measurements from a simple temperature sensor
 - Learn what SPI is, and what it is used for
 - Learn to use SPI with AVRs

### Resources
 - [MCP9700 Datasheet](https://no.mouser.com/datasheet/2/268/MCP970X_Family_Data_Sheet_DS20001942-3443103.pdf)

### Printf with floating point (Microchip Studio)
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%205%20-%20Sensors%20and%20SPI/Pictures/printf1.png)
 1. Open Toolchain
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%205%20-%20Sensors%20and%20SPI/Pictures/printf2.png)
 2. Enable use of vprintf library
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%205%20-%20Sensors%20and%20SPI/Pictures/printf3.png)
 3. Include libprintf_flt.a
![Error](https://github.com/rgholmse/AVR_Basics/blob/main/Part%205%20-%20Sensors%20and%20SPI/Pictures/printf4.png)
 4. Use floats with printf!
  A) Size of integer
  B) Number of deicmals
  C) Print as float

### Printf with floating point (MPLAB X)
 1. XC8 - Already inclduded and optimized. No need to do anything
 2. AVR GCC - Enable vprintf library in Properties/Linker - Printing floats should now work

