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
	push	20
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
.Lwhile_begin:
	push	0
	pop	rax
	cmp	rax, 0
	je	.Lwhile_end
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
	sub	rax, rdi
	push	rax
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	jmp	.Lwhile_begin
.Lwhile_end:
	pop	rax
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
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
