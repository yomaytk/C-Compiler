.intel_syntax noprefix
.global main

rsp_to_16n:
	push	0xffff
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	0
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	push	1
	push	2
	pop	rdi
	pop	rax
	cmp	rax, rdi
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lelse1
	push	2
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	jmp	.Lend2
.Lelse1:
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	5
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
.Lend2:
	pop	rax
	push	1
	push	3
	pop	rdi
	pop	rax
	cmp	rax, rdi
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lelse3
	push	6
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	jmp	.Lend4
.Lelse3:
	mov	rax, rbp
	sub	rax, 8
	push	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	7
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
.Lend4:
	pop	rax
	push	2
	push	5
	pop	rdi
	pop	rax
	cmp	rax, rdi
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lelse5
	push	129
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	jmp	.Lend6
.Lelse5:
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
.Lend6:
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
