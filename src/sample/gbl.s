.intel_syntax noprefix
.globl main

.bss
x:
    .zero 8
.bss
y:
    .zero 80

.text
main:
    push    rbp
    mov     rbp, rsp
    lea rax, x
    mov rdi, 9
    mov [rax], rdi
    mov rax, [rax]
    push rax
    lea rax, x
    add rax, 4
    mov rdi, 7
    mov [rax], rdi
    mov rax, [rax]
    pop rdi
    add rax, rdi
    mov rsp, rbp
    pop rbp
    ret
