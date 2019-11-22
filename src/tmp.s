.intel_syntax noprefix
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 56
	push	1
	push	2
	push	3
	push	4
	push	5
	push	6
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n1
	pop	rax
	push	0xffff
.Lrsp_eql_16n1:
	pop	rax
	pop	r9
	pop	r8
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	call	hogehoge1
	pop	rax
	push	1
	push	2
	push	3
	push	4
	push	5
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n2
	pop	rax
	push	0xffff
.Lrsp_eql_16n2:
	pop	rax
	pop	r8
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	call	hogehoge2
	pop	rax
	push	1
	push	2
	push	3
	push	4
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n3
	pop	rax
	push	0xffff
.Lrsp_eql_16n3:
	pop	rax
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	call	hogehoge3
	pop	rax
	push	1
	push	2
	push	3
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n4
	pop	rax
	push	0xffff
.Lrsp_eql_16n4:
	pop	rax
	pop	rdx
	pop	rsi
	pop	rdi
	call	hogehoge4
	pop	rax
	push	1
	push	2
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n5
	pop	rax
	push	0xffff
.Lrsp_eql_16n5:
	pop	rax
	pop	rsi
	pop	rdi
	call	hogehoge5
	pop	rax
	push	1
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n6
	pop	rax
	push	0xffff
.Lrsp_eql_16n6:
	pop	rax
	pop	rdi
	call	hogehoge6
	pop	rax
	push	rax
	mov	rax, rsp
	and	rax, 0xf
	cmp	rax, 0
	je	.Lrsp_eql_16n7
	pop	rax
	push	0xffff
.Lrsp_eql_16n7:
	pop	rax
	call	hogehogeend
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
