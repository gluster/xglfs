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
#include <string.h>
#include <xglfs.h>
#include <xglfs_fgetattr.h>
#include <xglfs_getattr.h>

int xglfs_fgetattr(const char* _path, struct stat* _statbuf, struct fuse_file_info* _info)
{
	debug("%s", __func__);

	int ret = 0;

	if (!strcmp(_path, "/"))
		return xglfs_getattr(_path, _statbuf);

	ret = glfs_fstat((glfs_fd_t*)(uintptr_t)_info->fh, _statbuf);
	if (ret < 0)
		return -errno;

	return ret;
}

