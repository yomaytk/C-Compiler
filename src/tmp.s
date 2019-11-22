.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	3
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 16
	push	rax
	push	2
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 24
	push	rax
	push	14
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n1
	pop	rax
	push	0xffff
.Lrsp_eql_16n1:
	pop	rax
	call	hogehoge
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
