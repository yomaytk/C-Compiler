.intel_syntax noprefix
.global main
main:
	push	32
	push	4
	push	4
	pop	rdi
	pop	rax
	imul	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	3
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	12
	push	90
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	32
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	cmp	rdi, rax
	setl	al
	movzb	rax, al
	push	rax
	pop	rax
	ret
