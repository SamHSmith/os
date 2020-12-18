
.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernel_main
.global loader

loader:
    mov $kernel_stack, %esp
    push %eax
    push %ebx
    call kernel_main

_stop:
    cli
    hlt
    jmp _stop

.intel_syntax noprefix

gdtr:
    .word 0
    .double 0
 
.global set_gdt
set_gdt:
   MOV   EAX, [esp + 4]
   MOV   [gdtr + 2], EAX
   MOV   AX, [ESP + 8]
   MOV   [gdtr], AX
   LGDT  [gdtr]
   RET

.att_syntax

.section .bss
.space 2*1024*1024; # 2 MiB

kernel_stack:

