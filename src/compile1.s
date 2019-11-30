.intel_syntax noprefix
.global main

hoge:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	[rbp-8], rdi
	mov	[rbp-16], rsi
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	mov	rax, rbp
	sub	rax, 8
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
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	push	17
	push	7
	pop	rsi
	pop	rdi
	call	hoge
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
