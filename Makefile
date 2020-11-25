GCCFLAGS = -m32 -fno-pie -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I./inc/

all: boot kernel install clean

boot: src/boot/boot.asm src/entry.asm
	nasm src/boot/boot.asm -f bin -o bin/boot.bin
	nasm src/entry.asm -f elf -o bin/entry.o

kernel: src/kmain.c
	gcc $(GCCFLAGS) -c src/kmain.c -o bin/kernel.o
	gcc $(GCCFLAGS) -c src/mem.c -o bin/mem.o
	gcc $(GCCFLAGS) -c src/port.c -o bin/port.o
	ld -n -m elf_i386 -nostdlib -nostartfiles \
	-o bin/kernel.bin -Ttext 0x1000 --oformat binary --strip-all \
	bin/*.o
	
install:
	cat bin/boot.bin bin/kernel.bin > bin/os.img
	
clean:
	rm bin/*.o bin/*.bin