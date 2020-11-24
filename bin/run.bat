@echo off

REM qemu-system-x86_64 -vga std -hda os.bin
qemu-system-x86_64 -blockdev driver=file,node-name=f0,filename=os.bin -device floppy,drive=f0