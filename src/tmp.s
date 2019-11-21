.intel_syntax noprefix
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	5
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
.Lfor_begin:
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	13
	pop	rdi
	pop	rax
	cmp	rdi, rax
	setl	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lelse
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
	jmp	.Lend
.Lelse:
	mov	rax, rbp
	sub	rax, 8
	push	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	1
	pop	rdi
	pop	rax
	add	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
.Lend:
	jmp	.Lfor_begin
.Lfor_end:
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
