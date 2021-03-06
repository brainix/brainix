/*----------------------------------------------------------------------------*\
 |	time.c								      |
 |									      |
 |	Copyright � 2002-2007, Team Brainix, original authors.		      |
 |		All rights reserved.					      |
\*----------------------------------------------------------------------------*/

/*
 | This program is free software: you can redistribute it and/or modify it under
 | the terms of the GNU General Public License as published by the Free Software
 | Foundation, either version 3 of the License, or (at your option) any later
 | version.
 |
 | This program is distributed in the hope that it will be useful, but WITHOUT
 | ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 | FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 | details.
 |
 | You should have received a copy of the GNU General Public License along with
 | this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fs/fs.h>

/*----------------------------------------------------------------------------*\
 |				 do_fs_utime()				      |
\*----------------------------------------------------------------------------*/
int do_fs_utime(const char *path, const struct utimbuf *times)
{

/* Perform various checks, and set an file's access and modification times.
 * Return 0 on success, or a negative error number. */

	inode_t *inode_ptr = path_to_inode(path);

	if (inode_ptr == NULL)
	{
		/* The path name could not be resolved. */
		inode_put(inode_ptr);
		return -err_code;
	}
	if (!perm(inode_ptr, W_OK, false))
	{
		/* No utime permission. */
		inode_put(inode_ptr);
		return -(err_code = EPERM);
	}

	/* All checks passed.  Set the file's times. */

	inode_ptr->i_atime = times == NULL ? do_time(NULL) : times->actime;
	inode_ptr->i_mtime = times == NULL ? do_time(NULL) : times->modtime;
	inode_ptr->dirty = true;
	inode_put(inode_ptr);
	return 0;
}
