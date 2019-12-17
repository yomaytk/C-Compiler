.intel_syntax noprefix
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 12
	mov	rax, rbp
	sub	rax, 4
	push	rax
	push	3
	pop	rbx
	pop	rax
	mov	[rax], ebx
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 12
	push	rax
	pop	rax
	mov	eax, [rax]
	push	rax
	pop	rax
	mov	rax, rbp
	sub	rax, 12
	push	rax
	mov	rax, rbp
	sub	rax, 4
	push	rax
	pop	rbx
	pop	rax
	mov	[rax], ebx
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 12
	push	rax
	pop	rax
	mov	eax, [rax]
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
