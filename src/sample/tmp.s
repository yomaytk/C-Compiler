.intel_syntax noprefix
.global main

.text

main:
  push rbp
  mov rbp, rsp
  sub rsp, 100
  mov rax, rbp
  sub rax, 4
  mov rdi, 10
  mov [rax], rdi
  mov rax, [rax]
  push rax
  mov rdi, 2
  add rax, rdi
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
