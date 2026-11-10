CC      = gcc
LD      = ld
OBJCOPY = objcopy
NASM    = nasm
QEMU    = qemu-system-x86_64

all: os-image.bin

boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin

kernel_entry.o: bridge.asm
	$(NASM) -f elf32 bridge.asm -o kernel_entry.o

kernel.o: kernel.cpp
	$(CC) -m32 -ffreestanding -c kernel.cpp -o kernel.o

kernel.tmp: kernel_entry.o kernel.o
	$(LD) -m i386pe -o kernel.tmp -Ttext 0x1000 kernel_entry.o kernel.o

kernel.bin: kernel.tmp
	$(OBJCOPY) -O binary -R .reloc -R .idata kernel.tmp kernel.bin

os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin
	cp os-image.bin os.bin

run: all
	$(QEMU) -drive format=raw,file=os-image.bin

clean:
	rm -f *.bin *.o *.tmp

.PHONY: all run clean
