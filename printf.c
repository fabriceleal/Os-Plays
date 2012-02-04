/**
 * @file printf.c
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * < ... goes here ... >
 *
 */

#include "monitor.h"
#include "printf.h"


// Simple stateful iterator

typedef struct char_iterator
{
	const char* buffer;
	int i;
} char_iterator;

/*
 * Advances the cursor and returns the char at the new point
 */
char next(char_iterator *it)
{	
	return (it->buffer[++it->i]);
}

/*
 * Returns the char at the current point
 */
char current(char_iterator *it)
{
	return (it->buffer[it->i]);
}

// **

typedef void (*char_processing_func) (char * bf, char dump);

void format_specifier_processing(char * bf, char_iterator *it);
void regular_char_processing(char * bf, char_iterator *it);

void format_int(char * bf, int value, char * formatting_flags, ...);
void format_hex(char * bf, u32int value, char * formatting_flags, ...);

// **

void sprintf(const char* format, ...)
{
	char read = 0;
	char_iterator it = {
			.buffer = format,
			.i = -1
		};

	while(next(&it))
	{
		switch(current(&it))
		{
			case '%':
				// Obviously, the extra args should be passed to this call
				format_specifier_processing(0x0, &it);
				break;
			default:		
				regular_char_processing(0x0, &it);				
		}
	}
}

void format_specifier_processing(char * bf, char_iterator *it)
{
	// ** Currently at the % that raised this function **

	// Read again
	char read = next(it);

	if(read == '%')
	{
		// Print it and return
		regular_char_processing(bf, it);
		return;
	}

	// Read possible flags, until the data type specifier (single letter)
	do{
		// TODO: ...

		// %[parameter][flags][width][.precision][length]type
		// parameter n$, n is the number of the parameter (base 1)
		// flags: '+',' ','-','#','0'
		// Width: decimal value
		// Precision: maximum number of chars up until truncation
		// Length: hh, h, l, ll, L, z, j, t, I, I32, I64, q
		// Type: (d, i), u, (f, F), (e, E), (g, G), (x, X), o, s, c, p, n
		
		// ** One must reach a 'type' char to end the reading of the specifier processing. **
		// Call the right formating function, then exit		
		switch(read)
		{
			case 'x':
			case 'X':
				format_hex(0x0, 0xDEADBEAF, "buffered_formatting");
				return;
			case 'd': 
			case 'i':
				format_int(0x0, (int) -123, "buffered_formatting");
				return;
		}
	}while(read = next(it));
}

// TODO: Make functions for writing floats, ints, longs, ... to string, that take a formatting and a value as parameters
// ...

/* 
	General useful macros. These macroes assume that you use a var named 'flags' for storing extra information
*/
#define TEST(bit_condition) (flags & (bit_condition))
#define SET(bit_condition)  flags |= bit_condition

#define ASCII_NBR(value_nbr) ((int)'0' - 0   + value_nbr)
#define ASCII_LTR(value_nbr) ((int)'A' - 0xA + value_nbr)

void format_hex(char * bf, u32int value, char * formatting_flags, ...)
{
	u32int mask  = 0xF0000000;
	u32int shift = 7*4; // (Number of zeroes at right of 'F' in mask) * 4

	#define ALREADY_FOUND_NON_ZERO 0x1
	#define IS_LAST                (mask == 0xF)

	u8int flags = 0x0;
	u8int value_read;

	monitor_write("0x");

	while( mask ){
		value_read = (mask & value) >> shift;

		if((value_read) && !(TEST( ALREADY_FOUND_NON_ZERO )) )
		{
			SET( ALREADY_FOUND_NON_ZERO );
		}

		// If already found a zero, one must print all values read
		// If is the last hex digit, print it anyways
		if( IS_LAST || TEST( ALREADY_FOUND_NON_ZERO ))
		{				
			switch(value_read)
			{
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
					monitor_put( ASCII_NBR(value_read) );
					break;
				// Else, A .. F
				default:
					monitor_put( ASCII_LTR(value_read) );
			}
		}
		mask >>= 4;
		shift -= 4;
	}

	#undef ALREADY_FOUND_NON_ZERO
	#undef IS_LAST
}

void format_int(char * bf, int value, char * formatting_flags, ...)
{
	// Assuming that int is ~-65537 < x < ~65536
	int mask = 10000;
	int value_read = 0;

	#define ALREADY_FOUND_NON_ZERO 0x1
	#define IS_POSITIVE_OR_ZERO    0x2
	#define IS_LAST                (mask == 1)

	u8int flags = 0x0;

	// Force to positive to simplify processing
	if(value >= 0)
	{
		SET(IS_POSITIVE_OR_ZERO);
	}
	else
	{
		value *= -1; // TODO: Is there a clever way to flip the signal (using bit ops)?
	}

	if(!TEST(IS_POSITIVE_OR_ZERO)){
		monitor_put('-');
	}

	while(mask)
	{
		// int-div here
		value_read = value / mask; 

		if(value_read && !(TEST(ALREADY_FOUND_NON_ZERO)))
		{
			SET(ALREADY_FOUND_NON_ZERO);
		}

		if(IS_LAST || TEST(ALREADY_FOUND_NON_ZERO))
		{
			monitor_put( ASCII_NBR(value_read) );
		}		
		
		value -= mask * value_read;
		mask /= 10;
	}

	#undef IS_LAST
	#undef IS_POSITIVE_OR_ZERO
	#undef ALREADY_FOUND_NON_ZERO
}

#undef ASCII_LTR
#undef ASCII_NBR

#undef SET
#undef TEST

void regular_char_processing(char * bf, char_iterator *it)
{
	monitor_put(current(it));
}
