
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mthumb -mfpu=vfp
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mthumb -mfpu=vfp

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
OOPS = -std-compile-opts

gcc : float03.gcc.thumb.bin float03.gcc.thumb2.bin 
clang : float03.clang.thumb.opt.bin float03.clang.thumb2.opt.bin
all : gcc clang

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

float03.gcc.thumb.o : float03.c 
	$(ARMGNU)-gcc $(COPS) -c float03.c -o float03.gcc.thumb.o

float03.gcc.thumb2.o : float03.c
	$(ARMGNU)-gcc $(COPS) -mcpu=cortex-m3 -march=armv7-m -c float03.c -o float03.gcc.thumb2.o


testfun.gcc.thumb.o : testfun.c 
	$(ARMGNU)-gcc $(COPS) -c testfun.c -o testfun.gcc.thumb.o

testfun.gcc.thumb2.o : testfun.c
	$(ARMGNU)-gcc $(COPS) -mcpu=cortex-m3 -march=armv7-m -c testfun.c -o testfun.gcc.thumb2.o




float03.gcc.thumb.bin : memmap vectors.o float03.gcc.thumb.o testfun.gcc.thumb.o
	$(ARMGNU)-ld -o float03.gcc.thumb.elf -T memmap vectors.o float03.gcc.thumb.o testfun.gcc.thumb.o
	$(ARMGNU)-objdump -D float03.gcc.thumb.elf > float03.gcc.thumb.list
	$(ARMGNU)-objcopy float03.gcc.thumb.elf float03.gcc.thumb.bin -O binary

float03.gcc.thumb2.bin : memmap vectors.o float03.gcc.thumb2.o testfun.gcc.thumb2.o
	$(ARMGNU)-ld -o float03.gcc.thumb2.elf -T memmap vectors.o float03.gcc.thumb2.o testfun.gcc.thumb2.o
	$(ARMGNU)-objdump -D float03.gcc.thumb2.elf > float03.gcc.thumb2.list
	$(ARMGNU)-objcopy float03.gcc.thumb2.elf float03.gcc.thumb2.bin -O binary




float03.clang.bc : float03.c
	clang $(LOPS) -c float03.c -o float03.clang.bc

testfun.clang.bc : testfun.c
	clang $(LOPS) -c testfun.c -o testfun.clang.bc

float03.clang.thumb.norm.bin : memmap vectors.o float03.clang.bc testfun.clang.bc
	llvm-link -o float03.clang.thumb.norm.bc float03.clang.bc testfun.clang.bc
	llc $(LLCOPS) float03.clang.thumb.norm.bc -o float03.clang.thumb.norm.s
	$(ARMGNU)-as float03.clang.thumb.norm.s -o float03.clang.thumb.norm.o
	$(ARMGNU)-ld -o float03.clang.thumb.norm.elf -T memmap vectors.o float03.clang.thumb.norm.o
	$(ARMGNU)-objdump -D float03.clang.thumb.norm.elf > float03.clang.thumb.norm.list
	$(ARMGNU)-objcopy float03.clang.thumb.norm.elf float03.clang.thumb.norm.bin -O binary

float03.clang.thumb.opt.bin : memmap vectors.o float03.clang.bc testfun.clang.bc
	llvm-link -o float03.clang.thumb.pre.bc float03.clang.bc testfun.clang.bc
	opt $(OOPS) float03.clang.thumb.pre.bc -o float03.clang.thumb.opt.bc
	llc $(LLCOPS) float03.clang.thumb.opt.bc -o float03.clang.thumb.opt.s
	$(ARMGNU)-as float03.clang.thumb.opt.s -o float03.clang.thumb.opt.o
	$(ARMGNU)-ld -o float03.clang.thumb.opt.elf -T memmap vectors.o float03.clang.thumb.opt.o
	$(ARMGNU)-objdump -D float03.clang.thumb.opt.elf > float03.clang.thumb.opt.list
	$(ARMGNU)-objcopy float03.clang.thumb.opt.elf float03.clang.thumb.opt.bin -O binary

float03.clang.thumb2.norm.bin : memmap vectors.o float03.clang.bc
	llvm-link -o float03.clang.thumb2.norm.bc float03.clang.bc testfun.clang.bc
	llc $(LLCOPS2) float03.clang.thumb2.norm.bc -o float03.clang.thumb2.norm.s
	$(ARMGNU)-as float03.clang.thumb2.norm.s -o float03.clang.thumb2.norm.o
	$(ARMGNU)-ld -o float03.clang.thumb2.norm.elf -T memmap vectors.o float03.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D float03.clang.thumb2.norm.elf > float03.clang.thumb2.norm.list
	$(ARMGNU)-objcopy float03.clang.thumb2.norm.elf float03.clang.thumb2.norm.bin -O binary

float03.clang.thumb2.opt.bin : memmap vectors.o float03.clang.bc
	llvm-link -o float03.clang.thumb2.pre.bc float03.clang.bc testfun.clang.bc
	opt $(OOPS) float03.clang.thumb2.pre.bc -o float03.clang.thumb2.opt.bc
	llc $(LLCOPS2) float03.clang.thumb2.opt.bc -o float03.clang.thumb2.opt.s
	$(ARMGNU)-as float03.clang.thumb2.opt.s -o float03.clang.thumb2.opt.o
	$(ARMGNU)-ld -o float03.clang.thumb2.opt.elf -T memmap vectors.o float03.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D float03.clang.thumb2.opt.elf > float03.clang.thumb2.opt.list
	$(ARMGNU)-objcopy float03.clang.thumb2.opt.elf float03.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


