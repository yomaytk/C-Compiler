.intel_syntax noprefix
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	push	1
	pop	rax
	cmp	rax, 0
	je	.Lelse
	push	20
.Lelse:
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
