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
#include <xglfs_setxattr.h>

int xglfs_setxattr(const char* _path, const char* _name, const char* _value, size_t _size, int _flags)
{
	debug("%s", __func__);

	int ret = 0;

	ret = glfs_lsetxattr(XGLFS_STATE->fs, _path, _name, _value, _size, _flags);
	if (ret < 0)
		return -errno;

	return ret;
}

