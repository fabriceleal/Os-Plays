#include <stdio.h>
#include "dummy_streaming.h"
#include "dummy_fs.h"


int main(int argc, char * argv[])
{
	printf("Hello from Dummy File System Generator\n");

	// Receive model path from command line args
	if (argc != 3)
	{
		printf("Usage: dummy_fs <model_path> <output_file>\n");
		return 1;
	}

	return generate_dummy_fs( argv[1], argv[2] );
}
