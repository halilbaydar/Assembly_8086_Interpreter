code segment
rot:
    mov w [1234d], 0a103h
    mov b[1246d], 10d
    nop
    nop
    mov cx, w[1234d]
    rcl cx, 2h
    mov var1, cl
    mov bl, b[1246d]
    rcr bl, 1d
    mov var2, bl
print:
    mov ax, 0
    mov al, var1
    mov cl, 10d
    div cl
    mov dl, al
    mov var1, ah
    add dl,    '0'
    mov ah, 02h
    int 21h
    xor ax,ax
    mov al,   var1
    mov cl, 10d
    div cl
    mov dl, ah
    add dl, '0'
    mov ah, 02h
    int 21h
    mov dl, ' '
    int 21h
    mov dl, var2
    add dl, '0'
    int 21h
ex:
    int 20h
var1 db 0
var2 db 0
code ends