CFLAGS = -m32 -nostdlib -fno-stack-protector -g
ASFLAGS = --32 -g
LDFLAGS = -melf_i386

objects = loader.o kernel.o interruptstubs.o

%.o: src/%.c
		gcc $(CFLAGS) -o $@ -c $<

%.o: src/%.s
		as $(ASFLAGS) -o $@ $<


kernel.bin: linker.ld $(objects)
		ld $(LDFLAGS) -T $< -o $@ $(objects)

isos.iso: kernel.bin
	rm -drf iso
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0\nset default=0\n\nmenuentry "InterstellarOS" {\n  multiboot /boot/kernel.bin\n  boot\n}' > iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -drf iso

run: isos.iso
	qemu-system-i386 isos.iso -curses

clean:
	rm -drf iso isos.iso $(objects) kernel.bin
