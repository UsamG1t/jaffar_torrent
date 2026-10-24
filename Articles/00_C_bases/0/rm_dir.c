#include <stdio.h>
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

        if (!S_ISDIR(buf.st_mode)) {
            continue;
        }

        DIR* internal_d = opendir(str);
        struct dirent* internal_dd;

        for (int i = 0; i < 2; i++) {
            internal_dd = readdir(internal_d);
        }

        if ((internal_dd = readdir(internal_d)) == NULL) {
            closedir(internal_d);
            rmdir(str);
        }
    }

    closedir(d);
    return 0;
}
