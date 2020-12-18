#include "types.h"
#include "gdt.h"
#include "port.h"
#include "interrupt.h"

void print_str(s8* str)
{
    u16* video_memory = (u16*) 0xb8000;

    static u8 x=0, y=0;

    for(int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] != '\n')
        {
            video_memory[80*y+x] = (video_memory[80*y+x] & 0xFF00) | str[i];
            x++;
        }
        else
        {
            x = 80;
        }

        if(x >= 80)
        {
            y++; x = 0;
        }
        if(y >= 25)
        {
            for(u8 tx = 0; tx < 80; tx++)
            {
                u16 tempval = video_memory[80*24+tx] & 0xFF00;
                for(s16 ty = 24; ty >= 0; ty--)
                {
                    u8 tempval2 = video_memory[80*ty+tx];
                    video_memory[80*ty+tx] = tempval;
                    tempval = tempval2;
                }
            }
            y--; x = 0;
        }
    }
}

void kernel_main(void* multiboot_structure, u32 magicnumber)
{

    print_str("\n\nHello from INTERSTELLAROS\n\n\n\n\n\n\n");

    struct GDT gdt = {0};
    gdt.null = seg_descr_create(0, 0, 0);
    gdt.unused=seg_descr_create(0, 0, 0);
    gdt.code = seg_descr_create(0, 60*1024*1024, 0x9A);
    gdt.data = seg_descr_create(0, 60*1024*1024, 0x92);

    set_gdt(&gdt, sizeof(struct GDT));

    setup_interrupts(&gdt);

    activate_interrupts();

    asm("int $0x20");

    u32 timer1 = 0;
    u32 timer2 = 1;
    while( timer1 < 20)
    {
        if(timer1 % 1 == 0) { print_str("yea... "); }
        timer1++;
    }

while(1) { }
}
