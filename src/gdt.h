
#ifndef __GDT_H
#define __GDT_H

#include "types.h"

struct SegDescr
{
    u16 limit_lo;
    u16 base_lo;
    u8 base_hi;
    u8 type;
    u8 flags_limit_hi;
    u8 base_vhi;
};

struct SegDescr seg_descr_create(u32 base, u32 limit, u8 flags)
{
    struct SegDescr s = {0};
    u8* target = (u8*) &s;
    if(limit <= 65536)
    {
        target[6] = 0x40;
    }
    else
    {
        if((limit & 0xFFF) != 0xFFF)
        {
            limit = (limit >> 12) -1;
        }
        else
        {
            limit = limit >> 12;
        }
        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |=(limit >> 16) & 0xF;

    target[2] =  base        & 0xFF;
    target[3] = (base >> 8)  & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flags;
}

u32 seg_descr_base(struct SegDescr s)
{
    u8* target = (u8*) &s;
    u32 b = target[7];
    b = (b << 8) | target[4];
    b = (b << 8) | target[3];
    b = (b << 8) | target[2];
    return b;
}

u32 seg_descr_limit(struct SegDescr s)
{
    u8* target = (u8*) &s;
    u32 l = target[6] & 0xF;
    l = (l << 8) | target[1];
    l = (l << 8) | target[0];

    if((target[6] & 0xC0) == 0xC0)
    {
        l = (l << 12) | 0xFFF;
    }
    return l;
}

struct GDT
{
    struct SegDescr null;
    struct SegDescr unused;
    struct SegDescr code;
    struct SegDescr data;
};

void set_gdt(struct GDT* gdt, u16 size);

u16 gdt_data_offset(struct GDT* g)
{
    return (u32)&g->data;
}

u16 gdt_code_offset(struct GDT* g)
{
    return (u32)&g->code;
}

#endif
