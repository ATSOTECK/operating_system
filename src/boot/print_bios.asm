print_bios:
    pusha                                   ;push registers on to the stack
    mov ah, 0x0E                            ;int  10/ah = 0eh -> scrolling  teletype  BIOS  routine
    mov al, [bx]                            ;set ah to address of bx

.print_bios_loop:
    cmp al, 0x0                             ;0 indicates end of string
    je .print_bios_done                     ;goto print_done if 0
    
    int 0x10                                ;print the current character
    add bx, 0x01                            ;move to the next character
    mov al, [bx]                            ;set ah to address of bx
    jmp .print_bios_loop                    ;keep printing the string
    
.print_bios_done:
    popa                                    ;pop registers
    ret                                     ;return
    
print_bios_hex:
    pusha                                   ;push registers on to the stack
    mov bx, .hex                            ;
    call print_bios                         ;print "0x"
    mov bx, dx                              ;use bx as a base register
    mov cx, 12                              ;counter, can do 4 chars
    
.bios_hex_loop:
    push bx                                 ;push bx on to the stack
    sar bx, cl                              ;?
    and bx, 0xF                             ;?
    mov ax, [.hexdigits + bx]               ;select the hex character
    mov [.char], al                         ;put this in .char
    mov bx, .char                           ;
    call print_bios                         ;print the char
    
    pop bx                                  ;restore bx
    sub cx, 4                               ;
    cmp cx, 0                               ;
    jge .bios_hex_loop                      ;
    
.bios_hex_done:
    popa                                    ;pop registers
    ret                                     ;return
    
.hexdigits: db '0123456789ABCDEF', 0
.char db "0", 0
.hex: db "0x", 0