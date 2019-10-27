.intel_syntax noprefix
.global main
main:
	push	78
	push	6
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	32
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	6
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	58
	push	62
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	125
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	1
	pop	rdi
	pop	rax
	imul	rax, rdi
	push	rax
	push	1
	pop	rdi
	pop	rax
	cqo
	idiv	rdi
	push	rax
	push	2
	pop	rdi
	pop	rax
	imul	rax, rdi
	push	rax
	push	2
	pop	rdi
	pop	rax
	cqo
	idiv	rdi
	push	rax
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rax
	ret
