#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>

enum
{
    SIZE = 2,
    BYTE_SIZE = 8
};

int
main(int argc, char** argv)
{
    int fd_1;
    char blocks[SIZE] = {};
    unsigned short a;

    if ((fd_1 = creat(argv[1], 0755)) < 0) {
        return 1;
    }

    while(scanf("%hu", &a) == 1) {
        for (int i = 0; i < SIZE; i++) {
            blocks[SIZE - i - 1] = a & 0xFF;
            a >>= BYTE_SIZE;
        }

        write(fd_1, blocks, SIZE);
    }

    close(fd_1);
    return 0;
}
