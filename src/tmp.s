.intel_syntax noprefix
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	mov	rax, rbp
	sub	rax, 8
	push	rax
	push	5
	pop	rdi
	pop	rax
	mov	[rax], rdi
	push	rdi
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
