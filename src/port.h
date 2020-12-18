
#ifndef __PORT_H
#define __PORT_H

#define PIC1        0x20        /* IO base address for master PIC */
#define PIC2        0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA    (PIC2+1)

#define ICW1_ICW4    0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4    0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL    0x08        /* Level triggered (edge) mode */
#define ICW1_INIT    0x10        /* Initialization - required! */
 
#define ICW4_8086    0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO    0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE    0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER    0x0C        /* Buffered mode/master */
#define ICW4_SFNM    0x10        /* Special fully nested (not) */

#include "types.h"

void port_outb(u16 portnum, u8 data)
{
    asm("outb %0, %1" : : "a" (data), "Nd" (portnum));
}

void port_outb_slow(u16 portnum, u8 data)
{
    asm("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnum));
}

u8 port_inb(u16 portnum)
{
    u8 r;
    asm("inb %1, %0" : "=a" (r) : "Nd" (portnum));
    return r;
}

void port_outw(u16 portnum, u16 data)
{
    asm("outw %0, %1" : : "a" (data), "Nd" (portnum));
}
 
u16 port_inw(u16 portnum)
{
    u16 r;
    asm("inw %1, %0" : "=a" (r) : "Nd" (portnum));
    return r;
}

void port_outl(u16 portnum, u32 data)
{
    asm("outl %0, %1" : : "a" (data), "Nd" (portnum));
}
 
u32 port_inl(u16 portnum)
{
    u32 r;
    asm("inl %1, %0" : "=a" (r) : "Nd" (portnum));
    return r;
}

#endif
