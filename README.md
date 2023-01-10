# jhieb_kernel
source code for kernel course.

https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

https://wiki.osdev.org/Main_Page

https://www.ctyme.com/rbrown.htm

## Setup

details in the docker_dev/Dockerfile definition.

Might need VM setup until i figure out how to expose qemu system from docker.

TODO(jhieb) add cross compiling support to docker container.

https://wiki.osdev.org/GCC_Cross-Compiler

## Building the boot loader and kernel

```
./build.sh
```

## Deassemble the boot loader

```
ndisasm ./bin/boot.bin
```

## Running the bootloader in QEMU

```
qemu-system-x86_64 -hda ./bin/boot.bin
```

## Writing the bootloader to USB

list disks to find USB

```
fdisk -l
```

write bin to usb
```
sudo dd if=./bin/boot.bin of=/dev/sdb
```

## GDB debugging

```
make
gdb
target remote | qemu-system-x86_64 -hda ./bin/boot.bin -S -gdb stdio
c
ctrl+c
layout asm
info registers
```
