gdt_start:

gdt_null:                                       ;mandatory null desciptor
    dd 0x0                                      ;dd means double word
    dd 0x0
    
gdt_code:                                       ;code esgment descriptor
    dw 0xffff                                   ;limit (bits 0 - 15)
    dw 0x0                                      ;base (bits 0 - 15)
    db 0x0                                      ;base (bits 16 - 23)
    db 10011010b                                ;type flags
    db 11001111b                                ;limit flags (bits 16 - 19)
    db 0x0                                      ;base (bits 24 - 31)
    
gdt_data:                                       ;data segment descriptor
    dw 0xffff                                   ;limit (bits 0 - 15)
    dw 0x0                                      ;base (bits 0 - 15)
    db 0x0                                      ;base (bits 16 - 23)
    db 10010010b                                ;type flags
    db 11001111b                                ;limit flags (bits 16 - 19)
    db 0x0                                      ;base (bits 24 - 31)
    
gdt_end:                                        ;used to calculate size of gdt

gdt_descriptor:
    dw gdt_end - gdt_start - 1                  ;size of gdt
    dd gdt_start                                ;start address
    
codeSeg equ gdt_code - gdt_start
dataSeg equ gdt_data - gdt_start
