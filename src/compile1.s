.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 800
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	1
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	0
	push	28
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	32
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	21
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
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
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	17
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
