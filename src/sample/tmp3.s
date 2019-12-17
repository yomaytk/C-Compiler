.intel_syntax noprefix
.text
.global main

fib:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 4
	mov	[rbp-4], edi
	mov	rax, rbp
	sub	rax, 4
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	0
	pop	rbx
	pop	rax
	cmp	rax, rbx
	sete	al
	movzb	rax, al
	push	rax
	pop	rax
	cmp	rax, 0
	je	.Lifelse2
	push	0
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	jmp	.Lifend1
.Lifelse2:
	mov	rax, rbp
	sub	rax, 4
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	1
	pop	rbx
	pop	rax
	sub	rax, rbx
	push	rax
	pop	rdi
	call	fib
	push	rax
	push	1
	pop	rbx
	pop	rax
	add	rax, rbx
	push	rax
.Lifend1:
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
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
