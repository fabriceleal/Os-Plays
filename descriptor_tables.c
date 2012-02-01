/**
 * @file descriptor_tables.c
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
#include "descriptor_tables.h"
#include "isr.h"

// Lets us access our ASM functions from our C code.
extern void gdt_flush(u32int);

extern void idt_flush(u32int);

extern void tss_flush();


// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32int,u32int,u32int,u8int,u8int);

static void init_idt();
static void idt_set_gate(u8int,u32int,u16int,u8int);

static void write_tss(s32int,u16int,u32int);

extern isr_t interrupt_handlers[];

/*
x86 segments:
cs (code segment), ds (data segment), es (extra segment), fs, gs, ss (stack segment)
*/
// At least 6, the last one NULL, or bad things will happen
#define SIZE_GDT 6
#define SIZE_IDT 256

#define GDT_NULL 0
#define GDT_KERNEL_CODE 1
#define GDT_KERNEL_DATA 2
#define GDT_USER_CODE 3
#define GDT_USER_DATA 4

/*
This function takes an index into the gdt_entries array, and gets the offset of the entry, relatively from the base
*/
#define tr_gdt_entry_to_offset(index) ((u16int) (index * sizeof(gdt_entry_t)))

gdt_entry_t gdt_entries[ SIZE_GDT ]; 
gdt_ptr_t   gdt_ptr; 

idt_entry_t idt_entries[ SIZE_IDT ];
idt_ptr_t   idt_ptr;

tss_entry_t tss_entry;

// initialises the GDT and IDT.
void init_descriptor_tables()
{
   // Initialise the global descriptor table
   init_gdt();

	// Initialise and null'ify the interrupt descriptor table
	init_idt();
	memset(&interrupt_handlers, 0, sizeof(isr_t)* SIZE_IDT );
}

static void init_gdt()
{	
   gdt_ptr.limit = (sizeof(gdt_entry_t) * SIZE_GDT );
   gdt_ptr.base  = (u32int)&gdt_entries;

	// Null segment (0x0), just because the whiny processor needs it
   gdt_set_gate( e_entry_present_no, 0, 0, 0, 0);

   gdt_set_gate(1, 0, 0xFFFFFFFF, 
			gdt_make_access(
					e_entry_present_yes, 
					e_ring_0, 
					e_descriptor_type_non_system, 
					e_segment_type_code), 
			gdt_make_granularity(
					e_granularity_kbyte_1, 
					e_operand_size_byte_32, 
					0x0));
	// Kernel mode code segment (at 0x8)

	// **
   gdt_set_gate(2, 0, 0xFFFFFFFF, 
			gdt_make_access(
					e_entry_present_yes, 
					e_ring_0, 
					e_descriptor_type_non_system, 
					e_segment_type_data), 
			gdt_make_granularity(
					e_granularity_kbyte_1, 
					e_operand_size_byte_32, 
					0x0)); 
	// Kernel mode data segment (0x10)

	// **
   gdt_set_gate(3, 0, 0xFFFFFFFF, 
			gdt_make_access(
					e_entry_present_yes, 
					e_ring_3, 
					e_descriptor_type_non_system, 
					e_segment_type_code), 
			gdt_make_granularity(
					e_granularity_kbyte_1, 
					e_operand_size_byte_32, 
					0x0));
	// User mode code segment (0x18)

	// **
   gdt_set_gate(4, 0, 0xFFFFFFFF, 
			gdt_make_access(
					e_entry_present_yes, 
					e_ring_3, 
					e_descriptor_type_non_system, 
					e_segment_type_data), 
			gdt_make_granularity(
					e_granularity_kbyte_1, 
					e_operand_size_byte_32, 
					0x0));
	// User mode data segment (0x20)

	/*

	// Some debugging code ...

	monitor_write_dec(gdt_make_access(e_segment_present_yes, e_ring_0, e_descriptor_type_non_system, e_segment_type_code));
	monitor_put('\n');
	monitor_write_dec(gdt_make_access(e_segment_present_yes, e_ring_0, e_descriptor_type_non_system, e_segment_type_data));
	monitor_put('\n');
	monitor_write_dec(gdt_make_access(e_segment_present_yes, e_ring_3, e_descriptor_type_non_system, e_segment_type_code));
	monitor_put('\n');
	monitor_write_dec(gdt_make_access(e_segment_present_yes, e_ring_3, e_descriptor_type_non_system, e_segment_type_data));
	monitor_put('\n');
	monitor_write_dec(gdt_make_granularity(e_granularity_kbyte_1, e_operand_size_byte_32, 0x0));
	monitor_put('\n');

	ASSERT(gdt_make_access(e_segment_present_yes, e_ring_0, e_descriptor_type_non_system, e_segment_type_code) == 0x9A); //, "Err!(1)...");
	ASSERT(gdt_make_access(e_segment_present_yes, e_ring_0, e_descriptor_type_non_system, e_segment_type_data) == 0x92); //, "Err!(2)...");
	ASSERT(gdt_make_access(e_segment_present_yes, e_ring_3, e_descriptor_type_non_system, e_segment_type_code) == 0xFA); //, "Err!(3)...");
	ASSERT(gdt_make_access(e_segment_present_yes, e_ring_3, e_descriptor_type_non_system, e_segment_type_data) == 0xF2); //, "Err!(4)...");
	ASSERT(gdt_make_granularity(e_granularity_kbyte_1, e_operand_size_byte_32, 0x0) == 0xC0); //, "Err!(5)...");

	*/

	write_tss(5, 0x10, 0x0); // (0x28)
	
   gdt_flush((u32int)&gdt_ptr);
	tss_flush();
}

// Set the value of one GDT entry.
static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
	// Weird endianess ... 

   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F; // The lower 4 bits of granularity are the 19:16 bits of the limit :S

   gdt_entries[num].granularity |= gran & 0xF0; // Only use upper 4 bits
   gdt_entries[num].access      = access;
} 

static void init_idt()
{
   idt_ptr.limit = sizeof(idt_entry_t) * SIZE_IDT;
   idt_ptr.base  = (u32int)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t)*SIZE_IDT);

	// Remap PICs
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	ASSERT( idt_make_flags(e_entry_present_yes, e_ring_0) == 0x8E );
	ASSERT( tr_gdt_entry_to_offset( GDT_KERNEL_CODE ) == 0X08 );

	// Set handlers for interrupts
   idt_set_gate( 0, (u32int)isr0   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 1, (u32int)isr1   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 2, (u32int)isr2   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 3, (u32int)isr3   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 4, (u32int)isr4   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 5, (u32int)isr5   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 6, (u32int)isr6   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 7, (u32int)isr7   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 8, (u32int)isr8   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate( 9, (u32int)isr9   , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(10, (u32int)isr10  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(11, (u32int)isr11  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(12, (u32int)isr12  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(13, (u32int)isr13  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(14, (u32int)isr14  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(15, (u32int)isr15  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(16, (u32int)isr16  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(17, (u32int)isr17  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(18, (u32int)isr18  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(19, (u32int)isr19  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(20, (u32int)isr20  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(21, (u32int)isr21  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(22, (u32int)isr22  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(23, (u32int)isr23  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(24, (u32int)isr24  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(25, (u32int)isr25  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(26, (u32int)isr26  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(27, (u32int)isr27  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(28, (u32int)isr28  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(29, (u32int)isr29  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(30, (u32int)isr30  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(31, (u32int)isr31  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(128, (u32int)isr128, tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));

	// Set handlers for irqs
   idt_set_gate(32, (u32int)irq0  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(33, (u32int)irq1  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(34, (u32int)irq2  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(35, (u32int)irq3  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(36, (u32int)irq4  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(37, (u32int)irq5  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(38, (u32int)irq6  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(39, (u32int)irq7  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(40, (u32int)irq8  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(41, (u32int)irq9  , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(42, (u32int)irq10 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(43, (u32int)irq11 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(44, (u32int)irq12 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(45, (u32int)irq13 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(46, (u32int)irq14 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));
   idt_set_gate(47, (u32int)irq15 , tr_gdt_entry_to_offset(GDT_KERNEL_CODE) , idt_make_flags(e_entry_present_yes, e_ring_0));

   idt_flush((u32int)&idt_ptr);
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;

   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
} 

// Initialise our task state segment structure.
static void write_tss(s32int num, u16int ss0, u32int esp0)
{
   // Firstly, let's compute the base and limit of our entry into the GDT.
   u32int base = (u32int) &tss_entry;
	
	u32int limit = base + sizeof(tss_entry);
	
	// Accordingly to (http://forum.osdev.org/viewtopic.php?t=22825&p=184330)
   //u32int limit = base + sizeof(tss_entry) - 1;

   // Now, add our TSS descriptor's address to the GDT.
   gdt_set_gate(num, base, limit, 0xE9, 0x00);
	//PANIC("After set gate");

   // Ensure the descriptor is initially zero.
   memset(&tss_entry, 0, sizeof(tss_entry));
	//PANIC("After memset");

   tss_entry.ss0  = ss0;  // Set the kernel stack segment.
   tss_entry.esp0 = esp0; // Set the kernel stack pointer.

   // Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
   // segments should be loaded when the processor switches to kernel mode. Therefore
   // they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
   // but with the last two bits set, making 0x0b and 0x13. The setting of these bits
   // sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
   // to switch to kernel mode from ring 3.

	// Accordingly to (http://f.osdev.org/viewtopic.php?t=20036&p=156804)
	tss_entry.cs   = 0x08;
   tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10;
   
	//tss_entry.cs   = 0x0b;
   //tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
	//PANIC("After set other segments");
}

void set_kernel_stack(u32int stack)
{
   tss_entry.esp0 = stack;
} 

// For now, you can't use this outside this source code file
// If you really want it, it might be better putting it in a 
// exported function (rather than extern'ing the gdt_entries 
// in every place ....)
#undef tr_gdt_entry_to_offset
