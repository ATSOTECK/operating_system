[bits 32]
videoMem equ 0xB8000
txtColor equ 0x4F                               ;white on red

print:
    pusha
    
.print_pos:
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
    
cls:
    pusha
    mov dword [videoMem], 0xB8000
    mov eax, 0xFFFF
    
.cls_loop:
    dec eax
    cmp eax, 0
    je .cls_done
    
    mov word[videoMem + eax], 0
    jmp .cls_loop
    
.cls_done:
    popa
    ret