/**
 * @file monitor.h
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * Functions for writing on the screen.
 *
 */


// VAMI stands for Validation Against Multiple Includes :)

#ifndef ___VAMI_MONITOR
#define ___VAMI_MONITOR

// A header file only declares the functions headers and structs used. 
// Implementations go into the .c file ;)
#include "common.h"

void monitor_put(char c);
void monitor_clear();
void monitor_write(char *c);


void monitor_write_hex(u32int value);
void monitor_write_dec(u32int value);

// TODO: This needs to be done, obviously this is not the signature of the printf function
void printf();


#endif // ___VAMI_MONITOR
