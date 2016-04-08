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
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "xglfs.h"
#include "xglfs_fops.h"

struct fuse_operations xglfs_ops =
{
	.getattr = xglfs_getattr,
	.readlink = xglfs_readlink,
	.getdir = NULL,
	.mknod = xglfs_mknod,
	.mkdir = xglfs_mkdir,
	.unlink = xglfs_unlink,
	.rmdir = xglfs_rmdir,
	.symlink = xglfs_symlink,
	.rename = xglfs_rename,
	.link = xglfs_link,
	.chmod = xglfs_chmod,
	.chown = xglfs_chown,
	.truncate = xglfs_truncate,
	.utime = NULL,
	.open = xglfs_open,
	.read = xglfs_read,
	.write = xglfs_write,
	.statfs = xglfs_statfs,
	.flush = xglfs_flush,
	.release = xglfs_release,
	.fsync = xglfs_fsync,
	.setxattr = xglfs_setxattr,
	.getxattr = xglfs_getxattr,
	.listxattr = xglfs_listxattr,
	.removexattr = xglfs_removexattr,
	.opendir = xglfs_opendir,
	.readdir = xglfs_readdir,
	.releasedir = xglfs_releasedir,
	.fsyncdir = xglfs_fsyncdir,
	.init = xglfs_init,
	.destroy = xglfs_destroy,
	.access = xglfs_access,
	.create = xglfs_create,
	.ftruncate = xglfs_ftruncate,
	.fgetattr = xglfs_fgetattr,
	.lock = xglfs_lock,
	.utimens = xglfs_utimens,
	.bmap = NULL
};

__attribute__((noreturn)) static void xglfs_usage(void)
{
	fprintf(stderr, "Usage: xglfs <[tcp|udp]:server:[port]:volume:[GlusterFS_logfile]:[GlusterFS_verbosity]:[foreground]> <mountpoint>\n");
	fprintf(stderr, "Example: xglfs :glusterfs.example.com::bigvolume::: /mnt/bigvolume\n");
	exit(EX_USAGE);
}

int main(int _argc, char** _argv)
{
	if (getuid() != 0 || geteuid() != 0)
	{
		fprintf(stderr, "You must be root in order to mount GlusterFS\n");
		exit(EX_NOPERM);
	}

	if (_argc != 3)
		xglfs_usage();

	struct xglfs_state* xglfs_state = calloc(1, sizeof(struct xglfs_state));

	char* source_i = strdup(_argv[1]);
	char* source_p = source_i;

	char* protocol = strsep(&source_i, ":");
	if (strcmp(protocol, "") == 0)
		xglfs_state->protocol = strdup(GLFS_DEFAULT_PROTOCOL);
	else
	{
		if (strcmp(protocol, "tcp") != 0 && strcmp(protocol, "udp") != 0)
			xglfs_usage();
		else
			xglfs_state->protocol = strdup(protocol);
	}

	char* server = strsep(&source_i, ":");
	if (strcmp(server, "") == 0)
		xglfs_usage();
	xglfs_state->server = strdup(server);

	char* port = strsep(&source_i, ":");
	if (strcmp(port, "") == 0)
		xglfs_state->port = GLFS_DEFAULT_PORT;
	else
	{
		char* invalid = NULL;
		xglfs_state->port = strtol(port, &invalid, 10);
		if (*invalid != '\0')
			xglfs_usage();
	}

	char* volume = strsep(&source_i, ":");
	if (strcmp(volume, "") == 0)
		xglfs_usage();
	xglfs_state->volume = strdup(volume);

	char* glfs_logfile = strsep(&source_i, ":");
	if (strcmp(glfs_logfile, "") == 0)
		xglfs_state->glfs_logfile = strdup(DEV_NULL);
	else
		xglfs_state->glfs_logfile = strdup(glfs_logfile);

	char* glfs_verbosity = strsep(&source_i, ":");
	if (strcmp(glfs_verbosity, "") == 0)
		xglfs_state->glfs_verbosity = GLFS_DEFAULT_VERBOSITY;
	else
	{
		char* invalid = NULL;
		xglfs_state->glfs_verbosity = strtol(glfs_verbosity, &invalid, 10);
		if (*invalid != '\0')
			xglfs_usage();
	}

	unsigned short int foreground = 0;
	char* stay_foreground = strsep(&source_i, ":");
	if (strcmp(stay_foreground, "") == 0)
		foreground = 0;
	else
		foreground = 1;

	free(source_p);

	char** args = calloc(foreground + 3, sizeof(char*));
	size_t index = 0;
	args[index++] = strdup(_argv[0]);
	args[index++] = strdup("-orw,suid,dev,direct_io,allow_other,default_permissions");
	if (foreground)
		args[index++] = strdup("-f");
	args[index++] = strdup(_argv[2]);

	exit(fuse_main(index, args, &xglfs_ops, xglfs_state));
}

