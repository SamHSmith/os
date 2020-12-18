

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_h

#include "types.h"
#include "port.h"
#include "gdt.h"

void print_str(char* str);

struct GateDescriptor
{
    u16 handler_address_lb;
    u16 gdt_code_segment_sel;
    u8 zero;
    u8 access;
    u16 handler_address_hb;
};

static struct GateDescriptor interrupt_descriptor_table[256];

struct idt_ptr
{
    u16 size;
    u32 base;
};

void set_idt_entry(u8 int_num, u16 code_seg_sel_offset, void (*handler)(), u8 privilege, u8 type)
{
    const u8 IDT_DESC_PRESENT = 0x80;

    interrupt_descriptor_table[int_num].handler_address_lb = ((u32) handler) & 0xFFFF;
    interrupt_descriptor_table[int_num].handler_address_hb = (((u32) handler) >> 16) & 0xFFFF;
    interrupt_descriptor_table[int_num].gdt_code_segment_sel = code_seg_sel_offset;
    interrupt_descriptor_table[int_num].access = IDT_DESC_PRESENT | type | ((privilege & 3) << 5);
    interrupt_descriptor_table[int_num].zero = 0;
}

u32 handle_interrupt(u8 interrupt_num, u32 esp)
{
    print_str("INTERRUPT");
while(1) {}
    return esp;
}

void ignore_interrupt_request();

void handle_interrupt_request0x00();

void handle_interrupt_request0x01();

// Mc 0x20 md 0x21 sc 0xA0 sd 0xA1

void setup_interrupts(struct GDT* gdt)
{
    u16 code_segment = gdt_code_offset(gdt);
    u8 IDT_INTERRUPT_GATE = 0xE;

    for(u16 i = 0; i < 256; i++)
    {
        set_idt_entry(i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
    }

    set_idt_entry(0x20, code_segment, &handle_interrupt_request0x00, 0, IDT_INTERRUPT_GATE);
    set_idt_entry(0x21, code_segment, &handle_interrupt_request0x01, 0, IDT_INTERRUPT_GATE);

    port_outb_slow(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);// ready master and slave for init
    port_outb_slow(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    port_outb_slow(PIC1_DATA, 0x20); // Vector offset
    port_outb_slow(PIC2_DATA, 0x28);

    port_outb_slow(PIC1_DATA, 4);
    port_outb_slow(PIC2_DATA, 2);

    port_outb_slow(PIC1_DATA, ICW4_8086);
    port_outb_slow(PIC2_DATA, ICW4_8086);

    struct idt_ptr p = {0};
    p.size = 256 * sizeof(struct GateDescriptor) -1;
    p.base = (u32)interrupt_descriptor_table;
    asm("lidt %0" : : "m" (p));
}

void activate_interrupts()
{
    port_outb_slow(0x21, 0xff);
    port_outb_slow(0xa1, 0xff);
    asm("sti");
}

#endif
