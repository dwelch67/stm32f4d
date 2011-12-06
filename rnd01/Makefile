
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : rnd01.gcc.thumb.bin rnd01.gcc.thumb2.bin rnd01.clang.thumb.norm.bin rnd01.clang.thumb.opt.bin rnd01.clang.thumb2.norm.bin rnd01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

rnd01.gcc.thumb.o : rnd01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c rnd01.c -o rnd01.gcc.thumb.o

rnd01.gcc.thumb2.o : rnd01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c rnd01.c -o rnd01.gcc.thumb2.o

rnd01.gcc.thumb.bin : memmap vectors.o rnd01.gcc.thumb.o
	$(ARMGNU)-ld -o rnd01.gcc.thumb.elf -T memmap vectors.o rnd01.gcc.thumb.o
	$(ARMGNU)-objdump -D rnd01.gcc.thumb.elf > rnd01.gcc.thumb.list
	$(ARMGNU)-objcopy rnd01.gcc.thumb.elf rnd01.gcc.thumb.bin -O binary

rnd01.gcc.thumb2.bin : memmap vectors.o rnd01.gcc.thumb2.o
	$(ARMGNU)-ld -o rnd01.gcc.thumb2.elf -T memmap vectors.o rnd01.gcc.thumb2.o
	$(ARMGNU)-objdump -D rnd01.gcc.thumb2.elf > rnd01.gcc.thumb2.list
	$(ARMGNU)-objcopy rnd01.gcc.thumb2.elf rnd01.gcc.thumb2.bin -O binary


rnd01.clang.bc : rnd01.c
	clang $(LOPS) -c rnd01.c -o rnd01.clang.bc

rnd01.clang.thumb.norm.bin : memmap vectors.o rnd01.clang.bc
	llc $(LLCOPS) rnd01.clang.bc -o rnd01.clang.thumb.norm.s
	$(ARMGNU)-as rnd01.clang.thumb.norm.s -o rnd01.clang.thumb.norm.o
	$(ARMGNU)-ld -o rnd01.clang.thumb.norm.elf -T memmap vectors.o rnd01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D rnd01.clang.thumb.norm.elf > rnd01.clang.thumb.norm.list
	$(ARMGNU)-objcopy rnd01.clang.thumb.norm.elf rnd01.clang.thumb.norm.bin -O binary

rnd01.clang.thumb.opt.bin : memmap vectors.o rnd01.clang.bc
	opt $(OOPS) rnd01.clang.bc -o rnd01.clang.thumb.opt.bc
	llc $(LLCOPS) rnd01.clang.thumb.opt.bc -o rnd01.clang.thumb.opt.s
	$(ARMGNU)-as rnd01.clang.thumb.opt.s -o rnd01.clang.thumb.opt.o
	$(ARMGNU)-ld -o rnd01.clang.thumb.opt.elf -T memmap vectors.o rnd01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D rnd01.clang.thumb.opt.elf > rnd01.clang.thumb.opt.list
	$(ARMGNU)-objcopy rnd01.clang.thumb.opt.elf rnd01.clang.thumb.opt.bin -O binary

rnd01.clang.thumb2.norm.bin : memmap vectors.o rnd01.clang.bc
	llc $(LLCOPS2) rnd01.clang.bc -o rnd01.clang.thumb2.norm.s
	$(ARMGNU)-as rnd01.clang.thumb2.norm.s -o rnd01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o rnd01.clang.thumb2.norm.elf -T memmap vectors.o rnd01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D rnd01.clang.thumb2.norm.elf > rnd01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy rnd01.clang.thumb2.norm.elf rnd01.clang.thumb2.norm.bin -O binary

rnd01.clang.thumb2.opt.bin : memmap vectors.o rnd01.clang.bc
	opt $(OOPS) rnd01.clang.bc -o rnd01.clang.thumb2.opt.bc
	llc $(LLCOPS2) rnd01.clang.thumb2.opt.bc -o rnd01.clang.thumb2.opt.s
	$(ARMGNU)-as rnd01.clang.thumb2.opt.s -o rnd01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o rnd01.clang.thumb2.opt.elf -T memmap vectors.o rnd01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D rnd01.clang.thumb2.opt.elf > rnd01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy rnd01.clang.thumb2.opt.elf rnd01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


