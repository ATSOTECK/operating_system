[org 0x7C00]                                ;Where the code will be loaded in memory
kernelOffset equ 0x1000                     ;memory offset to load kernel

    mov [bootDrive], dl                    ;BIOS stores boot drive in dl

    mov bp, 0x9000                          ;set the stack
    mov sp, bp
    
    mov bx, msgRealMode
    call println_bios
    
    call load_kernel
    
    call switch_to_pm
    
    jmp $

;%include "print_bios.asm"
;%include "disk.asm"
;%include "gdt.asm"
;%include "print.asm"
;%include "pm.asm"
    
%include "src/boot/print_bios.asm"
%include "src/boot/disk.asm"
%include "src/boot/gdt.asm"
%include "src/boot/print.asm"
%include "src/boot/pm.asm"

[bits 16]
load_kernel:
    mov bx, msgLoadKernel
    call println_bios
    
    mov bx, kernelOffset
    mov dh, 15
    mov dl, [bootDrive]
    call disk_load
    
    mov bx, msgLoadedKernel
    call println_bios
    
    ret

[bits 32]
[extern kmain]
begin_pm:
    call cls

    mov ebx, msgProtMode
    call print
    
    call kernelOffset
    
    jmp $

bootDrive db 0
msgLoadKernel db "Loading kernel...", 0
msgLoadedKernel db "Loaded kernel", 0
msgRealMode db "Boot 16-bit real mode", 0
msgProtMode db "Enter 32-bit protected mode, calling kernel...", 0
    
    times 510-($-$$) db 0                   ;fill with 0s to make sure it is 512 bytes
    db 0x55                                 ;last 2 bytes are legacy boot signature
    db 0xAA