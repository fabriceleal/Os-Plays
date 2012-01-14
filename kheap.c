/**
 * @file kheap.c
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

#include "common.h"
#include "kheap.h"

// end is defined in the linker script.
extern u32int end;
u32int placement_address = (u32int)&end;

u32int kmalloc(u32int sz)
{
	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

u32int kmalloc_a(u32int sz)
{
	if(placement_address & 0xFFFFF000)
	{
   	placement_address &= 0xFFFFF000;
   	placement_address += 0x1000;
	}
	// begin kmalloc
	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
	// end kmalloc
}

u32int kmalloc_p(u32int sz, u32int *phys)
{
	if (phys)
	{
		*phys = placement_address;
	}
	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

u32int kmalloc_ap(u32int sz, u32int *phys)
{
	if (placement_address & 0xFFFFF000) // If the address is not already page-aligned
	{
		// Align it.
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
	{
		*phys = placement_address;
	}
	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}
