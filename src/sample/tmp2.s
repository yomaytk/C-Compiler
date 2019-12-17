.intel_syntax noprefix
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	mov	rax, rbp
	sub	rax, 4
	push	rax
	push	2
	pop	rbx
	pop	rax
	mov	[rax], ebx
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	6
	pop	rbx
	pop	rax
	mov	[rax], ebx
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 4
	push	rax
	pop	rax
	mov	eax, [rax]
	push	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	eax, [rax]
	push	rax
	pop	rbx
	pop	rax
	add	rax, rbx
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
