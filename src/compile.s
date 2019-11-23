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
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 208
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n1
	pop	rax
	push	0xffff
.Lrsp_eql_16n1:
	pop	rax
	call	hoge
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
