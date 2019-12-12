.intel_syntax noprefix
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 1
	mov	rax, rbp
	sub	rax, 1
	push	rax
	push	10
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 1
	push	rax
	pop	rax
	movsx	rax, BYTE PTR [rax]
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
