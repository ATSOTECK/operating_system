@echo off

nasm boot.asm -f bin -o boot.bin

REM qemu-system-x86_64 -vga std -hda boot.bin
qemu-system-x86_64 -blockdev driver=file,node-name=f0,filename=boot.bin -device floppy,drive=f0