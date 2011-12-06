
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : nholed01.gcc.thumb.bin nholed01.gcc.thumb2.bin nholed01.clang.thumb.norm.bin nholed01.clang.thumb.opt.bin nholed01.clang.thumb2.norm.bin nholed01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

nholed01.gcc.thumb.o : nholed01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c nholed01.c -o nholed01.gcc.thumb.o

nholed01.gcc.thumb2.o : nholed01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c nholed01.c -o nholed01.gcc.thumb2.o

nholed01.gcc.thumb.bin : memmap vectors.o nholed01.gcc.thumb.o
	$(ARMGNU)-ld -o nholed01.gcc.thumb.elf -T memmap vectors.o nholed01.gcc.thumb.o
	$(ARMGNU)-objdump -D nholed01.gcc.thumb.elf > nholed01.gcc.thumb.list
	$(ARMGNU)-objcopy nholed01.gcc.thumb.elf nholed01.gcc.thumb.bin -O binary

nholed01.gcc.thumb2.bin : memmap vectors.o nholed01.gcc.thumb2.o
	$(ARMGNU)-ld -o nholed01.gcc.thumb2.elf -T memmap vectors.o nholed01.gcc.thumb2.o
	$(ARMGNU)-objdump -D nholed01.gcc.thumb2.elf > nholed01.gcc.thumb2.list
	$(ARMGNU)-objcopy nholed01.gcc.thumb2.elf nholed01.gcc.thumb2.bin -O binary


nholed01.clang.bc : nholed01.c
	clang $(LOPS) -c nholed01.c -o nholed01.clang.bc

nholed01.clang.thumb.norm.bin : memmap vectors.o nholed01.clang.bc
	llc $(LLCOPS) nholed01.clang.bc -o nholed01.clang.thumb.norm.s
	$(ARMGNU)-as nholed01.clang.thumb.norm.s -o nholed01.clang.thumb.norm.o
	$(ARMGNU)-ld -o nholed01.clang.thumb.norm.elf -T memmap vectors.o nholed01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D nholed01.clang.thumb.norm.elf > nholed01.clang.thumb.norm.list
	$(ARMGNU)-objcopy nholed01.clang.thumb.norm.elf nholed01.clang.thumb.norm.bin -O binary

nholed01.clang.thumb.opt.bin : memmap vectors.o nholed01.clang.bc
	opt $(OOPS) nholed01.clang.bc -o nholed01.clang.thumb.opt.bc
	llc $(LLCOPS) nholed01.clang.thumb.opt.bc -o nholed01.clang.thumb.opt.s
	$(ARMGNU)-as nholed01.clang.thumb.opt.s -o nholed01.clang.thumb.opt.o
	$(ARMGNU)-ld -o nholed01.clang.thumb.opt.elf -T memmap vectors.o nholed01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D nholed01.clang.thumb.opt.elf > nholed01.clang.thumb.opt.list
	$(ARMGNU)-objcopy nholed01.clang.thumb.opt.elf nholed01.clang.thumb.opt.bin -O binary

nholed01.clang.thumb2.norm.bin : memmap vectors.o nholed01.clang.bc
	llc $(LLCOPS2) nholed01.clang.bc -o nholed01.clang.thumb2.norm.s
	$(ARMGNU)-as nholed01.clang.thumb2.norm.s -o nholed01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o nholed01.clang.thumb2.norm.elf -T memmap vectors.o nholed01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D nholed01.clang.thumb2.norm.elf > nholed01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy nholed01.clang.thumb2.norm.elf nholed01.clang.thumb2.norm.bin -O binary

nholed01.clang.thumb2.opt.bin : memmap vectors.o nholed01.clang.bc
	opt $(OOPS) nholed01.clang.bc -o nholed01.clang.thumb2.opt.bc
	llc $(LLCOPS2) nholed01.clang.thumb2.opt.bc -o nholed01.clang.thumb2.opt.s
	$(ARMGNU)-as nholed01.clang.thumb2.opt.s -o nholed01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o nholed01.clang.thumb2.opt.elf -T memmap vectors.o nholed01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D nholed01.clang.thumb2.opt.elf > nholed01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy nholed01.clang.thumb2.opt.elf nholed01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


