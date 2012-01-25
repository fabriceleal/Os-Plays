// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"

extern u32int placement_address;
extern fs_node_t *fs_root;

u32int initial_esp;
u32int old_stack_pointer;
u32int old_base_pointer;
u32int initrd_location;
u32int initrd_end;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
	/* 
		Interruptors are disabled at this point
	*/
	
	// Save here the initial esp value
	initial_esp = initial_stack;

	// Initialize ISR's and segmentation
	init_descriptor_tables();
	// Initialize screen
	monitor_clear();

   // Initialise the PIT to 100Hz
   asm volatile("sti");
   init_timer(300); //(50);

	// Find location of original ramdisk
   ASSERT(mboot_ptr->mods_count > 0);
   initrd_location = *((u32int*)mboot_ptr->mods_addr);
   initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
   
	// Don't trample our module with placement accesses, please!
	// Do not overwrite module information! :D
   placement_address = initrd_end;
	
	// Initialize paging
	initialise_paging();

	// TODO: Create here snapshot of stack, send to initialise tasking ...
	{
		// ARGS
		monitor_write("Args size:");
		monitor_write_hex(sizeof(int) + sizeof(u32int));
		monitor_put('\n');

		asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
		asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer)); 
	
		// ESP
		monitor_write("(MAIN) esp = ");
		monitor_write_hex(old_stack_pointer);
		monitor_put('\n');

		// EBP
		monitor_write("(MAIN) ebp = ");
		monitor_write_hex(old_base_pointer);
		monitor_put('\n');

		// To copy
		monitor_write("(MAIN) to copy = ");
		monitor_write_hex(initial_esp-old_stack_pointer);
		monitor_put('\n');
	}
	// ***

   // Initialize multitasking.
   initialise_tasking();

	//PANIC("STOP");

   // Initialise the initial ramdisk, setting it as the filesystem root.
   fs_root = initialise_initrd(initrd_location);

	initialise_syscalls();

	switch_to_user_mode();

	syscall_monitor_write("Hello, user world!\n");
/*
	fork();

   asm volatile("cli");
	monitor_write("interrupts disabled\n");

	// Should hit twice, one for the parent and another for the child
	monitor_write("PID() = ");
   monitor_write_hex(getpid());
	monitor_put('\n');	

 	asm volatile("sti");
	monitor_write("interrupts enabled\n");
*/

	return 0;/*

	// TESTS ***
   // Create a new process in a new address space which is a clone of this.
	monitor_write("before fork\n");
   int ret = fork();

   monitor_write("fork() returned ");
   monitor_write_hex(ret);
   monitor_write(", and getpid() returned ");
   monitor_write_hex(getpid());
   monitor_write("\n============================================================================\n");

   // The next section of code is not reentrant so make sure we aren't interrupted during.
   asm volatile("cli");
	monitor_write("interrupts disabled\n");

   // list the contents of /
   int i = 0;
   struct dirent *node = 0;
   while ( (node = readdir_fs(fs_root, i)) != 0)
   {
       monitor_write("Found file ");
       monitor_write(node->name);
       fs_node_t *fsnode = finddir_fs(fs_root, node->name);

       if ((fsnode->flags&0x7) == FS_DIRECTORY)
       {
           monitor_write("\n\t(directory)\n");
       }
       else
       {
           monitor_write("\n\t contents: \"");
           char buf[256];
           u32int sz = read_fs(fsnode, 0, 256, buf);
           int j;
           for (j = 0; j < sz; j++)
               monitor_put(buf[j]);

           monitor_write("\"\n");
       }
       i++;
   }
   monitor_write("\n");
   asm volatile("sti");
	monitor_write("interrupts enabled\n");

	// ******
   return 0;*/
}
