.intel_syntax noprefix
.global main

.data
y:
  .zero 80

.text
main:
  push rbp
  mov rbp, rsp
  mov rax, y
  add rax, 8
  mov rdi, 9
  mov [rax], rdi
  mov rax, [rax]
  push rax
  mov rax, y
  add rax, 16
  mov rdi, 7
  mov [rax], rdi
  mov rax, [rax]
  push rax
  pop rdi
  pop rax
  add rax, rdi
  mov rsp, rbp
  pop rbp
  ret
