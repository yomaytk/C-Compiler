.intel_syntax noprefix
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	10
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rax, rbp
	sub	rax, 16
	push	rax
	push	11
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
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
	cmp	rax, rdi
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lend1
	push	20
	jmp	.Lend2
.Lend1:
	push	3
.Lend2:
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
