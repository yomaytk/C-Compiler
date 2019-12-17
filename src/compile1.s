.intel_syntax noprefix
.bss
a:
	.zero 1
.bss
p:
	.zero 8
.bss
q:
	.zero 4
.text
.global main

main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	lea	rax, a
	push	rax
	push	5
	pop	rbx
	pop	rax
	mov	[rax], bl
	push	rbx
	pop	rax
	lea	rax, q
	push	rax
	push	9
	pop	rbx
	pop	rax
	mov	[rax], ebx
	push	rbx
	pop	rax
	lea	rax, p
	push	rax
	lea	rax, q
	push	rax
	pop	rbx
	pop	rax
	mov	[rax], rbx
	push	rbx
	pop	rax
	lea	rax, a
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	lea	rax, p
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rax
	mov	rax, [rax]
	push	rax
	pop	rbx
	pop	rax
	add	rax, rbx
	push	rax
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
	pop	rax
	mov	rsp, rbp
	pop	rbp
	ret
