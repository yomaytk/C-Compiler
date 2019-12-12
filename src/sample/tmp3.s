.intel_syntax noprefix
.text
.global main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	push r12
	push r13
	push r14
	push r15
.L1:
	jmp .L2
.L2:
	mov rdi, 3
	lea r11, [rbp-1]
	mov [r11], dl
	lea r10, [rbp-1]
	mov r10b, [r10]
	mov r10, r10b
	mov rax, r10
	jmp .Lend4
.L3:
	mov rax, r10
	jmp .Lend4
	mov r10, 0
.Lend4:
	pop r15
	pop r14
	pop r13
	pop r12
	mov rsp, rbp
	pop rbp
	ret
