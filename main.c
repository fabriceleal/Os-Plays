// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"

extern u32int placement_address;

int main(struct multiboot *mboot_ptr)
{
	// Initialize ISR's and segmentation
	init_descriptor_tables();
	// Initialize screen
	monitor_clear();
	
	// Find location of original ramdisk
   ASSERT(mboot_ptr->mods_count > 0);
   u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
   u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
   // Don't trample our module with placement accesses, please!
   placement_address = initrd_end;

	// Initialize paging
	initialise_paging();

   // Initialise the initial ramdisk, setting it as the filesystem root.
   fs_root = initialise_initrd(initrd_location);

	// TESTS *****

	// list the contents of /
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
	  monitor_write("Found file ");
	  monitor_write(node->name);
	  fs_node_t *fsnode = finddir_fs(fs_root, node->name);

	  if ((fsnode->flags&0x7) == FS_DIRECTORY)
		 monitor_write("\n\t(directory)\n");
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

	// *****

   return 0;
}
