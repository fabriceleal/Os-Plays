/**
 * @file common.h
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * Some base typedefs and functions to write to ports and write to memory.
 *
 */


// VAMI stands for Validation Against Multiple Includes :)

#ifndef ___VAMI_COMMON
#define ___VAMI_COMMON

// A header file only declares the functions headers and structs used. 
// Implementations go into the .c file ;)

// typedefs to standardise cross-platform-ness.
// this typedefs are for x86

// TODO: Add here check-and-definition by processor architecture
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

void memcpy(void * origin, void * destiny, u32int length);
void memset(void * origin, u32int length, u32int new_value);

#endif // ___VAMI_COMMON

