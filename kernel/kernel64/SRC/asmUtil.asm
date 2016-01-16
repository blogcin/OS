[BITS 64]

SECTION .text

global inPortByte, outPortByte

inPortByte:
	push rdx

	mov rdx, rdi
	mov rax, 0
	in al, dx

	pop rdx
	ret

outPortByte:
	push rdx
	push rax
	
	mov rdx, rdi
	mov rax, rsi
	out dx, al

	pop rax
	pop rdx
	ret
