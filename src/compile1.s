.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 40
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	20
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 16
	push	rax
	push	21
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 24
	push	rax
	push	22
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 32
	push	rax
	push	23
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 40
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	mov	rax, rbp
	sub	rax, 40
	push	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 40
	push	rax
	mov	rax, rbp
	sub	rax, 40
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	3
	pop	rdi
	pop	rax
	imul	rdi, 8
	sub	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 40
	push	rax
	pop	rax
	mov	rax, [rax]
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
