.intel_syntax noprefix
.global main
main:
	push	0
	push	10
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	20
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	0
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	push	0
	push	20
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	push	20
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rax
	ret
