[ORG 0x7c00]
    xor ax, ax ; make it zero
    mov ds, ax
    cld
    
    mov si, msg
    call bios_print

hang:
    ;call bios_print
    jmp hang

msg db 'Hello World', 13, 10, 0

bios_print:
    lodsb
    or al, al ; zero = end of string
    jz hang
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    jmp bios_print
done:
    ret

    times 510-($-$$) db 0
    db 0x55 ; Legacy boot signature.
    db 0xAA