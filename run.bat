@echo off
nasm -f bin boot.asm -o boot.bin
nasm -f elf32 bridge.asm -o kernel_entry.o
gcc -m32 -ffreestanding -c kernel.cpp -o kernel.o
ld -m i386pe -o kernel.tmp -Ttext 0x1000 kernel_entry.o kernel.o
objcopy -O binary kernel.tmp kernel.bin
copy /b boot.bin + kernel.bin os-image.bin > nul
qemu-system-x86_64 -drive format=raw,file=os.bin
