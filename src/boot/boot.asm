[org 0x7C00]                                ;Where the code will be loaded in memory
    
    ;%include "file.asm"
    
    mov [.bootDrive], dl                    ;BIOS stores boot drive in dl
    
    mov bp, 0x8000                          ;move the stack out of the way
    mov sp, bp
    
    ;mov bx, .hello
    ;call print
    
    ;mov bx, .gbye
    ;call print
    
    mov bx, 0x9000                          ;load 5 sectors to 0x0000(es) : 0x9000(bx)
    mov dh, 5                               ;from the boot disk
    mov dl, [.bootDrive]
    call disk_load
    
    mov dx, [0x9000]                        ;print out first loaded word, should be 0xDADA
    call print_hex
    
    mov dx, [0x9000 + 512]                  ;print out second loaded word, should be 0xFACE
    call print_hex
    
    jmp $
    
print:
    pusha                                   ;push registers on to the stack
    mov ah, 0x0E                            ;int  10/ah = 0eh -> scrolling  teletype  BIOS  routine
    mov al, [bx]                            ;set ah to address of bx

.print_loop:
    cmp al, 0x0                             ;0 indicates end of string
    je .print_done                          ;goto print_done if 0
    
    int 0x10                                ;print the current character
    add bx, 0x01                            ;move to the next character
    mov al, [bx]                            ;set ah to address of bx
    jmp .print_loop                         ;keep printing the string
    
.print_done:
    popa                                    ;pop registers
    ret                                     ;return
    
print_hex:
    pusha                                   ;push registers on to the stack
    mov bx, .hex                            ;
    call print                              ;print "0x"
    mov bx, dx                              ;use bx as a base register
    mov cx, 12                              ;counter, can do 4 chars
    
.hex_loop:
    push bx                                 ;push bx on to the stack
    sar bx, cl                              ;?
    and bx, 0xF                             ;?
    mov ax, [.hexdigits + bx]               ;select the hex character
    mov [.char], al                         ;put this in .char
    mov bx, .char                           ;
    call print                              ;print the char
    
    pop bx                                  ;restore bx
    sub cx, 4                               ;
    cmp cx, 0                               ;
    jge .hex_loop                           ;
    
.hex_done:
    popa                                    ;pop registers
    ret                                     ;return
    
.hexdigits: db '0123456789ABCDEF', 0
.char db "0", 0
.hex: db "0x", 0
    
disk_load:
    push dx                                 ;backup dx
    
    mov ah, 0x02                            ;BIOS read sector function
    mov al, dh                              ;read dh sectors
    mov ch, 0x00                            ;select cylinder 0
    mov dh, 0x00                            ;select head 0
    mov cl, 0x02                            ;start reading from 2nd sector (after boot sector)
    
    int 0x13                                ;BIOS
    jc .disk_read_error                     ;jump if error reading disk
    
    pop dx                                  ;restore dx
    cmp dh, al                              ;if al sectors read != dh sectors requested
    jne .disk_error_sector_count            ;error
    ret
    
.disk_read_error:
    mov bx, .diskReadError
    call print
    jmp $                                   ;just hang after error
    
.disk_error_sector_count:
    mov bx, .diskSectorCountError
    call print
    jmp $                                   ;just hang after error     
    
.bootDrive: db 0
.hello: db "Hello, world! ", 0
.gbye: db "Goodbye cruel world! ", 0

.diskReadError: db "Disk read error!", 0
.diskSectorCountError: db "Sector count mismatch!", 0
    
    times 510-($-$$) db 0                   ;fill with 0s to make sure it is 512 bytes
    db 0x55                                 ; Legacy boot signature.
    db 0xAA
    
    ;extra data after boot sector to test disk reading
    times 256 dw 0xDADA
    times 256 dw 0xFACE