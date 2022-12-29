# jhieb_kernel
source code for kernel course.

https://www.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

https://wiki.osdev.org/Main_Page

https://www.ctyme.com/rbrown.htm

## Setup

details in the docker_dev/Dockerfile definition.

Might need VM setup until i figure out how to expose qemu system from docker.

## Building the boot loader

with nasm

```
cd src
nasm -f bin ./boot.asm -o ./boot.bin
```

with make

```
cd src
make
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

## Writing the bootloader to USB

list disks to find USB

```
fdisk -l
```

write bin to usb
```
sudo dd if=./boot.bin of=/dev/sdb
```
