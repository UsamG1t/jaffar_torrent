#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int
main(int argc, char** argv)
{
    if (argc < 3) {
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        return 1;
    }

    int fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd2 < 0) {
        return 1;
    }

    struct stat buf;
    if (lstat(argv[1], &buf) < 0) {
        return -1;
    }

    double* input = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd1, 0);

    ftruncate(fd2, buf.st_size);
    double* output = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    if (input == NULL || output == NULL) {
        return 1;
    }

    int count = buf.st_size / sizeof(*input);

    for (int i = 0; i < count; i++) {
        output[i] = input[count - i - 1];
    }

    munmap(input, buf.st_size);
    munmap(output, buf.st_size);

    return 0;
}
