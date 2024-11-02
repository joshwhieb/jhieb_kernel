#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int JHIEBOS_DISK_TYPE;

// Represents a real physical hard disk
#define JHIEBOS_DISK_TYPE_REAL 0

struct disk {
    JHIEBOS_DISK_TYPE type;
    int sector_size;
    int id;
    // Private data of the filesystem.
    void* fs_private;

    struct filesystem* filesystem;
};

void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif