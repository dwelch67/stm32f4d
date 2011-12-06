
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : uart04.gcc.thumb.bin uart04.gcc.thumb2.bin uart04.clang.thumb.norm.bin uart04.clang.thumb.opt.bin uart04.clang.thumb2.norm.bin uart04.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart04.gcc.thumb.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart04.c -o uart04.gcc.thumb.o

uart04.gcc.thumb2.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c uart04.c -o uart04.gcc.thumb2.o

uart04.gcc.thumb.bin : memmap vectors.o uart04.gcc.thumb.o
	$(ARMGNU)-ld -o uart04.gcc.thumb.elf -T memmap vectors.o uart04.gcc.thumb.o
	$(ARMGNU)-objdump -D uart04.gcc.thumb.elf > uart04.gcc.thumb.list
	$(ARMGNU)-objcopy uart04.gcc.thumb.elf uart04.gcc.thumb.bin -O binary

uart04.gcc.thumb2.bin : memmap vectors.o uart04.gcc.thumb2.o
	$(ARMGNU)-ld -o uart04.gcc.thumb2.elf -T memmap vectors.o uart04.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart04.gcc.thumb2.elf > uart04.gcc.thumb2.list
	$(ARMGNU)-objcopy uart04.gcc.thumb2.elf uart04.gcc.thumb2.bin -O binary


uart04.clang.bc : uart04.c
	clang $(LOPS) -c uart04.c -o uart04.clang.bc

uart04.clang.thumb.norm.bin : memmap vectors.o uart04.clang.bc
	llc $(LLCOPS) uart04.clang.bc -o uart04.clang.thumb.norm.s
	$(ARMGNU)-as uart04.clang.thumb.norm.s -o uart04.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart04.clang.thumb.norm.elf -T memmap vectors.o uart04.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart04.clang.thumb.norm.elf > uart04.clang.thumb.norm.list
	$(ARMGNU)-objcopy uart04.clang.thumb.norm.elf uart04.clang.thumb.norm.bin -O binary

uart04.clang.thumb.opt.bin : memmap vectors.o uart04.clang.bc
	opt $(OOPS) uart04.clang.bc -o uart04.clang.thumb.opt.bc
	llc $(LLCOPS) uart04.clang.thumb.opt.bc -o uart04.clang.thumb.opt.s
	$(ARMGNU)-as uart04.clang.thumb.opt.s -o uart04.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart04.clang.thumb.opt.elf -T memmap vectors.o uart04.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart04.clang.thumb.opt.elf > uart04.clang.thumb.opt.list
	$(ARMGNU)-objcopy uart04.clang.thumb.opt.elf uart04.clang.thumb.opt.bin -O binary

uart04.clang.thumb2.norm.bin : memmap vectors.o uart04.clang.bc
	llc $(LLCOPS2) uart04.clang.bc -o uart04.clang.thumb2.norm.s
	$(ARMGNU)-as uart04.clang.thumb2.norm.s -o uart04.clang.thumb2.norm.o
	$(ARMGNU)-ld -o uart04.clang.thumb2.norm.elf -T memmap vectors.o uart04.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D uart04.clang.thumb2.norm.elf > uart04.clang.thumb2.norm.list
	$(ARMGNU)-objcopy uart04.clang.thumb2.norm.elf uart04.clang.thumb2.norm.bin -O binary

uart04.clang.thumb2.opt.bin : memmap vectors.o uart04.clang.bc
	opt $(OOPS) uart04.clang.bc -o uart04.clang.thumb2.opt.bc
	llc $(LLCOPS2) uart04.clang.thumb2.opt.bc -o uart04.clang.thumb2.opt.s
	$(ARMGNU)-as uart04.clang.thumb2.opt.s -o uart04.clang.thumb2.opt.o
	$(ARMGNU)-ld -o uart04.clang.thumb2.opt.elf -T memmap vectors.o uart04.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D uart04.clang.thumb2.opt.elf > uart04.clang.thumb2.opt.list
	$(ARMGNU)-objcopy uart04.clang.thumb2.opt.elf uart04.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


