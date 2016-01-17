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
#include <xglfs_init.h>

void* xglfs_init(struct fuse_conn_info* _connection)
{
	pfcq_debug_init(XGLFS_STATE->verbose, XGLFS_STATE->debug, XGLFS_STATE->syslog);

	debug("%s", __func__);

	XGLFS_STATE->fs = glfs_new(XGLFS_STATE->volume);
	if (unlikely(!XGLFS_STATE->fs))
		panic("Unable to create GlusterFS instance");
	if (unlikely(glfs_set_logging(XGLFS_STATE->fs, XGLFS_STATE->glfs_logfile, 7)))
		panic("Unable to set GlusterFS logging");
	if (unlikely(glfs_set_volfile_server(
		XGLFS_STATE->fs, XGLFS_STATE->protocol, XGLFS_STATE->server, XGLFS_STATE->port)))
		panic("Unable to set GlusterFS volume file server");
	if (unlikely(glfs_init(XGLFS_STATE->fs)))
		panic("Unable to initialize GlusterFS");

	return XGLFS_STATE;
}

