# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o main.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector 
LDFLAGS=-Tlink.ld
ASFLAGS=-felf
OUTPUT=kernel

all: $(SOURCES) link 

# '-' at the beginning of the command supresses output
clean:
	-rm -f *.o $(OUTPUT) bochsout.txt

link:
	ld $(LDFLAGS) -o $(OUTPUT) $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $< 

