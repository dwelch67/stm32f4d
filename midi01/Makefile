
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : midi01.gcc.thumb.bin midi01.gcc.thumb2.bin midi01.clang.thumb.norm.bin midi01.clang.thumb.opt.bin midi01.clang.thumb2.norm.bin midi01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

midi01.gcc.thumb.o : midi01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c midi01.c -o midi01.gcc.thumb.o

midi01.gcc.thumb2.o : midi01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c midi01.c -o midi01.gcc.thumb2.o

midi01.gcc.thumb.bin : memmap vectors.o midi01.gcc.thumb.o
	$(ARMGNU)-ld -o midi01.gcc.thumb.elf -T memmap vectors.o midi01.gcc.thumb.o
	$(ARMGNU)-objdump -D midi01.gcc.thumb.elf > midi01.gcc.thumb.list
	$(ARMGNU)-objcopy midi01.gcc.thumb.elf midi01.gcc.thumb.bin -O binary

midi01.gcc.thumb2.bin : memmap vectors.o midi01.gcc.thumb2.o
	$(ARMGNU)-ld -o midi01.gcc.thumb2.elf -T memmap vectors.o midi01.gcc.thumb2.o
	$(ARMGNU)-objdump -D midi01.gcc.thumb2.elf > midi01.gcc.thumb2.list
	$(ARMGNU)-objcopy midi01.gcc.thumb2.elf midi01.gcc.thumb2.bin -O binary


midi01.clang.bc : midi01.c
	clang $(LOPS) -c midi01.c -o midi01.clang.bc

midi01.clang.thumb.norm.bin : memmap vectors.o midi01.clang.bc
	llc $(LLCOPS) midi01.clang.bc -o midi01.clang.thumb.norm.s
	$(ARMGNU)-as midi01.clang.thumb.norm.s -o midi01.clang.thumb.norm.o
	$(ARMGNU)-ld -o midi01.clang.thumb.norm.elf -T memmap vectors.o midi01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D midi01.clang.thumb.norm.elf > midi01.clang.thumb.norm.list
	$(ARMGNU)-objcopy midi01.clang.thumb.norm.elf midi01.clang.thumb.norm.bin -O binary

midi01.clang.thumb.opt.bin : memmap vectors.o midi01.clang.bc
	opt $(OOPS) midi01.clang.bc -o midi01.clang.thumb.opt.bc
	llc $(LLCOPS) midi01.clang.thumb.opt.bc -o midi01.clang.thumb.opt.s
	$(ARMGNU)-as midi01.clang.thumb.opt.s -o midi01.clang.thumb.opt.o
	$(ARMGNU)-ld -o midi01.clang.thumb.opt.elf -T memmap vectors.o midi01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D midi01.clang.thumb.opt.elf > midi01.clang.thumb.opt.list
	$(ARMGNU)-objcopy midi01.clang.thumb.opt.elf midi01.clang.thumb.opt.bin -O binary

midi01.clang.thumb2.norm.bin : memmap vectors.o midi01.clang.bc
	llc $(LLCOPS2) midi01.clang.bc -o midi01.clang.thumb2.norm.s
	$(ARMGNU)-as midi01.clang.thumb2.norm.s -o midi01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o midi01.clang.thumb2.norm.elf -T memmap vectors.o midi01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D midi01.clang.thumb2.norm.elf > midi01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy midi01.clang.thumb2.norm.elf midi01.clang.thumb2.norm.bin -O binary

midi01.clang.thumb2.opt.bin : memmap vectors.o midi01.clang.bc
	opt $(OOPS) midi01.clang.bc -o midi01.clang.thumb2.opt.bc
	llc $(LLCOPS2) midi01.clang.thumb2.opt.bc -o midi01.clang.thumb2.opt.s
	$(ARMGNU)-as midi01.clang.thumb2.opt.s -o midi01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o midi01.clang.thumb2.opt.elf -T memmap vectors.o midi01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D midi01.clang.thumb2.opt.elf > midi01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy midi01.clang.thumb2.opt.elf midi01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


