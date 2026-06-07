MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
GRAPHICS equ  1 << 2
FLAGS    equ  MBALIGN | MEMINFO | GRAPHICS
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    
    ; Graphics fields (used if bit 2 of flags is set)
    dd 0 ; header_addr
    dd 0 ; load_addr
    dd 0 ; load_end_addr
    dd 0 ; bss_end_addr
    dd 0 ; entry_addr
    dd 0 ; mode_type (0 = linear graphics mode)
    dd 800 ; width
    dd 600 ; height
    dd 32 ; depth

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main
_start:
    mov esp, stack_top
    
    ; Push multiboot info pointer and magic
    push ebx
    push eax
    
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
