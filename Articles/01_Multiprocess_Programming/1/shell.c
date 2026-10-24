#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int
main(int argc, char** argv)
{
    if (argc < 5) {
        return -1;
    }

    pid_t pid;

    if ((pid = fork()) < 0) {
        return -2;
    } else if (!pid) {
        int fd0 = open(argv[2], O_RDONLY);
        int fd1 = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0600);
        int fd2 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0600);

        dup2(fd0, 0);
        dup2(fd1, 1);
        dup2(fd2, 2);

        close(fd0);
        close(fd1);
        close(fd2);

        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    int status;
    while(wait(&status) != -1);
    
    return 0;
}
