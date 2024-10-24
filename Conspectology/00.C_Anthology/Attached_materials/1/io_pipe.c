#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char** argv)
{
    if (argc < 3) {
        return -1;
    }
    
    int fd[2];
    
    if (pipe(fd) < 0) {
        return -2;
    }


    pid_t pid;

    if ((pid = fork()) < 0) {
        return -3;
    } else if (!pid){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);

        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    
    if ((pid = fork()) < 0) {
        return -4;
    } else if (!pid) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);

        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    int status;
    while(wait(&status) != -1);
    
    return 0;
}
