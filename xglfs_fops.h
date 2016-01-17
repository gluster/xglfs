/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/*
 * xglfs - GlusterFS API FUSE client
 * Copyright (C) Oleksandr Natalenko <oleksandr@natalenko.name>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <xglfs_access.h>
#include <xglfs_chmod.h>
#include <xglfs_chown.h>
#include <xglfs_create.h>
#include <xglfs_destroy.h>
#include <xglfs_fgetattr.h>
#include <xglfs_flush.h>
#include <xglfs_fsync.h>
#include <xglfs_fsyncdir.h>
#include <xglfs_ftruncate.h>
#include <xglfs_getattr.h>
#include <xglfs_getxattr.h>
#include <xglfs_init.h>
#include <xglfs_link.h>
#include <xglfs_listxattr.h>
#include <xglfs_lock.h>
#include <xglfs_mkdir.h>
#include <xglfs_mknod.h>
#include <xglfs_open.h>
#include <xglfs_opendir.h>
#include <xglfs_read.h>
#include <xglfs_readdir.h>
#include <xglfs_readlink.h>
#include <xglfs_release.h>
#include <xglfs_releasedir.h>
#include <xglfs_removexattr.h>
#include <xglfs_rename.h>
#include <xglfs_rmdir.h>
#include <xglfs_setxattr.h>
#include <xglfs_statfs.h>
#include <xglfs_symlink.h>
#include <xglfs_truncate.h>
#include <xglfs_unlink.h>
#include <xglfs_utimens.h>
#include <xglfs_write.h>
