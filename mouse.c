/**
 * @file mouse.c
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * based in http://wiki.osdev.org/Mouse_Input
 * might be a little messy :S http://forum.osdev.org/viewtopic.php?t=10247
 * 
 */

#include "monitor.h"
#include "isr.h"
#include "common.h"
#include "mouse.h"


void mouse_int_handler(registers_t r);


void init_mouse()
{
	// You have to handle IRQ..., and listen at io port 0x60  - same as keyboard! >:(
	// TODO: Make this	
	//register_interrupt_handler(IRQ5, mouse_int_handler);
	
	//monitor_write("Installed!");
}

void mouse_int_handler(registers_t r)
{
	u8int scancode = inb(0x60);

	monitor_write("Entered!");
}

