/**
 * @file isr.c
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
#include "isr.h"
#include "monitor.h"

char *interrupt_messages[] = {
	"Division by zero exception",
	"Debug exception",
	"Non maskable interrupt",
	"Breakpoint exception",
	"'Into detected overflow'",
	"Out of bounds exception",
	"Invalid opcode exception",
	"No coprocessor exception",
	"Double fault (check error code)",
	"Coprocessor segment overrun",
	"Bad TSS (check error code)",
	"Segment not present (check error code)",
	"Stack fault (pushes an error code)",
	"General protection fault (check error code)",
	"Page fault (check error code)",
	"Unknown interrupt exception",
	"Coprocessor fault",
	"Alignment check exception",
	"Machine check exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

// C Interrupt dispatcher
void isr_handler(registers_t regs)
{
	// This line is important. When the processor extends the 8-bit interrupt number
   // to a 32bit value, it sign-extends, not zero extends. So if the most significant
   // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
   u8int int_no = regs.int_no & 0xFF;
	if (interrupt_handlers[int_no] != 0)
	{
		isr_t handler = interrupt_handlers[int_no];
		handler(regs);
	}
	else
	{

		monitor_set_forecolor(e_color_magenta);
		monitor_write("Unhandled interrupt #");
		monitor_write_dec(int_no);
		monitor_put('\n');
		monitor_write("Message: ");
		if(int_no < 32){
			monitor_write(interrupt_messages[int_no - 1]);
			monitor_put('\n');
		}
		monitor_set_forecolor(e_color_white);

		// ERR!
		PANIC("Unhandled interrupt");
	}
}


// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
	else
	{
		// Unhandled IRQ, log it !
		monitor_set_forecolor(e_color_magenta);
		monitor_write("IRQ");
		monitor_write_hex(regs.int_no);
		monitor_write(" raised!\n");
		monitor_set_forecolor(e_color_white);
	}
}

void register_interrupt_handler(u8int n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}
