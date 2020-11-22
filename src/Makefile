CC = gcc
CFLAGS = -Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I../inc/

LIBFILES = lib/math.c lib/print.c

all: kernel clean

kernel: libs kmain.c
	gcc $(CFLAGS) -mno-red-zone -c kmain.c -o kernel.o
	ld -r -b binary -o font.o font.psf
	ld -nostdlib -nostartfiles -T link.ld kernel.o font.o math.o print.o -o kernel.elf
	strip -s -K mmio -K fb -K bootboot -K environment kernel.elf
	readelf -hls kernel.elf >kernel.txt
	cp kernel.elf ../out/initrd/sys/core
	
libs: $(LIBFILES)
	gcc $(CFLAGS) -c lib/math.c -o math.o
	gcc $(CFLAGS) -c lib/print.c -o print.o
	
clean:
	rm *.o *.elf *.txt