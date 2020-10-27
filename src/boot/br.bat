@echo off

nasm boot.asm -f bin -o boot.bin
partcopy boot.bin 0 200 -f0

qemu-system-i386 -hda boot.bin