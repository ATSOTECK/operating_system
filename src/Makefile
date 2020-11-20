CC = gcc
CFLAGS = -Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I../inc/

all: kernel clean

kernel: kmain.c
	gcc $(CFLAGS) -mno-red-zone -c kmain.c -o kernel.o
	ld -r -b binary -o font.o font.psf
	ld -nostdlib -nostartfiles -T link.ld kernel.o font.o -o kernel.elf
	strip -s -K mmio -K fb -K bootboot -K environment kernel.elf
	readelf -hls kernel.elf >kernel.txt
	cp kernel.elf ../out/initrd/sys/core
	
clean:
	rm *.o *.elf *.txt