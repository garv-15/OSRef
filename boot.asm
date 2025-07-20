[BITS 16]
[org 0x7c00]

boot:
	mov si, welcomet
	mov ah, 0x0e
    jmp welcome

welcome:
	lodsb
	or al, al
	jz welcome_nl
	int 0x10
	jmp welcome 

welcome_nl:
    mov al, 13
    int 0x10
    mov al, 10
    int 0x10
    mov si, prompt
    jmp inputprompt


inputprompt: 
    lodsb 
    or al, al
    jz afprompt
    int 0x10
    jmp inputprompt

afprompt:
    
    jmp next

next:
	mov ax, 0
    mov ds, ax
	mov es, ax
	mov di, buffer
    jmp read

read:
    mov ah, 0
    int 0x16         
    cmp al, 13       
    je printnl
    stosb       
    mov ah, 0x0e
    int 0x10             
    jmp read

printnl:
    mov al, 0
    stosb
    jmp printbuf

printbuf:
    mov ah, 0x0e
    mov al, 13
    int 0x10
    mov al, 10
    int 0x10

    mov si, buffer

.next:
    lodsb
    test al, al
    jz halt
    mov ah, 0x0E
    int 0x10
    jmp .next

halt:
    jmp $

welcomet: db "Welcome", 0
prompt: db "Enter your name: ", 0
buffer: times 64 db 0
TIMES 510 - ($ - $$) db 0
dw 0xaa55
