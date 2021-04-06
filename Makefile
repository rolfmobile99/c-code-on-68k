# attempt to use Rim's Makefile for a simple c program that does not depend on libc
#
# -rolf  mar 2021
#
# hardware target: 68k nano board
#
# TODO:
# - 'printf' has been commented out!
# - implement either a minimal printf for strings, or low level putchar()
# - make it work on 68k nano board OR Tricky68k simulator
#
# - try -fomit-frame-pointer with gcc (should reduce code size)

TARGET = m68000

CFLAGS =-Wall -msoft-float -fomit-frame-pointer -Wuninitialized

ASFLAGS=-Felf -$(TARGET)
AS=vasmm68k_mot

all: demo1.run

simple2.run: simple2.c
	m68k-elf-gcc -$(TARGET) $(CFLAGS) -c $<
	m68k-elf-ld --defsym=_start=main -Ttext=0x2000 -Tdata=0x3000 -Tbss=0x4000 --section-start=.rodata=0x5000 simple2.o `m68k-elf-gcc -$(TARGET) -print-libgcc-file-name`
	m68k-elf-objcopy -I elf32-m68k -O srec a.out simple2.run

itoa1.s: itoa1.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<

itoa2.s: itoa2.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<

itoa3.s: itoa3.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<

simple1.s: simple1.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<

simple2.s: simple2.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<

factor1.s: factor1.c
	m68k-elf-gcc  -$(TARGET) $(CFLAGS) -nostdlib -nodefaultlibs -S $<


# rule for assembling standalone "asm" files using vasm
%.o : %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	$(RM) a.out simple2.o

