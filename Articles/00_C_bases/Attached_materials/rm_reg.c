#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

int
main(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    DIR* d = opendir(argv[1]);

    struct dirent* dd;

    while ((dd = readdir(d)) != NULL) {
        char str[PATH_MAX];
        snprintf(str, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat buf;
        if (lstat(str, &buf) < 0) {
            continue;
        }

        if (!S_ISREG(buf.st_mode)) {
            continue;
        }

        if ( (dd->d_name[strlen(dd->d_name) - 1] == '~') ) {
            unlink(str);
        } else if ( strlen(dd->d_name) >= 4 && 
                strcmp(dd->d_name + strlen(dd->d_name) - 4, ".bak") == 0) {
            unlink(str);
        }
    }

    closedir(d);
    return 0;
}
