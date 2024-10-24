#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>

enum
{
    SIZE = 4,
    BYTE_SIZE = 8
};

int
main(int argc, char** argv)
{
    int fd_1;
    char blocks[SIZE] = {};
    long sum = 0;

    if ((fd_1 = open(argv[1], O_RDONLY)) < 0) {
        return 1;
    }
        
    while(read(fd_1, blocks, SIZE) == SIZE) {
        unsigned int a = 0;
        
        for(int i = 0; i < SIZE; i++) {
            a <<= BYTE_SIZE;
            a += blocks[i];
        }
        sum += a;
    }
    
    printf("%ld\n", sum);

    close(fd_1);
    return 0;
}
