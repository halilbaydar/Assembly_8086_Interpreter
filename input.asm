code segment
   mov bx,offset dat2
   mov bp,bx
   dec bp
   add bx,2h
   mov w[bx],206fh
   add bx,2h
   mov w[bx],6f77h
   add bx,2h
   mov w[bx],6c72h
   inc bx
   mov b[bx],64h
   mov cx,0ah
   mov ah,02h
   dec bp
   mov bx,bp
more:
   mov dl,[bx]
   int 21h
   inc bx
   dec cx
   jnz more
   int 20h
dat1 dw 6568h
dat2 dw 6c6ch
code ends