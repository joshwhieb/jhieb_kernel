#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "task.h"
#include "config.h"

struct process {
    // the process id
    uint16_t id;
    char filename[JHIEBOS_MAX_PATH];

    // The main process task.
    struct task* task;

    // the memory (malloc) allocations of the process.
    void* allocations[JHIEBOS_MAX_PROGRAM_ALLOCATIONS];

    // the physical pointer to the process memory.
    void* ptr;

    // the physical pointer to the stack memory.
    void* stack;

    // the size of the data pointed to by "ptr".
    uint32_t size;


};

int process_load_for_slot(const char* filename, struct process** process, int process_slot);

#endif