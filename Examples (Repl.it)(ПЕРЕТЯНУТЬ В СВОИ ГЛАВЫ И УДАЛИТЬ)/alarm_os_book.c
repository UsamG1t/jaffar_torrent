#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void alrm(int sig){
  printf("Жду имя\n");
  alarm(5);
}

int main(void) {
  setbuf(stdout, 0);

  char s[80];
  signal(SIGALRM, alrm);
  alarm(5);
  printf("Введите имя: ");

  for(;;) {
      printf("Имя: ");
      if (fgets(s, 80, stdin) != NULL) break;
  }

  printf("OK\n");
  return 0;
}


/*
~/workspace$ gcc alarm_os_book.c -o alarm_os_book
~/workspace$ ./alarm_os_book 
Введите имя: Имя: Жду имя
Жду имя
Жду имя
Hello
OK
~/workspace$ ^C
~/workspace$ 
*/