.intel_syntax noprefix
.bss
a:
	.zero 8
.bss
b:
	.zero 8
.bss
c:
	.zero 8
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	push	8
	push	8
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	8
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
