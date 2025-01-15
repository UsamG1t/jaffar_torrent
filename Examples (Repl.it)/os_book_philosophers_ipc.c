#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

enum {
    THINKING,
    HUNGRY,
    EATING,
    N = 5
};

int left(int i) {
    return (i - 1) % N;
}

int right(int i) {
    return (i + 1) % N;
}

void think(int i) {
    printf("Philosopher %d thinks\n", i);
    sleep(1);
}

void eat(int i) {
    printf("Philosopher %d eats\n", i);
    sleep(2);
}

struct sembuf down_mutex = {N, -1, 0}, up_mutex = {N, 1, 0};

void test(int i, int semid, int* p) {
    if (p[i] == HUNGRY &&
        p[left(i)] != EATING &&
        p[right(i)] != EATING) {
            p[i] = EATING;

            struct sembuf up = {i, 1, 0};
            semop(semid, &up, 1);
        }
}

void take_forks(int i, int semid, int* p) {
    semop(semid, &down_mutex, 1);
    p[i] = HUNGRY;
    test(i, semid, p);
    semop(semid, &up_mutex, 1);

    struct sembuf down = {i, -1, 0};
    semop(semid, &down, 1);
}

void put_forks(int i, int semid, int* p) {
    semop(semid, &down_mutex, 1);
    p[i] = THINKING;
    test(left(i), semid, p);
    test(right(i), semid, p);
    semop(semid, &up_mutex, 1);
}

void philosopher(int i, int semid, int shmid) {
    int* p = shmat(shmid, 0, 0);

    while(1) {
        think(i);
        take_forks(i, semid, p);
        eat(i);
        put_forks(i, semid, p);
    }
}


int main(void) {
  setbuf(stdout, 0);

  int semid = semget(IPC_PRIVATE, N + 1, IPC_CREAT | 0600); // 0-(N-1) + mutex 
  int shmid = shmget(IPC_PRIVATE, N * sizeof(int), IPC_CREAT | 0600); // states 
  semctl(semid, N, SETVAL, 1);
  
  pid_t pid;


  for (int i = 0; i < 5; i++) {
    if ((pid = fork()) < 0) {
      _exit(1);
    } else if (!pid) {
      philosopher(i, semid, shmid);
      _exit(0);
    }
  }

  while (wait(NULL) != -1)
    ;
  semctl(semid, 0, IPC_RMID, 0);
  shmctl(shmid, IPC_RMID, 0);

  return 0;
}
