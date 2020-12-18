
.set IRQ_BASE, 0x20

.section .text

.extern handle_interrupt

.global ignore_interrupt_request

.macro handle_exception num
.global handle_interrupt_request\num\()
handle_interrupt_request\num\():
    movb $\num, (interrupt_number)
    jmp int_bottom
.endm

.macro handle_interrupt_request num
.global handle_interrupt_request\num\()
handle_interrupt_request\num\():
    movb $\num + IRQ_BASE, (interrupt_number)
    jmp int_bottom
.endm

handle_interrupt_request 0x00
handle_interrupt_request 0x01

int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interrupt_number)
    call handle_interrupt
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

ignore_interrupt_request:

    iret


.data
    interrupt_number: .byte 0
