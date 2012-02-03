/**
 * @file keyboard.h
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


#ifndef ___VAMI_KEYBOARD
#define ___VAMI_KEYBOARD

#include "isr.h"

// Scroll Lock
// Caps Lock
// Num Lock

// L Ctrl
// R Ctrl
// L Shift
// R Shift
// Alt
// Alt Gr
u32int modifiers;


typedef enum e_key_info_mode
{
	e_key_info_mode_not_typable = 0x0,
	e_key_info_mode_typable     = 0x1
} e_key_info_mode;

typedef enum e_key_code
{
	none_yet = 0x0
} e_key_code;

typedef struct t_key_info
{
	e_key_info_mode mode;

	char to_type;        // FIXME: Obsolete
	e_key_code key_code; // FIXME: Try to use this one instead ...
} t_key_info;

typedef void (*keyboard_handler)(t_key_info);
void init_keyboard();

void keyboard_int_handler(registers_t r);

#endif // ___VAMI_KEYBOARD
