.intel_syntax noprefix
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 12
	mov	rax, rbp
	sub	rax, 12
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	pop	rax
	push	10
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
