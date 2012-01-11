/**
 * @file common.c
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

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
   u8int ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

u16int inw(u16int port)
{
   u16int ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
} 

void memcpy(void * origin, void * destiny, u32int length)
{
   // TODO: ...
   return;
}

void memset(void * origin, u32int length, u32int new_value)
{
   // TODO: ...
   return;
}

void monitor_write_hex(u32int n)
{
   // TODO: ...
	return;
}

void monitor_write_dec(u32int n)
{
   // TODO: ...
	return;
} 

void printf(){
	// Refer to vsprintf at the linux0.1 code
	// TODO: ...
	return;
}
