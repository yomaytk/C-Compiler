.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80
	mov	rax, rbp
	sub	rax, 8
	push	rax
	pop	rax
	push	0
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
