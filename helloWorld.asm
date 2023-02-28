global _start

section .text

_start:
    mov rax, 1 ; write syscall
    mov rdi, 1 ; stdout = 1
    mov rsi, msg ; msg = "Hello World!"
    mov rdx, msgLen ; msgLen = strlen("Hello World!")
    syscall
    mov rax, 60 ; exit syscall
    mov rdi, 0 ; successful exit status
    syscall

section .rodata
    msg: db "Hello World!", 0x0a
    msgLen: equ $ - msg
