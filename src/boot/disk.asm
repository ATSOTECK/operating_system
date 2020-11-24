;mov [.bootDrive], dl                    ;BIOS stores boot drive in dl

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
    call println_bios
    jmp $                                   ;just hang after error
    
.disk_error_sector_count:
    mov bx, .diskSectorCountError
    call println_bios
    jmp $                                   ;just hang after error     
    
.diskReadError: db "Disk read error!", 0
.diskSectorCountError: db "Sector count mismatch!", 0

;extra data after boot sector to test disk reading
    ;times 256 dw 0xDADA
    ;times 256 dw 0xFACE