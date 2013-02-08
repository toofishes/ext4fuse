/*
 * Copyright (c) 2012, Dan McGee, dpmcgee@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 */


#include <sys/types.h>
#include <sys/statvfs.h>
#include <string.h>

#include "types/ext4_dentry.h"
#include "super.h"
#include "logging.h"

int op_statfs(const char *path, struct statvfs *stvfs)
{
    DEBUG("statfs(%s)", path);

    memset(stvfs, 0, sizeof(struct statvfs));

    stvfs->f_namemax = EXT4_NAME_LEN;
    stvfs->f_files = super_inodes_count();
    stvfs->f_ffree = super_free_inodes_count();
	/* TODO: blocks calculations aren't right just yet */
    stvfs->f_blocks = super_blocks_count();
    stvfs->f_bfree = super_free_blocks_count();
    stvfs->f_bavail = stvfs->f_blocks - stvfs->f_bfree;

    stvfs->f_flag |= ST_RDONLY;

    return 0;
}
