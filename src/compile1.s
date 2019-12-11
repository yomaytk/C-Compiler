.intel_syntax noprefix
.bss
b:
	.zero 800
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	lea	rax, b
	push	rax
	push	0
	pop	rdi
	pop	rax
	imul	rdi, 8
	add	rax, rdi
	push	rax
	push	3
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	lea	rax, b
	push	rax
	push	1
	pop	rdi
	pop	rax
	imul	rdi, 8
	add	rax, rdi
	push	rax
	push	4
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	lea	rax, b
	push	rax
	push	0
	pop	rdi
	pop	rax
	imul	rdi, 8
	add	rax, rdi
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	lea	rax, b
	push	rax
	push	1
	pop	rdi
	pop	rax
	imul	rdi, 8
	add	rax, rdi
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
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
