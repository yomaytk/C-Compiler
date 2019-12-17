.intel_syntax noprefix
.text
.global main

hoge:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 3
	mov	[rbp-1], dil
	mov	[rbp-2], sil
	mov	rax, rbp
	sub	rax, 3
	push	rax
	push	4
	pop	rbx
	pop	rax
	mov	[rax], bl
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 1
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	mov	rax, rbp
	sub	rax, 2
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rbx
	pop	rax
	add	rax, rbx
	push	rax
	mov	rax, rbp
	sub	rax, 3
	push	rax
	pop	rax
	movsx	rax, BYTE PTR [rax]
	push	rax
	pop	rbx
	pop	rax
	add	rax, rbx
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
	push	3
	push	4
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
