#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int fd[2] = {};

void hlr(int sig){
  int i;
  printf("~\n");
  int res = read(fd[0], &i, sizeof(i));
}

int main(void) {
  setbuf(stdout, 0);

  int res = pipe(fd);
  signal(SIGINT, hlr);

  pid_t pid;

  if ((pid = fork()) < 0) {
    _exit(1);
  } else if (!pid) {
    int count = 0;
    char *s = calloc(64640, sizeof(*s));
    int written = 0;
    while ((written = write(fd[1], s, sizeof(s))) > 0) {
      count++;
      printf("%d bytes written\n", written);
    }
  } else {
    while(1) {
      pause();
    }
  }
  
  return 0;
}


/*
Нерезультативный вывод...
*/
