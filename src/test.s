.intel_syntax noprefix
.data
.L.str1:
	.ascii "hello, world\000"

.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	mov	rax, rbp
	sub	rax, 8
	push	rax
	lea	r10, .L.str1
	push	r10
	pop	rbx
	pop	rax
	mov	[rax], rbx
	push	rbx
	pop	rax
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	push	3
	pop	rbx
	pop	rax
	add	rax, rbx
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
