
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
GTHUMB = -mthumb
GTHUMB2 = -mthumb -mcpu=cortex-m3 -march=armv7-m

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts



all : all_gcc all_llvm


all_gcc : all_gcc_rom all_gcc_ram

all_gcc_rom : thread02.gcc.thumb.rom.bin thread02.gcc.thumb2.rom.bin

all_gcc_ram : thread02.gcc.thumb.ram.bin thread02.gcc.thumb2.ram.bin

all_llvm : all_llvm_rom all_llvm_ram

all_llvm_rom : thread02.clang.thumb.norm.rom.bin thread02.clang.thumb.opt.rom.bin thread02.clang.thumb2.norm.rom.bin thread02.clang.thumb2.opt.rom.bin

all_llvm_ram : thread02.clang.thumb.norm.ram.bin thread02.clang.thumb.opt.ram.bin thread02.clang.thumb2.norm.ram.bin thread02.clang.thumb2.opt.ram.bin


vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

novectors.o : novectors.s
	$(ARMGNU)-as $(AOPS) novectors.s -o novectors.o


thread02.gcc.thumb.rom.bin : rommap vectors.o sched.c threada.c threadb.c threadc.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c sched.c -o sched.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threada.c -o threada.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadb.c -o threadb.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadc.c -o threadc.gcc.thumb.o
	$(ARMGNU)-ld -o thread02.gcc.thumb.rom.elf -T rommap vectors.o sched.gcc.thumb.o threada.gcc.thumb.o threadb.gcc.thumb.o threadc.gcc.thumb.o
	$(ARMGNU)-objdump -D thread02.gcc.thumb.rom.elf > thread02.gcc.thumb.rom.list
	$(ARMGNU)-objcopy thread02.gcc.thumb.rom.elf thread02.gcc.thumb.rom.bin -O binary

thread02.gcc.thumb2.rom.bin : rommap vectors.o sched.c threada.c threadb.c threadc.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c sched.c -o sched.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threada.c -o threada.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadb.c -o threadb.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadc.c -o threadc.gcc.thumb2.o
	$(ARMGNU)-ld -o thread02.gcc.thumb2.rom.elf -T rommap vectors.o sched.gcc.thumb2.o threada.gcc.thumb2.o threadb.gcc.thumb2.o threadc.gcc.thumb2.o
	$(ARMGNU)-objdump -D thread02.gcc.thumb2.rom.elf > thread02.gcc.thumb2.rom.list
	$(ARMGNU)-objcopy thread02.gcc.thumb2.rom.elf thread02.gcc.thumb2.rom.bin -O binary


thread02.gcc.thumb.ram.bin : rammap novectors.o sched.c threada.c threadb.c threadc.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c sched.c -o sched.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threada.c -o threada.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadb.c -o threadb.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadc.c -o threadc.gcc.thumb.o
	$(ARMGNU)-ld -o thread02.gcc.thumb.ram.elf -T rammap novectors.o sched.gcc.thumb.o threada.gcc.thumb.o threadb.gcc.thumb.o threadc.gcc.thumb.o
	$(ARMGNU)-objdump -D thread02.gcc.thumb.ram.elf > thread02.gcc.thumb.ram.list
	$(ARMGNU)-objcopy thread02.gcc.thumb.ram.elf thread02.gcc.thumb.ram.bin -O binary

thread02.gcc.thumb2.ram.bin : rammap novectors.o sched.c threada.c threadb.c threadc.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c sched.c -o sched.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threada.c -o threada.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadb.c -o threadb.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadc.c -o threadc.gcc.thumb2.o
	$(ARMGNU)-ld -o thread02.gcc.thumb2.ram.elf -T rammap novectors.o sched.gcc.thumb2.o threada.gcc.thumb2.o threadb.gcc.thumb2.o threadc.gcc.thumb2.o
	$(ARMGNU)-objdump -D thread02.gcc.thumb2.ram.elf > thread02.gcc.thumb2.ram.list
	$(ARMGNU)-objcopy thread02.gcc.thumb2.ram.elf thread02.gcc.thumb2.ram.bin -O binary







thread02.clang.bc : sched.c threada.c threadb.c threadc.c
	clang $(LOPS) -c sched.c -o sched.clang.bc
	clang $(LOPS) -c threada.c -o threada.clang.bc
	clang $(LOPS) -c threadb.c -o threadb.clang.bc
	clang $(LOPS) -c threadc.c -o threadc.clang.bc
	llvm-link sched.clang.bc threada.clang.bc threadb.clang.bc threadc.clang.bc -o thread02.clang.bc

thread02.clang.thumb.norm.rom.bin : rommap vectors.o thread02.clang.bc
	llc $(LLCOPS) thread02.clang.bc -o thread02.clang.thumb.norm.s
	$(ARMGNU)-as thread02.clang.thumb.norm.s -o thread02.clang.thumb.norm.o
	$(ARMGNU)-ld -o thread02.clang.thumb.norm.rom.elf -T rommap vectors.o thread02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D thread02.clang.thumb.norm.rom.elf > thread02.clang.thumb.norm.rom.list
	$(ARMGNU)-objcopy thread02.clang.thumb.norm.rom.elf thread02.clang.thumb.norm.rom.bin -O binary

thread02.clang.thumb.opt.rom.bin : rommap vectors.o thread02.clang.bc
	opt $(OOPS) thread02.clang.bc -o thread02.clang.thumb.opt.bc
	llc $(LLCOPS) thread02.clang.thumb.opt.bc -o thread02.clang.thumb.opt.s
	$(ARMGNU)-as thread02.clang.thumb.opt.s -o thread02.clang.thumb.opt.o
	$(ARMGNU)-ld -o thread02.clang.thumb.opt.rom.elf -T rommap vectors.o thread02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D thread02.clang.thumb.opt.rom.elf > thread02.clang.thumb.opt.rom.list
	$(ARMGNU)-objcopy thread02.clang.thumb.opt.rom.elf thread02.clang.thumb.opt.rom.bin -O binary

thread02.clang.thumb2.norm.rom.bin : rommap vectors.o thread02.clang.bc
	llc $(LLCOPS2) thread02.clang.bc -o thread02.clang.thumb2.norm.s
	$(ARMGNU)-as thread02.clang.thumb2.norm.s -o thread02.clang.thumb2.norm.o
	$(ARMGNU)-ld -o thread02.clang.thumb2.norm.rom.elf -T rommap vectors.o thread02.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D thread02.clang.thumb2.norm.rom.elf > thread02.clang.thumb2.norm.rom.list
	$(ARMGNU)-objcopy thread02.clang.thumb2.norm.rom.elf thread02.clang.thumb2.norm.rom.bin -O binary

thread02.clang.thumb2.opt.rom.bin : rommap vectors.o thread02.clang.bc
	opt $(OOPS) thread02.clang.bc -o thread02.clang.thumb2.opt.bc
	llc $(LLCOPS2) thread02.clang.thumb2.opt.bc -o thread02.clang.thumb2.opt.s
	$(ARMGNU)-as thread02.clang.thumb2.opt.s -o thread02.clang.thumb2.opt.o
	$(ARMGNU)-ld -o thread02.clang.thumb2.opt.rom.elf -T rommap vectors.o thread02.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D thread02.clang.thumb2.opt.rom.elf > thread02.clang.thumb2.opt.rom.list
	$(ARMGNU)-objcopy thread02.clang.thumb2.opt.rom.elf thread02.clang.thumb2.opt.rom.bin -O binary



thread02.clang.thumb.norm.ram.bin : rammap novectors.o thread02.clang.bc
	llc $(LLCOPS) thread02.clang.bc -o thread02.clang.thumb.norm.s
	$(ARMGNU)-as thread02.clang.thumb.norm.s -o thread02.clang.thumb.norm.o
	$(ARMGNU)-ld -o thread02.clang.thumb.norm.ram.elf -T rammap novectors.o thread02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D thread02.clang.thumb.norm.ram.elf > thread02.clang.thumb.norm.ram.list
	$(ARMGNU)-objcopy thread02.clang.thumb.norm.ram.elf thread02.clang.thumb.norm.ram.bin -O binary

thread02.clang.thumb.opt.ram.bin : rammap novectors.o thread02.clang.bc
	opt $(OOPS) thread02.clang.bc -o thread02.clang.thumb.opt.bc
	llc $(LLCOPS) thread02.clang.thumb.opt.bc -o thread02.clang.thumb.opt.s
	$(ARMGNU)-as thread02.clang.thumb.opt.s -o thread02.clang.thumb.opt.o
	$(ARMGNU)-ld -o thread02.clang.thumb.opt.ram.elf -T rammap novectors.o thread02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D thread02.clang.thumb.opt.ram.elf > thread02.clang.thumb.opt.ram.list
	$(ARMGNU)-objcopy thread02.clang.thumb.opt.ram.elf thread02.clang.thumb.opt.ram.bin -O binary

thread02.clang.thumb2.norm.ram.bin : rammap novectors.o thread02.clang.bc
	llc $(LLCOPS2) thread02.clang.bc -o thread02.clang.thumb2.norm.s
	$(ARMGNU)-as thread02.clang.thumb2.norm.s -o thread02.clang.thumb2.norm.o
	$(ARMGNU)-ld -o thread02.clang.thumb2.norm.ram.elf -T rammap novectors.o thread02.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D thread02.clang.thumb2.norm.ram.elf > thread02.clang.thumb2.norm.ram.list
	$(ARMGNU)-objcopy thread02.clang.thumb2.norm.ram.elf thread02.clang.thumb2.norm.ram.bin -O binary

thread02.clang.thumb2.opt.ram.bin : rammap novectors.o thread02.clang.bc
	opt $(OOPS) thread02.clang.bc -o thread02.clang.thumb2.opt.bc
	llc $(LLCOPS2) thread02.clang.thumb2.opt.bc -o thread02.clang.thumb2.opt.s
	$(ARMGNU)-as thread02.clang.thumb2.opt.s -o thread02.clang.thumb2.opt.o
	$(ARMGNU)-ld -o thread02.clang.thumb2.opt.ram.elf -T rammap novectors.o thread02.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D thread02.clang.thumb2.opt.ram.elf > thread02.clang.thumb2.opt.ram.list
	$(ARMGNU)-objcopy thread02.clang.thumb2.opt.ram.elf thread02.clang.thumb2.opt.ram.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


