#include <sys/vfs.h>
#include "libbb.h"

/*
 * Code from libguestfs
 */
int statfs(const char *file, struct statfs *buf)
{
	ULONGLONG free_bytes_available;         /* for user - similar to bavail */
	ULONGLONG total_number_of_bytes;
	ULONGLONG total_number_of_free_bytes;   /* for everyone - bfree */

	if ( !GetDiskFreeSpaceEx(file, (PULARGE_INTEGER) &free_bytes_available,
			(PULARGE_INTEGER) &total_number_of_bytes,
			(PULARGE_INTEGER) &total_number_of_free_bytes) ) {
		return -1;
	}

	/* XXX I couldn't determine how to get block size.  MSDN has a
	 * unhelpful hard-coded list here:
	 * http://support.microsoft.com/kb/140365
	 * but this depends on the filesystem type, the size of the disk and
	 * the version of Windows.  So this code assumes the disk is NTFS
	 * and the version of Windows is >= Win2K.
	 */
	if (total_number_of_bytes < UINT64_C(16) * 1024 * 1024 * 1024 * 1024)
		buf->f_bsize = 4096;
	else if (total_number_of_bytes < UINT64_C(32) * 1024 * 1024 * 1024 * 1024)
		buf->f_bsize = 8192;
	else if (total_number_of_bytes < UINT64_C(64) * 1024 * 1024 * 1024 * 1024)
		buf->f_bsize = 16384;
	else if (total_number_of_bytes < UINT64_C(128) * 1024 * 1024 * 1024 * 1024)
		buf->f_bsize = 32768;
	else
		buf->f_bsize = 65536;

	/* As with stat, -1 indicates a field is not known. */
	buf->f_frsize = buf->f_bsize;
	buf->f_blocks = total_number_of_bytes / buf->f_bsize;
	buf->f_bfree = total_number_of_free_bytes / buf->f_bsize;
	buf->f_bavail = free_bytes_available / buf->f_bsize;
	buf->f_files = -1;
	buf->f_ffree = -1;
	buf->f_favail = -1;
	buf->f_fsid = -1;
	buf->f_flag = -1;
	buf->f_namemax = FILENAME_MAX;

	return 0;
}
