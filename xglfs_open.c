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

#include <errno.h>
#include <glusterfs/api/glfs.h>
#include <pfcq.h>
#include <xglfs.h>
#include <xglfs_open.h>

int xglfs_open(const char* _pathname, struct fuse_file_info* _info)
{
	debug("%s", __func__);

	glfs_fd_t* fd = NULL;

	if ((_info->flags & O_CREAT) == O_CREAT)
		fd = glfs_creat(XGLFS_STATE->fs, _pathname, _info->flags, 0);
	else
		fd = glfs_open(XGLFS_STATE->fs, _pathname, _info->flags);
	if (!fd)
		return -errno;

	_info->fh = (uint64_t)(uintptr_t)fd;

	return 0;
}

