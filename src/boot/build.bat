@echo off

nasm boot.asm -f bin -o boot.bin
partcopy boot.bin 0 200 -f0