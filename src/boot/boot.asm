[org 0x7C00]                                ;Where the code will be loaded in memory
    mov bp, 0x9000                          ;set the stack
    mov sp, bp
    
    mov bx, msgRealMode
    call print_bios
    
    call switch_to_pm
    
    jmp $
    
%include "print_bios.asm"
%include "gdt.asm"
%include "print.asm"
%include "pm.asm"

[bits 32]

begin_pm:
    mov ebx, msgProtMode
    call print
    
    jmp $

msgRealMode db "Boot 16-bit real mode", 0
msgProtMode db "Entered 32-bit protected mode", 0
    
    times 510-($-$$) db 0                   ;fill with 0s to make sure it is 512 bytes
    db 0x55                                 ;last 2 bytes are legacy boot signature
    db 0xAA