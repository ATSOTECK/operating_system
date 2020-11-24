GCCFLAGS = -m32 -fno-pie -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I./inc/

all: boot kernel install

boot: src/boot/boot.asm src/boot/kernel_entry.asm
	nasm src/boot/boot.asm -f bin -o bin/boot.bin
	nasm src/boot/kernel_entry.asm -f elf -o bin/kernel_entry.o

kernel: src/kmain.c
	gcc $(GCCFLAGS) -c src/kmain.c -o bin/kernel.o
	
install:
	ld -n -m elf_i386 -o bin/kernel.bin -Ttext 0x1000 --oformat binary --strip-all \
	bin/kernel_entry.o bin/kernel.o
	cat bin/boot.bin bin/kernel.bin > bin/os.bin