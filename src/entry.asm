[bits 32]
[extern kmain]
[extern cls]
[extern print]
    call kmain
    
    call cls
    
    mov ebx, .msgDone
    call print
    
    jmp $
    
.msgDone db "done.", 0