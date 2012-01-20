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
#include "stdlib.h"
#include "string.h"

/*
	Inits the buffer structure.
*/
int init(buffer * obj)
{
	if (obj == NULL)
	{
		return BUFFER_NULL_PTR;
	}

	obj->len = -1;
	obj->buffer_size = -1;
	obj->buffer = NULL;

	return BUFFER_SUCCESS;
}

/*
	Writes a array of chars into buffer. Allocates dynamically if necessary.
*/
int write(buffer * obj, const char * write, const unsigned int len)
{
	int current_end = -1;
	if(obj == NULL)
	{
		return BUFFER_NULL_PTR;
	}
	
	LOG( "Length = %d\n" , obj->len );

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

		LOG_S("new ptr alloc'ed\n");
		
		if(obj->buffer != NULL)
		{
			// Copy old data to new ptr
			memcpy( new_ptr, obj->buffer, obj->len );

			// Free old ptr
			free( obj->buffer );

			current_end = 0;
		}

		// Update structure
		obj->buffer = new_ptr;

		// In this implementation, buffer_size == data_len
		obj->len = obj->buffer_size = new_size;
	}

	// Append new data
	memcpy( obj->buffer + current_end, write, len);

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
