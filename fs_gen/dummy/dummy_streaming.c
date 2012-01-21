/**
 * @file dummy_streaming.c
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * To be used in filesystem generators, not by the os. This is "standard" C.
 *
 */

#include "dummy_streaming.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	Inits the buffer structure.
*/
int init(buffer * obj)
{
	if (obj == NULL)
	{
		return BUFFER_NULL_PTR;
	}

	obj->len = 0;
	obj->buffer_size = 0;
	obj->buffer = NULL;

	return BUFFER_SUCCESS;
}

/*
	Writes a array of chars into buffer. Allocates dynamically if necessary.
*/
int write(buffer * obj, const char * write, const unsigned int len)
{
	int current_end = 0;
	if(obj == NULL)
	{
		return BUFFER_NULL_PTR;
	}

	if( len == 0 || write == NULL )
	{
		return BUFFER_INVALID_PARAMETER;
	}

	current_end = obj->len;
	
	if(obj->buffer == NULL || len + obj->len > obj->buffer_size )
	{
		// New alloc is needed
		int new_size = (len + obj->len);
		char * new_ptr = malloc( sizeof(char) * new_size );

		if(new_ptr == NULL)
		{
			return BUFFER_NO_MEM;
		}

		LOG("new ptr alloc'ed, size = %d\n", new_size);
		
		if(obj->buffer != NULL)
		{
			LOG_S("copying old data\n");

			// Copy old data to new ptr
			memcpy( new_ptr, obj->buffer, obj->len );

			// Free old ptr
			free( obj->buffer );
		}
		LOG_S("updateing struct\n");

		// Update structure
		obj->buffer = new_ptr;

		// In this implementation, buffer_size == data_len
		obj->len = obj->buffer_size = new_size;
	}

	LOG("appending new data (%x, %d, %x) (%d)\n", (unsigned int)obj->buffer, current_end, (unsigned int)(obj->buffer + current_end), len);

	// Append new data
	memcpy( obj->buffer + current_end, write, len);

	LOG_S("end copying\n");
	
	return BUFFER_SUCCESS;
}

/*
	Releases de internal data inside the supplied buffer. 
	The pointer of the buffer itself needs to be free'd by the caller.
*/
int destroy(buffer * obj)
{
	if(obj == NULL)
	{
		return BUFFER_NULL_PTR;
	}

	if(obj->buffer != NULL)
	{
		free( obj->buffer );
	}

	obj->buffer = NULL;
	obj->buffer_size = 0;
	obj->len = 0;

	return BUFFER_SUCCESS;
}

int tests()
{
	int i = 0;
	buffer * test = malloc(sizeof(buffer));
	init(test);
	write(test, "hello", 5); // do not include \0
	write(test, " ", 1); // do not include \0
	write(test, "world", 6);

	// TODO: Pretty print, put this on function
	printf("\n");
	printf("Buffer = %x\n", (unsigned int)test->buffer);
	printf("Buffer-Size = %d\n", test->buffer_size);
	printf("Data-Len = %d\n", test->len);
	for(i = 0; i < test->len; ++i)
	{
		printf("Offset %d, value %x, char %c\n", i, test->buffer[i], test->buffer[i]);
	}
	printf("\n");

	return 0;
}
