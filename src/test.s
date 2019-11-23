.intel_syntax noprefix
.global main

hoge:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	push	10
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	call hoge
	mov	rsp, rbp
	pop	rbp
	ret
