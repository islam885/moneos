CC := gcc
CFLAGS := -m32 -ffreestanding -fno-pie -fno-stack-protector -Wall -Wextra -Iinclude -Isrc
LDFLAGS := -m32 -ffreestanding -nostdlib -no-pie -T linker.ld

# Находим все .c файлы в src и подпапках
SOURCES := $(shell find src -name "*.c")
OBJECTS := $(SOURCES:.c=.o)
OBJECTS += src/boot.o src/cpu/interrupt.o

all: myos.iso

src/boot.o: src/boot.asm
	nasm -f elf32 $< -o $@

src/cpu/interrupt.o: src/cpu/interrupt.asm
	nasm -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

myos.bin: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

myos.iso: myos.bin grub.cfg
	mkdir -p iso/boot/grub
	cp myos.bin iso/boot/myos.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -display sdl

run-vnc: myos.iso
	qemu-system-i386 -cdrom myos.iso -vnc :0

clean:
	rm -rf $(OBJECTS) myos.bin myos.iso iso
	find src -name "*.o" -delete
