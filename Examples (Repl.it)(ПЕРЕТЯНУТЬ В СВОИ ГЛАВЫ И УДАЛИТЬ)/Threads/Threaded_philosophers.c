#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

// философы и вилки
// Каждый:
// 1) разговаривает [1:9]*0.3c - usleep(мкрс)
// 2) Ждёт вилок
// 3) ест
// 4) отдаёт вилки
// при SIGINT отец всех убивает

volatile int flag = 1;
char shared_memory[5] = {'t', 't', 't', 't', 't'};
pthread_mutex_t fork_mutex[5] = {
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher(void *args) {
  int id = *(int *)args;
  int talk_time = rand() / (RAND_MAX + 1.0) * 9 + 1 + id;
  int eat_time = rand() / (RAND_MAX + 1.0) * 9 + 1 + id - 1;

  // struct sembuf down[2] = {{i, -1, 0}, {(i + 1) % nproc, -1, 0}};
  // struct sembuf up[2] = {{i, 1, 0}, {(i + 1) % nproc, 1, 0}};

  while (1) {
    shared_memory[id] = 't';
    usleep(talk_time * 300000);
    shared_memory[id] = 'w';

    // semop(sem, down, 2);
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&fork_mutex[id]);
    pthread_mutex_lock(&fork_mutex[(id + 1) % 5]);

    shared_memory[id] = 'e';
    usleep(eat_time * 300000);
    shared_memory[id] = 'g';

    // semop(sem, up, 2);
    pthread_mutex_unlock(&fork_mutex[id]);
    pthread_mutex_unlock(&fork_mutex[(id + 1) % 5]);
    pthread_mutex_unlock(&mutex);
  }
}

void handler(int sig) {
  printf("Bye Bye\n");
  exit(0);
}

void info() {
  char full_states[4][20] = {"Thinking", "Wanna take forks", "Eating",
                             "Put forks on a table"};

  for (int i = 0; i < 5; i++) {
    printf("Philosopher %d: ", i);
    // fflush(stdout);

    switch (shared_memory[i]) {
    case 't':
      printf("%s\n", full_states[0]);
      // fflush(stdout);
      break;
    case 'w':
      printf("%s\n", full_states[1]);
      // fflush(stdout);
      break;
    case 'e':
      printf("%s\n", full_states[2]);
      // fflush(stdout);
      break;
    case 'g':
      printf("%s\n", full_states[3]);
      // fflush(stdout);
      break;
    }
  }

  printf("\n");
  // fflush(stdout);
}

int main(int argc, char **argv) {
  sigaction(SIGINT,
            &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART},
            NULL);

  srand(100000);

  pthread_t tid[5];
  for (int i = 0; i < 5; i++) {
    pthread_create(&tid[i], NULL, philosopher, &i);
    pthread_detach(tid[i]);
  }

  while (1) {
    usleep(1500000);
    info();
  }

  return 0;
}
