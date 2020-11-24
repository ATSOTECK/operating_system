@echo off

qemu-system-x86_64 -blockdev driver=file,node-name=f0,filename=boot.bin -device floppy,drive=f0