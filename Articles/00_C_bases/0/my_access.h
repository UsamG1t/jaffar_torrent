#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int
my_access(const char* name, int mode)
{
	struct stat buf;

	if (lstat(name, &buf) < 0) {
		return -5;
	}

	int access_rights = buf.st_mode & 0777;

	if (getuid() == buf.st_uid) {
		access_rights >>= 6;
	} else if (getgid() == buf.st_gid) {
		access_rights >>= 3;
	} 

	if ( mode == (access_rights & 7) ) {
		return 0;
	} else {
		return -1;
	}
}
