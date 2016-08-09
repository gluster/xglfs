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
#include <getopt.h>
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
	fprintf(stderr, "Usage: xglfs --server=<IP|DOMAIN> --volume=foobar --mountpoint=/mnt/foobar [--protocol=<tcp|udp>] [--port=24007] [--logfile=<file|stderr|null>] [--verbosity=7] [--foreground]\n");
	exit(EX_USAGE);
}

int main(int _argc, char** _argv)
{
	int opts = 0;
	unsigned short int foreground = 0;
	char* invalid = NULL;
	struct option cli_options[] =
	{
		{ "protocol",	required_argument,	NULL,	't' },
		{ "server",		required_argument,	NULL,	's' },
		{ "port",		required_argument,	NULL,	'p' },
		{ "volume",		required_argument,	NULL,	'l'	},
		{ "mountpoint",	required_argument,	NULL,	'm' },
		{ "logfile",	required_argument,	NULL,	'o' },
		{ "verbosity",	required_argument,	NULL,	'v' },
		{ "foreground",	no_argument,		NULL,	'f' },
		{ 0, 0, 0, 0}
	};

	if (getuid() != 0 || geteuid() != 0)
	{
		fprintf(stderr, "You must be root in order to mount GlusterFS\n");
		exit(EX_NOPERM);
	}

	struct xglfs_state* xglfs_state = calloc(1, sizeof(struct xglfs_state));

	while ((opts = getopt_long(_argc, _argv, "t:s:p:l:m:o:v:f", cli_options, NULL)) != -1)
	{
		switch (opts)
		{
			case 't':
				if (strcmp(optarg, "tcp") == 0 || strcmp(optarg, "udp") == 0)
					xglfs_state->protocol = strdup(optarg);
				else
					xglfs_usage();
				break;
			case 's':
				xglfs_state->server = strdup(optarg);
				break;
			case 'p':
				xglfs_state->port = strtol(optarg, &invalid, 10);
				if (*invalid != '\0')
					xglfs_usage();
				break;
			case 'l':
				xglfs_state->volume = strdup(optarg);
				break;
			case 'm':
				xglfs_state->mountpoint = strdup(optarg);
				break;
			case 'o':
				if (strcmp(optarg, "null") == 0)
					xglfs_state->glfs_logfile = strdup(DEV_NULL);
				else if (strcmp(optarg, "stderr") == 0)
					xglfs_state->glfs_logfile = strdup(DEV_STDERR);
				else
					xglfs_state->glfs_logfile = strdup(optarg);
				break;
			case 'v':
				xglfs_state->glfs_verbosity = strtol(optarg, &invalid, 10);
				if (*invalid != '\0')
					xglfs_usage();
				break;
			case 'f':
				foreground = 1;
				break;
			default:
				xglfs_usage();
				break;
		}
	}

	// Check mandatory options
	if (!xglfs_state->server ||
		!xglfs_state->volume ||
		!xglfs_state->mountpoint)
		xglfs_usage();

	// Check optional values
	if (!xglfs_state->protocol)
		xglfs_state->protocol = strdup(GLFS_DEFAULT_PROTOCOL);
	if (xglfs_state->port == 0)
		xglfs_state->port = GLFS_DEFAULT_PORT;
	if (!xglfs_state->glfs_logfile)
		xglfs_state->glfs_logfile = strdup(DEV_NULL);
	if (xglfs_state->glfs_verbosity == 0)
		xglfs_state->glfs_verbosity = GLFS_DEFAULT_VERBOSITY;

	char** args = calloc(foreground + 3, sizeof(char*));
	size_t index = 0;
	args[index++] = strdup(_argv[0]);
	args[index++] = strdup("-orw,suid,dev,direct_io,allow_other,default_permissions");
	if (foreground)
		args[index++] = strdup("-f");
	args[index++] = xglfs_state->mountpoint;

	exit(fuse_main(index, args, &xglfs_ops, xglfs_state));
}

