@echo off

qemu-system-x86_64 -vga std -hda os.img
REM qemu-system-x86_64 -blockdev driver=file,node-name=f0,filename=os.img -device floppy,drive=f0