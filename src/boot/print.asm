[bits 32]
videoMem equ 0xb8000
txtColor equ 0x4F                               ;white on red

print:
    pusha
    mov edx, videoMem                           ;set edx to start of video memory

.print_loop:
    mov al, [ebx]                               ;store the char at ebx in al
    mov ah, txtColor                            ;store attrib in ah
    
    cmp al, 0                                   ;0 indicates end of string
    je .print_done                              ;goto .print_done if 0
    
    mov [edx], ax                               ;store the char in the current cell
    add ebx, 1                                  ;inc ebx to next char
    add edx, 2                                  ;inc edx to next cell
    
    jmp .print_loop

.print_done:
    popa
    ret