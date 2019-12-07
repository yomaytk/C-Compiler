.intel_syntax noprefix
.globl main

x:
  .zero 4
y:
  .zero 80
main:
  push rbp
  mov rbp, rsp
  sub rsp, 296
  mov QWORD PTR [rbp-8], OFFSET FLAT:y+8
  lea rax, [rbp-416]
  add rax, 16
  mov QWORD PTR [rbp-16], rax
  mov edx, DWORD PTR y[rip+20]
  mov eax, DWORD PTR x[rip]
  add eax, edx
  leave
  ret