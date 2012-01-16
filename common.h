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

void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);

int strcmp(char *str1, char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

#endif // ___VAMI_COMMON

