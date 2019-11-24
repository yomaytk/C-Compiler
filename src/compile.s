.intel_syntax noprefix
.global main

hoge:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	mov	[rbp-8], rdi
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
hoge2:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	mov	[rbp-8], rdi
	mov	rax, rbp
	sub	rax, 24
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 40
	push	10
	pop	rdi
	call	hoge
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
