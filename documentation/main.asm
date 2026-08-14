bits 64

section .data
    msg db "Hello, World! wdada", 0x0a  ; String with a newline character
    eee db "ajdwwad", 0x0a
    len equ $ - msg               ; Length of the string

section .text
    global _start

_start:
    mov rax, 1                    ; sys_write system call number
    mov rdi, 1                    ; file descriptor (1 = stdout)
    mov rsi, msg                  ; pointer to string
    mov rdx, len                  ; string length
    syscall                       ; call linux kernel

    mov rax, 60                   ; sys_exit system call number
    xor rdi, rdi                  ; exit code 0
    syscall                       ; call linux kernel
