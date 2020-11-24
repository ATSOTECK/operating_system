@echo off

nasm boot.asm -f bin -o boot.bin
REM partcopy boot.bin 0 200 -f0