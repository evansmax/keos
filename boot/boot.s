MB_HEADER_MAGIC		equ	0x1bdab002

MB_PAGE_ALIGN		equ	1 << 0
MB_MEM_INFO			equ	1 << 1
MB_HEADER_FLAGS		equ	MB_PAGE_ALIGN | MB_MEM_INFO

MB_HEADER_CHECKSUM	equ	-(MB_HEADER_MAGIC + MB_HEADER_FLAGS)	; a 32-bit unsigned value

[BITS 32]

section .text

dd MB_HEADER_MAGIC
dd MB_HEADER_FLAGS
dd MB_HEADER_CHECKSUM

global start
global global_mb_ptr
extern kernel_entry

start:
	cli

	mov esp, STACK_TOP
	mov ebp, 0
	and esp, 0x0fffffff0
	mov [global_mb_ptr], ebx	; ebx register contains the physical address of a multiboot information data structure
	
	call kernel_entry
stop:
	hlt
	jmp stop


section .bbs

stack:
	resb 32*1024
global_mb_ptr:
	resb 4

STACK_TOP		equ	$ - stack -1
