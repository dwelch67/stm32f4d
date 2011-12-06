
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : bench01.gcc.thumb.bin bench01.gcc.thumb2.bin bench01.clang.thumb.norm.bin bench01.clang.thumb.opt.bin bench01.clang.thumb2.norm.bin bench01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

bench01.gcc.thumb.o : bench01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c bench01.c -o bench01.gcc.thumb.o

bench01.gcc.thumb2.o : bench01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c bench01.c -o bench01.gcc.thumb2.o

bench01.gcc.thumb.bin : memmap vectors.o bench01.gcc.thumb.o
	$(ARMGNU)-ld -o bench01.gcc.thumb.elf -T memmap vectors.o bench01.gcc.thumb.o
	$(ARMGNU)-objdump -D bench01.gcc.thumb.elf > bench01.gcc.thumb.list
	$(ARMGNU)-objcopy bench01.gcc.thumb.elf bench01.gcc.thumb.bin -O binary

bench01.gcc.thumb2.bin : memmap vectors.o bench01.gcc.thumb2.o
	$(ARMGNU)-ld -o bench01.gcc.thumb2.elf -T memmap vectors.o bench01.gcc.thumb2.o
	$(ARMGNU)-objdump -D bench01.gcc.thumb2.elf > bench01.gcc.thumb2.list
	$(ARMGNU)-objcopy bench01.gcc.thumb2.elf bench01.gcc.thumb2.bin -O binary


bench01.clang.bc : bench01.c
	clang $(LOPS) -c bench01.c -o bench01.clang.bc

bench01.clang.thumb.norm.bin : memmap vectors.o bench01.clang.bc
	llc $(LLCOPS) bench01.clang.bc -o bench01.clang.thumb.norm.s
	$(ARMGNU)-as bench01.clang.thumb.norm.s -o bench01.clang.thumb.norm.o
	$(ARMGNU)-ld -o bench01.clang.thumb.norm.elf -T memmap vectors.o bench01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D bench01.clang.thumb.norm.elf > bench01.clang.thumb.norm.list
	$(ARMGNU)-objcopy bench01.clang.thumb.norm.elf bench01.clang.thumb.norm.bin -O binary

bench01.clang.thumb.opt.bin : memmap vectors.o bench01.clang.bc
	opt $(OOPS) bench01.clang.bc -o bench01.clang.thumb.opt.bc
	llc $(LLCOPS) bench01.clang.thumb.opt.bc -o bench01.clang.thumb.opt.s
	$(ARMGNU)-as bench01.clang.thumb.opt.s -o bench01.clang.thumb.opt.o
	$(ARMGNU)-ld -o bench01.clang.thumb.opt.elf -T memmap vectors.o bench01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D bench01.clang.thumb.opt.elf > bench01.clang.thumb.opt.list
	$(ARMGNU)-objcopy bench01.clang.thumb.opt.elf bench01.clang.thumb.opt.bin -O binary

bench01.clang.thumb2.norm.bin : memmap vectors.o bench01.clang.bc
	llc $(LLCOPS2) bench01.clang.bc -o bench01.clang.thumb2.norm.s
	$(ARMGNU)-as bench01.clang.thumb2.norm.s -o bench01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o bench01.clang.thumb2.norm.elf -T memmap vectors.o bench01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D bench01.clang.thumb2.norm.elf > bench01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy bench01.clang.thumb2.norm.elf bench01.clang.thumb2.norm.bin -O binary

bench01.clang.thumb2.opt.bin : memmap vectors.o bench01.clang.bc
	opt $(OOPS) bench01.clang.bc -o bench01.clang.thumb2.opt.bc
	llc $(LLCOPS2) bench01.clang.thumb2.opt.bc -o bench01.clang.thumb2.opt.s
	$(ARMGNU)-as bench01.clang.thumb2.opt.s -o bench01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o bench01.clang.thumb2.opt.elf -T memmap vectors.o bench01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D bench01.clang.thumb2.opt.elf > bench01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy bench01.clang.thumb2.opt.elf bench01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


