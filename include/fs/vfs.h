#ifndef FS_VFS_H
#define FS_VFS_H

#include "kernel/types.h"

typedef struct vfs_file vfs_file_t;

int vfs_init(void);
vfs_file_t* vfs_open(const char* path, int flags);
void vfs_close(vfs_file_t* f);
ssize_t vfs_read(vfs_file_t* f, void* buf, size_t len);
ssize_t vfs_write(vfs_file_t* f, const void* buf, size_t len);

#endif
