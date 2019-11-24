.intel_syntax noprefix
.global main

fib:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	[rbp-8], rdi
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	0
	pop	rdi
	pop	rax
	cmp	rax, rdi
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lifelse1
	push	0
.Lifelse1:
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	1
	pop	rdi
	pop	rax
	sub	rax, rdi
	push	rax
	pop	rdi
	call	fib
	push	rax
	push	1
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	push	10
	pop	rdi
	call	fib
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
