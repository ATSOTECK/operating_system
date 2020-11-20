@echo off

mkbootimg.exe bb.json os.img

qemu-system-x86_64 -hda os.img