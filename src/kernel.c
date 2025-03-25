#include "kernel.h"

#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "string/string.h"
#include "task/task.h"
#include "task/process.h"
#include "fs/file.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "disk/streamer.h"
#include "gdt/gdt.h"
#include "config.h"
#include "memory/memory.h"
#include "task/tss.h"
#include "status.h"

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

void print(const char* str){
    size_t len = strlen(str);
    for(int i=0; i < len;i++){
        terminal_write_char(str[i], 15);
    }
}
static struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg){
    print(msg);
    while(1) {}
}

struct tss tss;
struct gdt gdt_real[JHIEBOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[JHIEBOS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00},   // Null segment.
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a}, //kernel code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92}, // kernel data segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8}, // user code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2},  // user data segment
    {.base = (uint32_t)&tss, .limit=sizeof(tss), .type = 0xE9} // TSS segment.
};

void kernel_main()
{
    terminal_initialize();
    print("hello world!\nthis is pretty cool\n");

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, JHIEBOS_TOTAL_GDT_SEGMENTS);
    gdt_load(gdt_real, sizeof(gdt_real));
    // init the heap.
    kheap_init();

    // initialize file systems
    fs_init();

    // init disks
    disk_search_and_init();

    // initialize interrupt descriptor table
    idt_init();

    // setup the tss
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;
    // load the TSS
    tss_load(0x28);

    // setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    // switch to kernel paging chunk
    paging_switch(kernel_chunk);

    // enable paging.
    enable_paging();

    struct process* process =0;
    int res = process_load("0:/blank.bin", &process);
    if(res != JHIEBOS_ALL_OK){
        panic("Failed to load blank.bin");
    }

    task_run_first_ever_task();

    while(1) {}

}