CC = gcc
CFLAGS = -Wall -m64 -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib \
	-I../inc/ -mno-red-zone -O0 -mno-mmx -mno-80387

LIBFILES = lib/math.c lib/print.c

all: kernel install clean

kernel: libs font kmain.c
	gcc $(CFLAGS) -c kmain.c -o kernel.o
	ld -nostdlib -nostartfiles -T link.ld kernel.o math.o print.o font.o -o kernel.elf
	
font: font.psf
	ld -r -b binary -o font.o font.psf
	
libs: $(LIBFILES)
	gcc $(CFLAGS) -c lib/math.c -o math.o
	gcc $(CFLAGS) -c lib/print.c -o print.o
	
install:
	strip -s -K mmio -K fb -K bootboot -K environment kernel.elf
	readelf -hls kernel.elf >kernel.txt
	cp kernel.elf ../out/initrd/sys/core
	
clean:
	rm *.o *.elf *.txt