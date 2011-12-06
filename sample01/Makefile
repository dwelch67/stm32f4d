
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
OOPS = -std-compile-opts

all : sample01.gcc.thumb2.bin sample01.clang.thumb2.opt.bin


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sample01.gcc.thumb.o : sample01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sample01.c -o sample01.gcc.thumb.o

sample01.gcc.thumb2.o : sample01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c sample01.c -o sample01.gcc.thumb2.o

sample01.gcc.thumb.bin : memmap vectors.o sample01.gcc.thumb.o
	$(ARMGNU)-ld -o sample01.gcc.thumb.elf -T memmap vectors.o sample01.gcc.thumb.o
	$(ARMGNU)-objdump -D sample01.gcc.thumb.elf > sample01.gcc.thumb.list
	$(ARMGNU)-objcopy sample01.gcc.thumb.elf sample01.gcc.thumb.bin -O binary

sample01.gcc.thumb2.bin : memmap vectors.o sample01.gcc.thumb2.o
	$(ARMGNU)-ld -o sample01.gcc.thumb2.elf -T memmap vectors.o sample01.gcc.thumb2.o
	$(ARMGNU)-objdump -D sample01.gcc.thumb2.elf > sample01.gcc.thumb2.list
	$(ARMGNU)-objcopy sample01.gcc.thumb2.elf sample01.gcc.thumb2.bin -O binary


sample01.clang.bc : sample01.c
	clang $(LOPS) -c sample01.c -o sample01.clang.bc

sample01.clang.thumb.norm.bin : memmap vectors.o sample01.clang.bc
	llc $(LLCOPS) sample01.clang.bc -o sample01.clang.thumb.norm.s
	$(ARMGNU)-as sample01.clang.thumb.norm.s -o sample01.clang.thumb.norm.o
	$(ARMGNU)-ld -o sample01.clang.thumb.norm.elf -T memmap vectors.o sample01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D sample01.clang.thumb.norm.elf > sample01.clang.thumb.norm.list
	$(ARMGNU)-objcopy sample01.clang.thumb.norm.elf sample01.clang.thumb.norm.bin -O binary

sample01.clang.thumb.opt.bin : memmap vectors.o sample01.clang.bc
	opt $(OOPS) sample01.clang.bc -o sample01.clang.thumb.opt.bc
	llc $(LLCOPS) sample01.clang.thumb.opt.bc -o sample01.clang.thumb.opt.s
	$(ARMGNU)-as sample01.clang.thumb.opt.s -o sample01.clang.thumb.opt.o
	$(ARMGNU)-ld -o sample01.clang.thumb.opt.elf -T memmap vectors.o sample01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D sample01.clang.thumb.opt.elf > sample01.clang.thumb.opt.list
	$(ARMGNU)-objcopy sample01.clang.thumb.opt.elf sample01.clang.thumb.opt.bin -O binary

sample01.clang.thumb2.norm.bin : memmap vectors.o sample01.clang.bc
	llc $(LLCOPS2) sample01.clang.bc -o sample01.clang.thumb2.norm.s
	$(ARMGNU)-as sample01.clang.thumb2.norm.s -o sample01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o sample01.clang.thumb2.norm.elf -T memmap vectors.o sample01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D sample01.clang.thumb2.norm.elf > sample01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy sample01.clang.thumb2.norm.elf sample01.clang.thumb2.norm.bin -O binary

sample01.clang.thumb2.opt.bin : memmap vectors.o sample01.clang.bc
	opt $(OOPS) sample01.clang.bc -o sample01.clang.thumb2.opt.bc
	llc $(LLCOPS2) sample01.clang.thumb2.opt.bc -o sample01.clang.thumb2.opt.s
	$(ARMGNU)-as sample01.clang.thumb2.opt.s -o sample01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o sample01.clang.thumb2.opt.elf -T memmap vectors.o sample01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D sample01.clang.thumb2.opt.elf > sample01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy sample01.clang.thumb2.opt.elf sample01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


