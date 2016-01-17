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
#include <xglfs_truncate.h>

int xglfs_truncate(const char* _path, off_t _length)
{
	XGLFS_FOP_START;

	int ret = 0;

	// Unexpectedly, no :)
	// return glfs_truncate(XGLFS_STATE->fs, _path, _length);

	// Workaround:
	glfs_fd_t* fd = glfs_open(XGLFS_STATE->fs, _path, O_CREAT | O_WRONLY);
	if (unlikely(!fd))
		ret = -errno;

	if (likely(ret == 0))
	{
		ret = glfs_ftruncate(fd, _length);
		if (unlikely(ret < 0))
		{
			int saved_errno = errno;
			glfs_close(fd);
			ret = -saved_errno;
		} else
		{
			ret = glfs_close(fd);
			if (unlikely(ret < 0))
				ret = -errno;
		}
	}

	XGLFS_FOP_RET;
	XGLFS_FOP_END;

	return ret;
}

