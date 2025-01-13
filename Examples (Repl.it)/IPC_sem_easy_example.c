#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  setbuf(stdout, 0);

  int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
  semctl(semid, 0, SETVAL, 1);
  pid_t pid;

  struct sembuf p = {0, -1, 0}, v = {0, 1, 0};

  for (int i = 0; i < 5; i++) {
    if ((pid = fork()) < 0) {
      _exit(1);
    } else if (!pid) {
      semop(semid, &p, 1);
      printf("Son");
      printf("%d like", i);
      printf(" you!\n");
      semop(semid, &v, 1);
      _exit(0);
    }
  }

  while (wait(NULL) != -1)
    ;
  semctl(semid, 0, IPC_RMID, 0);

  return 0;
}

/*
~/workspace$ gcc IPC_sem_easy_example.c -o IPC_sem_easy_example
~/workspace$ ./IPC_sem_easy_example 
Son0 like you!
Son1 like you!
Son2 like you!
Son3 like you!
Son4 like you!
~/workspace$ ./IPC_sem_easy_example 
Son0 like you!
Son1 like you!
Son2 like you!
Son3 like you!
Son4 like you!
~/workspace$ ./IPC_sem_easy_example 
Son0 like you!
Son4 like you!
Son3 like you!
Son2 like you!
Son1 like you!
~/workspace$ ./IPC_sem_easy_example 
Son0 like you!
Son2 like you!
Son4 like you!
Son1 like you!
Son3 like you!
~/workspace$ ./IPC_sem_easy_example 
Son0 like you!
Son1 like you!
Son3 like you!
Son4 like you!
Son2 like you!
~/workspace$

*/