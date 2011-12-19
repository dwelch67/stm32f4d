
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : uart05bb.gcc.thumb.bin uart05bb.gcc.thumb2.bin uart05bb.clang.thumb.norm.bin uart05bb.clang.thumb.opt.bin uart05bb.clang.thumb2.norm.bin uart05bb.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart05bb.gcc.thumb.o : uart05bb.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart05bb.c -o uart05bb.gcc.thumb.o

uart05bb.gcc.thumb2.o : uart05bb.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c uart05bb.c -o uart05bb.gcc.thumb2.o

uart05bb.gcc.thumb.bin : memmap vectors.o uart05bb.gcc.thumb.o
	$(ARMGNU)-ld -o uart05bb.gcc.thumb.elf -T memmap vectors.o uart05bb.gcc.thumb.o
	$(ARMGNU)-objdump -D uart05bb.gcc.thumb.elf > uart05bb.gcc.thumb.list
	$(ARMGNU)-objcopy uart05bb.gcc.thumb.elf uart05bb.gcc.thumb.bin -O binary

uart05bb.gcc.thumb2.bin : memmap vectors.o uart05bb.gcc.thumb2.o
	$(ARMGNU)-ld -o uart05bb.gcc.thumb2.elf -T memmap vectors.o uart05bb.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart05bb.gcc.thumb2.elf > uart05bb.gcc.thumb2.list
	$(ARMGNU)-objcopy uart05bb.gcc.thumb2.elf uart05bb.gcc.thumb2.bin -O binary


uart05bb.clang.bc : uart05bb.c
	clang $(LOPS) -c uart05bb.c -o uart05bb.clang.bc

uart05bb.clang.thumb.norm.bin : memmap vectors.o uart05bb.clang.bc
	llc $(LLCOPS) uart05bb.clang.bc -o uart05bb.clang.thumb.norm.s
	$(ARMGNU)-as uart05bb.clang.thumb.norm.s -o uart05bb.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart05bb.clang.thumb.norm.elf -T memmap vectors.o uart05bb.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart05bb.clang.thumb.norm.elf > uart05bb.clang.thumb.norm.list
	$(ARMGNU)-objcopy uart05bb.clang.thumb.norm.elf uart05bb.clang.thumb.norm.bin -O binary

uart05bb.clang.thumb.opt.bin : memmap vectors.o uart05bb.clang.bc
	opt $(OOPS) uart05bb.clang.bc -o uart05bb.clang.thumb.opt.bc
	llc $(LLCOPS) uart05bb.clang.thumb.opt.bc -o uart05bb.clang.thumb.opt.s
	$(ARMGNU)-as uart05bb.clang.thumb.opt.s -o uart05bb.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart05bb.clang.thumb.opt.elf -T memmap vectors.o uart05bb.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart05bb.clang.thumb.opt.elf > uart05bb.clang.thumb.opt.list
	$(ARMGNU)-objcopy uart05bb.clang.thumb.opt.elf uart05bb.clang.thumb.opt.bin -O binary

uart05bb.clang.thumb2.norm.bin : memmap vectors.o uart05bb.clang.bc
	llc $(LLCOPS2) uart05bb.clang.bc -o uart05bb.clang.thumb2.norm.s
	$(ARMGNU)-as uart05bb.clang.thumb2.norm.s -o uart05bb.clang.thumb2.norm.o
	$(ARMGNU)-ld -o uart05bb.clang.thumb2.norm.elf -T memmap vectors.o uart05bb.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D uart05bb.clang.thumb2.norm.elf > uart05bb.clang.thumb2.norm.list
	$(ARMGNU)-objcopy uart05bb.clang.thumb2.norm.elf uart05bb.clang.thumb2.norm.bin -O binary

uart05bb.clang.thumb2.opt.bin : memmap vectors.o uart05bb.clang.bc
	opt $(OOPS) uart05bb.clang.bc -o uart05bb.clang.thumb2.opt.bc
	llc $(LLCOPS2) uart05bb.clang.thumb2.opt.bc -o uart05bb.clang.thumb2.opt.s
	$(ARMGNU)-as uart05bb.clang.thumb2.opt.s -o uart05bb.clang.thumb2.opt.o
	$(ARMGNU)-ld -o uart05bb.clang.thumb2.opt.elf -T memmap vectors.o uart05bb.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D uart05bb.clang.thumb2.opt.elf > uart05bb.clang.thumb2.opt.list
	$(ARMGNU)-objcopy uart05bb.clang.thumb2.opt.elf uart05bb.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


