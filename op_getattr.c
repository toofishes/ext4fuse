/*
 * Copyright (c) 2010, Gerard Lledó Vives, gerard.lledo@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "inode.h"
#include "logging.h"

int op_getattr(const char *path, struct stat *stbuf)
{
    struct ext4_inode inode;
    uint32_t ino;
    int ret = 0;

    DEBUG("getattr(%s)", path);

    memset(stbuf, 0, sizeof(struct stat));
    ino = inode_get_idx_by_path(path);
    ret = inode_get_by_number(ino, &inode);

    if (ret < 0) {
        return ret;
    }

    DEBUG("getattr done");

    stbuf->st_ino = ino;
    stbuf->st_mode = inode.i_mode & ~0222;
    stbuf->st_nlink = inode.i_links_count;
    stbuf->st_size = inode_get_size(&inode);
    if (S_ISREG(inode.i_mode))
        stbuf->st_size = ((off_t)inode.i_size_high << 32) + inode.i_size_lo;
    else
        stbuf->st_size = inode.i_size_lo;
    stbuf->st_blocks = inode.i_blocks_lo;
    stbuf->st_uid = inode.i_uid;
    stbuf->st_gid = inode.i_gid;
    stbuf->st_atime = inode.i_atime;
    stbuf->st_mtime = inode.i_mtime;
    stbuf->st_ctime = inode.i_ctime;

    return 0;
}
