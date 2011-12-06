
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : midi02.gcc.thumb.bin midi02.gcc.thumb2.bin midi02.clang.thumb.norm.bin midi02.clang.thumb.opt.bin midi02.clang.thumb2.norm.bin midi02.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

midi02.gcc.thumb.o : midi02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c midi02.c -o midi02.gcc.thumb.o

midi02.gcc.thumb2.o : midi02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c midi02.c -o midi02.gcc.thumb2.o

midi02.gcc.thumb.bin : memmap vectors.o midi02.gcc.thumb.o
	$(ARMGNU)-ld -o midi02.gcc.thumb.elf -T memmap vectors.o midi02.gcc.thumb.o
	$(ARMGNU)-objdump -D midi02.gcc.thumb.elf > midi02.gcc.thumb.list
	$(ARMGNU)-objcopy midi02.gcc.thumb.elf midi02.gcc.thumb.bin -O binary

midi02.gcc.thumb2.bin : memmap vectors.o midi02.gcc.thumb2.o
	$(ARMGNU)-ld -o midi02.gcc.thumb2.elf -T memmap vectors.o midi02.gcc.thumb2.o
	$(ARMGNU)-objdump -D midi02.gcc.thumb2.elf > midi02.gcc.thumb2.list
	$(ARMGNU)-objcopy midi02.gcc.thumb2.elf midi02.gcc.thumb2.bin -O binary


midi02.clang.bc : midi02.c
	clang $(LOPS) -c midi02.c -o midi02.clang.bc

midi02.clang.thumb.norm.bin : memmap vectors.o midi02.clang.bc
	llc $(LLCOPS) midi02.clang.bc -o midi02.clang.thumb.norm.s
	$(ARMGNU)-as midi02.clang.thumb.norm.s -o midi02.clang.thumb.norm.o
	$(ARMGNU)-ld -o midi02.clang.thumb.norm.elf -T memmap vectors.o midi02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D midi02.clang.thumb.norm.elf > midi02.clang.thumb.norm.list
	$(ARMGNU)-objcopy midi02.clang.thumb.norm.elf midi02.clang.thumb.norm.bin -O binary

midi02.clang.thumb.opt.bin : memmap vectors.o midi02.clang.bc
	opt $(OOPS) midi02.clang.bc -o midi02.clang.thumb.opt.bc
	llc $(LLCOPS) midi02.clang.thumb.opt.bc -o midi02.clang.thumb.opt.s
	$(ARMGNU)-as midi02.clang.thumb.opt.s -o midi02.clang.thumb.opt.o
	$(ARMGNU)-ld -o midi02.clang.thumb.opt.elf -T memmap vectors.o midi02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D midi02.clang.thumb.opt.elf > midi02.clang.thumb.opt.list
	$(ARMGNU)-objcopy midi02.clang.thumb.opt.elf midi02.clang.thumb.opt.bin -O binary

midi02.clang.thumb2.norm.bin : memmap vectors.o midi02.clang.bc
	llc $(LLCOPS2) midi02.clang.bc -o midi02.clang.thumb2.norm.s
	$(ARMGNU)-as midi02.clang.thumb2.norm.s -o midi02.clang.thumb2.norm.o
	$(ARMGNU)-ld -o midi02.clang.thumb2.norm.elf -T memmap vectors.o midi02.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D midi02.clang.thumb2.norm.elf > midi02.clang.thumb2.norm.list
	$(ARMGNU)-objcopy midi02.clang.thumb2.norm.elf midi02.clang.thumb2.norm.bin -O binary

midi02.clang.thumb2.opt.bin : memmap vectors.o midi02.clang.bc
	opt $(OOPS) midi02.clang.bc -o midi02.clang.thumb2.opt.bc
	llc $(LLCOPS2) midi02.clang.thumb2.opt.bc -o midi02.clang.thumb2.opt.s
	$(ARMGNU)-as midi02.clang.thumb2.opt.s -o midi02.clang.thumb2.opt.o
	$(ARMGNU)-ld -o midi02.clang.thumb2.opt.elf -T memmap vectors.o midi02.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D midi02.clang.thumb2.opt.elf > midi02.clang.thumb2.opt.list
	$(ARMGNU)-objcopy midi02.clang.thumb2.opt.elf midi02.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


