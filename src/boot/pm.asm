[bits 16]
switch_to_pm:
    cli                                                     ;diable interrupts
    lgdt [gdt_descriptor]                                   ;load gdt
    
    mov eax, cr0                                            ;set first bit of cr0 to
    or eax, 0x1                                             ;get to protected mode
    mov cr0, eax
    
    jmp codeSeg:init_pm                                     ;make a far jump to flush pipeline
    
[bits 32]
init_pm:
    mov ax, dataSeg                                         ;point to new segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ebp, 0x90000                                        ;update stack position
    mov esp, ebp
    
    call begin_pm