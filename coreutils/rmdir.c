/* vi: set sw=4 ts=4: */
/*
 * Mini rmdir implementation for busybox
 *
 *
 * Copyright (C) 1999,2000 by Lineo, inc.
 * Written by Erik Andersen <andersen@lineo.com>, <andersee@debian.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "internal.h"
#include <stdio.h>
#include <errno.h>

const char rmdir_usage[] =
	"rmdir [OPTION]... DIRECTORY...\n"
#ifndef BB_FEATURE_TRIVIAL_HELP
	"\nRemove the DIRECTORY(ies), if they are empty.\n"
#endif
	;

extern int rmdir_main(int argc, char **argv)
{
	if (argc == 1 || **(argv + 1) == '-')
		usage(rmdir_usage);

	while (--argc > 0) {
		if (rmdir(*(++argv)) == -1) {
			errorMsg("%s\n", strerror(errno));
			exit(FALSE);
		}
	}
	return(TRUE);
}
