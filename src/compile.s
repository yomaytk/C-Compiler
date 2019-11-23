.intel_syntax noprefix
.global main

hoge:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	mov	[rbp-8], rdi
	mov	[rbp-16], rsi
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	mov	rax, rbp
	sub	rax, 16
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
	sub	rsp, 32
	push	rax
	mov	rax, rsp
	sub	rax, 2
	and	rax, 0xf
	cmp	rax, 0
	pop	rax
	je	.Lrsp_eql_16n1
	push	0xffff
.Lrsp_eql_16n1:
	push	5
	push	66
	pop	rsi
	pop	rdi
	call	hoge
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
