
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : uart02.gcc.thumb.bin uart02.gcc.thumb2.bin uart02.clang.thumb.norm.bin uart02.clang.thumb.opt.bin uart02.clang.thumb2.norm.bin uart02.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart02.gcc.thumb.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart02.c -o uart02.gcc.thumb.o

uart02.gcc.thumb2.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c uart02.c -o uart02.gcc.thumb2.o

uart02.gcc.thumb.bin : memmap vectors.o uart02.gcc.thumb.o
	$(ARMGNU)-ld -o uart02.gcc.thumb.elf -T memmap vectors.o uart02.gcc.thumb.o
	$(ARMGNU)-objdump -D uart02.gcc.thumb.elf > uart02.gcc.thumb.list
	$(ARMGNU)-objcopy uart02.gcc.thumb.elf uart02.gcc.thumb.bin -O binary

uart02.gcc.thumb2.bin : memmap vectors.o uart02.gcc.thumb2.o
	$(ARMGNU)-ld -o uart02.gcc.thumb2.elf -T memmap vectors.o uart02.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart02.gcc.thumb2.elf > uart02.gcc.thumb2.list
	$(ARMGNU)-objcopy uart02.gcc.thumb2.elf uart02.gcc.thumb2.bin -O binary


uart02.clang.bc : uart02.c
	clang $(LOPS) -c uart02.c -o uart02.clang.bc

uart02.clang.thumb.norm.bin : memmap vectors.o uart02.clang.bc
	llc $(LLCOPS) uart02.clang.bc -o uart02.clang.thumb.norm.s
	$(ARMGNU)-as uart02.clang.thumb.norm.s -o uart02.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart02.clang.thumb.norm.elf -T memmap vectors.o uart02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart02.clang.thumb.norm.elf > uart02.clang.thumb.norm.list
	$(ARMGNU)-objcopy uart02.clang.thumb.norm.elf uart02.clang.thumb.norm.bin -O binary

uart02.clang.thumb.opt.bin : memmap vectors.o uart02.clang.bc
	opt $(OOPS) uart02.clang.bc -o uart02.clang.thumb.opt.bc
	llc $(LLCOPS) uart02.clang.thumb.opt.bc -o uart02.clang.thumb.opt.s
	$(ARMGNU)-as uart02.clang.thumb.opt.s -o uart02.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart02.clang.thumb.opt.elf -T memmap vectors.o uart02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart02.clang.thumb.opt.elf > uart02.clang.thumb.opt.list
	$(ARMGNU)-objcopy uart02.clang.thumb.opt.elf uart02.clang.thumb.opt.bin -O binary

uart02.clang.thumb2.norm.bin : memmap vectors.o uart02.clang.bc
	llc $(LLCOPS2) uart02.clang.bc -o uart02.clang.thumb2.norm.s
	$(ARMGNU)-as uart02.clang.thumb2.norm.s -o uart02.clang.thumb2.norm.o
	$(ARMGNU)-ld -o uart02.clang.thumb2.norm.elf -T memmap vectors.o uart02.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D uart02.clang.thumb2.norm.elf > uart02.clang.thumb2.norm.list
	$(ARMGNU)-objcopy uart02.clang.thumb2.norm.elf uart02.clang.thumb2.norm.bin -O binary

uart02.clang.thumb2.opt.bin : memmap vectors.o uart02.clang.bc
	opt $(OOPS) uart02.clang.bc -o uart02.clang.thumb2.opt.bc
	llc $(LLCOPS2) uart02.clang.thumb2.opt.bc -o uart02.clang.thumb2.opt.s
	$(ARMGNU)-as uart02.clang.thumb2.opt.s -o uart02.clang.thumb2.opt.o
	$(ARMGNU)-ld -o uart02.clang.thumb2.opt.elf -T memmap vectors.o uart02.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D uart02.clang.thumb2.opt.elf > uart02.clang.thumb2.opt.list
	$(ARMGNU)-objcopy uart02.clang.thumb2.opt.elf uart02.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


