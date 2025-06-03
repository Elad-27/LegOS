#!/bin/sh
echo "assembling ASM code"
i686-elf-as bootloader/BootLeg.s -o Binaries/BootLeg.o
nasm -f elf32 INT/ISR/isrmacro.asm -o Binaries/isrmacro.o
nasm -f elf32 INT/GDT/gdtentr.asm -o Binaries/gdtentr.o
nasm -f elf32 INT/IRQ/irqhandler.asm -o Binaries/irqhandler.o

echo "compiling C code"
i686-elf-gcc -c functions/mathlib.c -o Binaries/mathlib.o -std=gnu99 -ffreestanding
i686-elf-gcc -c INT/IDT/lidt.c -o Binaries/lidt.o -std=gnu99 -ffreestanding	
i686-elf-gcc -c INT/PIC/picmod.c -o Binaries/picmod.o -std=gnu99 -ffreestanding
i686-elf-gcc -c INT/GDT/gdt.c -o Binaries/gdt.o -std=gnu99 -ffreestanding
i686-elf-gcc -c INT/ISR/isr.c -o Binaries/isr.o -std=gnu99 -ffreestanding
i686-elf-gcc -c INT/IRQ/irq.c -o Binaries/irq.o -std=gnu99 -ffreestanding
i686-elf-gcc -c functions/stringlib.c -o Binaries/stringlib.o -std=gnu99 -ffreestanding
i686-elf-gcc -c functions/shapes.c -o Binaries/shapes.o -std=gnu99 -ffreestanding
i686-elf-gcc -c Drivers/VGA/VGA_driver.c -o Binaries/VGA_driver.o -std=gnu99 -ffreestanding
i686-elf-gcc -c Drivers/PS2/ps2_driver.c -o Binaries/ps2_driver.o -std=gnu99 -ffreestanding
i686-elf-gcc -c Graphics/fonts.c -o Binaries/fonts.o -std=gnu99 -ffreestanding
i686-elf-gcc -c functions/in-out.c -o Binaries/in-out.o -std=gnu99 -ffreestanding
i686-elf-gcc -c functions/intf.c -o Binaries/intf.o -std=gnu99 -ffreestanding
i686-elf-gcc -c functions/datastruct.c -o Binaries/datastruct.o -std=gnu99 -ffreestanding
i686-elf-gcc -c Drivers/PIT/pit.c -o Binaries/pit.o -std=gnu99 -ffreestanding
i686-elf-gcc -c Drivers/PS2/ps2_keyboard.c -o Binaries/ps2_keyboard.o -std=gnu99 -ffreestanding
i686-elf-gcc -c terminal.c -o Binaries/terminal.o -std=gnu99 -ffreestanding

echo "linking files"
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib Binaries/BootLeg.o Binaries/isrmacro.o Binaries/terminal.o Binaries/gdtentr.o Binaries/irqhandler.o Binaries/datastruct.o Binaries/ps2_keyboard.o Binaries/VGA_driver.o Binaries/gdt.o Binaries/isr.o Binaries/irq.o Binaries/ps2_driver.o Binaries/in-out.o Binaries/mathlib.o Binaries/shapes.o Binaries/fonts.o Binaries/stringlib.o Binaries/lidt.o Binaries/picmod.o Binaries/intf.o Binaries/pit.o -lgcc

#ps: to run use: qemu-system-i386 -kernel myos.bin	
