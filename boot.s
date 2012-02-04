;
; boot.s -- Kernel start location. Also defines multiboot header.
; Based on Bran's kernel development tutorial file start.asm
;
; TODO: Awkward warning being thrown by ld ... if you comment the .bss section declaration, it goes away ... why?
; TODO: Refer to http://www.osdever.net/bkerndev/Docs/basickernel.htm,
; TODO: this might help with problems with the stack ...

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.
[GLOBAL start]                  ; Kernel entry point.
[EXTERN main]                   ; This is the entry point of our C code

start:
;	mov     esp, _kernel_stack    ; Setup new stack (Bran's start.asm)
	push    esp                   ; Needed for moving the stack successfully (http://www.jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html)
	push    ebx                   ; Load multiboot header location
											; Both this parameters will be sent to the main() function
	; Execute the kernel:
	cli                     	   ; Disable interrupts.
	call main							; call our main() function.
	jmp $                      	; Enter an infinite loop, to stop the processor
									      ; executing whatever rubbish is in the memory
									      ; after our kernel! 
ALIGN 4
mboot:
  dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
  dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct
   
  dd  mboot                     ; Location of this descriptor
  dd  code                      ; Start of kernel '.text' (code) section.
  dd  bss                       ; End of kernel '.data' section.
  dd  end                       ; End of kernel.
  dd  start                     ; Kernel entry point (initial EIP).

; As a stack growns downwards, one must first allocate the space,
; then declare the identifier (check Bran's start.asm)
SECTION .bss
	resb 8192                     ; Alloc 8 kB
_kernel_stack:


