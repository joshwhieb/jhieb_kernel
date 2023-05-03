#include "kernel.h"

#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char color){
    return (color << 8) | c;
}

void terminal_put_char(int x, int y, char c, char color){
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

void terminal_write_char(char c, char color){
    if(c == '\n'){
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    terminal_put_char(terminal_col, terminal_row, c, color);
    terminal_col += 1;
    if(terminal_col >= VGA_WIDTH){
        terminal_col = 0;
        terminal_row += 1;
    }
    if(terminal_row >= VGA_HEIGHT){
        terminal_row = 0;
    }
}

void terminal_initialize(){
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for(int y=0; y < VGA_HEIGHT;y++){
        for(int x = 0; x < VGA_WIDTH; x++){
            terminal_put_char(x, y, ' ', 0);
        }
    }
}

size_t strlen(const char* str){
    size_t len =0;
    while(str[len]){
        len++;
    }
    return len;
}

void print(const char* str){
    size_t len = strlen(str);
    for(int i=0; i < len;i++){
        terminal_write_char(str[i], 15);
    }
}

void kernel_main()
{
    terminal_initialize();
    print("hello world!\nthis is pretty cool\n");

    // init the heap.
    kheap_init();

    // initialize interrupt descriptor table
    idt_init();


}