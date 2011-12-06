
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : blinker04.gcc.thumb.bin blinker04.gcc.thumb2.bin blinker04.clang.thumb.norm.bin blinker04.clang.thumb.opt.bin blinker04.clang.thumb2.norm.bin blinker04.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker04.gcc.thumb.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker04.c -o blinker04.gcc.thumb.o

blinker04.gcc.thumb2.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker04.c -o blinker04.gcc.thumb2.o

blinker04.gcc.thumb.bin : memmap vectors.o blinker04.gcc.thumb.o
	$(ARMGNU)-ld -o blinker04.gcc.thumb.elf -T memmap vectors.o blinker04.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker04.gcc.thumb.elf > blinker04.gcc.thumb.list
	$(ARMGNU)-objcopy blinker04.gcc.thumb.elf blinker04.gcc.thumb.bin -O binary

blinker04.gcc.thumb2.bin : memmap vectors.o blinker04.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker04.gcc.thumb2.elf -T memmap vectors.o blinker04.gcc.thumb2.o
	$(ARMGNU)-objdump -D blinker04.gcc.thumb2.elf > blinker04.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker04.gcc.thumb2.elf blinker04.gcc.thumb2.bin -O binary


blinker04.clang.bc : blinker04.c
	clang $(LOPS) -c blinker04.c -o blinker04.clang.bc

blinker04.clang.thumb.norm.bin : memmap vectors.o blinker04.clang.bc
	llc $(LLCOPS) blinker04.clang.bc -o blinker04.clang.thumb.norm.s
	$(ARMGNU)-as blinker04.clang.thumb.norm.s -o blinker04.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker04.clang.thumb.norm.elf -T memmap vectors.o blinker04.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker04.clang.thumb.norm.elf > blinker04.clang.thumb.norm.list
	$(ARMGNU)-objcopy blinker04.clang.thumb.norm.elf blinker04.clang.thumb.norm.bin -O binary

blinker04.clang.thumb.opt.bin : memmap vectors.o blinker04.clang.bc
	opt $(OOPS) blinker04.clang.bc -o blinker04.clang.thumb.opt.bc
	llc $(LLCOPS) blinker04.clang.thumb.opt.bc -o blinker04.clang.thumb.opt.s
	$(ARMGNU)-as blinker04.clang.thumb.opt.s -o blinker04.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker04.clang.thumb.opt.elf -T memmap vectors.o blinker04.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker04.clang.thumb.opt.elf > blinker04.clang.thumb.opt.list
	$(ARMGNU)-objcopy blinker04.clang.thumb.opt.elf blinker04.clang.thumb.opt.bin -O binary

blinker04.clang.thumb2.norm.bin : memmap vectors.o blinker04.clang.bc
	llc $(LLCOPS2) blinker04.clang.bc -o blinker04.clang.thumb2.norm.s
	$(ARMGNU)-as blinker04.clang.thumb2.norm.s -o blinker04.clang.thumb2.norm.o
	$(ARMGNU)-ld -o blinker04.clang.thumb2.norm.elf -T memmap vectors.o blinker04.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D blinker04.clang.thumb2.norm.elf > blinker04.clang.thumb2.norm.list
	$(ARMGNU)-objcopy blinker04.clang.thumb2.norm.elf blinker04.clang.thumb2.norm.bin -O binary

blinker04.clang.thumb2.opt.bin : memmap vectors.o blinker04.clang.bc
	opt $(OOPS) blinker04.clang.bc -o blinker04.clang.thumb2.opt.bc
	llc $(LLCOPS2) blinker04.clang.thumb2.opt.bc -o blinker04.clang.thumb2.opt.s
	$(ARMGNU)-as blinker04.clang.thumb2.opt.s -o blinker04.clang.thumb2.opt.o
	$(ARMGNU)-ld -o blinker04.clang.thumb2.opt.elf -T memmap vectors.o blinker04.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D blinker04.clang.thumb2.opt.elf > blinker04.clang.thumb2.opt.list
	$(ARMGNU)-objcopy blinker04.clang.thumb2.opt.elf blinker04.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


