// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "monitor.h"
#include "timer.h"

int main(struct multiboot *mboot_ptr)
{
	init_descriptor_tables();

	monitor_clear();
	monitor_write("Hello, world!\n"); 
	
	//asm volatile ("int $0x3");
   //asm volatile ("int $0x4");

	// You got to re-enable interrupts
   //asm volatile("sti");
	//init_timer(50);

	initialise_paging();
   monitor_write("Hello, paging world!\n");

	u32int *ptr = (u32int*)0xA0000000;
   u32int do_page_fault = *ptr; // Page fault

   return 0;
	//return 0xDEADBABA;
}
