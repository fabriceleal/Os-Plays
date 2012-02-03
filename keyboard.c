/**
 * @file keyboard.c
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

#include "monitor.h"
#include "isr.h"
#include "common.h"
#include "keyboard.h"

#define define_key_info(k_mode,k_character) \
	{ \
		.mode    = k_mode ,\
		.to_type = k_character \
	}

/*
How to handle one key - several chars
*/

// US Layout :S as in http://www.osdever.net/bkerndev/Docs/keyboard.htm

t_key_info layout[128] = {
	define_key_info( e_key_info_mode_not_typable , 0 ),
	define_key_info( e_key_info_mode_not_typable , 27),

	define_key_info( e_key_info_mode_typable , '1'),
	define_key_info( e_key_info_mode_typable , '2'),
	define_key_info( e_key_info_mode_typable , '3'),
	define_key_info( e_key_info_mode_typable , '4'),
	define_key_info( e_key_info_mode_typable , '5'),
	define_key_info( e_key_info_mode_typable , '6'),
	define_key_info( e_key_info_mode_typable , '7'),
	define_key_info( e_key_info_mode_typable , '8'),
	define_key_info( e_key_info_mode_typable , '9'),
	define_key_info( e_key_info_mode_typable , '0'),

	define_key_info( e_key_info_mode_typable , '-'),
	define_key_info( e_key_info_mode_typable , '='),
	define_key_info( e_key_info_mode_typable , '\b'), // Backspace
	define_key_info( e_key_info_mode_typable , '\t'),


	define_key_info( e_key_info_mode_typable , 'q'),
	define_key_info( e_key_info_mode_typable , 'w'),
	define_key_info( e_key_info_mode_typable , 'e'),
	define_key_info( e_key_info_mode_typable , 'r'),
	define_key_info( e_key_info_mode_typable , 't'),
	define_key_info( e_key_info_mode_typable , 'y'),
	define_key_info( e_key_info_mode_typable , 'u'),
	define_key_info( e_key_info_mode_typable , 'i'),
	define_key_info( e_key_info_mode_typable , 'o'),
	define_key_info( e_key_info_mode_typable , 'p'),
	define_key_info( e_key_info_mode_typable , '['),
	define_key_info( e_key_info_mode_typable , ']'),
	define_key_info( e_key_info_mode_typable , '\n'), // Enter

	define_key_info( e_key_info_mode_not_typable , 0), // 29 - Control

	define_key_info( e_key_info_mode_typable , 'a'),
	define_key_info( e_key_info_mode_typable , 's'),
	define_key_info( e_key_info_mode_typable , 'd'),
	define_key_info( e_key_info_mode_typable , 'f'),
	define_key_info( e_key_info_mode_typable , 'g'),
	define_key_info( e_key_info_mode_typable , 'h'),
	define_key_info( e_key_info_mode_typable , 'j'),
	define_key_info( e_key_info_mode_typable , 'k'),
	define_key_info( e_key_info_mode_typable , 'l'),
	define_key_info( e_key_info_mode_typable , ';'),
	define_key_info( e_key_info_mode_typable , '\''),
	define_key_info( e_key_info_mode_typable , '`'),

	define_key_info( e_key_info_mode_typable , 0), // Left Shift

	define_key_info( e_key_info_mode_typable , '\\'),
	define_key_info( e_key_info_mode_typable , 'z'),
	define_key_info( e_key_info_mode_typable , 'x'),
	define_key_info( e_key_info_mode_typable , 'c'),
	define_key_info( e_key_info_mode_typable , 'v'),
	define_key_info( e_key_info_mode_typable , 'b'),
	define_key_info( e_key_info_mode_typable , 'n'),
	define_key_info( e_key_info_mode_typable , 'm'),
	define_key_info( e_key_info_mode_typable , ','),
	define_key_info( e_key_info_mode_typable , '.'),
	define_key_info( e_key_info_mode_typable , '/'),

	define_key_info( e_key_info_mode_not_typable , 0), // Right Shift

	define_key_info( e_key_info_mode_typable , '*'),

	define_key_info( e_key_info_mode_not_typable , 0), // Alt

	define_key_info( e_key_info_mode_typable , ' '),

	define_key_info( e_key_info_mode_not_typable , 0), // Caps Lock
	define_key_info( e_key_info_mode_not_typable , 0), // F1
	define_key_info( e_key_info_mode_not_typable , 0), // F2
	define_key_info( e_key_info_mode_not_typable , 0), // F3
	define_key_info( e_key_info_mode_not_typable , 0), // F4
	define_key_info( e_key_info_mode_not_typable , 0), // F5
	define_key_info( e_key_info_mode_not_typable , 0), // F6
	define_key_info( e_key_info_mode_not_typable , 0), // F7
	define_key_info( e_key_info_mode_not_typable , 0), // F8
	define_key_info( e_key_info_mode_not_typable , 0), // F9
	define_key_info( e_key_info_mode_not_typable , 0), // F10
	define_key_info( e_key_info_mode_not_typable , 0), // Num Lock
	define_key_info( e_key_info_mode_not_typable , 0), // Home Key
	define_key_info( e_key_info_mode_not_typable , 0), // Up Arrow
	define_key_info( e_key_info_mode_not_typable , 0), // Page Up

	define_key_info( e_key_info_mode_typable , '-'),

	define_key_info( e_key_info_mode_not_typable , 0), // Left Arrow
	define_key_info( e_key_info_mode_not_typable , 0), // ?:S
	define_key_info( e_key_info_mode_not_typable , 0), // Right Arrow

	define_key_info( e_key_info_mode_typable , '+'),

	define_key_info( e_key_info_mode_not_typable , 0), // End Key
	define_key_info( e_key_info_mode_not_typable , 0), // Down Arrow
	define_key_info( e_key_info_mode_not_typable , 0), // Page Down
	define_key_info( e_key_info_mode_not_typable , 0), // Insert Key
	define_key_info( e_key_info_mode_not_typable , 0), // Delete Key
	define_key_info( e_key_info_mode_not_typable , 0), // ?:S
	define_key_info( e_key_info_mode_not_typable , 0), // ?:S
	define_key_info( e_key_info_mode_not_typable , 0), // ?:S
	define_key_info( e_key_info_mode_not_typable , 0), // F11
	define_key_info( e_key_info_mode_not_typable , 0), // F12
	define_key_info( e_key_info_mode_not_typable , 0)  // ?:S (...)
};

keyboard_handler gp_kb_pressed_handler  = 0x0;
keyboard_handler gp_kb_released_handler = 0x0;

void init_keyboard()
{
	// Keyboard throws IRQ1 to signal that there is a key pressed / released
	register_interrupt_handler(IRQ1, keyboard_int_handler);
	monitor_write("Installed\n");
}

void keyboard_pressed_handler(t_key_info key_pressed)
{
	// ...
	// TODO: ...
	if(key_pressed.mode == e_key_info_mode_typable )
	{
		monitor_put(key_pressed.to_type);
	}
}

void keyboard_released_handler(t_key_info key_pressed)
{
	// ...
	// TODO: ...

}

// Low level dispatcher to keyboard_pressed_handler and keyboard_released_handler
void keyboard_int_handler(registers_t r)
{
	u8int scancode = inb(0x60);

	if(scancode & 0x80)
	{
		// Key was released
		// One can use this to unset "key-on" modifiers (shift's, control's, alt's)
		// TODO: ...
	}
	else
	{
		// Key was pressed
		keyboard_pressed_handler( layout[scancode] );
	}
}

void keyboard_send_command()
{
	// Wait until keyboard is not busy
	while((inb(0x64) & 2) != 0x0);
	
	// TODO: Send command here ...
	
	
}
