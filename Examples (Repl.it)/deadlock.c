#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
  short val[2] = {1, 1};
  semctl(semid, 0, SETALL, val);
  pid_t pid;

  struct sembuf p0 = {0, -1, 0}, p1 = {1, -1, 0};
  struct sembuf v0 = {0, +1, 0}, v1 = {1, +1, 0};

  if ((pid = fork()) < 0) {
    _exit(1);
  } else if (pid == 0) {
    semop(semid, &p0, 1);
    printf("Son blocks 0\n");
    fflush(stdout);
    sleep(1);
    printf("Son traya block 1\n");
    fflush(stdout);
    semop(semid, &p1, 1);
    semop(semid, &v0, 1);
    semop(semid, &v1, 1);
  } else {
    semop(semid, &p1, 1);
    printf("Father blocks 1\n");
    fflush(stdout);
    sleep(1);
    printf("Father traya block 0\n");
    fflush(stdout);
    semop(semid, &p0, 1);
    semop(semid, &v1, 1);
    semop(semid, &v0, 1);
  }

  semctl(semid, 0, IPC_RMID, 0);
  return 0;
}

/*
~/workspace$ gcc deadlock.c -o deadlock
~/workspace$ ./deadlock 
Father blocks 1
Son blocks 0
Father traya block 0
Son traya block 1
^C
~/workspace$
*/