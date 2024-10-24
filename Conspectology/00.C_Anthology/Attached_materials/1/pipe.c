#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char** argv)
{
    pid_t pid;
    int fd12[2];
    int fd21[2];

    if (pipe(fd12) < 0) {
        return -1;
    }
    if (pipe(fd21) < 0) {
        return -1;
    }

    if ((pid = fork()) < 0) {
        return -2;
    } else if (!pid) {
        int c1_pid = getpid();
        printf("Im c1, my pid is %d\n", c1_pid);
        
        if ((pid = fork()) < 0) {
            return -5;
        } else if (!pid) {
            int c2_pid_result, c1_pid = getppid();
            
            close(fd12[1]);
            if (read(fd12[0], &c2_pid_result, sizeof(c2_pid_result)) != sizeof(c2_pid_result)) {
                return 1;
            }
            close(fd12[0]);

            close(fd21[0]);
            if (write(fd21[1], &c1_pid, sizeof(c1_pid)) != sizeof(c1_pid)) {
                return 1;
            }
            close(fd21[1]);

            printf("Im b1, c2_pid = %d\n", c2_pid_result);
            return 0;
        }

        close(fd12[0]);
        close(fd12[1]);
        close(fd21[0]);
        close(fd21[1]);

        wait(NULL);
        return 0;

    }

    if ((pid = fork()) < 0) {
        return -2;
    } else if (!pid) {
        int c2_pid = getpid();
        printf("Im c2, my pid is %d\n", c2_pid);
        
        if ((pid = fork()) < 0) {
            return -5;
        } else if (!pid) {
            int c1_pid_result, c2_pid = getppid();
            
            close(fd12[0]);
            if (write(fd12[1], &c2_pid, sizeof(c2_pid)) != sizeof(c2_pid)) {
                return 1;
            }
            close(fd12[1]);

            close(fd21[1]);
            if (read(fd21[0], &c1_pid_result, sizeof(c1_pid_result)) != sizeof(c1_pid_result)) {
                return 1;
            }
            close(fd21[0]);

            printf("Im b2, c1_pid = %d\n", c1_pid_result);
            return 0;
        }

        close(fd12[0]);
        close(fd12[1]);
        close(fd21[0]);
        close(fd21[1]);

        wait(NULL);
        return 0;

    }

    int status;
    while(wait(&status) != -1);
    
    return 0;
}
