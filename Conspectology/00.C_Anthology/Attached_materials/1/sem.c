#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int
main(int argc, char** argv)
{
    if (argc < 3) {
        return -100;
    }

    char* p;
    unsigned nproc = strtol(argv[1], &p, 10);
    unsigned maxval = strtol(argv[2], &p, 10);
 

    pid_t pid;
    unsigned son_num, val = 0;
    int sem = semget(IPC_PRIVATE, nproc, IPC_CREAT | 0666);

    int fd[2];

    if (pipe(fd) < 0) {
        return -1;
    }

    for (int i = 0; i < nproc; i++) {
        son_num = i;

        if ((pid = fork()) < 0) {
            return i + 1;
        } else if(!pid) {
            struct sembuf p = {son_num, -1, 0};
            struct sembuf v = {(son_num + 1) % nproc, 1, 0};
            
            printf("Hello from %u\n", son_num + 1);
            fflush(stdout);

            while (semop(sem, &p, 1) >= 0) {
                if (read(fd[0], &val, sizeof(val)) != sizeof(val)) {
                    return son_num + 1;
                }

                printf("%u ping with %u \n", son_num + 1, val);
                fflush(stdout); // всегда пишем после printf для очистки буфера вывода от мусора

                val++;
                write(fd[1], &val, sizeof(val));
                
                if (val == maxval) {
                    printf("PONG\n");
                    fflush(stdout);
                    break;
                }

                semop(sem, &v, 1);
            }

            printf("Bye from %u\n", son_num + 1);
            fflush(stdout);

            semctl(sem, 0, IPC_RMID, 0);
            close(fd[0]);
            close(fd[1]);
            _exit(0);
        }
    }

    write(fd[1], &val, sizeof(val));
    struct sembuf start = {0, 1, 0};
    semop(sem, &start, 1);

    while(wait(NULL) > 0);

    return 0;
}
