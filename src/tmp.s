.intel_syntax noprefix
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	mov	rax, rbp
	sub	rax, 0
	push	rax
	push	10
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 0
	push	rax
	push	30
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 0
	push	rax
	push	5
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 0
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	mov	rax, rbp
	sub	rax, 0
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	mov	rax, rbp
	sub	rax, 0
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
