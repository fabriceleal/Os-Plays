// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "monitor.h"

int main(struct multiboot *mboot_ptr)
{
	init_descriptor_tables();

	monitor_clear();
	monitor_write("Hello, world!\n"); 
	
	asm volatile ("int $0x3");
   asm volatile ("int $0x4");

	return 0xDEADBABA;
}
