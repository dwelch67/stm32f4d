
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : blinker03.gcc.thumb.bin blinker03.gcc.thumb2.bin blinker03.clang.thumb.norm.bin blinker03.clang.thumb.opt.bin blinker03.clang.thumb2.norm.bin blinker03.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker03.gcc.thumb.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker03.c -o blinker03.gcc.thumb.o

blinker03.gcc.thumb2.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker03.c -o blinker03.gcc.thumb2.o

blinker03.gcc.thumb.bin : memmap vectors.o blinker03.gcc.thumb.o
	$(ARMGNU)-ld -o blinker03.gcc.thumb.elf -T memmap vectors.o blinker03.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker03.gcc.thumb.elf > blinker03.gcc.thumb.list
	$(ARMGNU)-objcopy blinker03.gcc.thumb.elf blinker03.gcc.thumb.bin -O binary

blinker03.gcc.thumb2.bin : memmap vectors.o blinker03.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker03.gcc.thumb2.elf -T memmap vectors.o blinker03.gcc.thumb2.o
	$(ARMGNU)-objdump -D blinker03.gcc.thumb2.elf > blinker03.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker03.gcc.thumb2.elf blinker03.gcc.thumb2.bin -O binary


blinker03.clang.bc : blinker03.c
	clang $(LOPS) -c blinker03.c -o blinker03.clang.bc

blinker03.clang.thumb.norm.bin : memmap vectors.o blinker03.clang.bc
	llc $(LLCOPS) blinker03.clang.bc -o blinker03.clang.thumb.norm.s
	$(ARMGNU)-as blinker03.clang.thumb.norm.s -o blinker03.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker03.clang.thumb.norm.elf -T memmap vectors.o blinker03.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker03.clang.thumb.norm.elf > blinker03.clang.thumb.norm.list
	$(ARMGNU)-objcopy blinker03.clang.thumb.norm.elf blinker03.clang.thumb.norm.bin -O binary

blinker03.clang.thumb.opt.bin : memmap vectors.o blinker03.clang.bc
	opt $(OOPS) blinker03.clang.bc -o blinker03.clang.thumb.opt.bc
	llc $(LLCOPS) blinker03.clang.thumb.opt.bc -o blinker03.clang.thumb.opt.s
	$(ARMGNU)-as blinker03.clang.thumb.opt.s -o blinker03.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker03.clang.thumb.opt.elf -T memmap vectors.o blinker03.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker03.clang.thumb.opt.elf > blinker03.clang.thumb.opt.list
	$(ARMGNU)-objcopy blinker03.clang.thumb.opt.elf blinker03.clang.thumb.opt.bin -O binary

blinker03.clang.thumb2.norm.bin : memmap vectors.o blinker03.clang.bc
	llc $(LLCOPS2) blinker03.clang.bc -o blinker03.clang.thumb2.norm.s
	$(ARMGNU)-as blinker03.clang.thumb2.norm.s -o blinker03.clang.thumb2.norm.o
	$(ARMGNU)-ld -o blinker03.clang.thumb2.norm.elf -T memmap vectors.o blinker03.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D blinker03.clang.thumb2.norm.elf > blinker03.clang.thumb2.norm.list
	$(ARMGNU)-objcopy blinker03.clang.thumb2.norm.elf blinker03.clang.thumb2.norm.bin -O binary

blinker03.clang.thumb2.opt.bin : memmap vectors.o blinker03.clang.bc
	opt $(OOPS) blinker03.clang.bc -o blinker03.clang.thumb2.opt.bc
	llc $(LLCOPS2) blinker03.clang.thumb2.opt.bc -o blinker03.clang.thumb2.opt.s
	$(ARMGNU)-as blinker03.clang.thumb2.opt.s -o blinker03.clang.thumb2.opt.o
	$(ARMGNU)-ld -o blinker03.clang.thumb2.opt.elf -T memmap vectors.o blinker03.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D blinker03.clang.thumb2.opt.elf > blinker03.clang.thumb2.opt.list
	$(ARMGNU)-objcopy blinker03.clang.thumb2.opt.elf blinker03.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


