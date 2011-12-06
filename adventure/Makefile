
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -mthumb -Wall -Werror -Os -nostdlib -nostartfiles -ffreestanding
COPS2 = -mthumb -mcpu=cortex-m3 -march=armv7-m -Wall -Werror -Os -nostdlib -nostartfiles -ffreestanding


LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

LLCOPS2 = -march=thumb -mcpu=cortex-m3 

OOPS = -std-compile-opts


all : adventure.gcc.thumb.bin adventure.gcc.thumb2.bin adventure.clang.thumb2.opt.bin adventure.clang.thumb2.norm.bin adventure.clang.thumb.opt.bin adventure.clang.thumb.norm.bin


vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list
	rm -f *.norm.s
	rm -f *.opt.s
	rm -f *.bc



adventure.gcc.thumb.bin : adventure.gcc.thumb.elf
	$(ARMGNU)-objcopy adventure.gcc.thumb.elf -O binary adventure.gcc.thumb.bin

adventure.gcc.thumb.elf : vectors.o uart.thumb.o crc.thumb.o data.thumb.o done.thumb.o init.thumb.o io.thumb.o main.thumb.o show.thumb.o subr.thumb.o vocab.thumb.o wizard.thumb.o
	#$(ARMGNU)-gcc $(COPS) -Xlinker memmap -o adventure.gcc.thumb.elf vectors.o uart.thumb.o crc.thumb.o data.thumb.o done.thumb.o init.thumb.o io.thumb.o main.thumb.o show.thumb.o subr.thumb.o vocab.thumb.o wizard.thumb.o -lgcc
	$(ARMGNU)-ld -T memmap -o adventure.gcc.thumb.elf vectors.o uart.thumb.o crc.thumb.o data.thumb.o done.thumb.o init.thumb.o io.thumb.o main.thumb.o show.thumb.o subr.thumb.o vocab.thumb.o wizard.thumb.o gcclib/_divsi3.o gcclib/_udivsi3.o gcclib/_thumb1_case_uqi.o gcclib/_thumb1_case_shi.o
	$(ARMGNU)-objdump -D adventure.gcc.thumb.elf > adventure.gcc.thumb.list


uart.thumb.o : uart.c
	$(ARMGNU)-gcc -c $(COPS) uart.c -o uart.thumb.o

crc.thumb.o : crc.c
	$(ARMGNU)-gcc -c $(COPS) crc.c -o crc.thumb.o

data.thumb.o : data.c
	$(ARMGNU)-gcc -c $(COPS) data.c -o data.thumb.o

done.thumb.o : done.c
	$(ARMGNU)-gcc -c $(COPS) done.c -o done.thumb.o

init.thumb.o : init.c
	$(ARMGNU)-gcc -c $(COPS) init.c -o init.thumb.o

io.thumb.o : io.c
	$(ARMGNU)-gcc -c $(COPS) io.c -o io.thumb.o

main.thumb.o : main.c
	$(ARMGNU)-gcc -c $(COPS) main.c -o main.thumb.o

show.thumb.o : show.c
	$(ARMGNU)-gcc -c $(COPS) show.c -o show.thumb.o

subr.thumb.o : subr.c
	$(ARMGNU)-gcc -c $(COPS) subr.c -o subr.thumb.o

vocab.thumb.o : vocab.c
	$(ARMGNU)-gcc -c $(COPS) vocab.c -o vocab.thumb.o

wizard.thumb.o : wizard.c
	$(ARMGNU)-gcc -c $(COPS) wizard.c -o wizard.thumb.o



adventure.gcc.thumb2.bin : adventure.gcc.thumb2.elf
	$(ARMGNU)-objcopy adventure.gcc.thumb2.elf -O binary adventure.gcc.thumb2.bin

adventure.gcc.thumb2.elf : vectors.o uart.thumb2.o crc.thumb2.o data.thumb2.o done.thumb2.o init.thumb2.o io.thumb2.o main.thumb2.o show.thumb2.o subr.thumb2.o vocab.thumb2.o wizard.thumb2.o
	#$(ARMGNU)-gcc $(COPS2) -Xlinker memmap -o adventure.gcc.thumb2.elf vectors.o uart.thumb2.o crc.thumb2.o data.thumb2.o done.thumb2.o init.thumb2.o io.thumb2.o main.thumb2.o show.thumb2.o subr.thumb2.o vocab.thumb2.o wizard.thumb2.o -lgcc
	$(ARMGNU)-ld -T memmap -o adventure.gcc.thumb2.elf vectors.o uart.thumb2.o crc.thumb2.o data.thumb2.o done.thumb2.o init.thumb2.o io.thumb2.o main.thumb2.o show.thumb2.o subr.thumb2.o vocab.thumb2.o wizard.thumb2.o
	$(ARMGNU)-objdump -D adventure.gcc.thumb2.elf > adventure.gcc.thumb2.list


uart.thumb2.o : uart.c
	$(ARMGNU)-gcc -c $(COPS2) uart.c -o uart.thumb2.o

crc.thumb2.o : crc.c
	$(ARMGNU)-gcc -c $(COPS2) crc.c -o crc.thumb2.o

data.thumb2.o : data.c
	$(ARMGNU)-gcc -c $(COPS2) data.c -o data.thumb2.o

done.thumb2.o : done.c
	$(ARMGNU)-gcc -c $(COPS2) done.c -o done.thumb2.o

init.thumb2.o : init.c
	$(ARMGNU)-gcc -c $(COPS2) init.c -o init.thumb2.o

io.thumb2.o : io.c
	$(ARMGNU)-gcc -c $(COPS2) io.c -o io.thumb2.o

main.thumb2.o : main.c
	$(ARMGNU)-gcc -c $(COPS2) main.c -o main.thumb2.o

show.thumb2.o : show.c
	$(ARMGNU)-gcc -c $(COPS2) show.c -o show.thumb2.o

subr.thumb2.o : subr.c
	$(ARMGNU)-gcc -c $(COPS2) subr.c -o subr.thumb2.o

vocab.thumb2.o : vocab.c
	$(ARMGNU)-gcc -c $(COPS2) vocab.c -o vocab.thumb2.o

wizard.thumb2.o : wizard.c
	$(ARMGNU)-gcc -c $(COPS2) wizard.c -o wizard.thumb2.o



uart.clang.bc : uart.c  
	clang $(LOPS) -c uart.c -o uart.clang.bc

crc.clang.bc : crc.c  
	clang $(LOPS) -c crc.c -o crc.clang.bc

data.clang.bc : data.c  
	clang $(LOPS) -c data.c -o data.clang.bc

done.clang.bc : done.c  
	clang $(LOPS) -c done.c -o done.clang.bc

init.clang.bc : init.c  
	clang $(LOPS) -c init.c -o init.clang.bc

io.clang.bc : io.c  
	clang $(LOPS) -c io.c -o io.clang.bc

main.clang.bc : main.c  
	clang $(LOPS) -c main.c -o main.clang.bc

show.clang.bc : show.c  
	clang $(LOPS) -c show.c -o show.clang.bc

subr.clang.bc : subr.c  
	clang $(LOPS) -c subr.c -o subr.clang.bc

vocab.clang.bc : vocab.c  
	clang $(LOPS) -c vocab.c -o vocab.clang.bc

wizard.clang.bc : wizard.c  
	clang $(LOPS) -c wizard.c -o wizard.clang.bc

adventure.clang.bc : crc.clang.bc data.clang.bc done.clang.bc init.clang.bc io.clang.bc main.clang.bc show.clang.bc subr.clang.bc vocab.clang.bc wizard.clang.bc uart.clang.bc
	llvm-link crc.clang.bc data.clang.bc done.clang.bc init.clang.bc io.clang.bc main.clang.bc show.clang.bc subr.clang.bc vocab.clang.bc wizard.clang.bc uart.clang.bc -o adventure.clang.bc


adventure.clang.thumb.norm.bin : memmap vectors.o adventure.clang.bc
	llc $(LLCOPS) adventure.clang.bc -o adventure.clang.thumb.norm.s
	$(ARMGNU)-as adventure.clang.thumb.norm.s -o adventure.clang.thumb.norm.o
	$(ARMGNU)-ld -o adventure.clang.thumb.norm.elf -T memmap vectors.o adventure.clang.thumb.norm.o gcclib/_modsi3.o gcclib/_divsi3.o gcclib/_umodsi3.o
	#$(ARMGNU)-gcc -o adventure.clang.thumb.norm.elf -Xlinker memmap vectors.o adventure.clang.thumb.norm.o -lgcc
	$(ARMGNU)-objdump -D adventure.clang.thumb.norm.elf > adventure.clang.thumb.norm.list
	$(ARMGNU)-objcopy adventure.clang.thumb.norm.elf adventure.clang.thumb.norm.bin -O binary

adventure.clang.thumb.opt.bin : memmap vectors.o adventure.clang.bc
	opt $(OOPS) adventure.clang.bc -o adventure.clang.thumb.opt.bc
	llc $(LLCOPS) adventure.clang.thumb.opt.bc -o adventure.clang.thumb.opt.s
	$(ARMGNU)-as adventure.clang.thumb.opt.s -o adventure.clang.thumb.opt.o
	$(ARMGNU)-ld -o adventure.clang.thumb.opt.elf -T memmap vectors.o adventure.clang.thumb.opt.o gcclib/_modsi3.o gcclib/_divsi3.o gcclib/_umodsi3.o
	#$(ARMGNU)-gcc -o adventure.clang.thumb.opt.elf -Xlinker memmap vectors.o adventure.clang.thumb.opt.o -lgcc
	$(ARMGNU)-objdump -D adventure.clang.thumb.opt.elf > adventure.clang.thumb.opt.list
	$(ARMGNU)-objcopy adventure.clang.thumb.opt.elf adventure.clang.thumb.opt.bin -O binary

adventure.clang.thumb2.norm.bin : memmap vectors.o adventure.clang.bc
	llc $(LLCOPS2) adventure.clang.bc -o adventure.clang.thumb2.norm.s
	$(ARMGNU)-as adventure.clang.thumb2.norm.s -o adventure.clang.thumb2.norm.o
	$(ARMGNU)-ld -o adventure.clang.thumb2.norm.elf -T memmap vectors.o adventure.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D adventure.clang.thumb2.norm.elf > adventure.clang.thumb2.norm.list
	$(ARMGNU)-objcopy adventure.clang.thumb2.norm.elf adventure.clang.thumb2.norm.bin -O binary

adventure.clang.thumb2.opt.bin : memmap vectors.o adventure.clang.bc
	opt $(OOPS) adventure.clang.bc -o adventure.clang.thumb2.opt.bc
	llc $(LLCOPS2) adventure.clang.thumb2.opt.bc -o adventure.clang.thumb2.opt.s
	$(ARMGNU)-as adventure.clang.thumb2.opt.s -o adventure.clang.thumb2.opt.o
	$(ARMGNU)-ld -o adventure.clang.thumb2.opt.elf -T memmap vectors.o adventure.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D adventure.clang.thumb2.opt.elf > adventure.clang.thumb2.opt.list
	$(ARMGNU)-objcopy adventure.clang.thumb2.opt.elf adventure.clang.thumb2.opt.bin -O binary








