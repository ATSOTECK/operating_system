[bits 32]
[extern kmain]
    mov eax, 0xFEEDEEF
    call kmain
    jmp $