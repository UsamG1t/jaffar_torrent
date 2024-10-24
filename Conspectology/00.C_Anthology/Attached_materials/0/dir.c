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
    unsigned long long size = 0;

    while ((dd = readdir(d)) != NULL) {
        char str[PATH_MAX];
        snprintf(str, PATH_MAX, "%s/%s", argv[1], dd->d_name);

        struct stat buf;
        if (lstat(str, &buf) < 0) {
            continue;
        }

        if ( S_ISREG(buf.st_mode) && !access(str, W_OK) ) {
            size += buf.st_size;
        }
    }

    printf("%llu\n", size);
    closedir(d);

    return 0;
}
