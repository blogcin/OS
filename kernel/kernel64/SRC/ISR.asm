[BITS 64]
 
SECTION .text

extern commonExceptionHandler, commonInterruptHandler, keyboardHandler

global ISRDivideError, ISRDebug, ISRNMI, ISRBreakPoint, ISROverflow
global ISRBoundRangeExceeded, ISRInvalidOpcode, ISRDeviceNotAvailable, ISRDoubleFault
global ISRCoprocessorSegmentOverrun, ISRInvalidTSS, ISRSegmentNotPresent
global ISRStackSegmentFault, ISRGeneralProtection, ISRPageFault, ISR15
global ISRFPUError, ISRAlignmentCheck, ISRMachineCheck, ISRSIMDError, ISRETCException

global ISRTimer, ISRKeyboard, ISRSlavePIC, ISRSerial2, ISRSerial1, ISRParallel2
global ISRFloppy, ISRParallel1, ISRRTC, ISRReserved, ISRNotUsed1, ISRNotUsed2
global ISRMouse, ISRCoprocessor, ISRHDD1, ISRHDD2, ISRETCInterrupt


; save context, and change
%macro SAVECONTEXT 0
	push rbp
	mov rbp, rsp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	

	mov ax, ds
	push rax
	mov ax, es
	push rax
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
%endmacro


%macro LOADCONTEXT 0
	pop gs
	pop fs
	pop rax
	mov es, ax
	pop rax
	mov ds, ax

	pop r15
	pop r14	
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
%endmacro

ISRDivideError:
	SAVECONTEXT

	mov rdi, 0
	call commonExceptionHandler
	
	LOADCONTEXT
	iretq

ISRDebug:
	SAVECONTEXT

	mov rdi, 1
	call commonExceptionHandler

	LOADCONTEXT
	iretq

ISRNMI:
	SAVECONTEXT

	mov rdi, 2
	call commonExceptionHandler

	LOADCONTEXT
	iretq

ISRBreakPoint:
	SAVECONTEXT
	
	mov rdi, 3
	call commonExceptionHandler
	
	LOADCONTEXT
	iretq

ISROverflow:
	SAVECONTEXT

	mov rdi, 4
	call commonExceptionHandler
	LOADCONTEXT

	iretq

ISRBoundRangeExceeded:
	SAVECONTEXT

	mov rdi, 5
	call commonExceptionHandler

	LOADCONTEXT
	iretq

ISRInvalidOpcode:
	SAVECONTEXT

	mov rdi, 6
	call commonExceptionHandler

	LOADCONTEXT
	iretq

ISRDeviceNotAvailable:
	SAVECONTEXT
	
	mov rdi, 7
	call commonExceptionHandler
	
	LOADCONTEXT
	iretq

ISRDoubleFault:
	SAVECONTEXT

	mov rdi, 8
	mov rsi, qword[rbp+8]
	call commonExceptionHandler

	LOADCONTEXT
	add rsp, 8
	iretq

ISRCoprocessorSegmentOverrun:
	SAVECONTEXT

	mov rdi, 9
	call commonExceptionHandler
	
	LOADCONTEXT
	iretq

ISRInvalidTSS:
	SAVECONTEXT

	mov rdi, 10
	mov rsi, qword[rbp+8]
	call commonExceptionHandler


	LOADCONTEXT
	add rsp, 8
	iretq

ISRSegmentNotPresent:
	SAVECONTEXT

	mov rdi, 11
	mov rsi, qword[rbp+8]
	call commonExceptionHandler


	LOADCONTEXT
	add rsp, 8
	iretq

ISRStackSegmentFault:
	SAVECONTEXT

	mov rdi, 12
	mov rsi, qword[rbp+8]
	call commonExceptionHandler


	LOADCONTEXT
	add rsp, 8
	iretq

ISRGeneralProtection:
	SAVECONTEXT

	mov rdi, 13
	mov rsi, qword[rbp+8]
	call commonExceptionHandler


	LOADCONTEXT
	add rsp, 8
	iretq

ISRPageFault:
	SAVECONTEXT

	mov rdi, 14
	mov rsi, qword[rbp+8]
	call commonExceptionHandler


	LOADCONTEXT
	add rsp, 8
	iretq

ISR15:
	SAVECONTEXT

	mov rdi, 15
	call commonExceptionHandler


	LOADCONTEXT
	iretq

ISRFPUError:
	SAVECONTEXT

	mov rdi, 16
	call commonExceptionHandler


	LOADCONTEXT
	iretq

ISRAlignmentCheck:
    SAVECONTEXT    

    
    mov rdi, 17
    mov rsi, qword [ rbp + 8 ]
    call commonExceptionHandler

    LOADCONTEXT    
    add rsp, 8      
    iretq           

ISRMachineCheck:
    SAVECONTEXT    

    
    mov rdi, 18
    call commonExceptionHandler

    LOADCONTEXT    
    iretq           

ISRSIMDError:
    SAVECONTEXT    

    
    mov rdi, 19
    call commonExceptionHandler

    LOADCONTEXT    
    iretq           

ISRETCException:
    SAVECONTEXT    

    
    mov rdi, 20
    call commonExceptionHandler

    LOADCONTEXT    
    iretq           


ISRTimer:
    SAVECONTEXT    

    
    mov rdi, 32
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRKeyboard:
    SAVECONTEXT    

    
    mov rdi, 33
    call keyboardHandler

    LOADCONTEXT    
    iretq           

ISRSlavePIC:
    SAVECONTEXT    

    
    mov rdi, 34
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRSerial2:
    SAVECONTEXT    

    
    mov rdi, 35
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRSerial1:
    SAVECONTEXT    

    
    mov rdi, 36
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRParallel2:
    SAVECONTEXT    

    
    mov rdi, 37
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRFloppy:
    SAVECONTEXT    

    
    mov rdi, 38
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRParallel1:
    SAVECONTEXT    

    
    mov rdi, 39
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRRTC:
    SAVECONTEXT    

    
    mov rdi, 40
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           


ISRReserved:
    SAVECONTEXT    

    
    mov rdi, 41
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRNotUsed1:
    SAVECONTEXT    

    
    mov rdi, 42
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           


ISRNotUsed2:
    SAVECONTEXT    

    
    mov rdi, 43
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRMouse:
    SAVECONTEXT    

    
    mov rdi, 44
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRCoprocessor:
    SAVECONTEXT    

    
    mov rdi, 45
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRHDD1:
    SAVECONTEXT    

    
    mov rdi, 46
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           


ISRHDD2:
    SAVECONTEXT    

    
    mov rdi, 47
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

ISRETCInterrupt:
    SAVECONTEXT    

    
    mov rdi, 48
    call commonInterruptHandler

    LOADCONTEXT    
    iretq           

