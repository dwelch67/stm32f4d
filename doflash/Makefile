
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : doflash.gcc.thumb.bin doflash.gcc.thumb2.bin doflash.clang.thumb.norm.bin doflash.clang.thumb.opt.bin doflash.clang.thumb2.norm.bin doflash.clang.thumb2.opt.bin

bintoc : bintoc.c
	gcc -o bintoc bintoc.c

flashme-bin.c : flashme-bin bintoc
	./bintoc flashme-bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

doflash.gcc.thumb.o : doflash.c flashme-bin.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c doflash.c -o doflash.gcc.thumb.o

doflash.gcc.thumb2.o : doflash.c flashme-bin.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c doflash.c -o doflash.gcc.thumb2.o

doflash.gcc.thumb.bin : memmap vectors.o doflash.gcc.thumb.o
	$(ARMGNU)-ld -o doflash.gcc.thumb.elf -T memmap vectors.o doflash.gcc.thumb.o
	$(ARMGNU)-objdump -D doflash.gcc.thumb.elf > doflash.gcc.thumb.list
	$(ARMGNU)-objcopy doflash.gcc.thumb.elf doflash.gcc.thumb.bin -O binary

doflash.gcc.thumb2.bin : memmap vectors.o doflash.gcc.thumb2.o
	$(ARMGNU)-ld -o doflash.gcc.thumb2.elf -T memmap vectors.o doflash.gcc.thumb2.o
	$(ARMGNU)-objdump -D doflash.gcc.thumb2.elf > doflash.gcc.thumb2.list
	$(ARMGNU)-objcopy doflash.gcc.thumb2.elf doflash.gcc.thumb2.bin -O binary


doflash.clang.bc : doflash.c flashme-bin.c
	clang $(LOPS) -c doflash.c -o doflash.clang.bc

doflash.clang.thumb.norm.bin : memmap vectors.o doflash.clang.bc
	llc $(LLCOPS) doflash.clang.bc -o doflash.clang.thumb.norm.s
	$(ARMGNU)-as doflash.clang.thumb.norm.s -o doflash.clang.thumb.norm.o
	$(ARMGNU)-ld -o doflash.clang.thumb.norm.elf -T memmap vectors.o doflash.clang.thumb.norm.o
	$(ARMGNU)-objdump -D doflash.clang.thumb.norm.elf > doflash.clang.thumb.norm.list
	$(ARMGNU)-objcopy doflash.clang.thumb.norm.elf doflash.clang.thumb.norm.bin -O binary

doflash.clang.thumb.opt.bin : memmap vectors.o doflash.clang.bc
	opt $(OOPS) doflash.clang.bc -o doflash.clang.thumb.opt.bc
	llc $(LLCOPS) doflash.clang.thumb.opt.bc -o doflash.clang.thumb.opt.s
	$(ARMGNU)-as doflash.clang.thumb.opt.s -o doflash.clang.thumb.opt.o
	$(ARMGNU)-ld -o doflash.clang.thumb.opt.elf -T memmap vectors.o doflash.clang.thumb.opt.o
	$(ARMGNU)-objdump -D doflash.clang.thumb.opt.elf > doflash.clang.thumb.opt.list
	$(ARMGNU)-objcopy doflash.clang.thumb.opt.elf doflash.clang.thumb.opt.bin -O binary

doflash.clang.thumb2.norm.bin : memmap vectors.o doflash.clang.bc
	llc $(LLCOPS2) doflash.clang.bc -o doflash.clang.thumb2.norm.s
	$(ARMGNU)-as doflash.clang.thumb2.norm.s -o doflash.clang.thumb2.norm.o
	$(ARMGNU)-ld -o doflash.clang.thumb2.norm.elf -T memmap vectors.o doflash.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D doflash.clang.thumb2.norm.elf > doflash.clang.thumb2.norm.list
	$(ARMGNU)-objcopy doflash.clang.thumb2.norm.elf doflash.clang.thumb2.norm.bin -O binary

doflash.clang.thumb2.opt.bin : memmap vectors.o doflash.clang.bc
	opt $(OOPS) doflash.clang.bc -o doflash.clang.thumb2.opt.bc
	llc $(LLCOPS2) doflash.clang.thumb2.opt.bc -o doflash.clang.thumb2.opt.s
	$(ARMGNU)-as doflash.clang.thumb2.opt.s -o doflash.clang.thumb2.opt.o
	$(ARMGNU)-ld -o doflash.clang.thumb2.opt.elf -T memmap vectors.o doflash.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D doflash.clang.thumb2.opt.elf > doflash.clang.thumb2.opt.list
	$(ARMGNU)-objcopy doflash.clang.thumb2.opt.elf doflash.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.bin.c
	rm -f flashme-bin.c
	rm -f bintoc


