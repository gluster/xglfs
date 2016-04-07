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

#include "xglfs.h"
#include "xglfs_readdir.h"

int xglfs_readdir(const char* _path, void* _buf, fuse_fill_dir_t _filler, off_t _offset, struct fuse_file_info* _info)
{
	int ret = 0;

	struct dirent* de = glfs_readdir(FH_TO_FD(_info->fh));
	if (unlikely(!de))
		ret = -errno;

	if (likely(ret == 0))
	{
		do
		{
			if (unlikely(_filler(_buf, de->d_name, NULL, 0) != 0))
			{
				ret = -ENOMEM;
				break;
			}
		} while (likely((de = glfs_readdir(FH_TO_FD(_info->fh))) != NULL));
	}

	return ret;
}

