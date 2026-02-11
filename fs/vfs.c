/**
 * vfs.c - Wirtualna warstwa systemu plików (VFS)
 * NexusOS - placeholder pod przyszły system plików / FAT32
 */

#include "kernel/types.h"
#include "fs/vfs.h"
#include <stddef.h>

/* Placeholder - do implementacji: montowanie, open, read, write, close */

int vfs_init(void)
{
    return 0;
}

vfs_file_t* vfs_open(const char* path, int flags)
{
    (void)path;
    (void)flags;
    return NULL;
}

void vfs_close(vfs_file_t* f)
{
    (void)f;
}

ssize_t vfs_read(vfs_file_t* f, void* buf, size_t len)
{
    (void)f;
    (void)buf;
    (void)len;
    return -1;
}

ssize_t vfs_write(vfs_file_t* f, const void* buf, size_t len)
{
    (void)f;
    (void)buf;
    (void)len;
    return -1;
}
