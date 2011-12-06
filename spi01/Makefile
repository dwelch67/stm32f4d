
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : spi01.gcc.thumb.bin spi01.gcc.thumb2.bin spi01.clang.thumb.norm.bin spi01.clang.thumb.opt.bin spi01.clang.thumb2.norm.bin spi01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

spi01.gcc.thumb.o : spi01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c spi01.c -o spi01.gcc.thumb.o

spi01.gcc.thumb2.o : spi01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c spi01.c -o spi01.gcc.thumb2.o

spi01.gcc.thumb.bin : memmap vectors.o spi01.gcc.thumb.o
	$(ARMGNU)-ld -o spi01.gcc.thumb.elf -T memmap vectors.o spi01.gcc.thumb.o
	$(ARMGNU)-objdump -D spi01.gcc.thumb.elf > spi01.gcc.thumb.list
	$(ARMGNU)-objcopy spi01.gcc.thumb.elf spi01.gcc.thumb.bin -O binary

spi01.gcc.thumb2.bin : memmap vectors.o spi01.gcc.thumb2.o
	$(ARMGNU)-ld -o spi01.gcc.thumb2.elf -T memmap vectors.o spi01.gcc.thumb2.o
	$(ARMGNU)-objdump -D spi01.gcc.thumb2.elf > spi01.gcc.thumb2.list
	$(ARMGNU)-objcopy spi01.gcc.thumb2.elf spi01.gcc.thumb2.bin -O binary


spi01.clang.bc : spi01.c
	clang $(LOPS) -c spi01.c -o spi01.clang.bc

spi01.clang.thumb.norm.bin : memmap vectors.o spi01.clang.bc
	llc $(LLCOPS) spi01.clang.bc -o spi01.clang.thumb.norm.s
	$(ARMGNU)-as spi01.clang.thumb.norm.s -o spi01.clang.thumb.norm.o
	$(ARMGNU)-ld -o spi01.clang.thumb.norm.elf -T memmap vectors.o spi01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D spi01.clang.thumb.norm.elf > spi01.clang.thumb.norm.list
	$(ARMGNU)-objcopy spi01.clang.thumb.norm.elf spi01.clang.thumb.norm.bin -O binary

spi01.clang.thumb.opt.bin : memmap vectors.o spi01.clang.bc
	opt $(OOPS) spi01.clang.bc -o spi01.clang.thumb.opt.bc
	llc $(LLCOPS) spi01.clang.thumb.opt.bc -o spi01.clang.thumb.opt.s
	$(ARMGNU)-as spi01.clang.thumb.opt.s -o spi01.clang.thumb.opt.o
	$(ARMGNU)-ld -o spi01.clang.thumb.opt.elf -T memmap vectors.o spi01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D spi01.clang.thumb.opt.elf > spi01.clang.thumb.opt.list
	$(ARMGNU)-objcopy spi01.clang.thumb.opt.elf spi01.clang.thumb.opt.bin -O binary

spi01.clang.thumb2.norm.bin : memmap vectors.o spi01.clang.bc
	llc $(LLCOPS2) spi01.clang.bc -o spi01.clang.thumb2.norm.s
	$(ARMGNU)-as spi01.clang.thumb2.norm.s -o spi01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o spi01.clang.thumb2.norm.elf -T memmap vectors.o spi01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D spi01.clang.thumb2.norm.elf > spi01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy spi01.clang.thumb2.norm.elf spi01.clang.thumb2.norm.bin -O binary

spi01.clang.thumb2.opt.bin : memmap vectors.o spi01.clang.bc
	opt $(OOPS) spi01.clang.bc -o spi01.clang.thumb2.opt.bc
	llc $(LLCOPS2) spi01.clang.thumb2.opt.bc -o spi01.clang.thumb2.opt.s
	$(ARMGNU)-as spi01.clang.thumb2.opt.s -o spi01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o spi01.clang.thumb2.opt.elf -T memmap vectors.o spi01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D spi01.clang.thumb2.opt.elf > spi01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy spi01.clang.thumb2.opt.elf spi01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


