#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int
main(int argc, char** argv)
{
    int sem = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    int shm = shmget(IPC_PRIVATE, 256, IPC_CREAT | 0666);


    pid_t pid1, pid2;

    if ((pid1 = fork()) < 0) {
        return -1;
    } else if (!pid1) {
        char* shm_writer = shmat(shm, NULL, 0);

        struct sembuf v1 = {0, 1, 0};
        struct sembuf p2 = {1, -1, 0};

        while (fgets(shm_writer, 256, stdin) != NULL) {
            semop(sem, &v1, 1);
            semop(sem, &p2, 1);
        }

        shmdt(shm_writer);
        semctl(sem, IPC_RMID, 0);
        shmctl(shm, IPC_RMID, 0);
    }

    if ((pid2 = fork()) < 0) {
        return -2;
    } else if (!pid2) {
        char* shm_reader = shmat(shm, NULL, SHM_RDONLY);

        struct sembuf p1 = {0, -1, 0};
        struct sembuf v2 = {1, 1, 0};

        while (semop(sem, &p1, 1) >= 0) {
            printf("%s\n", shm_reader);
            fflush(stdout);
            semop(sem, &v2, 1);
        }

        shmdt(shm_reader);
    }

    while(wait(NULL) > 0);

    return 0;
}
