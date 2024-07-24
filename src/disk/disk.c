#include "io/io.h"

int disk_read_sector(int lba, int total, void* buf)
{
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short* ptr = (unsigned short*) buf;
    for(int b =0;b < total;b++){
        // wait for buffer to be ready
        // TODO(jhieb) the polling isn't super?
        char c = insb(0x1F7);
        while(!(c & 0x08)){
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory one word at a time (2 bytes)
        for(int i=0;i< 256; i++){
            *ptr = insw(0x1F0);
            ptr++; // TODO(jhieb) the pointer increment is 2 bytes??
        }
    }

    return 0;
}
