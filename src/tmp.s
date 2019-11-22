.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 8
	push	3
	push	4
	push	5
	push	6
	push	7
	push	8
	pop	r9
	pop	r8
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	call	hogehoge
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
