.intel_syntax noprefix
.global main

.data
y:
  .zero 80

x:
  .zero 4
.text
main:
  push rbp
  mov rbp, rsp
  lea rax, y
  add rax, 8
  mov rdi, 9
  mov [rax], rdi
  mov rax, [rax]
  push rax
  lea rax, y
  add rax, 16
  mov rdi, 7
  mov [rax], rdi
  mov rax, [rax]
  push rax
  pop rdi
  pop rax
  add rax, rdi
  push rax
  lea r10, x
  mov rdi, 3
  mov [r10], rdi
  mov rax, [r10]
  pop rdi
  add rax, rdi
  mov rsp, rbp
  pop rbp
  ret
