# jhieb_kernel
source code for kernel course.

https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

## Building the boot loader

```
cd src
nasm -f bin ./boot.asm -o ./boot.bin
```

## Running the bootloader in QEMU

```
qemu-system-x86_64 -hda ./boot.bin
```
