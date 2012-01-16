// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "multiboot.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

int main(struct multiboot *mboot_ptr)
{
	init_descriptor_tables();
	monitor_clear();
	monitor_write("Hello, world!\n");;
	u32int a = kmalloc(8);
	initialise_paging();
	monitor_write("Hello, paging world!\n");
	u32int b = kmalloc(8);
	u32int c = kmalloc(8);
	monitor_write("a: ");
	monitor_write_hex(a);
	monitor_write(", b: ");
	monitor_write_hex(b);
	monitor_write("\nc: ");
	monitor_write_hex(c);

	kfree(c);
	kfree(b);
	u32int d = kmalloc(12);
	monitor_write(", d: ");
	monitor_write_hex(d); 

   return 0;
	//return 0xDEADBABA;
}
