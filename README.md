# jhieb_kernel
source code for kernel course.

https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

https://wiki.osdev.org/Main_Page

## Setup

details in the docker_dev/Dockerfile definition.

Might need VM setup until i figure out how to expose qemu system from docker.

## Building the boot loader

```
cd src
nasm -f bin ./boot.asm -o ./boot.bin
```

## Deassemble the boot loader

```
cd src
ndisasm ./boot.asm
```

## Running the bootloader in QEMU

```
qemu-system-x86_64 -hda ./boot.bin
```
