.intel_syntax noprefix
.global main

x:
  .zero 4
y:
  .zero 80
main:
  mov DWORD PTR y[rip+20], 5
  mov DWORD PTR x[rip], 7
  mov edx, DWORD PTR y[rip+20]
  mov eax, DWORD PTR x[rip]
  add eax, edx
  ret