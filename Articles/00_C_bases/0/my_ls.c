#include <stdio.h>
#include <sys/stat.h>

enum
{
    ACC_COUNT = 9
};

int
main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        struct stat buf;
        
        if (lstat(argv[i], &buf) < 0) {
            return 1;
        }

        if (S_ISREG(buf.st_mode)) {
            printf("- ");
        } else if (S_ISDIR(buf.st_mode)) {
            printf("d ");
        } else if (S_ISLNK(buf.st_mode)) {
            printf("l ");
        }

        char access[] = "rwxrwxrwx";
        for (int j = 0; j < ACC_COUNT; j++) {
            if (buf.st_mode & (1 << (ACC_COUNT - 1 - j) )) {
                printf("%c", access[j]);
            } else {
                printf("-");
            }
        }
        printf(" ");

        printf("%hu ", buf.st_nlink);
        printf("%lu ", buf.st_size);
        printf("%s\n", argv[i]);
    }

    return 0;
}
