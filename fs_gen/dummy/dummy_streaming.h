/**
 * @file dummy_streaming.h
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


#ifndef ___VAMI_DUMMY_STREAMING
#define ___VAMI_DUMMY_STREAMING

#define BUFFER_SUCCESS 0
#define BUFFER_NULL_PTR 1
#define BUFFER_INVALID_PARAMETER 2
#define BUFFER_NO_MEM 3

#define LOG_S(format) printf(format)
#define LOG(format,...) printf(format, __VA_ARGS__ )

typedef struct {	
	int len;
	int buffer_size;
	char * buffer;
} buffer;


int init(buffer * obj);
int write(buffer * dest, const char * write, const unsigned int len);
int destroy(buffer * obj);

int tests();

#endif // ___VAMI_DUMMY_STREAMING
