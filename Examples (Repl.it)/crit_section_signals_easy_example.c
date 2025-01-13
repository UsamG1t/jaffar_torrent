#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void hlr(int sig){}

int main(void) {
  setbuf(stdout, 0);
  pid_t pid[5] = {};

  signal(SIGUSR1, hlr);
  
  for (int i = 0; i < 5; i++) {
    if ((pid[i] = fork()) < 0) {
      _exit(1);
    } else if (!pid[i]) {
      printf("Son %d waits\n", i);
      pause();
      printf("Son %d starts\n", i);
      sleep(1);
      printf("Son ");
      printf("%d likes", i);
      printf(" you!\n");
      kill(getppid(), SIGUSR1);
      _exit(0);
    }
  }

  sleep(2);
  
  for (int i = 0; i < 5; i++) {
    printf("Father picks up %d\n", i);
    kill(pid[i], SIGUSR1);
    pause();
  }

  while (wait(NULL) != -1);

  return 0;
}


/*
~/workspace$ gcc crit_section_signals_easy_example.c -o crit_section_signals_easy_example
~/workspace$ time ./crit_section_signals_easy_example 
Son 0 waits
Son 1 waits
Son 3 waits
Son 4 waits
Son 2 waits
Father picks up 0
Son 0 starts
Son 0 likes you!
Father picks up 1
Son 1 starts
Son 1 likes you!
Father picks up 2
Son 2 starts
Son 2 likes you!
Father picks up 3
Son 3 starts
Son 3 likes you!
Father picks up 4
Son 4 starts
Son 4 likes you!

real    0m7.010s
user    0m0.006s
sys 0m0.005s
~/workspace$
*/