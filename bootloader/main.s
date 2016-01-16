[ORG 0X00]
[BITS 16]

SECTION .text

jmp 0x07C0:STARTBOOTLOADER

TOTALSECTORCOUNT: dw 0x02 ; OS IMAGE SIZE

STARTBOOTLOADER:
	mov ax, 0x07C0 ; 0x07C0 - BIOS read first sector (0x07c0), Convert to Segment register value
	;mov ds, 0x07C0
	mov ds, ax ; DS - Program Data segment's start address
	mov ax, 0xB800 ; Video Memory Address
	mov es, ax ; ES - Use caculate 'Set Memory address string'

	; 0x0000:0000 ~ 0x0000:FFFF 64KB Stack
	mov ax, 0x0000
	mov ss, ax ; SS - Able to realize stack in memory
	mov sp, 0xFFFE
	mov bp, 0xFFFE

	mov si, 0 ; SI regsiter initializing
	; to calc 'string control' 

.SCREENINIT:
	mov byte[es:si], 0
	mov byte[es:si+1], 0x07
	add si, 2

	cmp si, 80 * 25 * 2
	; si != 80 * 25 * 2
	jl .SCREENINIT

.INITMESSAGE:
	push MESSAGE
	push 0 ; Y
	push 0 ; X
	call PRINTMESSAGE
	add sp, 6
	
	push IMAGELOADING
	push 1 ; Y
	push 0 ; X
	call PRINTMESSAGE
	add sp, 6


RESETDISK:
	mov ax, 0
	mov dl, 0
	int 0x13

	jc HANDLEDISKERROR
	mov si, 0x1000
	mov es, si
	mov bx, 0x0000

	mov di, word[TOTALSECTORCOUNT]


READDATA:
	cmp di, 0
	je READEND
	sub di, 0x1

	mov ah, 0x02
	mov al, 0x1
	mov ch, byte[TRACKNUMBER]
	mov cl, byte[SECTORNUMBER]
	mov dh, byte[HEADNUMBER]
	mov dl, 0x00
	int 0x13 ; interrupt
	jc HANDLEDISKERROR

	add si, 0x0020
	mov es, si
	
	mov al, byte[SECTORNUMBER]
	add al, 0x01
	mov byte[SECTORNUMBER], al
	cmp al, 19
	jl READDATA
	
	xor byte[HEADNUMBER], 0x01
	mov byte[SECTORNUMBER], 0x01

	cmp byte[HEADNUMBER], 0x00
	jne READDATA

	add byte[TRACKNUMBER], 0x01
	jmp READDATA

READEND:
	push LOADINGCOMPLETE
	push 2
	push 0
	call PRINTMESSAGE
	add sp, 6

	jmp 0x1000:0x0000

HANDLEDISKERROR:
	push DISKERRORMESSAGE
	push 3
	push 0
	call PRINTMESSAGE

	jmp $

PRINTMESSAGE:
	push bp
	mov bp, sp

	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax

	mov ax, word[bp+6]
	mov si, 160
	mul si
	mov di, ax

	mov ax, word[bp+4]
	mov si, 2
	mul si
	add di, ax

	mov si, word[bp+8]

.MESSAGELOOP:
	mov cl, byte[si]

	cmp cl, 0
	je .MESSAGEEND

	mov byte[es:di], cl

	add si, 1
	add di, 2
	
	jmp .MESSAGELOOP

.MESSAGEEND:
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp	
	ret

MESSAGE: db "Bootloader started", 0
IMAGELOADING: db "Now, Image loading now!", 0
LOADINGCOMPLETE: db "Loading Complete!", 0
DISKERRORMESSAGE: db "Disk Error!", 0

SECTORNUMBER: db 0x02
HEADNUMBER: db 0x00
TRACKNUMBER: db 0x00

; MBR 
times 510 - ($ - $$) db 0x00
db 0x55
db 0xAA
