/*
 * Part 5 Solution MCP9700.cpp
 *
 * Created: 21.10.2024 16:00:59
 * Author : RalvGSHolmsen
 */ 

/*
	
*/

#include <avr/io.h>

int8_t mcp9700_naive_get()
{
	/*
		The use of floats makes this function inefficient as it is 
		called throughout the program. This is usually OK, but 
		sometimes we need functions to be efficient
		
		t = 0.02442 * (VREF * RES - 2047.5)
	*/
	float temperature;
	
	uint16_t res = ADC0.RES;
	temperature = 0.02442 * (3.3*res - 2047.5);
	
	return (int8_t) temperature;
}

int8_t mcp9700_get()
{
	/*
		This function makes use of integers to solve the same
		equation as above. Instead of using floating points we
		can either use fixed point arithmetic, or scale the
		values and round them to the nearest integer
		
		t = 0.02442 * (VREF * RES - 2047.5)
		t = 0.080586 * RES - 49.99995
		t = 10000/124091 * RES - 50
		t = 10000*RES / 124091 - 50
		
		RES can be in the range [0, 4095], and 10000*4095 = 1240910000
		log2(1240910000) is 30.2 so we need at least 31 bits to 
		represent the full scale. This is of course not considering
		that the maximum output code is 1.75/3.3 * 4095. This 
		only reduces the bitwidth with 1 bit and is therefore
		negligible. The closest type to 31 bit is int32_t and
		we must therefore use int32_t to do our first rescale.
		This is still more efficient that floating points.
		
		At the cost of resolution we could also have used 
		t = 10 / 124 * RES - 50
		which means we could have used int16_t instead if 
		measurement accuracy is not important
	*/
	int32_t temperature;
	
	temperature = (10000*ADC0.RES) / 124091 - 50;
	
	return (int8_t)temperature;
}

int main(void)
{

    while (1) 
    {
    }
}

