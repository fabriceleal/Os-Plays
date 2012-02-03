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
 * based on http://www.osdever.net/bkerndev/Docs/keyboard.htm
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

	// TODO: Use also this ...
	e_key_code key_code; 

	// TODO: Extend this to allow capitalization, extra chars (like AltGr+3 = £), etc ... and extra keys ! (like NumLockOff + NumPad7 = Home)
	char to_type; 

	// TODO: Add support to extra handlers ...
	// ...

} t_key_info;

typedef void (*keyboard_handler)(t_key_info);
void init_keyboard();

// TODO: Take off this from here, and put in .c
void keyboard_int_handler(registers_t r);

#endif // ___VAMI_KEYBOARD
